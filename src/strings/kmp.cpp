// Knuth-Morris-Pratt (KMP)
//
// Computes prefix function and performs linear-time substring search with optional automaton construction.
//
// complexity: O(n + m), O(n)

vll pi(string& s) {
    vll p(sz(s));
    for (ll i = 1, j = 0; i < sz(s); i++) {
        while (j && s[j] != s[i]) j = p[j-1];
        if (s[j] == s[i]) j++;
        p[i] = j;
    }
    return p;
}

vll match(string& pat, string& s) {
    vll p = pi(pat), match;
    for (ll i = 0, j = 0; i < sz(s); i++) {
        while (j && pat[j] != s[i]) j = p[j-1];
        if (pat[j] == s[i]) j++;
        if (j == sz(pat)) match.push_back(i-j+1), j = p[j-1];
    }
    return match;
}
    
struct KMPaut : vector<vll> {
    KMPaut(){}
    KMPaut (string& s) : vector<vll>(26, vll(sz(s)+1)) {
        vll p = pi(s);
        auto& aut = *this;
        aut[s[0]-'a'][0] = 1;
        for (char c = 0; c < 26; c++)
            for (int i = 1; i <= sz(s); i++)
                aut[c][i] = s[i]-'a' == c ? i+1 : aut[c][p[i-1]];
    }
};
