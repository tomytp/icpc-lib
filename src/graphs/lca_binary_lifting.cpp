// LCA com binary lifting
//
// Assume que um vertice eh ancestral dele mesmo, ou seja,
// se a eh ancestral de b, lca(a, b) = a
// Constroi com o grafo (lista de adjacencia) e a raiz
//
// Complexidades:
// build - O(n log(n))
// lca - O(log(n))

struct LCA_BL {
    ll n, LOG, p = 0;
    vector<vll> parent;
    vll in, out;

    LCA_BL(vector<vll>& g, ll raiz) : n(sz(g)), LOG(__lg(max(n, 1ll)) + 1),
        parent(LOG, vll(n)), in(n, -1), out(n) {
        forn(i,0,n) parent[0][i] = i;
        dfs(g, raiz);
        forn(k,1,LOG) forn(i,0,n)
            parent[k][i] = parent[k - 1][parent[k - 1][i]];
    }

    void dfs(vector<vll>& g, ll k) {
        in[k] = p++;
        for(ll v: g[k]){
            if(in[v] == -1){
                parent[0][v] = k;
                dfs(g, v);
            }
        }
        out[k] = p++;
    }

    bool anc(ll a, ll b) { // se a eh ancestral de b
        return in[a] <= in[b] && out[a] >= out[b];
    }

    ll lca(ll a, ll b) {
        if (anc(a, b)) return a;
        if (anc(b, a)) return b;
        for (ll k = LOG - 1; k >= 0; k--)
            if (!anc(parent[k][a], b)) a = parent[k][a];
        return parent[0][a];
    }
};