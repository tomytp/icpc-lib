// Fenwick Tree with Range Updates
//
// complexity: O(log n) por op, O(n) build

struct Node {
    ll v = 0;
    Node() {}
    //   Node() -> identidade
    Node(ll x) : v(x) {}
    Node operator+(const Node& o) const { return Node(v ^ o.v); }
    //   a + b -> operacao do grupo (associativa, comutativa)
    Node operator-() const { return *this; }             
    //   -a -> inverso
    Node operator-(const Node& o) const { return Node(v ^ o.v); }
    //   a - b -> a + (-b)
    Node operator*(ll k) const { return Node((k & 1) ? v : 0); }
    //   a * k (k: ll) -> a aplicado k vezes  (acao escalar, k e indice)
};

template <class T = ll>
struct BitRange {
    ll n;
    vector<array<T, 2>> bit;  // {mul, add}
    BitRange(ll _n = 0) : n(_n), bit(n + 2) {}
    BitRange(vector<T>& v) : BitRange(sz(v)) {
        forn(i,1,n+1) {
            bit[i][1] = bit[i][1] + v[i - 1];
            ll j = i + (i & -i);
            if (j <= n) bit[j][1] = bit[j][1] + bit[i][1];
        }
    }
    void add(ll i, const T& m, const T& a) {
        for (; i <= n; i += i & -i)
            bit[i][0] = bit[i][0] + m, bit[i][1] = bit[i][1] + a;
    }
    T pref(ll i) {  // combina [0, i-1]
        T m{}, a{};
        for (ll j = i; j; j -= j & -j)
            m = m + bit[j][0], a = a + bit[j][1];
        return m * i + a;
    }
    void update(ll l, ll r, const T& x) {  // aplica x em [l, r]
        add(l + 1, x, -(x * l));
        add(r + 2, -x, x * (r + 1));
    }
    T query(ll l, ll r) { return pref(r + 1) - pref(l); }
};
