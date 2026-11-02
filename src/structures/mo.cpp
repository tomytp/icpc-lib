// Mo's Algorithm (Offline Range Queries)
//
// Responde queries offline em intervalos [l, r] fechados e 0-indexados,
// ordenando-as (Hilbert ou blocos) para mover pouco os ponteiros.
// Preencha o estado do problema, insert(p), erase(p) e ans.
//
// complexity: O((N + Q) sqrt N), O(N + Q)

struct Mo {
    ll n, ans = 0;
    // estado do problema aqui

    Mo(ll n) : n(n) {}

    void insert(ll p) {
    }

    void erase(ll p) {
    }

    ll hilbert(ll x, ll y, ll N) {
        ll d = 0;
        for (ll s = N / 2; s > 0; s /= 2) {
            ll rx = (x & s) > 0, ry = (y & s) > 0;
            d += s * s * ((3 * rx) ^ ry);
            if (ry == 0) {
                if (rx == 1) x = N - 1 - x, y = N - 1 - y;
                swap(x, y);
            }
        }
        return d;
    }

    vll run(vector<pll>& q, bool use_hilbert = true) {
        ll m = sz(q);
        vll ord(m), ret(m);
        iota(ord.begin(), ord.end(), 0);
        if (use_hilbert) {
            ll N = 1ll << (__lg(max(n, 1ll)) + 1);
            vll h(m);
            forn(i, 0, m) h[i] = hilbert(q[i].first, q[i].second, N);
            sort(ord.begin(), ord.end(), [&](ll l, ll r) { return h[l] < h[r]; });
        } 
        if (!use_hilbert) {
            ll SQ = max(1ll, (ll) sqrt(n));
            sort(ord.begin(), ord.end(), [&](ll l, ll r) {
                if (q[l].first / SQ != q[r].first / SQ) return q[l].first < q[r].first;
                if ((q[l].first / SQ) % 2) return q[l].second > q[r].second;
                return q[l].second < q[r].second;
            });
        }
        ll l = 0, r = -1;
        for (ll i : ord) {
            auto [ql, qr] = q[i];
            while (r < qr) insert(++r);
            while (l > ql) insert(--l);
            while (l < ql) erase(l++);
            while (r > qr) erase(r--);
            ret[i] = ans;
        }
        return ret;
    }
};