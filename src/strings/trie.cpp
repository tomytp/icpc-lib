// Trie (Prefix Tree)
//
// Stores strings over a fixed alphabet to support insert, erase, and prefix counting in linear time.
// to[x*sigma + c] = child of node x by char c (0 = none). Root is node 0.
// erase(s) does nothing and returns false if s is not in the trie.
//
// complexity: O(|s|) per op, O(sigma * total inserted chars) memory

struct trie {
    vll to, end, pref;
    ll sigma; char norm;

    trie(ll sigma_=26, char norm_='a') : to(sigma_), end(1), pref(1), sigma(sigma_), norm(norm_) {}

    void insert(string &s) {
        ll x = 0;
        for (auto c : s) {
            ll id = x*sigma + c-norm;
            if (!to[id]) {
                to[id] = sz(end);
                to.resize(to.size() + sigma);
                end.push_back(0), pref.push_back(0);
            }
            x = to[id], pref[x]++;
        }
        end[x]++, pref[0]++;
    }

    bool erase(string &s) {
        ll x = find(s);
        if (x < 0 || !end[x]) return false;
        x = 0;
        for (auto c : s) {
            ll &nxt = to[x*sigma + c-norm];
            x = nxt;
            if (!--pref[x]) nxt = 0;
        }
        end[x]--, pref[0]--;
        return true;
    }

    ll find(string &s) {
        ll x = 0;
        for (auto c : s) {
            x = to[x*sigma + c-norm];
            if (!x) return -1;
        }
        return x;
    }

    ll count_pref(string &s) {
        ll id = find(s);
        return id >= 0 ? pref[id] : 0;
    }
};