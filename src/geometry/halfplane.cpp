mt19937_64 rng((ll) chrono::steady_clock::now().time_since_epoch().count());

// só entra retas nao verticais, processar as retas verticais antes e encodar essa info no minx e maxx
// se quiser o ponto mais acima, fazer best_dir = (0,1)
// dependendo do best_dir, best = (+-INF, +-INF)

pt hpi_1d(vector<line> v, pt best_dir, ll n, line suport, ll minx, ll maxx){
	pt low = inter(line(pt(minx, 0), pt(minx,1)), suport), upp = inter(line(pt(maxx, 0), pt(maxx,1)), suport);
	forn(i, 0, n){
		pt intersection = inter(v[i], suport);
		debug(low);
		debug(upp);
		debug(intersection);
		debug(v[i]);

		if(!ccw(v[i].p, v[i].q, low)) low = intersection;
		else if(!ccw(v[i].p, v[i].q, upp)) upp = intersection;
	}

	if(cmp(low.x, upp.x) > 0) return {DINF, DINF};
	
	if(cmp(best_dir*low, best_dir*upp) <= 0) return upp;
	return low;
}

pt hpi(vector<line> v, pt best_dir, ll minx, ll maxx){
	shuffle(v.begin(), v.end(), rng);

	ll n = sz(v);
	pt best(1, 1);

	forn(i, 0, n){
		debug(best);
		debug(v[i]);
		if(ccw(v[i].p, v[i].q, best)) continue;
		best = hpi_1d(v, best_dir, i, v[i], minx, maxx);
		assert(!(best ==pt(DINF, DINF)));
	}

	return best;
}