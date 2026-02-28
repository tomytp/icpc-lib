#include "src/extra/template.cpp"
#include "src/graphs/dinitz.cpp"

int main() {
    _;
    ll n, m, s, t; cin >> n >> m >> s >> t;
    dinitz d(n, true);
    while (m--) {
        ll u, v, c; cin >> u >> v >> c;
        d.add(u, v, c);
    }
    ll f = d.max_flow(s, t);
    auto cuts = d.min_cut();
    cout << f << ln;
    cout << sz(cuts) << ln;
    for (auto [u, v] : cuts) cout << u << " " << v << ln;
}