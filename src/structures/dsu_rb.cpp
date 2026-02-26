// DSU with Rollback (Persistent/Undoable Union-Find)
//
// Allows merging sets and undoing operations to a previous state
// using a stack of modifications. No Path Compression to keep
// tree structure reversible.
//
// complexity: O(log N) per find/unite, O(1) per checkpoint/rollback

struct dsu_rb {
    v64 id, len;
    ll allbip = true;
    stack<pair<ll&, ll>> st;

    dsu_rb(ll n) : id(n), len(n, 1) { 
        iota(id.begin(), id.end(), 0); 
    }
    
	void save(ll &x) {  st.emplace(x, x);}
    
	ll time() { return sz(st); }
    void rollback(ll t) {
        while(sz(st) > t) {
            auto [end, val] = st.top(); st.pop();
            end = val;
        }
    }
    
	ll find(ll a) { return a == id[a] ? a : find(id[a]); }
    
	bool unite(ll a, ll b) {
        a = find(a), b = find(b);
        if (a == b) return false;        
        if (len[a] < len[b]) swap(a, b);
		save(len[a]);
        save(id[b]);
        len[a] += len[b], id[b] = a;
		return true;
    }
};
