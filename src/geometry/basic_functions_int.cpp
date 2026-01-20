// Integer Geometry Primitives
//
// Defines 2D point and line structures with orientation, area, and angle comparisons plus a sweep-line comparator.

struct pt { // point
    ll x, y;
    pt(ll x_ = 0, ll y_ = 0) : x(x_), y(y_) {}
    bool operator < (const pt p) const {
        if (x != p.x) return x < p.x;
        return y < p.y;
    }
    bool operator == (const pt p) const {
        return x == p.x and y == p.y;
    }
    pt operator + (const pt p) const { return pt(x+p.x, y+p.y); }
    pt operator - (const pt p) const { return pt(x-p.x, y-p.y); }
    pt operator * (const ll c) const { return pt(x*c, y*c); }
    ll operator * (const pt p) const { return x*(ll)p.x + y*(ll)p.y; } // dot product
    ll operator ^ (const pt p) const { return x*(ll)p.y - y*(ll)p.x; } // cross product
    friend istream& operator >> (istream& in, pt& p) {
        return in >> p.x >> p.y;
    }
};

struct line { // line segment
    pt p, q;

    line() {}
    line(pt p_, pt q_) : p(p_), q(q_) {}
    bool operator < (const line l) const {
        if (!(p == l.p)) return p < l.p;
        return q < l.q;
    }
    bool operator == (const line l) const {
        return p == l.p and q == l.q;
    }
    friend istream& operator >> (istream& in, line& r) {
        return in >> r.p >> r.q;
    }
};

ll sarea2(pt p, pt q, pt r) { // 2 * signed area
    return (q-p)^(r-q);
}

bool ccw(pt p, pt q, pt r) { // true if p, q, r are 
    return sarea2(p, q, r) > 0;
}

int quad(pt p) { // quadrant of a point
    return (p.x<0)^3*(p.y<0);
}

bool compare_angle(pt p, pt q) { // returns true if angle(p) < angle(q)
    if (quad(p) != quad(q)) return quad(p) < quad(q);
    return ccw(q, pt(0, 0), p);
}

bool isinseg(pt p, line r) { // se p pertence ao seg de r
	pt a = r.p - p, b = r.q - p;
	return (a ^ b) == 0 and (a * b) <= 0;
}

bool interseg(line r, line s) { // se o seg de r intersecta o seg de s
	if (isinseg(r.p, s) or isinseg(r.q, s)
		or isinseg(s.p, r) or isinseg(s.q, r)) return 1;

	return ccw(r.p, r.q, s.p) != ccw(r.p, r.q, s.q) and
			ccw(s.p, s.q, r.p) != ccw(s.p, s.q, r.q);
}

ll polarea2(vector<pt> v) { // 2 * area do poligono
	ll ret = 0;
	for (int i = 0; i < v.size(); i++)
		ret += sarea2(pt(0, 0), v[i], v[(i + 1) % v.size()]);
	return abs(ret);
}

// comparator for set to perform sweep line with segments
struct cmp_sweepline {
    bool operator () (const line& a, const line& b) const {
        // assumes segments have p < q
        if (a.p == b.p) return ccw(a.p, a.q, b.q);
        if (a.p.x != a.q.x and (b.p.x == b.q.x or a.p.x < b.p.x))
            return ccw(a.p, a.q, b.p);
        return ccw(a.p, b.q, b.p);
    }
};
