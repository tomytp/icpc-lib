// ccRadius: calcula o circumraio funciona com pt de int
// ccCentar: calcula o circumcentro funciona somente com pt de double

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