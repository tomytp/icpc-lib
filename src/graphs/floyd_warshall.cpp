// Floyd-Warshall Algorithm
//
// Computes all-pairs shortest paths via DP over intermediate vertices.
// Initialize d[i][j] = edge weight (INF if no edge, 0 on diagonal).
// Returns true if a negative cycle is detected.
//
// complexity: O(N^3), O(N^2)

bool floyd_warshall(ll n, vector<v64>& d) {
    forn(k,0,n)
    forn(i,0,n)
    forn(j,0,n)
        if (d[i][k] != INF && d[k][j] != INF)
            d[i][j] = min(d[i][j], d[i][k] + d[k][j]);

    forn(i,0,n)
        if (d[i][i] < 0) return true;

    return false;
}