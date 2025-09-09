// Virtual Tree
//
// Comprime uma arvore dado um conjunto S de vertices, de forma que
// o conjunto de vertices da arvore comprimida contenha S e seja
// minimal e fechado sobre a operacao de LCA
// Se |S| = k, a arvore comprimida tem menos que 2k vertices
// As arestas de virt possuem a distancia do vertice ate o vizinho em arestas 
// Retorna a raiz da virtual tree
// 
// Precisa: 
// lca::pos deve ser a ordem de visitacao no dfs
// lca::lca, lca::dist
//
// O(k * T), T tempo do lca::lca

vector<pair<ll, ll>> virt[MAX];

#warning lembrar de buildar o LCA antes
ll build_virt(vector<ll> v) {
	auto cmp = [&](ll i, ll j) { return lca::pos[i] < lca::pos[j]; };
	sort(v.begin(), v.end(), cmp);
	for (ll i = v.size()-1; i; i--) v.push_back(lca::lca(v[i], v[i-1]));
	sort(v.begin(), v.end(), cmp);
    v.erase(unique(v.begin(), v.end()), v.end());
    forn(i,0,v.size()) virt[v[i]].clear();
	forn(i,1,v.size()) virt[lca::lca(v[i-1], v[i])].clear();
    forn(i,1,v.size()) {
		ll parent = lca::lca(v[i-1], v[i]);
		ll d = lca::dist(parent, v[i]);
#warning soh to colocando aresta descendo
		virt[parent].emplace_back(v[i], d);
	}

	return v[0];
}