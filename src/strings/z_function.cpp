// Z-funtion
// For any index i, finds the lcp between s[i..n) ans s
//
// O(n)

v64 get_z(string s) {
	ll n = sz(s);
	v64 z(n, 0);

	ll l = 0, r = 0;
	forn(i, 1, n) {
		if (i <= r) z[i] = min(r - i + 1, z[i - l]);
		while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
		if (i + z[i] - 1 > r) l = i, r = i + z[i] - 1;
	}

	return z;
}
