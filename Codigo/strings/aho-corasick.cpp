// Aho–Corasick Automaton
//
// Builds a trie with failure links for multi-pattern matching; insert is O(|s|), build is linear in total length, and queries run in linear time in the text.
//
// complexity: varies, O(total patterns length)

namespace aho {
	map<char, ll> to[MAX];
	ll link[MAX], idx, term[MAX], exit[MAX], sobe[MAX];
    vector<ll> max_match(MAX, 0);

	void insert(string& s) {
		ll at = 0;
		for (char c : s) {
			auto it = to[at].find(c);
			if (it == to[at].end()) at = to[at][c] = ++idx;
			else at = it->second;
		}
		term[at]++, sobe[at]++;
        max_match[at] = s.size();
	}

    void build() {
		queue<ll> q;
		q.push(0);
		link[0] = exit[0] = -1;
		while (q.size()) {
			ll i = q.front(); q.pop();
			for (auto [c, j] : to[i]) {
				ll l = link[i];
				while (l != -1 and !to[l].count(c)) l = link[l];
				link[j] = l == -1 ? 0 : to[l][c];
				exit[j] = term[link[j]] ? link[j] : exit[link[j]];

                max_match[j] = max(max_match[link[j]], max_match[j]);
				if (exit[j]+1) sobe[j] += sobe[exit[j]];
				q.push(j);
			}
		}
	}
	ll query(string& s) {
		ll at = 0, ans = 0;
		for (char c : s){
			while (at != -1 and !to[at].count(c)) at = link[at];
			at = at == -1 ? 0 : to[at][c];
			ans += sobe[at];
		}
		return ans;
	}
    vector<ll> match_vec(string& s) {
        ll at = 0, n = s.size();
        vector<ll> v(n, 0);
		forn(i, 0, n){
            char c = s[i];
			while (at != -1 and !to[at].count(c)) at = link[at];
			at = at == -1 ? 0 : to[at][c];

            v[i] = max_match[at]; // quero isso
        }
		return v;
	}
}
