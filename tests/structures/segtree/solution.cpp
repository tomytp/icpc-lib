#include "src/extra/template.cpp"
#include "src/structures/segtree.cpp"

int main() {
    _;
    ll n, q; cin >> n >> q;
    vector<node> leaves(n);
    forn (i, 0, n) cin >> leaves[i].val;
    segtree<node> st(n);
    st.set_leaves(leaves);
    while (q--) {
        ll t; cin >> t;
        if (t == 1) {
            ll i, v; cin >> i >> v;
            st.update(i, {v});
        } else {
            ll l, r; cin >> l >> r;
            cout << st.query(l, r).val << ln;
        }
    }
}
