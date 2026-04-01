// Linear Sieve (prime precomputation)
//
// Computes primes up to N in O(N) using the smallest prime factor (lp).
// pr will contain all primes <= N.
// lp[x] stores the smallest prime dividing x.
//
// complexity: O(N)


pair<vll, vll> sieve(ll n){
    vll lp(n+1);
    vll pr;
    forn(i, 2, n+1) {
        if (lp[i] == 0) {
            lp[i] = i;
            pr.push_back(i);
        }
        for (ll j = 0; i * pr[j] <= n; ++j) {
            lp[i * pr[j]] = pr[j];
            if (pr[j] == lp[i]) {
                break;
            }
        }
    }
    return {pr, lp};
}

vll calc_mu(vll& lp) {
    ll n = sz(lp) - 1;
    vll ans(n);
    ans[1] = 1;
    forn(i,2,n) {
        ll p = lp[i], x = i/p;
        if (lp[x] == p) ans[i] = 0;
        else ans[i] = -ans[x];
    }
    return ans;
}

vll calc_d(vll& lp) {
    ll n = sz(lp) - 1;
    vll ans(n);
    ans[1] = 1;
    forn(i, 2, n) {
        ll p = lp[i], x = i / p;
        if (lp[x] != p) ans[i] = 1;
        else ans[i] = ans[x] + 1;
    }
    forn(i, 2, n) {
        ll p = lp[i], x = i / p;
        if (ans[i] == 1) ans[i] = ans[x] * 2;
        else ans[i] = (ans[x] / ans[i]) * (ans[i] + 1);
    }
    return ans;
}
