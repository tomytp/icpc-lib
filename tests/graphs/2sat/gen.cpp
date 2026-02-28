// Test: graphs/2sat
//
// Input:
//   N M   (N boolean variables 0..N-1, M clauses)
//   M lines: a b   (clause "a OR b")
//     Literal encoding (1-indexed): +k means x_{k-1}=true, -k means x_{k-1}=false
//
// Output:
//   "NO"  if unsatisfiable
//   "YES" followed by N space-separated 0/1 values if satisfiable

#include "src/extra/template.cpp"
#include "src/extra/random.cpp"

int main(int argc, char* argv[]) {
    rng.seed(atoll(argv[1]));
    // Keep N small so the checker's 2^N brute-force stays fast.
    ll n = uniform(1, 12);
    ll m = uniform(1, min(n * n, 20LL));
    cout << n << " " << m << ln;
    forn(i, 0, m) {
        ll a = uniform(1, n); if (uniform(0, 1)) a = -a;
        ll b = uniform(1, n); if (uniform(0, 1)) b = -b;
        cout << a << " " << b << ln;
    }
}