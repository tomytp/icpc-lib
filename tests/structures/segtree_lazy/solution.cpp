// Test: structures/segtree_lazy (range add, range set, range sum)

#include "src/extra/template.cpp"
#include "src/structures/segtree_lazy.cpp"

int main() {
    _;
    ll n, q; cin >> n >> q;
    vector<Node> leaves(n);
    forn(i, 0, n) {
        cin >> leaves[i].sum;
        leaves[i].cnt = 1;
    }
    segtree<Node, Update> st(n);
    st.set_leaves(leaves);
    while (q--) {
        ll t; cin >> t;
        if (t == 1) {
            ll l, r, v; cin >> l >> r >> v;
            st.update(l, r, Update{v});
        } else if (t == 2) {
            ll l, r, v; cin >> l >> r >> v;
            st.update(l, r, Update{0, v});
        } else {
            ll l, r; cin >> l >> r;
            cout << st.query(l, r).sum << ln;
        }
    }
}
