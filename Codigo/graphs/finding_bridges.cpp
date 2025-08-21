// Bridge Detection (Tarjan)
//
// Finds all bridges in an undirected graph via DFS timestamps and low-link values.
//
// complexity: O(N + M), O(N + M)

vector<v64> g;
vector<bool> visited;
vector<ll> tin, low;
ll timer = 0;

void dfs(ll u, ll p = -1) {
    visited[u] = true;
    tin[u] = low[u] = timer++;
    for (ll v : g[u]) {
        if (v == p) continue;
        
        if (visited[v]) {
            low[u] = min(low[u], tin[v]);
        } else {
            dfs(v, u);
            low[u] = min(low[u], low[v]);
            if (low[v] > tin[u]) {
                // THIS IS A BRIDGE
            }
        }
    }
}

void find_bridges() {
    timer = 0;
    visited.assign(n, false);
    tin.assign(n, -1);
    low.assign(n, -1);
    forn(i, 0, n) {
        if (!visited[i])
            dfs(i);
    }
}
