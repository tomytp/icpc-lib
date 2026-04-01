// Heavy Light Decomposition (Path query + Path update)
//
// Decomposes a tree into vertex disjoint heavy paths and light edges such that 
// the path from any leaf to the root contains at most log(n) light edges.
// All values initialized to the segtree default. Root must be 0.
//
// Uses a Lazy Segment Tree  
//
// complexity: O((log N)^2) per op, O(N) build

#include "src/structures/segtree_lazy.cpp"

template <bool VALS_EDGES> struct HLD {
    ll N, tim = 0;
    vector<vll> adj;
    vll parent, siz, head, pos;
    vector<Node> vseg;
    std::unique_ptr<segtree<Node, Update>> seg;
    HLD(vector<vll> adj_, vll vals)
        : N(sz(adj_)), adj(adj_), parent(N, -1), siz(N, 1),
          head(N),pos(N),vseg(N, {0}){ dfsSz(0); dfsHld(0);
            seg = make_unique<segtree<Node, Update>>(N);
            seg->set_leaves(vseg);
        }
    void dfsSz(ll v) { // get heavy son
        for (ll& u : adj[v]) {
            adj[u].erase(find(adj[u].begin(), adj[u].end(), v));
            parent[u] = v;
            dfsSz(u);
            siz[v] += siz[u];
            if (siz[u] > siz[adj[v][0]]) swap(u, adj[v][0]);
        }
    }
    void dfsHld(ll v) { // "linearizes" the tree
        pos[v] = tim++;
        for (ll u : adj[v]) {
            head[u] = (u == adj[v][0] ? head[v] : u);
            dfsHld(u);
        }
    }
    template <class B> void process(ll u, ll v, B op) {
        for (;; v = parent[head[v]]) {
            if (pos[head[u]] > pos[head[v]]) swap(u, v);
            if (head[u] == head[v]) break;
            op(pos[head[v]], pos[v]);
        }
        if (pos[u] > pos[v]) swap(u, v);
        op(pos[u] + VALS_EDGES, pos[v]);
    }
    void modifyPath(ll u, ll v, ll val) { 
        process(u, v, [&](ll l, ll r) { 
            seg->update(l, r, {val}); // Modify depending on problem 
        });
    }
    ll queryPath(ll u, ll v) { // Modify depending on problem
        ll res = -INF;
        process(u, v, [&](ll l, ll r) {
                res = max(res, seg->query(l, r).val);
        });
        return res;
    }
    ll querySubtree(ll v) { // modifySubtree is similar
        return seg->query(pos[v] + VALS_EDGES, pos[v] + siz[v] - 1).val;
    }
};