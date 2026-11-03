// Flattened Divisor Sieve (CSR format)
 
tuple<vll, vll, vll> get_divisor(ll n) {
    vll head(n), tail(n);
 
    forn(i,1,n)
        for (ll j = i; j < n; j += i)
            head[j]++;
 
    ll curr = 0;
    forn(i,1,n) {
        ll cnt = head[i];
        head[i] = curr;
        tail[i] = curr;
        curr += cnt;
    }
 
    vll divs(curr);
 
    forn(i,1,n)
        for (ll j = i; j < n; j += i)
            divs[tail[j]++] = i;
 
    return {head, tail, divs};
}