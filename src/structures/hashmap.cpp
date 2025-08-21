// Custom Hash for hash table
//
// Provides 64-bit hashers for integers and pairs to use with __gnu_pbds::gp_hash_table.
//
// complexity: O(1) average, O(n)

#include <bits/extc++.h> 

// for ll
struct chash {
	const uint64_t C = ll(4e18 * acos(0)) | 71;
	ll operator()(ll x) const { return __builtin_bswap64(x*C); }
};

// for p64
struct chash {
    size_t operator()(const p64& p) const {
        return p.first ^ __builtin_bswap64(p.second);
    }
};

__gnu_pbds::gp_hash_table<ll, ll, chash> h({},{},{},{},{1<<16});
__gnu_pbds::gp_hash_table<p64, ll, chash> h({},{},{},{},{1<<16});
