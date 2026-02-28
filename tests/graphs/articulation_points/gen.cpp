// Test: graphs/articulation_points
//
// Input:
//   N M
//   M lines: u v  (undirected edge, 0-indexed)
//
// Output:
//   K  (number of articulation points)
//   K lines: v  (sorted)

#include "src/extra/template.cpp"
#include "src/extra/random.cpp"

int main(int argc, char* argv[]) {
    rng.seed(atoll(argv[1]));
    ll n = uniform(2, 12);

    vector<p64> cands;
    forn(u, 0, n) forn(v, u+1, n) cands.push_back({u, v});
    shuffle(cands.begin(), cands.end(), rng);

    ll m = uniform(0, min(sz(cands), 20ll));
    cands.resize(m);

    cout << n << " " << m << ln;
    for (auto [u, v] : cands)
        cout << u << " " << v << ln;
}
