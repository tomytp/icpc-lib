// Dijkstra's Shortest Paths
//
// Computes single-source shortest paths on non-negative weighted graphs using a priority queue.
//
// complexity: O((N + M) log N), O(N + M)

vector<vector<p64>> g;

// d = distance | p = from/path
void dijkstra(ll s, v64 &d, v64& p) {
    ll n = g.size();
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

        for (auto edge : g[u]) {
            ll v = edge.first;
            ll w_v = edge.second;

            if (d[u] + w_v < d[v]) {
                d[v] = d[u] + w_v;
                p[v] = u;
                pq.push({-d[v], v});
            }
        }
    }
}
