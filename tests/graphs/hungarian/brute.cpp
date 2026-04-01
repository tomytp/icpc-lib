// Brute-force reference for graphs/weighted_matching
// Tries all n! permutations and returns the minimum cost.

#include "src/extra/template.cpp"

int main() {
    _;
    ll n; cin >> n;
    vector<vll> a(n, vll(n));
    forn(i, 0, n) forn(j, 0, n) cin >> a[i][j];

    vll perm(n);
    iota(perm.begin(), perm.end(), 0);
    ll best = INF;
    do {
        ll cost = 0;
        forn(i, 0, n) cost += a[i][perm[i]];
        best = min(best, cost);
    } while (next_permutation(perm.begin(), perm.end()));

    cout << best << ln;
}
