// Circumcircle and Circumradius
//
// ccRadius: raio do circulo circunscrito; funciona com pt de int.
// ccCenter: centro do circulo circunscrito; so funciona com pt de double.
//
// complexity: O(1)

ld ccRadius(pt a, pt b, pt c) {
    ld s = sarea(a,b,c);
    if(s == 0) return DINF;
	return dist(a,b)*dist(b,c)*dist(c,a)/(4*abs(s));
}

pt ccCenter(pt a, pt b, pt c) {
	c = c-a, b = b-a;
    if((b^c) == 0) return {DINF,DINF};
	return a + perp((b*norm_sq(c)-c*norm_sq(b)))/(2*(b^c));
}