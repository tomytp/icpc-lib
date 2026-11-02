// Segment Tree Over Time (Offline Dynamic Connectivity)
//
// Offline: edges are added/removed over time; DSU can't delete, so each op
// is active in a time interval [l, r] and stored in a segtree over TIME.
// A DFS on it applies ops entering a node and rolls back leaving it, so at
// leaf t the DSU has exactly the ops active at time t.
// Example: add/remove edges, add value to vertex, query component sum.
// Edit time_op, apply and leaf for your problem.
//
// complexity: O(T log T log N)

#include "src/structures/dsu_rb.cpp"

struct time_op { ll type, a, b; }; // type 0: edge (a, b); type 1: add b to vertex a

// node covering times [lm, rm]
struct time_node {
    ll lm, rm;
    unique_ptr<time_node> lc, rc;

    vector<time_op> op; // ops active during the WHOLE [lm, rm]

    time_node(ll lm_, ll rm_){
        lm = lm_;
        rm = rm_;
        if (lm != rm) {
            ll mid = (lm + rm) / 2;
            lc = make_unique<time_node>(lm, mid);
            rc = make_unique<time_node>(mid + 1, rm);
        }
    }

    // op x active in [lq, rq]: stored in O(log T) nodes (like a range update)
    void add_query(ll lq, ll rq, time_op x) {
        if (rq < lm || lq > rm) return;
        if (lq <= lm && rm <= rq) {
            op.push_back(x);
            return;
        }
        lc->add_query(lq, rq, x);
        rc->add_query(lq, rq, x);
    }
};

struct dyn_conn {
    dsu_rb d;
    vll sum, qv, ans; // qv[t] = vertex asked at time t (-1 if none)

    dyn_conn(vll& a, ll T) : d(sz(a)), sum(a), qv(T, -1), ans(T, -1) {}

    // every change must be undoable: d.save(x) BEFORE changing extra data
    void apply(time_op& o) {
        if (o.type == 0) {
            ll ra = d.find(o.a), rb = d.find(o.b);
            if (!d.unite(ra, rb)) return;
            ll r = d.find(ra), other = r == ra ? rb : ra; // new root / absorbed
            d.save(sum[r]), sum[r] += sum[other];
        } else {
            ll r = d.find(o.a);
            d.save(sum[r]), sum[r] += o.b;
        }
    }

    // here the DSU has exactly the ops active at time t
    void leaf(ll t) {
        if (qv[t] != -1) ans[t] = sum[d.find(qv[t])];
    }

    void dfs(time_node& node) {
        ll t = d.time();
        for (time_op& o : node.op) apply(o); // valid for every leaf below
        if (node.lm == node.rm) leaf(node.lm);
        else dfs(*node.lc), dfs(*node.rc);
        d.rollback(t); // sibling subtree must not see these ops
    }
};