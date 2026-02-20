// DSU with Rollback (Persistent/Undoable Union-Find)
//
// Allows merging sets and undoing operations to a previous state
// using a stack of modifications. No Path Compression to keep
// tree structure reversible.
//
// complexity: O(log N) per find/unite, O(1) per checkpoint/rollback

struct dsu {
	vector<ll> id, len;
	stack<stack<pair<ll&, ll>>> st;

	dsu(int n) : id(n), len(n, 1) { 
		iota(id.begin(), id.end(), 0), st.emplace(); 
	}
		
	void save(ll &x) { st.top().emplace(x, x); }

	void checkpoint() { st.emplace(); }
	
	void rollback() {
		while(st.top().size()) {
			auto [end, val] = st.top().top(); st.top().pop();
			end = val;
		}
		st.pop();
	}

	int find(int a) { return a == id[a] ? a : find(id[a]); }

	void unite(int a, int b) {
		a = find(a), b = find(b);
		if (a == b) return;
		if (len[a] < len[b]) swap(a, b);
		save(len[a]), save(id[b]);
		len[a] += len[b], id[b] = a;
	}
};