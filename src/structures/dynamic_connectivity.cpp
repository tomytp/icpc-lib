// Segment Tree Over Time (Dynamic Connectivity Skeleton)
//
// Stores edge activation intervals in a segment tree over time to enable offline dynamic connectivity with rollback DSU.

struct time_query{
    ll l, r;
    time_query(ll l_, ll r_){
        l = l_;
        r = r_;
    }
};

struct time_node {
    ll lm, rm;
    unique_ptr<time_node> lc, rc;

    vector<time_query> op;

    time_node(ll lm_, ll rm_){
        lm = lm_;
        rm = rm_;
        if (lm != rm) {
            ll mid = (lm + rm) / 2;
            lc = make_unique<time_node>(lm, mid);
            rc = make_unique<time_node>(mid + 1, rm);
        }
    }

    void add_query(ll lq, ll rq, time_query x) {
        if (rq < lm || lq > rm) return;
        if (lq <= lm && rm <= rq) {
            op.push_back(x);
            return;
        }
        lc->add_query(lq, rq, x);
        rc->add_query(lq, rq, x);
    }
};
