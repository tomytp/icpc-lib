// Suffix Array - O(n log n)
//
// kasai recebe o suffix array e calcula lcp[i],
// o lcp entre s[sa[i],...,n-1] e s[sa[i+1],..,n-1]
//
// Complexidades:
// suffix_array - O(n log(n))
// kasai - O(n)

v64 suffix_array(string s) {
	s.push_back('$'); // 0 caso v64 (CHECAR SE PODE)
	ll n = s.size(), N = max(n, 260ll);
	v64 sa(n), ra(n);
	forn(i, 0, n) sa[i] = i, ra[i] = s[i];

	for(ll k = 0; k < n; k ? k *= 2 : k++) {
		v64 nsa(sa), nra(n), cnt(N);

		for(int i = 0; i < n; i++) nsa[i] = (nsa[i]-k+n)%n, cnt[ra[i]]++;
        
		forn(i, 1, N) cnt[i] += cnt[i-1];
		for(ll i = n-1; i+1; i--) sa[--cnt[ra[nsa[i]]]] = nsa[i];

		for(ll i = 1, r = 0; i < n; i++) nra[sa[i]] = r += ra[sa[i]] !=
			ra[sa[i-1]] or ra[(sa[i]+k)%n] != ra[(sa[i-1]+k)%n];
		ra = nra;
		if (ra[sa[n-1]] == n-1) break; 
	}
	return v64(sa.begin()+1, sa.end());
}

v64 kasai(string s, v64 sa) {
	ll n = s.size(), k = 0;
	v64 ra(n), lcp(n);
	forn(i, 0, n) ra[sa[i]] = i;

	for (ll i = 0; i < n; i++, k -= !!k) {
		if (ra[i] == n-1) { k = 0; continue; }
		ll j = sa[ra[i]+1];
		while (i+k < n and j+k < n and s[i+k] == s[j+k]) k++;
		lcp[ra[i]] = k;
	}
	return lcp;
}