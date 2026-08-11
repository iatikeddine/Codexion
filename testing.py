#!/usr/bin/env python3
"""
Runs `./codexion 10 600 200 100 100 10 100 fifo` in a loop (10 iterations),
scans the (noisy) stdout/stderr for lines containing "burned out"
(case-insensitive), and reports how many times a burnout occurred
overall, plus a per-run breakdown.

Usage:
    python3 run_codexion_tests.py
    python3 run_codexion_tests.py --runs 20 --binary ./codexion
"""

import argparse
import subprocess
import sys

DEFAULT_ARGS = ["200", "1410", "100", "100", "100", "10", "600", "fifo"]
BURNOUT_NEEDLE = "burned out"  # matched case-insensitively


def run_once(binary, args, run_index, verbose):
    """Run the binary once, return the number of 'burned out' lines found."""
    cmd = [binary] + args
    print(f"\n=== Run {run_index}: {' '.join(cmd)} ===")

    burnout_count = 0

    try:
        # Merge stderr into stdout so we don't miss logs printed on either stream.
        proc = subprocess.Popen(
            cmd,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            bufsize=1,  # line-buffered
        )
    except FileNotFoundError:
        print(f"ERROR: could not find/execute '{binary}'. Check the path.")
        sys.exit(1)

    # Stream line by line so huge log volume doesn't blow up memory,
    # and so we can print progress as it happens.
    for line in proc.stdout:
        line = line.rstrip("\n")
        if BURNOUT_NEEDLE in line.lower():
            burnout_count += 1
            print(f"  [BURNOUT DETECTED] {line}")
        elif verbose:
            print(f"  {line}")

    proc.wait()

    if proc.returncode != 0:
        print(f"  (note: run {run_index} exited with code {proc.returncode})")

    print(f"=== Run {run_index} finished: {burnout_count} burnout(s) ===")
    return burnout_count


def main():
    parser = argparse.ArgumentParser(description="Run codexion test suite repeatedly and count burnouts.")
    parser.add_argument("--binary", default="./codexion", help="Path to the codexion binary (default: ./codexion)")
    parser.add_argument("--args", nargs="*", default=DEFAULT_ARGS,
                         help="Arguments passed to codexion (default: 10 600 200 100 100 10 100 fifo)")
    parser.add_argument("--runs", type=int, default=10, help="Number of loop iterations (default: 10)")
    parser.add_argument("--verbose", action="store_true",
                         help="Print every log line, not just burnout lines")
    args = parser.parse_args()

    total_burnouts = 0
    per_run_counts = []

    for i in range(1, args.runs + 1):
        count = run_once(args.binary, args.args, i, args.verbose)
        per_run_counts.append(count)
        total_burnouts += count

    print("\n" + "=" * 40)
    print("SUMMARY")
    print("=" * 40)
    for i, count in enumerate(per_run_counts, start=1):
        print(f"  Run {i:2d}: {count} burnout(s)")
    print("-" * 40)
    print(f"Total burnouts across {args.runs} runs: {total_burnouts}")


if __name__ == "__main__":
    main()
