// Minimum Enclosing Circle
// calcula o minimum enclosing circle, probalistico mas e O(n)
// so se o pt for de double!!
// talvez usar r*(1+eps);

pair<pt, ld> mec(vector<pt> v) {
	shuffle(v.begin(), v.end(), mt19937(time(0)));
	pt o = v[0];
	ld r = 0;
	forn(i, 0, sz(v)) if (cmp(dist(o, v[i]),r) > 0) {
		o = v[i], r = 0;
		forn(j, 0, i) if (cmp(dist(o, v[j]),r) > 0) {
			o = (v[i] + v[j]) / 2;
            r = dist(o, v[i]);
			forn(k, 0, j) if (cmp(dist(o, v[k]),r) > 0) {
				o = ccCenter(v[i], v[j], v[k]);
                r = dist(o, v[i]);
			}
		}
	}
	return {o, r};
}
