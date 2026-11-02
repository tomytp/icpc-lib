// Euler Path / Euler Cycle
//
// euler<true> E(n): grafo direcionado com n vertices (false: nao direcionado).
// Retornam {existe, caminho}, caminho = lista de {vertice, id da aresta usada para chegar nele}.
// get_path(src): caminho ou ciclo euleriano comecando em src; o primeiro id e -1.
// src certo: direcionado, saida - entrada = 1; nao direcionado, grau impar;
// se nao houver, qualquer vertice com aresta. Com src errado retorna false.
// get_cycle(): ciclo euleriano sem repetir o vertice inicial (C3: [0, 1, 2]).
// Sem arestas, retorna {true, {}}.
//
// complexity: O(n + m)

template<bool directed=false> struct euler {
	ll n;
	vector<vector<pll>> g;
	vll used;

	euler(ll n_) : n(n_), g(n) {}
	void add(ll a, ll b) {
		ll at = sz(used);
		used.push_back(0);
		g[a].emplace_back(b, at);
		if (!directed) g[b].emplace_back(a, at);
	}
#warning chamar para o src certo!
	pair<bool, vector<pll>> get_path(ll src) {
		if (!sz(used)) return {true, {}};
		vll beg(n, 0);
		for (ll& i : used) i = 0;
		// {{vertice, anterior}, label}
		vector<pair<pll, ll>> ret, st = {{{src, -1}, -1}};
		while (sz(st)) {
			ll at = st.back().first.first;
			ll& it = beg[at];
			while (it < sz(g[at]) && used[g[at][it].second]) it++;
			if (it == sz(g[at])) {
				if (sz(ret) && ret.back().first.second != at)
					return {false, {}};
				ret.push_back(st.back()), st.pop_back();
			} else {
				st.push_back({{g[at][it].first, at}, g[at][it].second});
				used[g[at][it].second] = 1;
			}
		}
		if (sz(ret) != sz(used)+1) return {false, {}};
		vector<pll> ans;
		for (auto i : ret) ans.emplace_back(i.first.first, i.second);
		reverse(ans.begin(), ans.end());
		return {true, ans};
	}
	pair<bool, vector<pll>> get_cycle() {
		if (!sz(used)) return {true, {}};
		ll src = 0;
		while (!sz(g[src])) src++;
		auto ans = get_path(src);
		if (!ans.first || ans.second[0].first != ans.second.back().first)
			return {false, {}};
		ans.second[0].second = ans.second.back().second;
		ans.second.pop_back();
		return ans;
	}
};