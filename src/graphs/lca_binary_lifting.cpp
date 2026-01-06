// LCA com binary lifting
//
// Assume que um vertice eh ancestral dele mesmo, ou seja,
// se a eh ancestral de b, lca(a, b) = a
// MAXLOG = ceil(log(MAX))
//
// Complexidades:
// build - O(n log(n))
// lca - O(log(n))

vector<v64> g(MAX);
ll n, p;
ll parent[MAXLOG][MAX];
ll in[MAX], out[MAX];

void dfs(ll k) {
    in[k] = p++;
    for(ll v: g[k]){
        if(in[v] == -1){
            parent[0][v] = k;
            dfs(v);
        }
    }
    out[k] = p++;
}

void build(ll raiz) {
    forn(i,0,n) parent[0][i] = i;
    p = 0, memset(in, -1, sizeof in);
    dfs(raiz);
    forn(k,1,MAXLOG) forn(i,0,n)
        parent[k][i] = parent[k - 1][parent[k - 1][i]];
}

bool anc(ll a, ll b) { // se a eh ancestral de b
    return in[a] <= in[b] and out[a] >= out[b];
}

ll lca(ll a, ll b) {
    if (anc(a, b)) return a;
    if (anc(b, a)) return b;
    for (ll k = MAXLOG - 1; k >= 0; k--)
        if (!anc(parent[k][a], b)) a = parent[k][a];
    return parent[0][a];
}
