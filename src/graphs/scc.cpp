// Strongly Connected Components (Kosaraju)
//
// Computes SCCs using two DFS passes and builds the condensation graph.
//
// complexity: O(N + M), O(N + M)

vector<bool> visited;

void dfs(ll v, vector<v64>& g, vector<ll> &out) {
    visited[v] = true;
    for(auto u : g[v]) if(!visited[u]) dfs(u, g, out);
    out.push_back(v);
}

vector<v64> scc(vector<v64>& g) {
    int n = g.size();
    v64 order, roots(n, 0);

    vector<v64> adj_rev(n);
    forn(u, 0, n) for (ll v : g[u]) adj_rev[v].push_back(u);

    visited.assign(n, false);
    forn(i, 0, n) if (!visited[i]) dfs(i, g, order);
    reverse(order.begin(), order.end());
    
    visited.assign(n, false);
    ll curr_comp = 0;
    for (auto v : order) {
        if (!visited[v]) {
            v64 component; dfs(v, adj_rev, component);
            for (auto u : component) roots[u] = curr_comp;
            curr_comp++;
        }
    }

    set<p64> edges;
    vector<v64> cond_g(curr_comp);
    forn(u, 0, n) {
        for (auto v : g[u]) {
            if (roots[u] != roots[v] && !edges.count({roots[u], roots[v]})) {
                cond_g[roots[u]].push_back(roots[v]);
                edges.emplace(roots[u], roots[v]);
            }
        }
    }
    return cond_g;          
}
