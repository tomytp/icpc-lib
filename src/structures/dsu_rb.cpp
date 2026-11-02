// DSU with Rollback
//
// Union-Find sem path compression para permitir desfazer uniões.
// t = time() salva um checkpoint; rollback(t) desfaz tudo depois dele.
// len[x] so e valido se x for raiz.
//
// complexity: O(log N) find/unite, O(1) amortizado por operacao desfeita

struct dsu_rb {
    vll id, len;
    stack<pair<ll&, ll>> st;

    dsu_rb(ll n) : id(n), len(n, 1) { iota(id.begin(), id.end(), 0); }

    void save(ll& x) { st.emplace(x, x); }

    ll time() { return sz(st); }

    void rollback(ll t) {
        while (sz(st) > t) {
            auto [ref, val] = st.top(); st.pop();
            ref = val;
        }
    }

    ll find(ll a) { return a == id[a] ? a : find(id[a]); }

    bool unite(ll a, ll b) {
        a = find(a), b = find(b);
        if (a == b) return false;
        if (len[a] < len[b]) swap(a, b);
        save(len[a]), save(id[b]);
        len[a] += len[b], id[b] = a;
        return true;
    }
};