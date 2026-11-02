// Point Inside Polygon
//
// Diz se um ponto esta dentro do poligono (ray casting).
// Com strict = true, pontos na borda nao contam como interior.
//
// complexity: O(n)

#include "src/geometry/geo_double.cpp"

bool inpolygon(vector<pt> &p, pt a, bool strict = true) {
	ll cnt = 0, n = sz(p);
	forn(i,0,n) {
		pt q = p[(i + 1) % n];
		if (isinseg(a, line(p[i], q))) return !strict;
		cnt ^= (((cmp(a.y, p[i].y) < 0) - (cmp(a.y,q.y) < 0))*((p[i]-a)^(q-a))) > 0 ;
	}
	return cnt;
}