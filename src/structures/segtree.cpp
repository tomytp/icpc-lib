// Segment Tree (Range Query + Point Update)
//
// segtree<node> S(n); set_leaves(v), update(i, v), query(l, r) on [l, r].
// node::comb is static and only needs to be associative (order is preserved).
// max_right(l, x): smallest r >= l with pred(comb(a[l..r]), x), or n if none.
//   pred is a member of segtree (edit it); default: sum > x (values >= 0).
//   ex: first prefix with sum > x -> max_right(0, x)
// Size is rounded up to a power of 2 (max_right needs it).
//
// complexity: O(log N) per op, O(N) mem

struct node {
    ll val = 0;

    static node comb(const node& a, const node& b) {
        return {a.val + b.val};
    }
};

template<typename T> struct segtree {
  ll n, s;
  T neutral;
  vector<T> tree;

  segtree(ll _n, T _neutral = T()) {
    n = _n, neutral = _neutral;
    for (s = 1; s < n; ) s *= 2;
    tree.assign(2*s, neutral);
  }

  void set_leaves(vector<T> &leaves) {
    copy(leaves.begin(), leaves.end(), tree.begin() + s);

    for (ll i = s - 1; i > 0; i--)
      tree[i] = T::comb(tree[2 * i], tree[2 * i + 1]);
  }

  // for SET segtree
  // for (i += s; i; i /= 2) 
  //    tree[i].erase(tree[i].find(old)), tree[i].insert(new);
  void update(ll i, T v) {
    i += s;
    tree[i] = v;
    while (i > 1) {
      i /= 2;
      tree[i] = T::comb(tree[2 * i], tree[2 * i + 1]);
    }
  }
  // for SET segtree, same loop (ex: smallest >= k)
  //    if((i&1) == 1) ans = min(ans, *tree[i++].lower_bound(k));
  //    if((j&1) == 0) ans = min(ans, *tree[j--].lower_bound(k));
  T query(ll i, ll j) {
    T rl = neutral, rr = neutral;
    for(i += s, j += s; i <= j; i /= 2, j /= 2){
      if((i&1) == 1) rl = T::comb(rl, tree[i++]);
      if((j&1) == 0) rr = T::comb(tree[j--], rr);
    }
    return T::comb(rl, rr);
  }

  bool pred(const T& a, ll x) { return a.val > x; }

  ll max_right(ll l, ll x) {
    if (l == n) return n;
    T acc = neutral;
    ll v = l + s;
    do {
      while ((v&1) == 0) v /= 2; // biggest block from here
      if (pred(T::comb(acc, tree[v]), x)) { // descend in v
        while (v < s) {
          v *= 2;
          if (!pred(T::comb(acc, tree[v]), x))
            acc = T::comb(acc, tree[v++]);
        }
        return v - s;
      }
      acc = T::comb(acc, tree[v++]);
    } while (v & (v-1)); // past the end
    return n;
  }
};