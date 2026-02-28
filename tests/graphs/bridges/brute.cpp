// Brute-force reference for graphs/bridges
// For each edge (u,v), remove it and check if u and v become disconnected.

#include "src/extra/template.cpp"

int main() {
    _;
    ll n, m; cin >> n >> m;
    vector<p64> edges(m);
    for (auto& [u, v] : edges) cin >> u >> v;

    // BFS from s to t, skipping edge at index skip_e
    auto connected = [&](ll s, ll t, ll skip_e) -> bool {
        v64 vis(n, 0);
        vis[s] = 1;
        queue<ll> q; q.push(s);
        while (!q.empty()) {
            ll v = q.front(); q.pop();
            forn(i, 0, m) {
                if (i == skip_e) continue;
                auto [u, w] = edges[i];
                if (u == v && !vis[w]) { vis[w] = 1; q.push(w); }
                if (w == v && !vis[u]) { vis[u] = 1; q.push(u); }
            }
        }
        return vis[t];
    };

    vector<p64> bridges;
    forn(i, 0, m) {
        auto [u, v] = edges[i];
        if (!connected(u, v, i))
            bridges.push_back({min(u, v), max(u, v)});
    }
    sort(bridges.begin(), bridges.end());
    cout << sz(bridges) << ln;
    for (auto [u, v] : bridges) cout << u << " " << v << ln;
}
