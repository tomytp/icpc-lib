// SegTree 2D Iterativa
//
// Consultas 0-based
// seg2d<node> S(n, init); init[x][y] = valor inicial em (x, y)
// Query: comb do retangulo ((x1, y1), (x2, y2))
// Update: muda o valor da posicao (x, y) para val
// node::comb precisa ser associativa E comutativa (soma, min, max, xor, gcd...),
// a ordem dos elementos nao e preservada
// Nao pergunte como que essa coisa funciona
//
// Para query com distancia de manhattan <= d, faca
// nx = x+y, ny = x-y
// Update em (nx, ny), query em ((nx-d, ny-d), (nx+d, ny+d))
//
// Complexidades:
// build - O(n^2)
// query - O(log^2(n))
// update - O(log^2(n))

struct node {
	ll val = 0;

	static node comb(const node& a, const node& b) {
		return {a.val + b.val};
	}
};

template<typename T> struct seg2d{
	ll n;
	T neutral;
	vector<vector<T>> seg;

	seg2d(ll n_, vector<vector<T>>& init, T neutral_ = T()) : n(n_), neutral(neutral_),
		seg(2 * (n+1), vector<T>(2 * (n+1), neutral)) {
		forn(i,0,n) forn(j,0,n)
			seg[i + n][j + n] = init[i][j];
		for (ll x = 2*n; x; x--) for (ll y = 2*n; y; y--) {
			if (x < n) seg[x][y] = T::comb(seg[2*x][y], seg[2*x+1][y]);
			if (y < n) seg[x][y] = T::comb(seg[x][2*y], seg[x][2*y+1]);
		}
	}
	
	T query(ll x1, ll y1, ll x2, ll y2) {
		T ret = neutral;
		ll y3 = y1 + n, y4 = y2 + n;
		for (x1 += n, x2 += n; x1 <= x2; ++x1 /= 2, --x2 /= 2)
		for (y1 = y3, y2 = y4; y1 <= y2; ++y1 /= 2, --y2 /= 2) {
			if (x1%2 == 1 && y1%2 == 1) ret = T::comb(ret, seg[x1][y1]);
			if (x1%2 == 1 && y2%2 == 0) ret = T::comb(ret, seg[x1][y2]);
			if (x2%2 == 0 && y1%2 == 1) ret = T::comb(ret, seg[x2][y1]);
			if (x2%2 == 0 && y2%2 == 0) ret = T::comb(ret, seg[x2][y2]);
		}
		return ret;
	}
	
	void update(ll x, ll y, T val) {
		ll y2 = y += n;
		for (x += n; x; x /= 2, y = y2) {
			if (x >= n) seg[x][y] = val;
			else seg[x][y] = T::comb(seg[2*x][y], seg[2*x+1][y]);
			
			while (y /= 2) seg[x][y] = T::comb(seg[x][2*y], seg[x][2*y+1]);
		}
	}
};