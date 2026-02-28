// Dijkstra's Shortest Paths
//
// Computes single-source shortest paths on non-negative weighted graphs using a priority queue.
//
// complexity: O((N + M) log N), O(N + M)

// d = distance | p = from/path
void dijkstra(ll s, v64 &d, v64& p, vector<vector<p64>>& g) {
    ll n = sz(g);
    d.assign(n, INF);
    p.assign(n, -1);

    d[s] = 0;
    priority_queue<p64> pq;
    pq.push({0, s});
    while (!pq.empty()) {
        ll u = pq.top().second;
        ll d_u = -pq.top().first;
        pq.pop();

        if (d_u != d[u]) continue;

        for (auto [v, w_v] : g[u]) {
            if (d[u] + w_v < d[v]) {
                d[v] = d[u] + w_v;
                p[v] = u;
                pq.push({-d[v], v});
            }
        }
    }
}
