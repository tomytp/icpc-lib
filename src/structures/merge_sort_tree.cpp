// Merge Sort Tree (2D Points)
//
// Static points (x, y): count / list the points in a rectangle and find the
// k-th smallest x among points with y in a range. Leaves are the points sorted
// by x; each node keeps its points sorted by y plus go_left[k] = how many of
// its first k came from the left child, so y is binary searched only once, at
// the root. ids are positions in the input vector. On an array v: T(v) uses
// points (i, v[i]); T(v, true) uses (v[i], i).
//
// complexity: O(n log n) build and mem, O(log n) count / kth, O(log n + k) report

struct merge_sort_tree {
    ll n;
    vll xs, ys;          // point i is (xs[i], ys[i])
    vll order;           // ids sorted by (x, y, id): leaves
    vll sorted_x;        // sorted_x[i] = xs[order[i]]
    vll all_y;           // every y, sorted
    vector<vll> ids;     // node's ids, sorted by (y, id)
    vector<vll> go_left; // # of ids[v][0..k) from left child

    merge_sort_tree(const vector<pll>& pts) : n(sz(pts)),
        xs(n), ys(n), order(n), sorted_x(n),
        ids(4 * n + 4), go_left(4 * n + 4) {
        forn(i, 0, n) xs[i] = pts[i].first;
        forn(i, 0, n) ys[i] = pts[i].second;
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](ll a, ll b) {
            return tie(xs[a], ys[a], a) < tie(xs[b], ys[b], b);
        });
        forn(i, 0, n) sorted_x[i] = xs[order[i]];
        if (n > 0) build(1, 0, n - 1);
        for (ll id : ids[1]) all_y.push_back(ys[id]);
    }

    // array v: by_value = false -> points (i, v[i])
    //          by_value = true  -> points (v[i], i)
    static vector<pll> to_points(const vll& v, bool by_value) {
        vector<pll> pts(sz(v));
        forn(i, 0, sz(v)) {
            if (by_value) pts[i] = {v[i], i};
            else pts[i] = {i, v[i]};
        }
        return pts;
    }
    merge_sort_tree(const vll& v, bool by_value = false)
        : merge_sort_tree(to_points(v, by_value)) {}

    // node v covers order[l..r]
    void build(ll v, ll l, ll r) {
        if (l == r) {  // leaves never read go_left
            ids[v] = {order[l]};
            return;
        }
        ll m = (l + r) / 2;
        build(2 * v, l, m), build(2 * v + 1, m + 1, r);

        const vll& L = ids[2 * v];
        const vll& R = ids[2 * v + 1];
        ids[v].reserve(r - l + 1);
        go_left[v].reserve(r - l + 2);
        go_left[v].push_back(0);
        ll i = 0, j = 0;
        while (i < sz(L) || j < sz(R)) {  // merge by (y, id)
            bool from_left = j == sz(R) || (i < sz(L) &&
                pll(ys[L[i]], L[i]) < pll(ys[R[j]], R[j]));
            ids[v].push_back(from_left ? L[i++] : R[j++]);
            go_left[v].push_back(go_left[v].back() + from_left);
        }
    }

    // # of y < y0 / # of y <= y0
    ll first_y(ll y0) {
        return lower_bound(all_y.begin(), all_y.end(), y0)
            - all_y.begin();
    }
    ll past_y(ll y0) {
        return upper_bound(all_y.begin(), all_y.end(), y0)
            - all_y.begin();
    }

    // ids[v][a..b) = node v's points with y in range;
    // left child: [go_left[a], go_left[b]), right: the rest
    ll count_dfs(ll v, ll l, ll r, ll a, ll b, ll x1, ll x2) {
        ll lo = sorted_x[l], hi = sorted_x[r];
        if (a == b || x2 < lo || hi < x1) return 0;
        if (x1 <= lo && hi <= x2) return b - a;
        ll m = (l + r) / 2;
        ll la = go_left[v][a], lb = go_left[v][b];
        ll ra = a - la, rb = b - lb;
        return count_dfs(2 * v, l, m, la, lb, x1, x2)
            + count_dfs(2 * v + 1, m + 1, r, ra, rb, x1, x2);
    }
    // # of points with x in [x1, x2] and y in [y1, y2]
    // array T(v): # of p in [x1, x2] with v[p] in [y1, y2]
    ll count(ll x1, ll x2, ll y1, ll y2) {
        ll a = first_y(y1), b = past_y(y2);
        return count_dfs(1, 0, n - 1, a, b, x1, x2);
    }

    void report_dfs(ll v, ll l, ll r, ll a, ll b,
                    ll x1, ll x2, vll& out) {
        ll lo = sorted_x[l], hi = sorted_x[r];
        if (a == b || x2 < lo || hi < x1) return;
        if (x1 <= lo && hi <= x2) {
            forn(k, a, b) out.push_back(ids[v][k]);
            return;
        }
        ll m = (l + r) / 2;
        ll la = go_left[v][a], lb = go_left[v][b];
        ll ra = a - la, rb = b - lb;
        report_dfs(2 * v, l, m, la, lb, x1, x2, out);
        report_dfs(2 * v + 1, m + 1, r, ra, rb, x1, x2, out);
    }
    // ids of the points counted by count(), in any order
    vll report(ll x1, ll x2, ll y1, ll y2) {
        vll out;
        ll a = first_y(y1), b = past_y(y2);
        report_dfs(1, 0, n - 1, a, b, x1, x2, out);
        return out;
    }

    // id with the k-th (0-based) smallest x among points with
    // y in [y1, y2], ties by (x, y, id); -1 if not enough points
    // array T(v, true): position of k-th smallest in v[y1..y2]
    ll kth(ll y1, ll y2, ll k) {
        ll v = 1, a = first_y(y1), b = past_y(y2);
        if (k < 0 || k >= b - a) return -1;
        while (b - a > 1) {  // left child has the smaller x
            ll la = go_left[v][a], lb = go_left[v][b];
            if (k < lb - la) {
                v = 2 * v, a = la, b = lb;
            } else {
                k -= lb - la;
                v = 2 * v + 1, a -= la, b -= lb;
            }
        }
        return ids[v][a];
    }
};