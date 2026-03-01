// Brute-force reference for structures/perseg

#include "src/extra/template.cpp"

int main() {
    _;
    ll n, q; cin >> n >> q;
    vector<v64> vers(1, v64(n));
    forn(i, 0, n) cin >> vers[0][i];
    while (q--) {
        ll t; cin >> t;
        if (t == 1) {
            ll ver, idx, val; cin >> ver >> idx >> val;
            v64 nv = vers[ver];
            nv[idx] = val;
            vers.push_back(nv);
        } else {
            ll ver, l, r; cin >> ver >> l >> r;
            ll s = 0;
            forn(i, l, r + 1) s += vers[ver][i];
            cout << s << ln;
        }
    }
}
