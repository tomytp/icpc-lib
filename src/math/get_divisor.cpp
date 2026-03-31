// Flattened Divisor Sieve (CSR format)
// Precomputes all divisors up to MAX. 
// Faster than vector<v64> for cache reasons 
// Time Complexity: O(MAX log MAX) | Space Complexity: O(MAX log MAX)

void get_divisor() {
    forn(i, 1, MAX)
        for (ll j = i; j < MAX; j += i)
            head[j]++;

    ll curr = 0;
    forn(i, 1, MAX) {
        ll cnt = head[i];
        head[i] = curr;
        tail[i] = curr;
        curr += cnt;
    }

    forn(i, 1, MAX) 
        for (ll j = i; j < MAX; j += i) 
            divs[tail[j]++] = i;    
}
// for d | x =
// forn(k,head[x], tail[x]){
//     ll d = divs[k];
// }