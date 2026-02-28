// Test: graphs/dijkstra
//
// Input:
//   N M S   (N vertices 0..N-1, M directed edges, source S)
//   M lines: u v w  (edge u→v with non-negative weight w)
//
// Output:
//   N space-separated distances from S; -1 if unreachable

#include "src/extra/template.cpp"
#include "src/extra/random.cpp"

int main(int argc, char* argv[]) {
    rng.seed(atoll(argv[1]));
    ll n = uniform(2, 12);
    ll s = uniform(0, n - 1);

    vector<p64> cands;
    forn(u, 0, n) forn(v, 0, n) if (u != v) cands.push_back({u, v});
    shuffle(cands.begin(), cands.end(), rng);

    ll m = uniform(0, min(sz(cands), 20ll));
    cands.resize(m);

    cout << n << " " << m << " " << s << ln;
    for (auto [u, v] : cands)
        cout << u << " " << v << " " << uniform(0, 100) << ln;
}