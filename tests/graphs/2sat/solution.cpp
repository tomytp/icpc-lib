#include "src/extra/template.cpp"
#include "src/graphs/2sat.cpp"

// Convert 1-indexed literal to the 0-indexed form expected by sat::add_cl:
//   +k (k>0) → k-1   (variable k-1 is true)
//   -k (k<0) → k     (NOT variable |k|-1, since ~(|k|-1) = -(|k|-1)-1 = k)
static inline ll lit(ll x) { return x > 0 ? x - 1 : x; }

int main() {
    _;
    ll n, m; cin >> n >> m;
    sat s(n);
    while (m--) {
        ll a, b; cin >> a >> b;
        s.add_cl(lit(a), lit(b));
    }
    auto [ok, ans] = s.solve();
    if (!ok) {
        cout << "NO" << ln;
    } else {
        cout << "YES" << ln;
        forn(i, 0, n) cout << ans[i] << " \n"[i == n - 1];
    }
}
