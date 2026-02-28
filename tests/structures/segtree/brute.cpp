#include "src/extra/template.cpp"

int main() {
    _;
    ll n, q; cin >> n >> q;
    v64 a(n);
    forn(i, 0, n) cin >> a[i];
    while (q--) {
        ll t; cin >> t;
        if (t == 1) {
            ll i, v; cin >> i >> v;
            a[i] = v;
        } else {
            ll l, r; cin >> l >> r;
            ll s = 0;
            forn(i, l, r+1) s += a[i];
            cout << s << ln;
        }
    }
}
