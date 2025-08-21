// Sparse Table (Idempotent Range Query)
//
// Preprocesses static array to answer idempotent range queries (e.g., min/max) in O(1) after O(N log N) build.
//
// complexity: O(N log N) build, O(1) query; O(N log N) space

ll m[MAXN][MAXLOGN];

void build(v64& v) {
    ll sz = v.size();
    
    forn(i, 0, sz) {
        m[i][0] = v[i];
    }
    
    for (ll j = 1; (1 << j) <= sz; j++) {
        for (ll i = 0; i + (1 << j) <= sz; i++) {
            m[i][j] = max(m[i][j-1], m[i + (1 << (j-1))][j-1]);
        }
    }
}

ll query(ll a, ll b) {
    ll j = __builtin_clzll(1) - __builtin_clzll(b - a + 1);
    return max(m[a][j], m[b - (1 << j) + 1][j]);
}
