#include "src/extra/template.cpp"

int main() {
    _;
    ll n, q; cin >> n >> q;
    vector<v64> g(n);
    while (q--) {
        ll t, u, v; cin >> t >> u >> v;
        if (t == 1) {
            g[u].push_back(v);
            g[v].push_back(u);
        } else {
            vector<bool> vis(n, false);
            queue<ll> bfs;
            bfs.push(u); vis[u] = true;
            while (!bfs.empty() && !vis[v]) {
                ll cur = bfs.front(); bfs.pop();
                for (ll nb : g[cur]) if (!vis[nb]) { vis[nb] = true; bfs.push(nb); }
            }
            cout << (vis[v] ? "Yes" : "No") << ln;
        }
    }
}
