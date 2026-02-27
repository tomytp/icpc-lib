// Knuth-Morris-Pratt (KMP)
//
// Computes prefix function and performs linear-time substring search with optional automaton construction.
//
// complexity: O(n + m), O(n)

v64 pi(string& s) {
    v64 p(sz(s));
    for (ll i = 1, j = 0; i < sz(s); i++) {
        while (j && s[j] != s[i]) j = p[j-1];
        if (s[j] == s[i]) j++;
        p[i] = j;
    }
    return p;
}

v64 match(string& pat, string& s) {
    v64 p = pi(pat), match;
    for (ll i = 0, j = 0; i < sz(s); i++) {
        while (j && pat[j] != s[i]) j = p[j-1];
        if (pat[j] == s[i]) j++;
        if (j == sz(pat)) match.push_back(i-j+1), j = p[j-1];
    }
    return match;
}
    
struct KMPaut : vector<v64> {
    KMPaut(){}
    KMPaut (string& s) : vector<v64>(26, v64(sz(s)+1)) {
        v64 p = pi(s);
        auto& aut = *this;
        aut[s[0]-'a'][0] = 1;
        for (char c = 0; c < 26; c++)
            for (int i = 1; i <= sz(s); i++)
                aut[c][i] = s[i]-'a' == c ? i+1 : aut[c][p[i-1]];
    }
};
