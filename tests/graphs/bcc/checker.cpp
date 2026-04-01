// Checker for graphs/bcc
// Called as: ./checker <input_file> <ignored> <output_file>
//
// Verifies:
//   1. Every input edge appears in exactly one BCC.
//   2. Multi-edge BCCs are 2-edge-connected.
//   3. Single-edge BCCs are bridges in the original graph.

#include "src/extra/template.cpp"

bool connected_subgraph(ll n, vector<vll>& adj, vll& verts) {
    if (verts.empty()) return true;
    set<ll> vert_set(verts.begin(), verts.end());
    set<ll> vis;
    queue<ll> q;
    q.push(verts[0]); vis.insert(verts[0]);
    while (!q.empty()) {
        ll v = q.front(); q.pop();
        for (ll u : adj[v])
            if (vert_set.count(u) && !vis.count(u)) { vis.insert(u); q.push(u); }
    }
    return sz(vis) == sz(verts);
}

int main(int argc, char* argv[]) {
    ifstream inf(argv[1]), ouf(argv[3]);

    ll n, m; inf >> n >> m;
    vector<pll> all_edges(m);
    for (auto& [u, v] : all_edges) inf >> u >> v;

    map<pll, ll> edge_cnt;
    for (auto [u, v] : all_edges)
        edge_cnt[{min(u, v), max(u, v)}]++;

    ll k; ouf >> k;
    map<pll, ll> covered;

    for (ll b = 0; b < k; b++) {
        ll bsz; ouf >> bsz;
        vector<pll> bcc(bsz);
        for (auto& [u, v] : bcc) {
            ouf >> u >> v;
            pll e = {min(u, v), max(u, v)};
            if (!edge_cnt.count(e)) {
                cerr << "WA: BCC " << b << " contains edge " << u << " " << v << " not in input\n";
                return 1;
            }
            covered[e]++;
            if (covered[e] > edge_cnt[e]) {
                cerr << "WA: edge " << u << " " << v << " appears more than once across BCCs\n";
                return 1;
            }
        }

        // Collect vertices of this BCC
        set<ll> vert_set;
        for (auto [u, v] : bcc) { vert_set.insert(u); vert_set.insert(v); }
        vll verts(vert_set.begin(), vert_set.end());

        if (bsz == 1) {
            // Must be a bridge: removing it must disconnect its endpoints
            ll eu = bcc[0].first, ev = bcc[0].second;
            vector<vll> adj(n);
            bool skipped = false;
            for (auto [u, v] : all_edges) {
                if (!skipped && min(u,v) == min(eu,ev) && max(u,v) == max(eu,ev)) {
                    skipped = true; continue;
                }
                adj[u].push_back(v);
                adj[v].push_back(u);
            }
            set<ll> vis;
            queue<ll> q;
            q.push(eu); vis.insert(eu);
            while (!q.empty()) {
                ll v = q.front(); q.pop();
                for (ll u : adj[v]) if (!vis.count(u)) { vis.insert(u); q.push(u); }
            }
            if (vis.count(ev)) {
                cerr << "WA: BCC " << b << " has single edge " << eu << "-" << ev << " but it is not a bridge\n";
                return 1;
            }
        } else {
            // Must be 2-edge-connected: removing any single edge keeps BCC vertices connected
            for (ll i = 0; i < bsz; i++) {
                vector<vll> adj(n);
                for (ll j = 0; j < bsz; j++) {
                    if (j == i) continue;
                    adj[bcc[j].first].push_back(bcc[j].second);
                    adj[bcc[j].second].push_back(bcc[j].first);
                }
                if (!connected_subgraph(n, adj, verts)) {
                    cerr << "WA: BCC " << b << " is not 2-edge-connected\n";
                    return 1;
                }
            }
        }
    }

    if (covered != edge_cnt) {
        cerr << "WA: not all input edges are covered by BCCs\n";
        return 1;
    }

    return 0;
}
