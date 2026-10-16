// Fenwick Tree (Binary Indexed Tree)
//
// Update pontual e query de prefixo/intervalo em O(log n). Interface 0-based,
// internamente 1-based. T e um grupo abeliano (mesmo Node da BitRange:
// T() identidade, a + b, a - b). Se so usar pref, basta monoide comutativo.
// upper_bound so faz sentido se T tem ordem e todo elemento e >= identidade
// (ex: somas de valores nao negativos), pois precisa de prefixos monotonos.
//
// complexity: O(log n) por op, O(n) build

template <class T = ll>
struct Bit {
    ll n;
    vector<T> bit;
    Bit(ll _n = 0) : n(_n), bit(n + 1) {}
    Bit(vector<T>& v) : Bit(sz(v)) {
        forn(i,1,n+1) {
            bit[i] = bit[i] + v[i - 1];
            ll j = i + (i & -i);
            if (j <= n) bit[j] = bit[j] + bit[i];
        }
    }
    void update(ll i, const T& x) {  // soma x na posicao i
        for (i++; i <= n; i += i & -i) bit[i] = bit[i] + x;
    }
    T pref(ll i) {  // combina [0, i-1]
        T r{};
        for (; i; i -= i & -i) r = r + bit[i];
        return r;
    }
    T query(ll l, ll r) { return pref(r + 1) - pref(l); }  // combina [l, r]
    // menor p (0-based) com pref(p + 1) > x, ou n se nao existir
    // = quantos elementos do comeco cabem com soma <= x
    ll upper_bound(T x) {
        ll p = 0;
        for (ll k = n ? __lg(n) : -1; k >= 0; k--)
            if (p + (1LL << k) <= n && bit[p + (1LL << k)] <= x)
                x = x - bit[p += 1LL << k];
        return p;
    }
};