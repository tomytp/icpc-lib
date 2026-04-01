// LCA com Euler tour + Sparse Table (RMQ)
//
// Constroi com o grafo (lista de adjacencia) e a raiz 0
// Assume arvore enraizada em 0
//
// Complexidades:
// build - O(n log(n))
// lca - O(1)
// dist - O(1)

#include "src/structures/sparse_table.cpp"

struct LCA {
	ll T = 0;
	vll time, path, ret, dep;
	sparse rmq;

	LCA(vector<vll>& C, ll root) : time(sz(C)), dep(sz(C)), rmq((dfs(C,root,-1), ret)) {}
	void dfs(vector<vll>& C, ll v, ll par) {
		time[v] = T++;
        if(par != -1) dep[v] = dep[par] + 1;            
		for (ll y : C[v]) if (y != par) {
			path.push_back(v), ret.push_back(time[v]);
			dfs(C, y, v);
		}
	}

	ll lca(ll a, ll b) {
		if (a == b) return a;
		tie(a, b) = minmax(time[a], time[b]);
		return path[rmq.query(a, b-1)];
	}
	ll dist(ll a,ll b){return dep[a] + dep[b] - 2*dep[lca(a,b)];}
};
