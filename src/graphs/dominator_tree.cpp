// Dominator Tree (Lengauer-Tarjan)
//
// builds the dominator tree given a directed graph and a source.
// returns a vector where ret[u] is the immediate dominator of u.
//
// complexity: O((N+M) log N), O(N+M) mem

v64 DomTree(vector<v64>& graph, ll src) {
    ll n = graph.size();
    vector<v64> tree(n), trans(n), buck(n);
    v64 semi(n), par(n), dom(n), label(n), atob(n, -1), btoa(n, -1),
        link(n, -1);

    function<ll(ll, ll)> find=[&](ll u, ll d){
        if(link[u] == -1) return d ? -1 : u;
        ll v = find(link[u], d + 1);
        if(v < 0) return u;
        if(semi[label[link[u]]] < semi[label[u]]) label[u] = label[link[u]];
        link[u] = v;
        return d ? v : label[u];
    };
    ll t = 0;
    function<void(ll)> dfs=[&](ll u){
        atob[u] = t;
        btoa[t] = u;
        label[t] = semi[t] = t;
        t++;
        for(auto v : graph[u]){
        if(atob[v] == -1) dfs(v), par[atob[v]] = atob[u];
        trans[atob[v]].push_back(atob[u]);
        }
    };
    dfs(src);
    for(ll u = t - 1; u >= 0; --u){
        for(auto v : trans[u]) semi[u] = min(semi[u], semi[find(v, 0)]);
        if(u) buck[semi[u]].push_back(u);
        for(auto w : buck[u]){
            ll v = find(w, 0);
            dom[w] = semi[v] == semi[w] ? semi[w] : v;
        }
        if(u) link[u] = par[u];
    }
    v64 ret(n, -1);
    forn(u,1,t) {
        if(dom[u] != semi[u]) dom[u] = dom[dom[u]];
        ret[btoa[u]] = btoa[dom[u]];
    }
    return ret;
}
