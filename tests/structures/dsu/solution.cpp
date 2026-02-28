#include "src/extra/template.cpp"
#include "src/structures/dsu.cpp"

int main() {
    _;
    ll n, q; cin >> n >> q;
    dsu d(n);
    while (q--) {
        ll t, u, v; cin >> t >> u >> v;
        if (t == 1) d.uni(u, v);
        else cout << (d.find(u) == d.find(v) ? "Yes" : "No") << ln;
    }
}
