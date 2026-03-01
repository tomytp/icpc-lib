// Extremal Vertex in Convex Polygon
// Dado comparador, calcula o maior vertice nessa ordem
// O(log(n))

#include "src/geometry/geo_int.cpp"

ll extreme(vector<pt>&pol, const function<bool(pt, pt)>& cmp) {
    ll n = sz(pol);
    auto extr = [&](ll i, bool& cur_dir) {
        cur_dir = cmp(pol[(i+1)%n], pol[i]);
        return !cur_dir && !cmp(pol[(i+n-1)%n], pol[i]);
    };
    bool last_dir, cur_dir;
    if (extr(0, last_dir)) return 0;
    ll l = 0, r = n;
    while (l+1 < r) {
        ll m = (l+r)/2;
        if (extr(m, cur_dir)) return m;
        bool rel_dir = cmp(pol[m], pol[l]);
        if ((!last_dir && cur_dir) || (last_dir == cur_dir && rel_dir == cur_dir)) {
            l = m;
            last_dir = cur_dir;
        } else r = m;
    }
    return l;
}
ll max_dot(vector<pt>&pol, pt v) {
    return extreme(pol, [&](pt p, pt q) { return p*v > q*v; });
}
pair<ll, ll> tangents(vector<pt>&pol, pt p) {
    auto L = [&](pt q, pt r) { return ccw(p, r, q); };
    auto R = [&](pt q, pt r) { return ccw(p, q, r); };
    return {extreme(pol, L), extreme(pol, R)};
}