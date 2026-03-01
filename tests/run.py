#!/usr/bin/env python3
"""icpc-lib stress test runner.

Usage:
  python tests/run.py                        # run all tests
  python tests/run.py structures/dsu         # run specific test
  python tests/run.py -n 500                 # number of iterations (default: 200)
  python tests/run.py --new graphs/scc       # scaffold new test directory
  python tests/run.py --list                 # list all discovered tests

Each test directory under tests/ must contain:
  solution.cpp   - wraps the lib code with I/O
  gen.cpp        - random test-case generator (uses testlib.h)
  brute.cpp      - naive reference solution (for exact-output comparison)
  OR
  checker.cpp    - output validator (for problems with multiple valid answers)

Run modes:
  brute:    gen -> solution vs brute, compare outputs
  checker:  gen -> solution, checker validates output
"""

from pathlib import Path
import argparse, subprocess, sys, os, concurrent.futures, io

TESTS_DIR = Path(__file__).parent
ROOT_DIR = TESTS_DIR.parent

CXX = "g++"
CXXFLAGS = ["-std=c++20", "-O2", f"-I{ROOT_DIR}"]

# ANSI colors
R    = "\033[31m"
G    = "\033[32m"
Y    = "\033[33m"
BOLD = "\033[1m"
RST  = "\033[0m"

def red(t):    return f"{R}{t}{RST}"
def green(t):  return f"{G}{t}{RST}"
def yellow(t): return f"{Y}{t}{RST}"
def bold(t):   return f"{BOLD}{t}{RST}"


# ─── Compilation ──────────────────────────────────────────────────────────────

def compile_file(src: Path, out: Path, out_file=sys.stdout) -> bool:
    r = subprocess.run(
        [CXX] + CXXFLAGS + [str(src), "-o", str(out)],
        capture_output=True, text=True,
    )
    if r.returncode != 0:
        print(red(f"  Compile error in {src.relative_to(ROOT_DIR)}:"), file=out_file)
        print(r.stderr, file=out_file)
        return False
    return True


# ─── Test execution ───────────────────────────────────────────────────────────

def run_proc(args, stdin=None, timeout=5, pass_fds=()):
    try:
        return subprocess.run(
            args, input=stdin, capture_output=True, text=True, timeout=timeout,
            pass_fds=pass_fds,
        )
    except subprocess.TimeoutExpired:
        return None


def run_test(test_dir: Path, n_iters: int, keep_bins: bool = False, out_file=sys.stdout) -> bool:
    name = str(test_dir.relative_to(TESTS_DIR))
    print(f"\n{bold('=== ' + name + ' ===')}", file=out_file)

    sol_src     = test_dir / "solution.cpp"
    gen_src     = test_dir / "gen.cpp"
    brute_src   = test_dir / "brute.cpp"
    checker_src = test_dir / "checker.cpp"

    if not sol_src.exists() or not gen_src.exists():
        print(yellow("  Skipping: missing solution.cpp or gen.cpp"), file=out_file)
        return True

    sol_bin     = test_dir / ".solution"
    gen_bin     = test_dir / ".gen"
    brute_bin   = test_dir / ".brute"
    checker_bin = test_dir / ".checker"
    bins = [sol_bin, gen_bin]

    has_brute   = brute_src.exists()
    has_checker = checker_src.exists()

    compile_tasks = [(sol_src, sol_bin), (gen_src, gen_bin)]
    if has_brute:
        compile_tasks.append((brute_src, brute_bin))
        bins.append(brute_bin)
    if has_checker:
        compile_tasks.append((checker_src, checker_bin))
        bins.append(checker_bin)

    with concurrent.futures.ThreadPoolExecutor() as executor:
        futures = {executor.submit(compile_file, src, out, out_file): src for src, out in compile_tasks}
        for future in concurrent.futures.as_completed(futures):
            if not future.result():
                return False

    if not has_brute and not has_checker:
        print(yellow("  Warning: no brute.cpp or checker.cpp — only tested compilation."), file=out_file)

    # ── Stress loop ────────────────────────────────────────────────────────────
    def run_seed(seed):
        # 1. Generate input
        gen_r = run_proc([str(gen_bin), str(seed)], timeout=5)
        if gen_r is None:
            return {'failed': True, 'msg': f"Generator TLE at seed {seed}"}
        test_input = gen_r.stdout

        # 2. Run solution
        sol_r = run_proc([str(sol_bin)], stdin=test_input, timeout=5)
        if sol_r is None:
            return {'failed': True, 'seed': seed, 'input': test_input, 'msg': f"Solution TLE at seed {seed}"}
        if sol_r.returncode != 0:
            return {'failed': True, 'seed': seed, 'input': test_input, 'msg': sol_r.stderr}

        # 3a. Brute comparison
        if has_brute:
            brute_r = run_proc([str(brute_bin)], stdin=test_input, timeout=10)
            if brute_r is None:
                return {'failed': True, 'msg': f"Brute TLE at seed {seed}"}
            if sol_r.stdout.strip() != brute_r.stdout.strip():
                return {'failed': True, 'seed': seed, 'input': test_input, 'expected': brute_r.stdout, 'got': sol_r.stdout}

        # 3b. Checker validation
        elif has_checker:
            inp_r, inp_w = os.pipe()
            out_r, out_w = os.pipe()
            os.write(inp_w, test_input.encode()); os.close(inp_w)
            os.write(out_w, sol_r.stdout.encode()); os.close(out_w)
            check_r = run_proc(
                [str(checker_bin), f"/dev/fd/{inp_r}", "/dev/null", f"/dev/fd/{out_r}"],
                pass_fds=(inp_r, out_r), timeout=10,
            )
            os.close(inp_r); os.close(out_r)

            if check_r is None or check_r.returncode != 0:
                return {'failed': True, 'seed': seed, 'input': test_input, 'got': sol_r.stdout, 'msg': (check_r.stderr if check_r else "TLE")}
        
        return {'failed': False}

    failed     = False
    fail_seed  = None
    fail_input = fail_expected = fail_got = fail_msg = None

    completed = 0
    with concurrent.futures.ThreadPoolExecutor() as executor:
        futures = {executor.submit(run_seed, seed): seed for seed in range(1, n_iters + 1)}
        for future in concurrent.futures.as_completed(futures):
            res = future.result()
            if res['failed']:
                failed = True
                fail_seed = res.get('seed', futures[future])
                fail_input = res.get('input')
                fail_expected = res.get('expected')
                fail_got = res.get('got')
                fail_msg = res.get('msg')
                for f in futures:
                    f.cancel()
                break
            completed += 1
            if out_file == sys.stdout and (completed % 50 == 0 or completed == n_iters):
                print(f"  {green('OK')} {completed}/{n_iters}", end="\r", file=out_file)

    # ── Cleanup ────────────────────────────────────────────────────────────────
    if not keep_bins:
        for b in bins:
            if b.exists(): b.unlink()

    # ── Report ─────────────────────────────────────────────────────────────────
    if failed:
        print(f"\n  {red('FAILED')} at seed {fail_seed}", file=out_file)
        if fail_input:
            print(f"\n  Input:\n{fail_input.rstrip()}", file=out_file)
        if fail_expected:
            print(f"\n  Expected (brute):\n{fail_expected.rstrip()}", file=out_file)
        if fail_got:
            print(f"\n  Got:\n{fail_got.rstrip()}", file=out_file)
        if fail_msg:
            print(f"\n  {red('Error:')} {fail_msg.rstrip()}", file=out_file)
        return False

    if out_file != sys.stdout:
        print(f"  {green('OK')} {completed}/{n_iters}", file=out_file)
    print(f"\n  {green('PASSED')} ({n_iters} iterations)", file=out_file)
    return True


# ─── Scaffold ─────────────────────────────────────────────────────────────────

SCAFFOLD_SOLUTION = """\
// Test: {category}/{problem}
//
// Input format:
//   ...
//
// Output format:
//   ...

#include "src/extra/template.cpp"
#include "src/{category}/{problem}.cpp"

int main() {{
    _;
    // TODO
}}
"""

SCAFFOLD_GEN = """\
#include "src/extra/template.cpp"
#include "src/extra/random.cpp"

int main(int argc, char* argv[]) {{
    rng.seed(atoll(argv[1]));
    // TODO: generate a random test case
    // ll n = uniform(1, 20);
    // cout << n << ln;
}}
"""

SCAFFOLD_BRUTE = """\
// Brute-force reference for {category}/{problem}
// Must produce the same output as solution.cpp for any valid input.

#include "src/extra/template.cpp"

int main() {{
    _;
    // TODO: implement brute-force solution
}}
"""

SCAFFOLD_CHECKER = """\
// Checker for {category}/{problem}
// Called as: ./checker <input_file> <ignored> <output_file>
// Return 0 = OK, nonzero = WA/PE

#include "src/extra/template.cpp"

int main(int argc, char* argv[]) {{
    ifstream inf(argv[1]), ouf(argv[3]);
    // TODO: read inf and ouf, verify correctness
    // cerr << "WA: reason\\n"; return 1;
    return 0;
}}
"""


def scaffold_new(test_path: str):
    parts = test_path.strip("/").split("/")
    if len(parts) != 2:
        print(red("Error: path must be CATEGORY/PROBLEM (e.g. graphs/scc)"))
        sys.exit(1)
    category, problem = parts
    test_dir = TESTS_DIR / category / problem
    if test_dir.exists():
        print(yellow(f"Already exists: {test_dir}"))
        sys.exit(1)
    test_dir.mkdir(parents=True)

    (test_dir / "solution.cpp").write_text(
        SCAFFOLD_SOLUTION.format(category=category, problem=problem)
    )
    (test_dir / "gen.cpp").write_text(SCAFFOLD_GEN)
    (test_dir / "brute.cpp").write_text(
        SCAFFOLD_BRUTE.format(category=category, problem=problem)
    )

    print(green(f"Scaffolded: {test_dir.relative_to(ROOT_DIR)}/"))
    print(f"  Files: solution.cpp, gen.cpp, brute.cpp")
    print(f"  Tip:   replace brute.cpp with checker.cpp for non-exact outputs")
    print(f"  Run:   python tests/run.py {test_path}")


# ─── Discovery ────────────────────────────────────────────────────────────────

def discover_tests() -> list[Path]:
    return sorted(p.parent for p in TESTS_DIR.rglob("solution.cpp"))


# ─── Main ─────────────────────────────────────────────────────────────────────

def main():
    parser = argparse.ArgumentParser(
        description="icpc-lib stress test runner",
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    parser.add_argument(
        "test", nargs="?",
        help="Test to run, e.g. structures/dsu  (default: run all)"
    )
    parser.add_argument(
        "-n", "--iters", type=int, default=1000,
        help="Stress test iterations (default: 1000)"
    )
    parser.add_argument(
        "--new", metavar="CATEGORY/PROBLEM",
        help="Scaffold a new test directory"
    )
    parser.add_argument(
        "--list", action="store_true",
        help="List all discovered tests and exit"
    )
    parser.add_argument(
        "--keep-bins", action="store_true",
        help="Keep compiled binaries after running"
    )
    args = parser.parse_args()

    if args.new:
        scaffold_new(args.new)
        return

    tests = discover_tests()

    if args.list:
        print(f"Tests in {TESTS_DIR.relative_to(ROOT_DIR)}/:")
        for t in tests:
            print(f"  {t.relative_to(TESTS_DIR)}")
        return

    if args.test:
        target = TESTS_DIR / args.test
        if not target.is_dir():
            print(red(f"Not found: {target}"))
            sys.exit(1)
        ok = run_test(target, args.iters, args.keep_bins)
    else:
        print(f"Found {bold(str(len(tests)))} test(s)")
        ok = True
        
        def run_test_captured(t):
            buf = io.StringIO()
            res = run_test(t, args.iters, args.keep_bins, out_file=buf)
            return res, buf.getvalue()

        with concurrent.futures.ThreadPoolExecutor(max_workers=os.cpu_count()) as executor:
            futures = {executor.submit(run_test_captured, t): t for t in tests}
            for future in concurrent.futures.as_completed(futures):
                res, out = future.result()
                sys.stdout.write(out)
                sys.stdout.flush()
                ok = ok and res

    print()
    if ok:
        print(bold(green("All tests passed!")))
    else:
        print(bold(red("Some tests FAILED.")))
        sys.exit(1)


if __name__ == "__main__":
    main()
