// Convex Hull (Monotone Chain)
//
// Computes the convex hull of a set of points using Andrew's monotone chain; handles collinear points based on ccw condition.
//
// complexity: O(N log N), O(N)

// se contar pontos colineares, faz o ccw com >=
vector<pt> convex_hull(vector<pt> &v) {
    v.erase(unique(v.begin(), v.end()), v.end());
    if(sz(v) <=1) return v;
    vector<pt> l, u;
    forn(i, 0, sz(v)) {
        while(sz(l) > 1 && !ccw(l.end()[-2], l.end()[-1], v[i]))
            l.pop_back();
        l.push_back(v[i]);
    }
    for(ll i = sz(v)-1; i>=0; i--) {
        while(sz(u) > 1 && !ccw(u.end()[-2], u.end()[-1], v[i]))
            u.pop_back();
        u.push_back(v[i]);
    }
    l.pop_back();
    u.pop_back();
    for(pt i : u) l.push_back(i);
    return l;
}
