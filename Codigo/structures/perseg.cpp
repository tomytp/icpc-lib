// Rollback Segment Tree (Min)
//
// Segment tree supporting range min with versioned updates via a change log enabling O(1) rollback per change.
//
// complexity: O(log N) per update/query, O(N + U)

struct node {
    ll lm, rm;
    ll mn;
    unique_ptr<node> lc, rc;

    node(ll l, ll r, const vector<ll>& a) : lm(l), rm(r) {
        if (lm == rm) { 
            mn = a[lm];
             return; 
        }

        ll m = (lm + rm) >> 1;
        lc = make_unique<node>(lm, m, a);
        rc = make_unique<node>(m+1, rm, a);
        pull();
    }

    static ll comb(ll a, ll b) {
        return min(a, b);
    }

    void pull() {
        mn = comb(lc->mn, rc->mn);
    }

    void upd(ll lq, ll rq, ll x, vector<pair<node*,ll>>& log) {
        if (lq > rm || lm > rq) return;
        if (lq <= lm && rm <= rq) {
            if (mn < x) {
                log.emplace_back(this, mn);
                mn = x;
            }
            return;
        }

        lc->upd(lq, rq, x, log);
        rc->upd(lq, rq, x, log);
        
        ll nxt = comb(lc->mn, rc->mn);
        
        if (mn < nxt) {
            log.emplace_back(this, mn);
            mn = nxt;
        }
    }

    ll get(ll lq, ll rq) const {
        if (lq > rm || lm > rq) return INF;
        if (lq <= lm && rm <= rq)  return mn;
        ll res = min(lc->get(lq, rq), rc->get(lq, rq));
        return max(res, mn);
    }
};

struct segtree {
    unique_ptr<node> root;
    vector<pair<node*,ll>> log;

    segtree(const v64& a) {
        root = make_unique<node>(0, (ll)a.size()-1, a);
    }

    void upd(ll l, ll r, ll x){
        root->upd(l, r, x, log);
    }

    ll get(ll l, ll r){
        return root->get(l, r);
    }

    ll version() const {
        return (ll)log.size();
    }

    void rollback(ll ver){
        while ((ll)log.size() > ver){
            auto [p, old] = log.back();
            log.pop_back();
            p->mn = old;
        }
    }
};
