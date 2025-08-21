// Floyd–Warshall Algorithm
//
// Computes all-pairs shortest paths and detects negative cycles using dynamic programming over path lengths.
//
// complexity: O(N^3), O(N^2)

ll n;
ll d[MAX][MAX];

bool floyd_warshall() {
	for (int k = 0; k < n; k++)
	for (int i = 0; i < n; i++)
	for (int j = 0; j < n; j++)
		d[i][j] = min(d[i][j], d[i][k] + d[k][j]);

	for (int i = 0; i < n; i++)
		if (d[i][i] < 0) return 1;

	return 0;
}
