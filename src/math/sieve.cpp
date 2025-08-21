// Euler Totient Linear Sieve
//
// Computes Euler's totient for all numbers up to n using a linear sieve and collects primes.
//
// complexity: O(n), O(n)

v64 primes;
vector<bool> is_comp(MAXN,false);
ll phi[MAXN];
ll cum_sum[MAXN];

void sieve(ll n){
  phi[1] = 1;
  forn(i,2,n){
    if(!is_comp[i]){
      phi[i] = i-1;
      primes.push_back(i);
    }
    
    forn(j,0,primes.size()){
      if(i*primes[j] > n) break;
      is_comp[i*primes[j]] = true;
      
      if(i % primes[j] == 0){
        phi[i*primes[j]] = phi[i]*primes[j]; 
        break;
      }
      phi[i*primes[j]] = phi[i]*phi[primes[j]];    
    }
  }
}
