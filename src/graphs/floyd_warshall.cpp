// Floyd-Warshall Algorithm
//
// Computes all-pairs shortest paths and detects negative cycles using dynamic programming over path lengths.
//
// complexity: O(N^3), O(N^2)

ll n;
ll d[MAX][MAX];

bool floyd_warshall() {
    forn(k,0,n)
    forn(i,0,n)
    forn(j,0,n)
        d[i][j] = min(d[i][j], d[i][k] + d[k][j]);

    forn(i,0,n)
        if (d[i][i] < 0) return 1;

    return 0;
}
