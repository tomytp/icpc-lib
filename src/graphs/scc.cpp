// Strongly Connected Components (Kosaraju)
//
// Computes SCCs using two DFS passes and builds the condensation graph.
// roots[v] = id of v's SCC; ids are in topological order of the condensation
// (every edge goes from a smaller id to a larger one). Vertices 0-indexed.
// Recursive DFS: a path of ~1.3e5 vertices overflows an 8MB stack.
//
// complexity: O(N + M log M), O(N + M)

vector<bool> visited;

void dfs(ll v, vector<vll>& g, vll &out) {
    visited[v] = true;
    for(auto u : g[v]) if(!visited[u]) dfs(u, g, out);
    out.push_back(v);
}

vector<vll> scc(vector<vll>& g, vll& roots) {
    int n = sz(g);
    vll order; roots.assign(n, 0);

    vector<vll> adj_rev(n);
    forn(u, 0, n) for (ll v : g[u]) adj_rev[v].push_back(u);

    visited.assign(n, false);
    forn(i, 0, n) if (!visited[i]) dfs(i, g, order);
    reverse(order.begin(), order.end());
    
    visited.assign(n, false);
    ll curr_comp = 0;
    for (auto v : order) {
        if (!visited[v]) {
            vll component; dfs(v, adj_rev, component);
            for (auto u : component) roots[u] = curr_comp;
            curr_comp++;
        }
    }

    set<pll> edges;
    vector<vll> cond_g(curr_comp);
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
