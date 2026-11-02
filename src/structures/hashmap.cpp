// Custom Hash for hash table
//
// Provides 64-bit hashers for integers and pairs to use with __gnu_pbds::gp_hash_table.
//
// complexity: O(1) esperado por operacao, O(n) memoria

#include <bits/extc++.h>

// for ll
struct chash {
    const uint64_t C = ll(4e18 * acos(0)) | 71;
    ll operator()(ll x) const { return __builtin_bswap64(x*C); }
};

// for pll
struct chash {
    const uint64_t C = ll(4e18 * acos(0)) | 71;
    ll h(ll x) const { return __builtin_bswap64(x * C); }
    ll operator()(const pll& p) const { return h(p.first ^ h(p.second)); }
};

__gnu_pbds::gp_hash_table<ll, ll, chash> h({},{},{},{},{1<<16});
__gnu_pbds::gp_hash_table<pll, ll, chash> h({},{},{},{},{1<<16});