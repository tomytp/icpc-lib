// Convex Hull (Monotone Chain)
//
// Computes the convex hull of a set of points using Andrew's monotone chain.
// Returns the hull in counterclockwise order, without repeated points.
// col = true keeps collinear points on the edges.
//
// complexity: O(N log N), O(N)

vector<pt> convex_hull(vector<pt> v, bool col = false) {
    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());
    if (sz(v) <= 1) return v;
    vector<pt> l, u;
    forn(i, 0, sz(v)) {
        while (sz(l) > 1 && !ccw(l.end()[-2], l.end()[-1], v[i], !col))
            l.pop_back();
        l.push_back(v[i]);
    }
    if (sz(l) == sz(v)) return v; // todos na cadeia inferior (inclui caso colinear)
    for (ll i = sz(v) - 1; i >= 0; i--) {
        while (sz(u) > 1 && !ccw(u.end()[-2], u.end()[-1], v[i], !col))
            u.pop_back();
        u.push_back(v[i]);
    }
    l.pop_back(); u.pop_back();
    for (pt p : u) l.push_back(p);
    return l;
}