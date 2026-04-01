// Test: structures/perseg (persistent segment tree)

#include "src/extra/template.cpp"
#include "src/structures/perseg.cpp"

int main() {
    _;
    ll n, q; cin >> n >> q;
    vll a(n);
    forn(i, 0, n) cin >> a[i];
    perseg ps(a, q);
    while (q--) {
        ll t; cin >> t;
        if (t == 1) {
            ll ver, idx, val; cin >> ver >> idx >> val;
            ps.set(idx, val, ver);
        } else {
            ll ver, l, r; cin >> ver >> l >> r;
            cout << ps.query(l, r, ver) << ln;
        }
    }
}
