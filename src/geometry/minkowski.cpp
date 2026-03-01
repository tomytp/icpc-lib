// Minkowski Sum
// Soma poligonos de tamanho a e b em O(a+b)

#include "src/geometry/geo_int.cpp"

vector<pt> minkowski(vector<pt> p, vector<pt> q) {
	auto fix = [](vector<pt>& P) {
		rotate(P.begin(), min_element(P.begin(), P.end()), P.end());
		P.push_back(P[0]), P.push_back(P[1]);
	};
	fix(p), fix(q);
	vector<pt> ret;
	int i = 0, j = 0;
	while (i < p.size()-2 or j < q.size()-2) {
		ret.push_back(p[i] + q[j]);
		auto c = ((p[i+1] - p[i]) ^ (q[j+1] - q[j]));
		if (c >= 0) i = min<int>(i+1, p.size()-2);
		if (c <= 0) j = min<int>(j+1, q.size()-2);
	}
	return ret;
}