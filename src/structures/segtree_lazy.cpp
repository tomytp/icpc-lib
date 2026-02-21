// Segment Tree (Range Query + Range Update)
//
// Tree for range queries with a customizable combine; supports range updates and range queries.
//
// complexity: O(log N) per op, O(N)

struct Node {
    ll sum;
    ll cnt;
    Node() : sum(0), cnt(0) {}
    Node(ll s, ll c = 1) : sum(s), cnt(c) {}
    Node operator*(const Node &o) const { return {sum + o.sum, cnt + o.cnt}; }
};

struct Update {
    ll add = 0;
    optional<ll> set;
    Update() = default;
    Update(ll a, optional<ll> s) : add(a), set(s) {}

    Node operator()(const Node &n) const {
        ll res = set.has_value() ? *set * n.cnt : n.sum;
        return {res + add * n.cnt, n.cnt};
    }

    Update operator+(const Update &o) const {
        Update res = *this;
        if (o.set.has_value()) {
            res.set = o.set;
            res.add = 0;
        }
        if (o.add != 0) {
            if (res.set.has_value()) *res.set += o.add;
            else res.add += o.add;
        }
        return res;
    }
};

template<typename T, typename U> struct seg_tree_lazy {
  ll S, H;

  T identity;
  vector<T> value;

  U noop;
  vector<bool> dirty;
  vector<U> prop;

  seg_tree_lazy<T, U>(ll _S, T _identity = T(), U _noop = U()) {
    identity = _identity, noop = _noop;
    for (S = 1, H = 1; S < _S; ) S *= 2, H++;

    value.assign(2*S, identity);
    dirty.assign(2*S, false);
    prop.assign(2*S, noop);
  }

  void set_leaves(vector<T> &leaves) {
    copy(leaves.begin(), leaves.end(), value.begin() + S);

    for (ll i = S - 1; i > 0; i--)
      value[i] = value[2 * i] * value[2 * i + 1];
    dirty.assign(2*S, false);
    prop.assign(2*S, noop);
  }

  void apply(ll i, U &update) {
    value[i] = update(value[i]);
    if(i < S) {
      prop[i] = prop[i] + update;
      dirty[i] = true;
    }
  }

  void rebuild(ll i) {
    for (ll l = i/2; l; l /= 2) {
      T combined = value[2*l] * value[2*l+1];
      value[l] = prop[l](combined);
    }
  }
  void propagate(ll i) {
    for (ll h = H; h > 0; h--) {
      ll l = i >> h;

      if (dirty[l]) {
        apply(2*l, prop[l]);
        apply(2*l+1, prop[l]);

        prop[l] = noop;
        dirty[l] = false;
      }
    }
  }

  void upd(ll i, ll j, U update) {
    i += S, j += S;
    propagate(i), propagate(j);

    for (ll l = i, r = j; l <= r; l /= 2, r /= 2) {
      if((l&1) == 1) apply(l++, update);
      if((r&1) == 0) apply(r--, update);
    }

    rebuild(i), rebuild(j);
  }

  T query(ll i, ll j){
    i += S, j += S;
    propagate(i), propagate(j);

    T res_left = identity, res_right = identity;
    for(; i <= j; i /= 2, j /= 2){
      if((i&1) == 1) res_left = res_left * value[i++];
      if((j&1) == 0) res_right = value[j--] * res_right;
    }
    return res_left * res_right;
  }
};