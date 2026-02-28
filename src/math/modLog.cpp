// Discrete Logarithm (Baby-step Giant-step)
//
// Finds the smallest x >= 1 such that a^x ≡ b (mod m),
// or returns -1 if no solution exists.
// Uses Baby-step Giant-step with a hash map for O(sqrt(m)) lookups.
//
// complexity: O(sqrt(m) * log(sqrt(m)))

ll modLog(ll a, ll b, ll m) {
	ll n = (ll) sqrt(m) + 1, e = 1, f = 1, j = 1;
	map<ll, ll> mp;
	while (j <= n && (e = f = e * a % m) != b % m)
		mp[e * b % m] = j++;
	if (e == b % m) return j;
	if (gcd(m, e) == gcd(m, b)) 
		forn(i,2,n+2) if (mp.count(e = e * f % m))
			return n * i - mp[e];
	return -1;
}