// soma de subconjunto
v64 sos_dp_sub(v64 f) {
	ll n = __builtin_ctzll((ll)f.size());
	assert((1ll<<n) == (ll)f.size());

    forn(i,0,n) forn(mask, 0, (1<<n))
		if (mask>>i&1) f[mask] += f[mask^(1<<i)];
	return f;
}

// soma de super-conjunto
v64 sos_dp_super(v64 f) {
	ll n = __builtin_ctzll((ll)f.size());
	assert((1ll<<n) == (ll)f.size());

    forn(i,0,n) forn(mask, 0, (1<<n))    
		if (~mask>>i&1) f[mask] += f[mask^(1<<i)];
	return f;
}