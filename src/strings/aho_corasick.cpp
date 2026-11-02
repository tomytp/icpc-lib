// Aho-Corasick Automaton
//
// Builds a trie with failure links for multi-pattern matching; insert is O(|s| log sigma),
// build is O(total length * log sigma), and queries run in O(|text| log sigma) amortized.
// Node v represents the string read from the root to v. After build():
//   link: failure link, node of the longest proper suffix of v that is in the trie (-1 for root)
//   ext:  exit link, nearest node in the link chain that is the end of a pattern (-1 if none)
//   term: number of patterns that end exactly at v
//   cnt:  number of patterns that are suffixes of v (term summed over the link chain)
//   mx:   length of the longest pattern that is a suffix of v (0 if none)
// To list every pattern matched at v, follow v, ext[v], ext[ext[v]], ... (skip v if !term[v]).
//
// complexity: O((sum |patterns| + |text|) log sigma)

struct aho {
    struct node { map<char, ll> to; ll link = 0, ext = -1, term = 0, cnt = 0, mx = 0; };
    vector<node> t = {node{{}, -1}};

    void insert(const string& s) {
        ll at = 0;
        for (char c : s) {
            if (!t[at].to.count(c)) t[at].to[c] = sz(t), t.emplace_back();
            at = t[at].to[c];
        }
        t[at].term++, t[at].cnt++, t[at].mx = sz(s);
    }

    ll next(ll at, char c) {
        while (at != -1 && !t[at].to.count(c)) at = t[at].link;
        return at == -1 ? 0 : t[at].to[c];
    }

    void build() {
        queue<ll> q; q.push(0);
        while (sz(q)) {
            ll i = q.front(); q.pop();
            for (auto [c, j] : t[i].to) {
                ll l = t[j].link = next(t[i].link, c);
                t[j].ext = t[l].term ? l : t[l].ext;
                t[j].cnt += t[l].cnt, t[j].mx = max(t[j].mx, t[l].mx);
                q.push(j);
            }
        }
    }

    // total number of pattern occurrences in s
    ll query(const string& s) {
        ll at = 0, ans = 0;
        for (char c : s) at = next(at, c), ans += t[at].cnt;
        return ans;
    }

    // v[i] = length of the longest pattern ending at position i (0 if none)
    vll match_vec(const string& s) {
        ll at = 0, n = sz(s);
        vll v(n, 0);
        forn(i, 0, n) at = next(at, s[i]), v[i] = t[at].mx;
        return v;
    }
};