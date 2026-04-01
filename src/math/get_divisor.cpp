// Flattened Divisor Sieve (CSR format)
// Precomputes all divisors up to n. 
// Faster than vector<v64> for cache reasons 
// Time Complexity: O(n log n) | Space Complexity: O(n log n)

tuple<v64, v64, v64> get_divisor(ll n) {
    v64 head(n);
    v64 tail(n);
    v64 divs(n*(log(n)+1));

    forn(i, 1, n)
        for (ll j = i; j < n; j += i)
            head[j]++;

    ll curr = 0;
    forn(i, 1, n) {
        ll cnt = head[i];
        head[i] = curr;
        tail[i] = curr;
        curr += cnt;
    }

    forn(i, 1, n) 
        for (ll j = i; j < n; j += i) 
            divs[tail[j]++] = i; 
    return {head, tail, divs};   
}
// for d | x =
// forn(k,head[x], tail[x]){
//     ll d = divs[k];
// }