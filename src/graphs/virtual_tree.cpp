// Virtual Tree
//
// Comprime uma arvore dado um conjunto S de vertices, de forma que
// o conjunto de vertices da arvore comprimida contenha S e seja
// minimal e fechado sobre a operacao de LCA
// Se |S| = k, a arvore comprimida tem menos que 2k vertices
// As arestas de virt possuem a distancia do vertice ate o vizinho em arestas 
// virt deve ter tamanho n; so as listas dos vertices usados sao limpas
// Retorna a raiz da virtual tree (S nao pode ser vazio)
// 
// Precisa: 
// lca::time deve ser a ordem de visitacao no dfs
// lca::lca, lca::dist
//
// O(k * T), T tempo do lca::lca

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