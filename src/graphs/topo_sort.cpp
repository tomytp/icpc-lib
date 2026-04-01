// Topological Sort (Kahn's Algorithm)
//
// Produces a topological ordering of a DAG using indegree counting and a queue-like frontier.
//
// complexity: O(N + M), O(N)

vll topo_sort(const vector<vll>& g) {
    vll indeg(sz(g)), q;
    for (auto& li : g) for (int x : li) indeg[x]++;
    forn(i, 0, sz(g)) if (indeg[i] == 0) q.push_back(i);
    forn(j, 0, sz(q)) for(int x : g[q[j]]) if(--indeg[x] == 0) q.push_back(x);
    return q;
}
