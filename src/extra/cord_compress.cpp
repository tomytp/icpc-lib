// 1D Coordinate Compression
//
// Maps large or sparse values to a dense range [0, N-1] using sort, unique and lower_bound.
//
// complexity: O(N log N), O(N)


void compress(){
    vll vals;
    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());
 
    auto compress = [&](ll x) {
        return (ll)(lower_bound(vals.begin(), vals.end(), x) - vals.begin());
    };
}