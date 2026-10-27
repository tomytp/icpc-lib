// Merge Sort Tree (2D points, O(log n) queries)
//
// Built on points (x, y). Leaves are the points sorted by (x, y, id); each node
// keeps its points sorted by (y, id) plus go_left[k] = how many of its first k
// points came from the left child. A y-range is binary searched once at the
// root and then carried down as a block [a, b) of each node's list in O(1).
//
// count(x1, x2, y1, y2)  -> # points with x in [x1, x2] and y in [y1, y2]
// report(x1, x2, y1, y2) -> their ids (any order)
// kth(y1, y2, k)         -> id of the point with the k-th smallest x (0-based,
//                           ties by (x, y, id)) among points with y in [y1, y2];
//                           -1 if there are fewer than k+1
// ids are positions in the input vector.
//
// On an array v:
//   merge_sort_tree T(v)       -> points (i, v[i]): count(i, j, a, b) = # p in [i, j]
//                                 with v[p] in [a, b]; report gives those positions
//   merge_sort_tree T(v, true) -> points (v[i], i): kth(l, r, k) = position of the
//                                 k-th smallest value of v[l..r] (value: v[kth(...)])
//
// complexity: O(n log n) build and mem, O(log n) count / kth, O(log n + k) report

struct merge_sort_tree {
    ll n;
    vll xs, ys;            // coordinates of point i
    vll order;             // point ids sorted by (x, y, id): leaf order
    vll sorted_x;          // sorted_x[i] = x of order[i]; node [l, r] has x in [sorted_x[l], sorted_x[r]]
    vll all_y;             // every y, sorted (the root's list)
    vector<vll> ids;       // ids[v]: points of node v, sorted by (y, id)
    vector<vll> go_left;   // go_left[v][k]: how many of ids[v][0..k) came from the left child

    merge_sort_tree(const vector<pll>& pts) : n(sz(pts)), xs(n), ys(n), order(n),
        sorted_x(n), ids(4 * n + 4), go_left(4 * n + 4) {
        forn(i, 0, n) xs[i] = pts[i].first, ys[i] = pts[i].second;
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](ll a, ll b) {
            return tie(xs[a], ys[a], a) < tie(xs[b], ys[b], b);
        });
        forn(i, 0, n) sorted_x[i] = xs[order[i]];
        if (n > 0) build(1, 0, n - 1);
        for (ll id : ids[1]) all_y.push_back(ys[id]);
    }

    static vector<pll> array_points(const vll& v, bool by_value) {
        vector<pll> pts(sz(v));
        forn(i, 0, sz(v)) pts[i] = by_value ? pll(v[i], i) : pll(i, v[i]);
        return pts;
    }
    merge_sort_tree(const vll& v, bool by_value = false)
        : merge_sort_tree(array_points(v, by_value)) {}

    void build(ll v, ll l, ll r) {  // node v covers order[l..r]
        if (l == r) {               // leaf: go_left is never read here
            ids[v] = {order[l]};
            return;
        }
        ll m = (l + r) / 2;
        build(2 * v, l, m), build(2 * v + 1, m + 1, r);

        const vll& left = ids[2 * v];
        const vll& right = ids[2 * v + 1];
        ids[v].reserve(r - l + 1);
        go_left[v].reserve(r - l + 2);
        go_left[v].push_back(0);
        ll i = 0, j = 0;
        while (i < sz(left) || j < sz(right)) {   // merge by (y, id)
            bool take_left = j == sz(right) ||
                (i < sz(left) && pll(ys[left[i]], left[i]) < pll(ys[right[j]], right[j]));
            ids[v].push_back(take_left ? left[i++] : right[j++]);
            go_left[v].push_back(go_left[v].back() + take_left);
        }
    }

    ll first_y(ll y) { return lower_bound(all_y.begin(), all_y.end(), y) - all_y.begin(); }
    ll past_y(ll y) { return upper_bound(all_y.begin(), all_y.end(), y) - all_y.begin(); }

    // In every function below, ids[v][a..b) are exactly node v's points with y in range.
    // The left child gets [go_left[a], go_left[b]), the right gets the rest.

    ll count_dfs(ll v, ll l, ll r, ll a, ll b, ll x1, ll x2) {
        if (a == b || x2 < sorted_x[l] || sorted_x[r] < x1) return 0;
        if (x1 <= sorted_x[l] && sorted_x[r] <= x2) return b - a;
        ll m = (l + r) / 2, la = go_left[v][a], lb = go_left[v][b];
        return count_dfs(2 * v, l, m, la, lb, x1, x2)
             + count_dfs(2 * v + 1, m + 1, r, a - la, b - lb, x1, x2);
    }
    ll count(ll x1, ll x2, ll y1, ll y2) {
        return count_dfs(1, 0, n - 1, first_y(y1), past_y(y2), x1, x2);
    }

    void report_dfs(ll v, ll l, ll r, ll a, ll b, ll x1, ll x2, vll& out) {
        if (a == b || x2 < sorted_x[l] || sorted_x[r] < x1) return;
        if (x1 <= sorted_x[l] && sorted_x[r] <= x2) {
            forn(k, a, b) out.push_back(ids[v][k]);
            return;
        }
        ll m = (l + r) / 2, la = go_left[v][a], lb = go_left[v][b];
        report_dfs(2 * v, l, m, la, lb, x1, x2, out);
        report_dfs(2 * v + 1, m + 1, r, a - la, b - lb, x1, x2, out);
    }
    vll report(ll x1, ll x2, ll y1, ll y2) {
        vll out;
        report_dfs(1, 0, n - 1, first_y(y1), past_y(y2), x1, x2, out);
        return out;
    }

    ll kth(ll y1, ll y2, ll k) {
        ll v = 1, a = first_y(y1), b = past_y(y2);
        if (k < 0 || k >= b - a) return -1;
        while (b - a > 1) {         // the left child holds the smaller x
            ll la = go_left[v][a], lb = go_left[v][b];
            if (k < lb - la) v = 2 * v, a = la, b = lb;
            else k -= lb - la, v = 2 * v + 1, a -= la, b -= lb;
        }
        return ids[v][a];
    }
};