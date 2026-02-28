// Modular Arithmetic Helpers
//
// Provides fast modular mul, fast exponentiation.
//
// complexity: O(log E) for power/inverse

typedef unsigned long long ull;
const ll MOD = 1'000'000'007;

ull modmul(ull a, ull b, ull M){ 
    ll ret = a*b - M * ull(1.L / M*a*b);    
    return ret + M * (ret < 0) - M * (ret >= (ll)M);
};

ull modpow(ull b, ull e, ull mod){
    ull ans = 1;
    for(; e; b = modmul(b, b, mod), e /= 2)
        if(e & 1) ans = modmul(ans, b, mod);
    return ans;
}

