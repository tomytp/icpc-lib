// Dinic's Maximum Flow (with Scaling)
//
// Computes max flow using Dinic's algorithm with optional capacity scaling to speed up BFS levels.
//
// complexity: O(E V^2) worst-case, O(E)

struct dinitz {
	const bool scaling = true;
	ll lim;
	struct edge {
		ll to, cap, rev, flow;
		bool res;
		edge(ll to_, ll cap_, ll rev_, bool res_)
			: to(to_), cap(cap_), rev(rev_), flow(0), res(res_) {}
	};

	vector<vector<edge>> g;
	vector<ll> lev, beg;
	ll F;
	dinitz(ll n) : g(n), F(0) {}

	void add(ll a, ll b, ll c) {
		g[a].emplace_back(b, c, g[b].size(), false);
		g[b].emplace_back(a, 0, g[a].size()-1, true);
	}

	bool bfs(ll s, ll t) {
		lev = vector<ll>(g.size(), -1); lev[s] = 0;
		beg = vector<ll>(g.size(), 0);
		queue<ll> q; q.push(s);
		while (q.size()) {
			ll u = q.front(); q.pop();
			for (auto& i : g[u]) {
				if (lev[i.to] != -1 or (i.flow == i.cap)) continue;
				if (scaling and i.cap - i.flow < lim) continue;
				lev[i.to] = lev[u] + 1;
				q.push(i.to);
			}
		}
		return lev[t] != -1;
	}

	ll dfs(ll v, ll s, ll f = INF) {
		if (!f or v == s) return f;
		for (ll& i = beg[v]; i < g[v].size(); i++) {
			auto& e = g[v][i];
			if (lev[e.to] != lev[v] + 1) continue;
			ll foi = dfs(e.to, s, min(f, e.cap - e.flow));
			if (!foi) continue;
			e.flow += foi, g[e.to][e.rev].flow -= foi;
			return foi;
		}
		return 0;
	}

	ll max_flow(ll s, ll t) {
		for (lim = scaling ? (1<<30) : 1; lim; lim /= 2)
			while (bfs(s, t)) while (ll ff = dfs(s, t)) F += ff;
		return F;
	}

	void reset() {
		F = 0;
		for (auto& edges : g) for (auto& e : edges) e.flow = 0; 
	}
};
