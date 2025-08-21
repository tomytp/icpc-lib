// FFT/NTT Convolution
//
// Implements iterative FFT over complex numbers and NTT over supported primes; provides convolution utility.
//
// complexity: O(N log N), O(N)

// Para FFT
void get_roots(bool f, int n, vector<complex<double>>& roots) {
	const static double PI = acosl(-1);
	for (int i = 0; i < n/2; i++) {
		double alpha = i*((2*PI)/n);
		if (f) alpha = -alpha;
		roots[i] = {cos(alpha), sin(alpha)};
	}
}

// Para NTT
template<int p>
void get_roots(bool f, int n, vector<mod_int<p>>& roots) {
	mod_int<p> r;
	int ord;
	if (p == 998244353) {
		r = 102292;
		ord = (1 << 23);
	} else if (p == 754974721) {
		r = 739831874;
		ord = (1 << 24);
	} else if (p == 167772161) {
		r = 243;
		ord = (1 << 25);
	} else assert(false);

	if (f) r = r^(p - 1 -ord/n);
	else r = r^(ord/n);
	roots[0] = 1;
	for (int i = 1; i < n/2; i++) roots[i] = roots[i-1]*r;
}

template<typename T> void fft(vector<T>& a, bool f, int N, vector<int>& rev) {
	for (int i = 0; i < N; i++) if (i < rev[i]) swap(a[i], a[rev[i]]);
	int l, r, m;
	vector<T> roots(N);
	for (int n = 2; n <= N; n *= 2) {
		get_roots(f, n, roots);
		for (int pos = 0; pos < N; pos += n) {
			l = pos + 0, r = pos + n/2, m = 0;
			while (m < n/2) {
				auto t = roots[m] * a[r];
				a[r] = a[l] - t;
				a[l] = a[l] + t;
				l++, r++, m++;
			}
		}
	}
	if (f) {
		auto invN = T(1) / T(N);
		for (int i = 0; i < N; i++) a[i] = a[i] * invN;
	}
}

template<typename T> vector<T> convolution(vector<T>& a, vector<T>& b) {
	vector<T> l(a.begin(), a.end()), r(b.begin(), b.end());
	int N = l.size()+r.size()-1;
	int n = 1, log_n = 0;
	while (n <= N) n *= 2, log_n++;
	vector<int> rev(n);
	for (int i = 0; i < n; i++) {
		rev[i] = 0;
		for (int j = 0; j < log_n; j++) if (i>>j&1)
			rev[i] |= 1 << (log_n-1-j);
	}

	assert(N <= n);
	l.resize(n);
	r.resize(n);
	fft(l, false, n, rev);
	fft(r, false, n, rev);
	for (int i = 0; i < n; i++) l[i] *= r[i];
	fft(l, true, n, rev);
	l.resize(N);
	return l;
}

// NTT
template<int p, typename T> 
vector<mod_int<p>> ntt(vector<T>& a, vector<T>& b) {
	vector<mod_int<p>> A(a.begin(), a.end()), B(b.begin(), b.end());
	return convolution(A, B);
}
