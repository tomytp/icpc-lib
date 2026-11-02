// Point Inside Convex Polygon
//
// Diz se um ponto esta dentro do poligono convexo (busca binaria).
// Com strict = true, pontos na borda nao contam como interior.
//
// complexity: O(log n)

#include "src/geometry/geo_int.cpp"

bool inpolygonconv(vector<pt> &pol, pt p, bool strict = true) {
    if (pol.size() == 0) return false;
    if (pol.size() == 1) return (!strict && p == pol[0]);
    ll l = 1, r = pol.size();
    while (l < r) {
        ll m = (l+r)/2;
        if (ccw(p, pol[0], pol[m])) l = m+1;
        else r = m;
    }
    if (l == 1) return (!strict && isinseg(p, line(pol[0], pol[1])));
    if (l == pol.size()) return false;
    return !ccw(p, pol[l], pol[l-1], !strict);
}
