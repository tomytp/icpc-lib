// Circumcircle and Circumradius
//
// ccRadius: raio do circulo circunscrito.
// ccCenter: centro do circulo circunscrito.
// Pontos colineares -> retorna DINF / pt(DINF, DINF).
// Requer geo_double (pt de ld).
//
// complexity: O(1)

pt perp(pt p) { return pt(-p.y, p.x); } // rotaciona 90 graus anti-horario

ld ccRadius(pt a, pt b, pt c) {
	ld s = sarea(a, b, c);
	if (cmp(s) == 0) return DINF;
	return dist(a, b) * dist(b, c) * dist(c, a) / (4 * abs(s));
}

pt ccCenter(pt a, pt b, pt c) {
	b = b - a, c = c - a;
	if (cmp(b ^ c) == 0) return pt(DINF, DINF);
	return a + perp(b * norm_sq(c) - c * norm_sq(b)) / (2 * (b ^ c));
}