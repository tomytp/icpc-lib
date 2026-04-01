// Flattened Divisor Sieve (CSR format)
// Precomputes all divisors up to n. 
// Faster than vector<vll> for cache reasons 
// Time Complexity: O(n log n) | Space Complexity: O(n log n)

tuple<vll, vll, vll> get_divisor(ll n) {
    vll head(n);
    vll tail(n);
    vll divs(n*(log(n)+1));

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