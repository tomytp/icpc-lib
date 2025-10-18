// 2-SAT
//
// solve() retorna um par, o first fala se eh possivel
// atribuir, o second fala se cada variavel eh verdadeira
//
// O(|V|+|E|) = O(#variaveis + #restricoes)
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef vector<ll> v64;

struct sat {
	ll n, tot;
	vector<v64> g;
	v64 vis, comp, id, ans;
	stack<ll> s;

	sat() {}
	sat(ll n_) : n(n_), tot(n), g(2*n) {}

	ll dfs(ll i, ll& t) {
		ll lo = id[i] = t++;
		s.push(i), vis[i] = 2;
		for (ll j : g[i]) {
			if (!vis[j]) lo = min(lo, dfs(j, t));
			else if (vis[j] == 2) lo = min(lo, id[j]);
		}
		if (lo == id[i]) while (1) {
			ll u = s.top(); s.pop();
			vis[u] = 1, comp[u] = i;
			if ((u>>1) < n and ans[u>>1] == -1) ans[u>>1] = ~u&1;
			if (u == i) break;
		}
		return lo;
	}

	void add_impl(ll x, ll y) { // x -> y = !x ou y
		x = x >= 0 ? 2*x : -2*x-1;
		y = y >= 0 ? 2*y : -2*y-1;
		g[x].push_back(y);
		g[y^1].push_back(x^1);
	}
	void add_cl(ll x, ll y) { // x ou y
		add_impl(~x, y);
	}
	void add_xor(ll x, ll y) { // x xor y
		add_cl(x, y), add_cl(~x, ~y);
	}
	void add_eq(ll x, ll y) { // x = y
		add_xor(~x, y);
	}
	void add_true(ll x) { // x = T
		add_impl(~x, x);
	}
	void at_most_one(v64 v) { // no max um verdadeiro
		g.resize(2*(tot+v.size()));
		for (ll i = 0; i < v.size(); i++) {
			add_impl(tot+i, ~v[i]);
			if (i) {
				add_impl(tot+i, tot+i-1);
				add_impl(v[i], tot+i-1);
			}
		}
		tot += v.size();
	}

	pair<bool, v64> solve() {
		ans = v64(n, -1);
		ll t = 0;
		vis = comp = id = v64(2*tot, 0);
		for (ll i = 0; i < 2*tot; i++) if (!vis[i]) dfs(i, t);
		for (ll i = 0; i < tot; i++)
			if (comp[2*i] == comp[2*i+1]) return {false, {}};
		return {true, ans};
	}
};