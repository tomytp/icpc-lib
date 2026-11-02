// Virtual Tree
//
// Comprime a arvore num conjunto S: o resultado contem S, e fechado por LCA
// e tem menos de 2|S| vertices. S nao pode ser vazio. Retorna a raiz.
// virt[u] = arestas descendo {filho, distancia em arestas}; virt deve ter tamanho n
// e so as listas dos vertices usados sao limpas.
// Precisa de lca.cpp (lca::time, lca::lca, lca::dist).
//
// complexity: O(k log k + k * T), T = tempo do lca::lca

#include "src/graphs/lca.cpp"

ll build_virt(vll v, LCA& lca, vector<vector<pll>>& virt) {
	auto cmp = [&](ll i, ll j) { return lca.time[i] < lca.time[j]; };
	sort(v.begin(), v.end(), cmp);
	for (ll i = sz(v)-1; i; i--) v.push_back(lca.lca(v[i], v[i-1]));
	sort(v.begin(), v.end(), cmp);
    v.erase(unique(v.begin(), v.end()), v.end());
    forn(i,0,sz(v)) virt[v[i]].clear();
    forn(i,1,sz(v)) {
		ll parent = lca.lca(v[i-1], v[i]);
		ll d = lca.dist(parent, v[i]);
// #warning soh to colocando aresta descendo
		virt[parent].emplace_back(v[i], d);
	}

	return v[0];
}