
// NTT Convolution
//
// Implements iterative NTT over supported primes.
// Supports convolution of two vectors.
//
// Complexity: O(N log N) time, O(N) memory.

#include "src/math/mint.cpp" // for nnt

template<ll p>
void get_roots(bool f, ll n, vector<mod_int<p>>& roots) {
    mod_int<p> r;
    ll ord;

    if (p == 998244353) {
        r = 102292;
        ord = (1LL << 23);
    } else if (p == 754974721) {
        r = 739831874;
        ord = (1LL << 24);
    } else if (p == 167772161) {
        r = 243;
        ord = (1LL << 25);
    } else {
        assert(false);
    }

    assert(n <= ord && ord % n == 0);

    if (f) r = r^(p - 1 - ord/n);
    else r = r^(ord/n);

    roots[0] = 1;
    forn(i,1,n/2) roots[i] = roots[i-1]*r;
}

template<typename T>
void fft(vector<T>& a, bool f, ll N, vector<ll>& rev) {
    assert(N > 0 && (N & (N-1)) == 0);
    assert(sz(a) == N && sz(rev) == N);

    forn(i,0,N) if (i < rev[i]) swap(a[i], a[rev[i]]);

    ll l, r, m;
    vector<T> roots(N);

    for (ll n = 2; n <= N; n *= 2) {
        get_roots(f, n, roots);

        for (ll pos = 0; pos < N; pos += n) {
            l = pos, r = pos + n/2, m = 0;

            while (m < n/2) {
                auto t = roots[m]*a[r];
                a[r] = a[l]-t;
                a[l] = a[l]+t;
                l++, r++, m++;
            }
        }
    }

    if (f) {
        auto invN = T(1)/T(N);
        forn(i,0,N) a[i] *= invN;
    }
}

template<typename T>
vector<T> convolution(vector<T> l, vector<T> r) {
    if (l.empty() || r.empty()) return {};

    ll N = sz(l)+sz(r)-1;
    ll n = 1, log_n = 0;

    while (n < N) n *= 2, log_n++;

    vector<ll> rev(n);

    forn(i,0,n) {
        rev[i] = 0;
        forn(j,0,log_n) if (i>>j&1)
            rev[i] |= 1LL << (log_n-1-j);
    }

    l.resize(n);
    r.resize(n);

    fft(l, false, n, rev);
    fft(r, false, n, rev);

    forn(i,0,n) l[i] *= r[i];

    fft(l, true, n, rev);

    l.resize(N);
    return l;
}

// NTT: converts the input vectors to mod_int<p>.
template<ll p, typename T>
vector<mod_int<p>> ntt(const vector<T>& a, const vector<T>& b) {
    vector<mod_int<p>> A(a.begin(), a.end());
    vector<mod_int<p>> B(b.begin(), b.end());

    return convolution(move(A), move(B));
}
