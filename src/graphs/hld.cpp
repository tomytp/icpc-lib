// Heavy Light Decomposition (Path query + Path update)
//
// Decomposes a tree into vertex disjoint heavy paths and light edges such that 
// the path from any leaf to the root contains at most log(n) light edges.
// All values initialized to the segtree default. Root must be 0.
//
// head[u]: Highest node (closest to root) of the heavy path containing u.
// pos[u]: Position of node u in the Segment Tree array (DFS order).
// Uses a Lazy Segment Tree  
//
// complexity: O((log N)^2) per op, O(N) build

#include "src/structures/segtree_lazy.cpp"

template <bool VALS_EDGES> struct HLD {
    ll N, tim = 0;
    vector<vll> adj;
    vll parent, siz, head, pos;
    std::unique_ptr<segtree<Node, Update>> seg;
    HLD(vector<vll> adj_)
        : N(sz(adj_)), adj(adj_), parent(N, -1), siz(N, 1),
          head(N),pos(N){ dfsSz(0); dfsHld(0);
            seg = make_unique<segtree<Node, Update>>(N);
        }
    void set_leaves(const vector<Node>& lvs) {
        // if VALS_EDGES lvs[i] is the weight of i -> parent[i]
        vector<Node> leaves(N); 
        forn(i, 0, N) leaves[pos[i]] = lvs[i]; 
        seg->set_leaves(leaves);
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
    void modifyPath(ll u, ll v, Update upd) { 
        process(u, v, [&](ll l, ll r) { 
            seg->update(l, r, upd); 
        });
    }
    Node queryPath(ll u, ll v) { 
        Node res = Node();
        // Non-commutative ops require exact merge order.
        process(u, v, [&](ll l, ll r) {
            res = res*seg->query(l, r);
        });
        return res;
    }
    Node querySubtree(ll v) { 
        return seg->query(pos[v] + VALS_EDGES, pos[v] + siz[v] - 1);
    }
    void modifySubtree(ll v, Update upd) {
        seg->update(pos[v] + VALS_EDGES, pos[v] + siz[v] - 1, upd);
    }
};