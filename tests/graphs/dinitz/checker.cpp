// Checker for graphs/dinitz
// Called as: ./checker <input_file> <ignored> <output_file>
//
// 1. Brute-force (BFS Ford-Fulkerson) verifies F = true max flow
// 2. Cut edges exist in graph, cut capacity == F, and they disconnect s from t
//    => min_cut() returns a valid min cut

#include "src/extra/template.cpp"

ll brute_flow(ll n, ll s, ll t, map<pll,ll> cap) {
    ll flow = 0;
    while (true) {
        vll par(n, -1); par[s] = s;
        queue<ll> q; q.push(s);
        while (!q.empty() && par[t] == -1) {
            ll u = q.front(); q.pop();
            forn(v, 0, n) if (par[v]==-1 && cap[{u,v}]>0) { par[v]=u; q.push(v); }
        }
        if (par[t] == -1) break;
        ll f = INF;
        for (ll v=t; v!=s; v=par[v]) f = min(f, cap[{par[v],v}]);
        for (ll v=t; v!=s; v=par[v]) { cap[{par[v],v}]-=f; cap[{v,par[v]}]+=f; }
        flow += f;
    }
    return flow;
}

int main(int argc, char* argv[]) {
    ifstream inf(argv[1]), ouf(argv[3]);

    ll n, m, s, t; inf >> n >> m >> s >> t;
    vector<tuple<ll,ll,ll>> edges(m);
    for (auto& [u, v, c] : edges) inf >> u >> v >> c;

    ll F; ouf >> F;
    ll k; ouf >> k;
    set<pll> cut_set;
    forn(i, 0, k) { ll u, v; ouf >> u >> v; cut_set.insert({u, v}); }

    // Build capacity map
    map<pll, ll> cap;
    for (auto [u, v, c] : edges) cap[{u, v}] += c;

    // 1. Verify F = true max flow
    ll expected = brute_flow(n, s, t, cap);
    if (F != expected) {
        cerr << "WA: flow " << F << " != expected " << expected << "\n"; return 1;
    }

    // 2. Verify cut edges exist and sum capacities
    ll cut_cap = 0;
    for (auto [u, v] : cut_set) {
        if (!cap.count({u, v})) {
            cerr << "WA: cut edge " << u << "->" << v << " not in graph\n"; return 1;
        }
        cut_cap += cap[{u, v}];
    }
    if (cut_cap != F) {
        cerr << "WA: cut capacity " << cut_cap << " != flow " << F << "\n"; return 1;
    }

    // 3. Verify t unreachable from s after removing cut edges
    vector<bool> vis(n, false);
    queue<ll> q;
    q.push(s); vis[s] = true;
    while (!q.empty()) {
        ll u = q.front(); q.pop();
        for (auto& [eu, ev, ec] : edges)
            if (eu == u && !cut_set.count({eu, ev}) && !vis[ev])
                vis[ev] = true, q.push(ev);
    }
    if (vis[t]) {
        cerr << "WA: t still reachable from s after removing cut edges\n"; return 1;
    }

    return 0;
}