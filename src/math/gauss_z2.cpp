// Gauss - Z2
//
// D eh dimensao do espaco vetorial
// add(v) - adiciona o vetor v na base (retorna se ele jah pertencia ao span da base)
// coord(v) - retorna as coordenadas (c) de v na base atual (basis^T.c = v)
// recover(v) - retorna as coordenadas de v nos vetores na ordem em que foram inseridos
// coord(v).first e recover(v).first - se v pertence ao span
//
// Complexidade:
// add, coord, recover: O(D^2 / 64)

template<ll D> struct gauss_z2 {
	bitset<D> basis[D], keep[D];
	ll rk, in, dim;
	v64 id;
 
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
	pair<bool, v64> recover(bitset<D> v) {
		auto [span, bc] = coord(v);
		if (not span) return {false, {}};
		bitset<D> aux;
		for (ll i = dim - 1; i >= 0; i--) if (bc[i]) aux ^= keep[i];
		v64 oc;
		for (ll i = dim - 1; i >= 0; i--) if (aux[i]) oc.push_back(id[i]);
		return {true, oc};
	}
};