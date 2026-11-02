// Floyd-Warshall Algorithm
//
// Computes all-pairs shortest paths via DP over intermediate vertices.
// Initialize d[i][j] = INF, d[i][i] = 0, then d[u][v] = min(d[u][v], w)
// for each edge (handles multi-edges and negative self-loops).
// Returns true if a negative cycle is detected.
// Optional pass at the end sets d[i][j] = -INF for pairs whose shortest
// path is unbounded (passes through a negative cycle).
//
// complexity: O(N^3), O(N^2)

bool floyd_warshall(ll n, vector<vll>& d) {
    forn(k,0,n)
    forn(i,0,n)
    forn(j,0,n)
        if (d[i][k] < INF && d[k][j] < INF)
            d[i][j] = max(-INF, min(d[i][j], d[i][k] + d[k][j]));

    bool neg = false;
    forn(i,0,n)
        if (d[i][i] < 0) neg = true;

    // optional: mark pairs affected by negative cycles
    if (neg)
        forn(k,0,n) if (d[k][k] < 0)
        forn(i,0,n) if (d[i][k] < INF)
        forn(j,0,n) if (d[k][j] < INF)
        d[i][j] = -INF;

    return neg;
}