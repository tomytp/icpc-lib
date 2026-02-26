// Sum over Subsets DP (SOS DP)
//
// sos_dp_sub: for each mask, computes the sum of f[s] over all
// subsets s of mask (i.e., f'[mask] = sum of f[s] for s in mask).
//
// sos_dp_super: for each mask, computes the sum of f[s] over all
// supersets s of mask (i.e., f'[mask] = sum of f[s] for mask in s).
//
// input size must be a power of two (2^n).
//
// complexity: O(n * 2^n), O(2^n) mem

v64 sos_dp_sub(v64 f) {
    ll n = __builtin_ctzll(sz(f));
    assert((1ll<<n) == sz(f));

    forn(i,0,n) forn(mask, 0, (1<<n))
        if (mask>>i&1) f[mask] += f[mask^(1<<i)];
        // -= for inverse
    return f;
}

v64 sos_dp_super(v64 f) {
    ll n = __builtin_ctzll(sz(f));
    assert((1ll<<n) == sz(f));

    forn(i,0,n) forn(mask, 0, (1<<n))    
        if (~mask>>i&1) f[mask] += f[mask^(1<<i)];
        // -= for inverse
    return f;
}