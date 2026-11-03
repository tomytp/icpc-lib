// Modular Integer
//
// Fixed-modulus integer type with +, -, *, /, and exponentiation.
// Division and negative exponents use Fermat's little theorem;
// therefore, the modulus must be prime.
//
// Preconditions:
// - p > 1.
// - p is prime for division and negative exponents.
// - p^2 fits in ll.
// - Exponents must not be LLONG_MIN.
//
// complexity:
// - +, -, *: O(1).
// - /: O(log p).
// - ^: O(log |e|).

template<ll p> struct mod_int {
    static_assert(p > 1);
    static_assert((__int128)p * p <= LLONG_MAX);

    ll expo(ll b, ll e) const {
        ll ret = 1;
        while (e) {
            if (e % 2) ret = ret * b % p;
            e /= 2, b = b * b % p;
        }
        return ret;
    }

    ll inv(ll b) const {
        assert(b != 0);
        return expo(b, p-2);
    }

    using m = mod_int;
    ll v;

    mod_int() : v(0) {}

    mod_int(ll v_) {
        if (v_ >= p || v_ <= -p) v_ %= p;
        if (v_ < 0) v_ += p;
        v = v_;
    }

    m& operator +=(const m& a) {
        v += a.v;
        if (v >= p) v -= p;
        return *this;
    }

    m& operator -=(const m& a) {
        v -= a.v;
        if (v < 0) v += p;
        return *this;
    }

    m& operator *=(const m& a) {
        v = v * a.v % p;
        return *this;
    }

    m& operator /=(const m& a) {
        assert(a.v != 0);
        v = v * inv(a.v) % p;
        return *this;
    }

    m operator -() const {
        return m(-v);
    }

    m& operator ^=(ll e) {
        assert(e != LLONG_MIN);
        if (e < 0) {
            v = inv(v);
            e = -e;
        }
        v = expo(v, e);
        return *this;
    }

    bool operator ==(const m& a) const {
        return v == a.v;
    }

    bool operator !=(const m& a) const {
        return v != a.v;
    }

    friend istream& operator >>(istream& in, m& a) {
        ll val;
        in >> val;
        a = m(val);
        return in;
    }

    friend ostream& operator <<(ostream& out, m a) {
        return out << a.v;
    }

    friend m operator +(m a, m b) { return a += b; }
    friend m operator -(m a, m b) { return a -= b; }
    friend m operator *(m a, m b) { return a *= b; }
    friend m operator /(m a, m b) { return a /= b; }
    friend m operator ^(m a, ll e) { return a ^= e; }
};