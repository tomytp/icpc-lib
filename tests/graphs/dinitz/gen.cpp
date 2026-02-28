// Test: graphs/dinitz
//
// Input:
//   N M S T   (N nodes 0..N-1, M directed edges, source S, sink T)
//   M lines: u v c  (edge u->v with capacity c)
//
// Output:
//   F         (max flow)
//   K         (number of min-cut edges)
//   K lines: u v

#include "src/extra/template.cpp"
#include "src/extra/random.cpp"

int main(int argc, char* argv[]) {
    rng.seed(atoll(argv[1]));
    ll n = uniform(2, 8);

    vector<p64> cands;
    forn(u, 0, n) forn(v, 0, n) if (u != v) cands.push_back({u, v});
    shuffle(cands.begin(), cands.end(), rng);
    ll m = uniform(0, min(sz(cands), 15ll));
    cands.resize(m);

    cout << n << " " << m << " " << 0 << " " << n-1 << ln;
    for (auto [u, v] : cands)
        cout << u << " " << v << " " << uniform(1, 10) << ln;
}