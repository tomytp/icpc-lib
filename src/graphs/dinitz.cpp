// Dinic's Maximum Flow (with Scaling)
//
// Computes max flow using Dinic's algorithm with optional capacity scaling to speed up BFS levels.
//
// complexity: O(E V^2) worst-case, O(E)

struct dinitz {
    const bool scaling;
    ll lim;
    struct edge {
        ll to, cap, rev, flow;
        bool res;
        edge(ll to_, ll cap_, ll rev_, bool res_)
            : to(to_), cap(cap_), rev(rev_), flow(0), res(res_) {}
    };

    vector<vector<edge>> g;
    v64 lev, beg;
    ll F;
    dinitz(ll n, bool scaling_) : scaling(scaling_), g(n), F(0){}

    void add(ll a, ll b, ll c) {
        g[a].emplace_back(b, c, sz(g[b]), false);
        g[b].emplace_back(a, 0, sz(g[a])-1, true);
    }

    bool bfs(ll s, ll t) {
        lev = v64(sz(g), -1); lev[s] = 0;
        beg = v64(sz(g), 0);
        queue<ll> q; q.push(s);
        while (sz(q)) {
            ll u = q.front(); q.pop();
            for (auto& i : g[u]) {
                if (lev[i.to] != -1 || (i.flow == i.cap)) continue;
                if (scaling && i.cap - i.flow < lim) continue;
                lev[i.to] = lev[u] + 1;
                q.push(i.to);
            }
        }
        return lev[t] != -1;
    }

    ll dfs(ll v, ll t, ll f = INF) {
        if (!f || v == t) return f;
        for (ll& i = beg[v]; i < sz(g[v]); i++) {
            auto& e = g[v][i];
            if (lev[e.to] != lev[v] + 1) continue;
            ll foi = dfs(e.to, t, min(f, e.cap - e.flow));
            if (!foi) continue;
            e.flow += foi, g[e.to][e.rev].flow -= foi;
            return foi;
        }
        return 0;
    }

    ll max_flow(ll s, ll t) {
        for (lim = scaling ? (1ll<<30) : 1; lim; lim /= 2)
            while (bfs(s, t)) while (ll ff = dfs(s, t)) F += ff;
        return F;
    }

    vector<p64> min_cut() {
        vector<p64> cut;
        forn(u, 0, sz(g))
            if (lev[u] != -1)
                for (auto& e : g[u])
                    if (lev[e.to] == -1 && !e.res)
                        cut.push_back({u, e.to});
        return cut;
    }

    void reset() {
        F = 0;
        for (auto& edges : g) for (auto& e : edges) e.flow = 0; 
    }
};
