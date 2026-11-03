// Float Geometry Primitives
//
// Defines 2D point and line structures with orientation, area, line intersection and circle-circle / circle-line intersections.

typedef long double ld;

const ld DINF = 1e18;
const ld eps = 1e-9;
const ld pi = 2*acos(0);

#define sq(x) ((x)*(x))

ll cmp(ld x, ld y = 0, ld tol = eps) {
	return (x <= y + tol) ? (x + tol < y) ? -1 : 0 : 1;
}

struct pt { // ponto
	ld x, y;
	pt(ld x_ = 0, ld y_ = 0) : x(x_), y(y_) {}
	bool operator < (const pt p) const {
		if (cmp(x, p.x) != 0) return cmp(x, p.x) < 0;
		if (cmp(y, p.y) != 0) return cmp(y, p.y) < 0;
		return 0;
	}
	bool operator == (const pt p) const {
		return cmp(x, p.x) == 0 && cmp(y, p.y) == 0;
	}
	pt operator + (const pt p) const { return pt(x+p.x, y+p.y); }
	pt operator - (const pt p) const { return pt(x-p.x, y-p.y); }
	pt operator * (const ld c) const { return pt(x*c  , y*c  ); }
	pt operator / (const ld c) const { return pt(x/c  , y/c  ); }
	ld operator * (const pt p) const { return x*p.x + y*p.y; }
	ld operator ^ (const pt p) const { return x*p.y - y*p.x; }
	friend istream& operator >> (istream& in, pt& p) {
		return in >> p.x >> p.y;
	}
};

struct line { // reta
	pt p, q;
	line() {}
	line(pt p_, pt q_) : p(p_), q(q_) {}
	friend istream& operator >> (istream& in, line& r) {
		return in >> r.p >> r.q;
	}
};

ld norm_sq(pt p){ return p*p;}

ld norm(pt p){ return sqrt(norm_sq(p)); }

ld dist_sq(pt p, pt q){ return norm_sq(p - q);}

ld dist(pt p,pt q){ return sqrt(dist_sq(p, q));}

ld angle(pt v) { // angulo do vetor com o eixo x
	ld ang = atan2(v.y, v.x);
	if (cmp(ang, 0) < 0) ang += 2*pi;
	return ang;
}

ld sarea(pt p, pt q, pt r) { // area com sinal
	return ((q-p)^(r-q))/2;
}

bool col(pt p, pt q, pt r) { // se p, q e r sao colin.
	return cmp(sarea(p, q, r), 0) == 0;
}

bool ccw(pt p, pt q, pt r, bool strict = true) { // se p, q, r estao em sentido anti-horario
	return cmp(sarea(p, q, r)) > 0 || (!strict && cmp(sarea(p, q, r)) == 0);
}

bool isinseg(pt p, line r) { // se p pertence ao seg de r
	pt a = r.p - p, b = r.q - p;
	return cmp(a ^ b) == 0 && cmp(a * b) <= 0;
}

ld polarea(vector<pt>& v) { // area do poligono
	ld ret = 0;
	forn(i, 0, sz(v))
		ret += sarea(pt(0, 0), v[i], v[(i + 1) % sz(v)]);
	return abs(ret);
}

ld get_t(pt v, line r) { // retorna t tal que t*v pertence a reta r
	return (r.p^r.q) / ((r.p-r.q)^v);
}

pt inter(line r, line s) { // r inter s
	if (cmp((r.p - r.q) ^ (s.p - s.q), 0) == 0) return pt(DINF, DINF);
	r.q = r.q - r.p, s.p = s.p - r.p, s.q = s.q - r.p;
	return r.q * get_t(r.q, s) + r.p;
}

// Interseccao de circulos: pontos inexistentes = (DINF, DINF)
// concentricos (mesmo se iguais) -> nenhum ponto
pair<pt, pt> circint(pt o1, ld r1, pt o2, ld r2) {
	pt in(DINF, DINF), dir = o2 - o1;
	ld d = norm(dir);
	if (cmp(d) == 0) return {in, in};
	if (cmp(d, r1 + r2) > 0 || cmp(d, abs(r1 - r2)) < 0) return {in, in};
	ld x = (d*d - r2*r2 + r1*r1) / (2*d);
	pt m = o1 + dir * (x / d);
	if (cmp(d, r1 + r2) == 0 || cmp(d, abs(r1 - r2)) == 0) return {m, in}; // tangentes
	pt k = pt(-dir.y, dir.x) * (sqrt(max((ld)0, r1*r1 - x*x)) / d);
	return {m + k, m - k};
}

// Interseccao de circulo e reta

pt foot(pt p, pt a, pt b) { // projecao de p na reta ab
	pt dir = b - a;
	return (dir * ((p - a) * dir)) / (dir * dir) + a;
}

pair<pt, pt> cline(pt o, ld r, pt a, pt b) {
	pt h = foot(o, a, b), in(DINF, DINF), dir = b - a;
	ld d = norm(h - o);
	if (cmp(d, r) > 0) return {in, in};
	if (cmp(d, r) == 0) return {h, in}; // tangente
	pt k = dir * (sqrt(r*r - d*d) / norm(dir));
	return {h + k, h - k};
}