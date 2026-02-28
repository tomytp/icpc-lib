// Brute-force reference for graphs/articulation_points
// For each vertex v, remove it and count connected components.
// If cc increases, v is an articulation point.

#include "src/extra/template.cpp"

int main() {
    _;
    ll n, m; cin >> n >> m;
    vector<p64> edges(m);
    for (auto& [u, v] : edges) cin >> u >> v;

    auto count_cc = [&](ll skip_v) -> ll {
        v64 vis(n, 0);
        ll comp = 0;
        forn(s, 0, n) {
            if (s == skip_v || vis[s]) continue;
            vis[s] = 1; comp++;
            queue<ll> q; q.push(s);
            while (!q.empty()) {
                ll v = q.front(); q.pop();
                for (auto [u, w] : edges) {
                    if (u == skip_v || w == skip_v) continue;
                    if (u == v && !vis[w]) { vis[w] = 1; q.push(w); }
                    if (w == v && !vis[u]) { vis[u] = 1; q.push(u); }
                }
            }
        }
        return comp;
    };

    ll orig = count_cc(-1);
    v64 arts;
    forn(v, 0, n) if (count_cc(v) > orig) arts.push_back(v);

    cout << sz(arts) << ln;
    for (ll v : arts) cout << v << ln;
}
