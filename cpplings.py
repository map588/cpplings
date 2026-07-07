#!/usr/bin/env python3
"""cpplings — a rustlings-style C++ exercise runner.

Usage:
  python3 cpplings.py              # watch mode (default): work through exercises
  python3 cpplings.py run NAME     # build & run one exercise
  python3 cpplings.py hint NAME    # show hints for one exercise
  python3 cpplings.py list         # show all exercises and their status
  python3 cpplings.py verify       # check all exercises in order, stop at first failure
  python3 cpplings.py check-solutions  # CI: solutions must pass, shipped exercises must fail

Environment:
  CPPLINGS_CXX            override compiler (default: clang++, then g++)
  CPPLINGS_NO_SANITIZERS  set to disable ASan/UBSan (some platforms misbehave)

Requires Python 3.11+ (tomllib) and a C++20 compiler.
"""

import argparse
import os
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

    def supported_here(self) -> bool:
        return (not self.platforms
                or any(sys.platform.startswith(p) for p in self.platforms))

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


def compile_flags(mode: str, sanitizers: str = "address") -> list[str]:
    flags = list(BASE_FLAGS)
    if mode != "run" or os.environ.get("CPPLINGS_NO_SANITIZERS"):
        return flags
    if sanitizers == "thread":
        flags += ["-fsanitize=thread"]
    elif sanitizers == "address":
        flags += SANITIZER_FLAGS
    return flags


def build_and_run(cxx: str, sources: list[Path], name: str, mode: str,
                  sanitizers: str = "address") -> tuple[bool, str]:
    """Returns (passed, output)."""
    BUILD_DIR.mkdir(exist_ok=True)
    binary = BUILD_DIR / name
    cmd = [cxx, *compile_flags(mode, sanitizers), *map(str, sources),
           "-o", str(binary)]
    proc = subprocess.run(cmd, capture_output=True, text=True)
    if proc.returncode != 0:
        return False, proc.stderr
    if mode == "compile":
        return True, proc.stderr  # warnings, if any
    try:
        run = subprocess.run([str(binary)], capture_output=True, text=True,
                             timeout=30)
    except subprocess.TimeoutExpired:
        return False, "(timed out after 30s — infinite loop?)"
    output = run.stdout + run.stderr
    return run.returncode == 0, output


def progress_bar(done: int, total: int) -> str:
    width = 40
    filled = int(width * done / total)
    return f"[{'#' * filled}{'-' * (width - filled)}] {done}/{total}"


# ---------------------------------------------------------------- commands


def cmd_watch(exercises: list[Exercise], cxx: str) -> None:
    hint_level = 0
    current: Exercise | None = None
    last_mtime = 0.0

    def pick_current() -> Exercise | None:
        for ex in exercises:
            if ex.has_marker() and ex.supported_here():
                return ex
        return None

    def show(ex: Exercise) -> bool:
        nonlocal hint_level
        passed, output = build_and_run(cxx, ex.sources(), ex.name, ex.mode, ex.sanitizers)
        os.system("clear" if os.name == "posix" else "cls")
        done = sum(1 for e in exercises if not e.has_marker())
        print(progress_bar(done, len(exercises)))
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
                  f"{yellow('q')}+Enter = quit")
        return passed

    print(f"cpplings watch mode — compiler: {cxx}")
    while True:
        ex = pick_current()
        if ex is None:
            # all markers removed; double-check everything actually passes
            print(green(bold("\nAll exercises done — running final verify...\n")))
            cmd_verify(exercises, cxx)
            return
        if ex is not current:
            current = ex
            hint_level = 0
            last_mtime = ex.dir_mtime()
            show(ex)
        # poll: keyboard or file change
        ready, _, _ = select.select([sys.stdin], [], [], 0.5)
        if ready:
            line = sys.stdin.readline().strip().lower()
            if line == "q":
                return
            if line == "h":
                if hint_level < len(current.hints):
                    print(f"\n{yellow(f'Hint {hint_level + 1}:')} "
                          f"{current.hints[hint_level]}")
                    hint_level += 1
                else:
                    print(yellow("No more hints. Check the solution in solutions/ "
                                 "if you're stuck."))
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
        print(yellow(f"~ {ex.name} needs {'/'.join(ex.platforms)} "
                     f"(you're on {sys.platform}) — skipped"))
        return
    passed, output = build_and_run(cxx, ex.sources(), ex.name, ex.mode, ex.sanitizers)
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
            status = yellow("SKIP   ") + f" (needs {'/'.join(ex.platforms)})"
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
            print(f"{yellow('SKIP')} {ex.name} — needs {'/'.join(ex.platforms)}")
            continue
        if ex.has_marker():
            print(f"{yellow('PENDING')} {ex.name} — still has the marker")
            sys.exit(1)
        passed, output = build_and_run(cxx, ex.sources(), ex.name, ex.mode, ex.sanitizers)
        if not passed:
            print(f"{red('FAIL')} {ex.name}")
            print(output.strip()[:4000])
            sys.exit(1)
        print(f"{green('PASS')} {ex.name}")
    print(green(bold("\nAll exercises verified. 🎉")))


def cmd_check_solutions(exercises: list[Exercise], cxx: str) -> None:
    """CI check: every solution passes; every shipped exercise fails."""
    problems = 0
    checked = 0
    for ex in exercises:
        if not ex.supported_here():
            print(f"{yellow('SKIP')} {ex.name} — needs {'/'.join(ex.platforms)}")
            continue
        checked += 1
        missing = [s for s in ex.solution_sources() if not s.exists()]
        if missing:
            print(f"{red('MISSING')} solution file(s) for {ex.name}: "
                  f"{', '.join(str(m) for m in missing)}")
            problems += 1
            continue
        sol_ok, sol_out = build_and_run(cxx, ex.solution_sources(),
                                        f"sol_{ex.name}", ex.mode,
                                        ex.sanitizers)
        ex_ok, _ = build_and_run(cxx, ex.sources(), f"ex_{ex.name}", ex.mode,
                                 ex.sanitizers)
        if not sol_ok:
            print(f"{red('SOLUTION FAILS')} {ex.name}")
            print(sol_out.strip()[:2000])
            problems += 1
        elif ex_ok:
            print(f"{red('EXERCISE ALREADY PASSES')} {ex.name} "
                  f"(shipped exercises must fail)")
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
    sub.add_parser("watch")
    p_run = sub.add_parser("run")
    p_run.add_argument("name")
    p_hint = sub.add_parser("hint")
    p_hint.add_argument("name")
    sub.add_parser("list")
    sub.add_parser("verify")
    sub.add_parser("check-solutions")
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
        cmd_watch(exercises, cxx)
    elif command == "run":
        cmd_run(exercises, cxx, args.name)
    elif command == "verify":
        cmd_verify(exercises, cxx)
    elif command == "check-solutions":
        cmd_check_solutions(exercises, cxx)


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print()
