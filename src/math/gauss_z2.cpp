// Gauss - Z2
//
// D eh a dimensao maxima do espaco vetorial.
//
// add(v): insere v na base.
//         Retorna true se v eh independente dos vetores anteriores.
//
// coord(v): retorna as coordenadas de v na base atual.
//           basis^T * c = v, considerando soma XOR.
//
// recover(v): retorna os indices dos vetores originalmente inseridos
//             cuja soma XOR eh v. Os indices sao 0-based e ordenados.
//
// coord(v).first e recover(v).first indicam se v pertence ao span.
//
// Complexity:
// add, coord, recover: O(D^2 / 64)
// Memory: O(D^2 / 64)
 
template<ll D>
struct gauss_z2 {
    bitset<D> basis[D], keep[D];
    ll rk, in, dim;
    vll id;
 
    gauss_z2() : rk(0), in(-1), dim(D), id(D, -1) {}
 
    gauss_z2(ll dim_) : rk(0), in(-1), dim(dim_), id(D, -1) {
        assert(0 <= dim && dim <= D);
    }
 
    void check_dim(const bitset<D>& v) const {
        forn(i,dim,D) assert(!v[i]);
    }
 
    bool add(bitset<D> v) {
        check_dim(v);
        in++;
 
        bitset<D> k;
 
        for (ll i = dim-1; i >= 0; i--) if (v[i]) {
            if (basis[i][i]) {
                v ^= basis[i];
                k ^= keep[i];
            } else {
                k[i] = true;
                id[i] = in;
                keep[i] = k;
                basis[i] = v;
                rk++;
                return true;
            }
        }
 
        return false;
    }
 
    pair<bool, bitset<D>> coord(bitset<D> v) const {
        check_dim(v);
 
        bitset<D> c;
 
        for (ll i = dim-1; i >= 0; i--) if (v[i]) {
            if (basis[i][i]) {
                v ^= basis[i];
                c[i] = true;
            } else {
                return {false, bitset<D>()};
            }
        }
 
        return {true, c};
    }
 
    pair<bool, vll> recover(bitset<D> v) const {
        auto [span, bc] = coord(v);
 
        if (!span) return {false, {}};
 
        bitset<D> aux;
 
        for (ll i = dim-1; i >= 0; i--)
            if (bc[i]) aux ^= keep[i];
 
        vll oc;
 
        for (ll i = dim-1; i >= 0; i--)
            if (aux[i]) oc.push_back(id[i]);
 
        sort(oc.begin(), oc.end());
 
        return {true, oc};
    }
};