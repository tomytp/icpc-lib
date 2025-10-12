// MinCostMaxFlow
//
// min_cost_flow(s, t, f) computa o par (fluxo, custo)
// com max(fluxo) <= f que tenha min(custo)
// min_cost_flow(s, t) -> Fluxo maximo de custo minimo de s pra t
// Se for um dag, da pra substituir o SPFA por uma DP pra nao
// pagar O(nm) no comeco
// Se nao tiver aresta com custo negativo, nao precisa do SPFA
//
// O(nm + f * m log n)

template<typename T> struct mcmf {
    struct edge {
        ll to, rev, flow, cap; // para, id da reversa, fluxo, capacidade
        bool res; // se eh reversa
        T cost; // custo da unidade de fluxo
        edge() : to(0), rev(0), flow(0), cap(0), cost(0), res(false) {}
        edge(ll to_, ll rev_, ll flow_, ll cap_, T cost_, bool res_)
            : to(to_), rev(rev_), flow(flow_), cap(cap_), res(res_), cost(cost_) {}
    };

    vector<vector<edge>> g;
    v64 par_idx, par;
    T inf;
    vector<T> dist;

    mcmf(ll n) : g(n), par_idx(n), par(n), inf(numeric_limits<T>::max()/3) {}

    void add(ll u, ll v, ll w, T cost) { // de u pra v com cap w e custo cost
        edge a = edge(v, g[v].size(), 0, w, cost, false);
        edge b = edge(u, g[u].size(), 0, 0, -cost, true);

        g[u].push_back(a);
        g[v].push_back(b);
    }

    vector<T> spfa(ll s) { // nao precisa se nao tiver custo negativo
        deque<ll> q;
        vector<bool> is_inside(g.size(), 0);
        dist = vector<T>(g.size(), inf);

        dist[s] = 0;
        q.push_back(s);
        is_inside[s] = true;

        while (!q.empty()) {
            ll v = q.front();
            q.pop_front();
            is_inside[v] = false;

            forn(i,0,sz(g[v])){
                auto [to, rev, flow, cap, res, cost] = g[v][i];
                if (flow < cap and dist[v] + cost < dist[to]) {
                    dist[to] = dist[v] + cost;

                    if (is_inside[to]) continue;
                    if (!q.empty() and dist[to] > dist[q.front()]) q.push_back(to);
                    else q.push_front(to);
                    is_inside[to] = true;
                }
            }
        }
        return dist;
    }
    bool dijkstra(ll s, ll t, vector<T>& pot) {
        priority_queue<pair<T, ll>, vector<pair<T, ll>>, greater<>> q;
        dist = vector<T>(g.size(), inf);
        dist[s] = 0;
        q.emplace(0, s);
        while (q.size()) {
            auto [d, v] = q.top();
            q.pop();
            if (dist[v] < d) continue;
            forn(i,0,sz(g[v])) {
                auto [to, rev, flow, cap, res, cost] = g[v][i];
                cost += pot[v] - pot[to];
                if (flow < cap and dist[v] + cost < dist[to]) {
                    dist[to] = dist[v] + cost;
                    q.emplace(dist[to], to);
                    par_idx[to] = i, par[to] = v;
                }
            }
        }
        return dist[t] < inf;
    }

    pair<ll, T> min_cost_flow(ll s, ll t, ll flow = INF) {
        vector<T> pot(g.size(), 0);
        pot = spfa(s); // mudar algoritmo de caminho minimo aqui
        // sem custo negativo não precisa de nada
        // se grafo for um DAG usar dp
        ll f = 0;
        T ret = 0;
        while (f < flow and dijkstra(s, t, pot)) {
            forn(i,0,sz(g))
                if (dist[i] < inf) pot[i] += dist[i];

            ll mn_flow = flow - f, u = t;
            while (u != s){
                mn_flow = min(mn_flow,
                    g[par[u]][par_idx[u]].cap - g[par[u]][par_idx[u]].flow);
                u = par[u];
            }

            ret += pot[t] * mn_flow;

            u = t;
            while (u != s) {
                g[par[u]][par_idx[u]].flow += mn_flow;
                g[u][g[par[u]][par_idx[u]].rev].flow -= mn_flow;
                u = par[u];
            }

            f += mn_flow;
        }

        return make_pair(f, ret);
    }

    // Opcional: retorna as arestas originais por onde passa flow = cap
    vector<p64> recover() {
        vector<p64> used;
        forn(i, 0, sz(g)) for (edge e : g[i])
            if(e.flow == e.cap && !e.res) used.push_back({i, e.to});
        return used;
    }
};