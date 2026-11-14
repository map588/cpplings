#!/usr/bin/env python3
"""cpplings — a rustlings-style C++ exercise runner.

Usage:
  python3 cpplings.py              # watch mode (default): work through exercises
  python3 cpplings.py watch NAME   # watch mode, starting at exercise NAME
  python3 cpplings.py run NAME     # build & run one exercise
  python3 cpplings.py hint NAME    # show hints for one exercise
  python3 cpplings.py list         # show all exercises and their status
  python3 cpplings.py verify       # check all exercises in order, stop at first failure
  python3 cpplings.py check-solutions [NAME ...]
                                   # CI: solutions must pass warning-free, shipped
                                   # exercises must fail at the stage their entry names

Environment:
  CPPLINGS_CXX            override compiler (default: clang++, then g++)
  CPPLINGS_NO_SANITIZERS  set to disable ASan/UBSan (some platforms misbehave)

Requires Python 3.11+ (tomllib) and a C++20 compiler.
"""

import argparse
import os
import platform
import select
import shutil
import subprocess
import sys
import tomllib
from pathlib import Path

ROOT = Path(__file__).resolve().parent
BUILD_DIR = ROOT / "build"
MARKER = "// I AM NOT DONE"

BASE_FLAGS = ["-std=c++20", "-Wall", "-Wextra", "-Werror=return-type", "-g"]
SANITIZER_FLAGS = [
    "-fsanitize=address,undefined",
    "-fno-sanitize-recover=undefined",  # make UB findings fatal, not just warnings
    "-fno-omit-frame-pointer",
]

USE_COLOR = sys.stdout.isatty()


def color(code: str, text: str) -> str:
    return f"\033[{code}m{text}\033[0m" if USE_COLOR else text


def green(t):  return color("32", t)
def red(t):    return color("31", t)
def yellow(t): return color("33", t)
def bold(t):   return color("1", t)


class Exercise:
    def __init__(self, entry: dict):
        self.name: str = entry["name"]
        self.rel_path: str = entry["path"]
        self.path: Path = ROOT / entry["path"]
        self.mode: str = entry.get("mode", "run")
        self.hints: list[str] = entry.get("hints", [])
        # extra .cpp files compiled together with the main file, for
        # multi-translation-unit exercises (linkage, ODR, ...)
        self.extra_files: list[str] = entry.get("extra_files", [])
        # "address" (ASan+UBSan, default) | "thread" (TSan — they can't
        # be combined) | "none"
        self.sanitizers: str = entry.get("sanitizers", "address")
        # sys.platform prefixes this exercise needs (e.g. ["linux"] for
        # /proc users). Empty = runs everywhere. Unsupported exercises
        # are skipped, not failed.
        self.platforms: list[str] = entry.get("platforms", [])
        # platform.machine() prefixes this exercise needs (e.g.
        # ["arm64", "aarch64", "x86_64", "AMD64"] for intrinsics /
        # inline-asm exercises). Empty = any CPU. Same skip semantics
        # as platforms.
        self.arch: list[str] = entry.get("arch", [])
        # extra compiler flags for this exercise (e.g. ["-O2"] for the
        # performance module)
        self.extra_flags: list[str] = entry.get("extra_flags", [])
        # the stage at which the SHIPPED exercise must fail: "compile"
        # (it does not build) or "run" (it builds, then an assert, a
        # sanitizer, or the timeout stops it). check-solutions enforces
        # this, so an exercise that breaks for the wrong reason (a
        # missing #include on another compiler, say) cannot hide.
        self.fails: str = entry.get("fails", "run")
        if self.fails not in ("compile", "run"):
            sys.exit(f"error: exercise '{self.name}': fails must be "
                     f"\"compile\" or \"run\", not \"{self.fails}\"")

    def supported_here(self) -> bool:
        os_ok = (not self.platforms
                 or any(sys.platform.startswith(p) for p in self.platforms))
        machine = platform.machine()
        arch_ok = (not self.arch
                   or any(machine.startswith(a) for a in self.arch))
        return os_ok and arch_ok

    def skip_reason(self) -> str:
        parts = []
        if self.platforms:
            parts.append("/".join(self.platforms))
        if self.arch:
            parts.append("/".join(self.arch))
        return " + ".join(parts)

    def sources(self) -> list[Path]:
        return [self.path] + [ROOT / p for p in self.extra_files]

    def solution_sources(self) -> list[Path]:
        return [self._to_solution(p) for p in self.sources()]

    @staticmethod
    def _to_solution(path: Path) -> Path:
        rel = path.relative_to(ROOT / "exercises")
        return ROOT / "solutions" / rel

    def has_marker(self) -> bool:
        return MARKER in self.path.read_text(encoding="utf-8")

    def dir_mtime(self) -> float:
        """Newest mtime of any file in the exercise's directory, so edits to
        headers and companion .cpp files retrigger the watcher too."""
        return max(p.stat().st_mtime
                   for p in self.path.parent.iterdir() if p.is_file())


def load_exercises() -> list[Exercise]:
    toml_path = ROOT / "exercises.toml"
    if not toml_path.exists():
        sys.exit("error: exercises.toml not found")
    with open(toml_path, "rb") as f:
        data = tomllib.load(f)
    exercises = [Exercise(e) for e in data.get("exercises", [])]
    if not exercises:
        sys.exit("error: no exercises defined in exercises.toml")
    return exercises


def find_compiler() -> str:
    cxx = os.environ.get("CPPLINGS_CXX")
    if cxx:
        if not shutil.which(cxx):
            sys.exit(f"error: CPPLINGS_CXX={cxx} not found on PATH")
        return cxx
    for candidate in ("clang++", "g++"):
        if shutil.which(candidate):
            return candidate
    sys.exit("error: no C++ compiler found (need clang++ or g++, or set CPPLINGS_CXX)")


def compile_flags(mode: str, sanitizers: str = "address",
                  extra_flags: list[str] | None = None,
                  werror: bool = False) -> list[str]:
    flags = list(BASE_FLAGS)
    if werror:
        flags.append("-Werror")  # check-solutions: a warning is a failure
    if extra_flags:
        flags += extra_flags
    if mode != "run" or os.environ.get("CPPLINGS_NO_SANITIZERS"):
        return flags
    if sanitizers == "thread":
        flags += ["-fsanitize=thread"]
    elif sanitizers == "address":
        flags += SANITIZER_FLAGS
    return flags


def build_and_run(cxx: str, sources: list[Path], ex: "Exercise",
                  name: str | None = None,
                  werror: bool = False) -> tuple[bool, str, str]:
    """Returns (passed, output, stage). stage is "compile" when the build
    itself failed, and "run" otherwise."""
    BUILD_DIR.mkdir(exist_ok=True)
    binary = BUILD_DIR / (name or ex.name)
    cmd = [cxx,
           *compile_flags(ex.mode, ex.sanitizers, ex.extra_flags, werror),
           *map(str, sources), "-o", str(binary)]
    proc = subprocess.run(cmd, capture_output=True, text=True)
    if proc.returncode != 0:
        return False, proc.stderr, "compile"
    if ex.mode == "compile":
        return True, proc.stderr, "run"  # warnings, if any
    try:
        run = subprocess.run([str(binary)], capture_output=True, text=True,
                             timeout=30)
    except subprocess.TimeoutExpired:
        return False, "(timed out after 30s — infinite loop?)", "run"
    output = run.stdout + run.stderr
    return run.returncode == 0, output, "run"


def progress_bar(done: int, total: int) -> str:
    width = 40
    filled = int(width * done / total)
    return f"[{'#' * filled}{'-' * (width - filled)}] {done}/{total}"


# ---------------------------------------------------------------- commands


def cmd_watch(exercises: list[Exercise], cxx: str,
              start_name: str | None = None) -> None:
    hint_level = 0
    current: Exercise | None = None
    last_mtime = 0.0

    def next_pending(idx: int, direction: int) -> Exercise | None:
        """Nearest pending supported exercise, scanning from idx (exclusive)
        in the given direction, with wraparound."""
        n = len(exercises)
        for step in range(1, n + 1):
            ex = exercises[(idx + direction * step) % n]
            if ex.has_marker() and ex.supported_here():
                return ex
        return None

    def show(ex: Exercise) -> bool:
        nonlocal hint_level
        passed, output, _ = build_and_run(cxx, ex.sources(), ex)
        os.system("clear" if os.name == "posix" else "cls")
        # count like cmd_list: exercises this machine cannot run are
        # not part of the total
        here = [e for e in exercises if e.supported_here()]
        done = sum(1 for e in here if not e.has_marker())
        print(progress_bar(done, len(here)))
        print(f"\n{bold('Current exercise:')} {ex.rel_path}\n")
        if passed:
            print(green("✓ Exercise compiles and passes!"))
            if output.strip():
                print(f"\n--- output ---\n{output.strip()}\n--------------")
            print(f"\nWhen you've understood it, delete the line\n"
                  f"  {yellow(MARKER)}\nfrom the file to move on.")
        else:
            print(red("✗ Not yet:"))
            print(output.strip()[:4000])
            print(f"\n{yellow('h')}+Enter = hint ({len(ex.hints)} available)   "
                  f"{yellow('n')}/{yellow('p')} = next/prev   "
                  f"{yellow('l')} = list   {yellow('q')} = quit   "
                  f"or type an exercise name")
        return passed

    had_marker = False

    def switch_to(ex: Exercise) -> None:
        nonlocal current, hint_level, last_mtime, had_marker
        current = ex
        hint_level = 0
        last_mtime = ex.dir_mtime()
        had_marker = ex.has_marker()
        show(ex)

    if start_name is not None:
        start = next((e for e in exercises if e.name == start_name), None)
        if start is None:
            sys.exit(f"error: no exercise named '{start_name}' "
                     f"(see: cpplings.py list)")
        if not (start.has_marker() and start.supported_here()):
            # done or unsupported here — begin at the next pending one after it
            start = next_pending(exercises.index(start), +1)
    else:
        start = next_pending(-1, +1)

    print(f"cpplings watch mode — compiler: {cxx}")
    while True:
        if current is None or (had_marker and not current.has_marker()):
            # first pick, or the marker was removed — move forward
            idx = exercises.index(current) if current else -1
            nxt = start if current is None else next_pending(idx, +1)
            start = None
            if nxt is None:
                # all markers removed; double-check everything actually passes
                print(green(bold("\nAll exercises done — "
                                 "running final verify...\n")))
                cmd_verify(exercises, cxx)
                return
            switch_to(nxt)
        # poll: keyboard or file change
        ready, _, _ = select.select([sys.stdin], [], [], 0.5)
        if ready:
            raw = sys.stdin.readline()
            if raw == "":
                return  # stdin is at EOF: stay ready forever → quit
            line = raw.strip().lower()
            if line == "q":
                return
            elif line == "h":
                if hint_level < len(current.hints):
                    print(f"\n{yellow(f'Hint {hint_level + 1}:')} "
                          f"{current.hints[hint_level]}")
                    hint_level += 1
                else:
                    print(yellow("No more hints. Check the solution in solutions/ "
                                 "if you're stuck."))
            elif line in ("n", "p"):
                direction = +1 if line == "n" else -1
                nxt = next_pending(exercises.index(current), direction)
                if nxt is None or nxt is current:
                    print(yellow("No other pending exercise."))
                else:
                    switch_to(nxt)
            elif line == "l":
                print()
                cmd_list(exercises)
            elif line:
                target = next((e for e in exercises if e.name == line), None)
                if target is None:
                    print(yellow(f"Unknown input '{line}'.  n/p = next/prev   "
                                 "l = list   h = hint   q = quit   "
                                 "or type an exercise name"))
                elif not target.supported_here():
                    print(yellow(f"~ {target.name} needs "
                                 f"{target.skip_reason()} — can't run here"))
                else:
                    switch_to(target)
        try:
            mtime = current.dir_mtime()
        except FileNotFoundError:
            continue
        if mtime != last_mtime:
            last_mtime = mtime
            show(current)


def cmd_run(exercises: list[Exercise], cxx: str, name: str) -> None:
    ex = next((e for e in exercises if e.name == name), None)
    if ex is None:
        sys.exit(f"error: no exercise named '{name}' (see: cpplings.py list)")
    if not ex.supported_here():
        print(yellow(f"~ {ex.name} needs {ex.skip_reason()} "
                     f"(you're on {sys.platform}/{platform.machine()}) — skipped"))
        return
    passed, output, _ = build_and_run(cxx, ex.sources(), ex)
    if output.strip():
        print(output.strip())
    if passed:
        print(green(f"✓ {ex.name} passes"))
        if ex.has_marker():
            print(f"  (remove the '{MARKER}' line to mark it done)")
    else:
        print(red(f"✗ {ex.name} fails"))
        sys.exit(1)


def cmd_hint(exercises: list[Exercise], name: str) -> None:
    ex = next((e for e in exercises if e.name == name), None)
    if ex is None:
        sys.exit(f"error: no exercise named '{name}'")
    if not ex.hints:
        print("No hints for this one.")
        return
    for i, hint in enumerate(ex.hints, 1):
        print(f"{yellow(f'Hint {i}:')} {hint}")


def cmd_list(exercises: list[Exercise]) -> None:
    width = max(len(e.name) for e in exercises) + 2
    for ex in exercises:
        if not ex.supported_here():
            status = yellow("SKIP   ") + f" (needs {ex.skip_reason()})"
        elif ex.has_marker():
            status = yellow("PENDING")
        else:
            status = green("DONE   ")
        print(f"{ex.name:<{width}} {status} {ex.rel_path}")
    here = [e for e in exercises if e.supported_here()]
    done = sum(1 for e in here if not e.has_marker())
    print(f"\n{progress_bar(done, len(here))}")


def cmd_verify(exercises: list[Exercise], cxx: str) -> None:
    for ex in exercises:
        if not ex.supported_here():
            print(f"{yellow('SKIP')} {ex.name} — needs {ex.skip_reason()}")
            continue
        if ex.has_marker():
            print(f"{yellow('PENDING')} {ex.name} — still has the marker")
            sys.exit(1)
        passed, output, _ = build_and_run(cxx, ex.sources(), ex)
        if not passed:
            print(f"{red('FAIL')} {ex.name}")
            print(output.strip()[:4000])
            sys.exit(1)
        print(f"{green('PASS')} {ex.name}")
    print(green(bold("\nAll exercises verified. 🎉")))


def cmd_check_solutions(exercises: list[Exercise], cxx: str,
                        names: list[str] | None = None) -> None:
    """CI check: every solution passes with no compiler warning; every
    shipped exercise fails, at the stage its entry names (`fails`).
    With names, checks only those exercises."""
    if names:
        known = {e.name for e in exercises}
        unknown = [n for n in names if n not in known]
        if unknown:
            sys.exit(f"error: no exercise named {', '.join(unknown)} "
                     f"(see: cpplings.py list)")
        exercises = [e for e in exercises if e.name in names]
    problems = 0
    checked = 0
    for ex in exercises:
        if not ex.supported_here():
            print(f"{yellow('SKIP')} {ex.name} — needs {ex.skip_reason()}")
            continue
        checked += 1
        missing = [s for s in ex.solution_sources() if not s.exists()]
        if missing:
            print(f"{red('MISSING')} solution file(s) for {ex.name}: "
                  f"{', '.join(str(m) for m in missing)}")
            problems += 1
            continue
        # -Werror for the solution only: a learner's build never fails on
        # a warning, but a shipped solution must be warning-free.
        sol_ok, sol_out, _ = build_and_run(cxx, ex.solution_sources(), ex,
                                           name=f"sol_{ex.name}", werror=True)
        ex_ok, _, ex_stage = build_and_run(cxx, ex.sources(), ex,
                                           name=f"ex_{ex.name}")
        if not sol_ok:
            print(f"{red('SOLUTION FAILS')} {ex.name}")
            print(sol_out.strip()[:2000])
            problems += 1
        elif ex_ok:
            print(f"{red('EXERCISE ALREADY PASSES')} {ex.name} "
                  f"(shipped exercises must fail)")
            problems += 1
        elif ex_stage != ex.fails:
            print(f"{red('EXERCISE FAILS IN WRONG PHASE')} {ex.name} "
                  f"(must fail at {ex.fails}, failed at {ex_stage})")
            problems += 1
        else:
            print(f"{green('OK')} {ex.name}")
    if problems:
        print(red(f"\n{problems} problem(s) found"))
        sys.exit(1)
    skipped = len(exercises) - checked
    note = f" ({skipped} skipped on this platform)" if skipped else ""
    print(green(f"\nAll {checked} exercises check out.{note}"))


def main() -> None:
    parser = argparse.ArgumentParser(description="cpplings exercise runner")
    sub = parser.add_subparsers(dest="command")
    p_watch = sub.add_parser("watch")
    p_watch.add_argument("name", nargs="?", default=None)
    p_run = sub.add_parser("run")
    p_run.add_argument("name")
    p_hint = sub.add_parser("hint")
    p_hint.add_argument("name")
    sub.add_parser("list")
    sub.add_parser("verify")
    p_check = sub.add_parser("check-solutions")
    p_check.add_argument("names", nargs="*")
    args = parser.parse_args()

    exercises = load_exercises()
    command = args.command or "watch"

    if command == "hint":
        cmd_hint(exercises, args.name)
        return
    if command == "list":
        cmd_list(exercises)
        return

    cxx = find_compiler()
    if command == "watch":
        cmd_watch(exercises, cxx, getattr(args, "name", None))
    elif command == "run":
        cmd_run(exercises, cxx, args.name)
    elif command == "verify":
        cmd_verify(exercises, cxx)
    elif command == "check-solutions":
        cmd_check_solutions(exercises, cxx, args.names)


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print()
