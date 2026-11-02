// Gauss - Z2
//
// Base de vetores em Z2 de dimensao D.
// add(v): adiciona v na base; retorna se v ja estava no span.
// coord(v): coordenadas c de v na base atual (basis^T c = v).
// recover(v): coordenadas de v nos vetores na ordem em que foram inseridos.
// Em coord e recover, .first diz se v esta no span.
//
// complexity: O(D^2 / 64) por add, coord e recover

template<ll D> struct gauss_z2 {
	bitset<D> basis[D], keep[D];
	ll rk, in, dim;
	vll id;
 
	gauss_z2 () : rk(0), in(-1), id(D, -1) {};

    gauss_z2 (ll dim_) : rk(0), in(-1), id(D, -1), dim(dim_) {};

	bool add(bitset<D> v) {
		in++;
		bitset<D> k;
		for (ll i = dim - 1; i >= 0; i--) if (v[i]) {
			if (basis[i][i]) v ^= basis[i], k ^= keep[i];
			else {
				k[i] = true, id[i] = in, keep[i] = k;
				basis[i] = v, rk++;
				return true;
			}
		}
		return false;
	}
	pair<bool, bitset<D>> coord(bitset<D> v) { // private
		bitset<D> c;
		for (ll i = dim - 1; i >= 0; i--) if (v[i]) {
			if (basis[i][i]) v ^= basis[i], c[i] = true;
			else return {false, bitset<D>()};
		}
		return {true, c};
	}
	pair<bool, vll> recover(bitset<D> v) {
		auto [span, bc] = coord(v);
		if (not span) return {false, {}};
		bitset<D> aux;
		for (ll i = dim - 1; i >= 0; i--) if (bc[i]) aux ^= keep[i];
		vll oc;
		for (ll i = dim - 1; i >= 0; i--) if (aux[i]) oc.push_back(id[i]);
		return {true, oc};
	}
};