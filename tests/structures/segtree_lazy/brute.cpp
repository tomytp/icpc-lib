// Brute-force reference for structures/segtree_lazy

#include "src/extra/template.cpp"

int main() {
    _;
    ll n, q; cin >> n >> q;
    vll a(n);
    forn(i, 0, n) cin >> a[i];
    while (q--) {
        ll t; cin >> t;
        if (t == 1) {
            ll l, r, v; cin >> l >> r >> v;
            forn(i, l, r + 1) a[i] += v;
        } else if (t == 2) {
            ll l, r, v; cin >> l >> r >> v;
            forn(i, l, r + 1) a[i] = v;
        } else {
            ll l, r; cin >> l >> r;
            ll s = 0;
            forn(i, l, r + 1) s += a[i];
            cout << s << ln;
        }
    }
}
