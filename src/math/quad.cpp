// Quadratic Field Extension (Z_p[sqrt(D)])
//
// Extension for numbers of form x + y*sqrt(D) with +, -, *, /, and exponentiation.
//
// complexity: O(1) per arithmetic op, O(log MOD) for division/inversion.

template <long long D> 
struct quad {
    mint x, y;
    using m = quad<D>; 

    quad() : x(0), y(0) {}
    quad(ll x_) : x(x_), y(0) {}
    quad(mint x_, mint y_) : x(x_), y(y_) {}

    m& operator += (const m& a) {
        x += a.x; y += a.y; return *this;
    }
    m& operator -= (const m& a) {
        x -= a.x; y -= a.y; return *this;
    }
    
    m& operator *= (const m& a) {
        mint old_x = x;
        x = x * a.x + mint(D) * y * a.y; 
        y = old_x * a.y + y * a.x; 
        return *this;
    }

    m inv() const {
        mint dem = x * x - mint(D) * y * y; 
        return m(x / dem, -y / dem);
    }
    
    m& operator /=(const m& a) {
        *this *= a.inv(); 
        return *this;           
    }
    
    m expo(m b, ll e){
        m ret = m(1, 0);
        while (e) {
            if (e % 2) ret = ret * b;
            b = b * b;
            e /= 2;
        }
        return ret;
    }

    m& operator ^=(ll e) {
        if (e < 0) {
            *this = this->inv(); 
            e = -e;
        }
        *this = expo(*this, e);
        return *this;
    }
    
    bool operator ==(const m& a){ return (x == a.x) && (y == a.y); }
    bool operator !=(const m& a){ return (x != a.x) || (y != a.y); }
    
    friend ostream& operator <<(ostream& out, const m& a) {
        return out << a.x << ":" << a.y;
    }

    friend m operator +(m a, const m& b) { return a += b; }
    friend m operator -(m a, const m& b) { return a -= b; }
    friend m operator *(m a, const m& b) { return a *= b; }
    friend m operator /(m a, const m& b) { return a /= b; }
    friend m operator ^(m a, ll b) { return a ^= b; }
};