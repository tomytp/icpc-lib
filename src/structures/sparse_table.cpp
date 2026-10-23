// Sparse Table (Idempotent Range Query)
//
// Preprocesses static array to answer idempotent range queries (e.g., min/max) in O(1) after O(N log N) build.
//
// complexity: O(N log N) build, O(1) query; O(N log N) space

struct sparse {
    vector<vll> m;
    sparse(vll& v) : m(1, v) {
        ll n = sz(v);
        for (ll j = 1; (1LL << j) <= n; j++) {
            m.emplace_back(n - (1LL << j) + 1);
            forn(i, 0, sz(m[j]))
                m[j][i] = min(m[j-1][i], m[j-1][i + (1LL << (j-1))]);
        }
    }
    ll query(ll a, ll b) {  // min de [a, b], exige a <= b
        ll j = __lg(b - a + 1);
        return min(m[j][a], m[j][b - (1LL << j) + 1]);
    }
};