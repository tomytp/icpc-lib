// Polygon Cut
// Corta um poligono deixando somente a parte ccw com a reta l dada
// O(n)

vector<pt> polygonCut(const vector<pt>& poly, line l) {
	vector<pt> res;
	forn(i,0,sz(poly)) {
		pt curr = poly[i], prev = i ? poly[i-1] : poly.back();
		bool a = ccw(l.p, l.q, curr), b = ccw(l.p, l.q, prev);
		if (a != b)
			res.push_back(inter(line(curr, prev), l));
		if (a)
			res.push_back(curr);
	}
	return res;
}