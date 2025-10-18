// Linear Sieve (prime precomputation)
//
// Computes primes up to N in O(N) using the smallest prime factor (lp).
// pr will contain all primes <= N.
// lp[x] stores the smallest prime dividing x.
//
// complexity: O(N)
// escrito pelo gpt, cuidado

const ll N = 10000000;
v64 lp(N+1), pr;

forn(i, 2, N+1) {
    if (lp[i] == 0) {
        lp[i] = i;
        pr.push_back(i);
    }
    for (ll j = 0; i * pr[j] <= N; ++j) {
        lp[i * pr[j]] = pr[j];
        if (pr[j] == lp[i]) {
            break;
        }
    }
}