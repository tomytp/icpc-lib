// Sparse Table (Idempotent Range Query)
//
// Preprocesses static array to answer idempotent range queries (e.g., min/max) in O(1) after O(N log N) build.
//
// complexity: O(N log N) build, O(1) query; O(N log N) space

struct sparse {
    vector<v64> m;
    
    sparse(v64& v) {
        ll n = sz(v);
        ll logn = 64 - __builtin_clzll(n - 1);
        m.resize(n+1, v64(logn+1));

        forn(i, 0, n) m[i][0] = v[i];

        for (ll j = 1; (1 << j) <= n; j++)
        for (ll i = 0; i + (1 << j) <= n; i++)
            m[i][j] = min(m[i][j-1], m[i + (1 << (j-1))][j-1]);    
    }
    
    ll query(ll a, ll b) { 
        ll j = __builtin_clzll(1) - __builtin_clzll(b - a + 1);
        return min(m[a][j], m[b - (1 << j) + 1][j]);
    }
};