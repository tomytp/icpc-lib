// Random Utilities
//
// Initializes a 64-bit Mersenne Twister RNG and provides uniform integer sampling in a range.

mt19937_64 rng((ll) chrono::steady_clock::now().time_since_epoch().count());

ll uniform(ll l, ll r){
	uniform_int_distribution<ll> uid(l, r);
	return uid(rng);
}
