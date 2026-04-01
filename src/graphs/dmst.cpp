// Directed Minimum Spanning Tree (Edmonds / Chu-Liu)
//
// finds the minimum cost arborescence rooted at src in a
// weighted directed graph. returns {cost, parent} where
// parent[u] is the predecessor of u in the optimal tree.
// returns {-1, {}} if no arborescence exists.
//
// complexity: O(M log N), O(N+M) me

#include "src/structures/dsu_rb.cpp"

struct Edge {ll a, b, w; };
struct Node {
	Edge key;
	Node *l, *r;
	ll delta;
	void prop() {
		key.w += delta;
		if (l) l->delta += delta;
		if (r) r->delta += delta;
		delta = 0;
	}
	Edge top() { prop(); return key; }
};
Node *merge(Node *a, Node *b) {
	if (!a || !b) return a ?: b;
	a->prop(), b->prop();
	if (a->key.w > b->key.w) swap(a, b);
	swap(a->l, (a->r = merge(b, a->r)));
	return a;
}
void pop(Node*& a) { a->prop(); a = merge(a->l, a->r); }

pair<ll, vll> dmst(ll n, ll src, vector<Edge>& g) {
	dsu_rb uf(n);
	vector<Node*> heap(n);
	for (Edge e : g) heap[e.b] = merge(heap[e.b], new Node{e});
	ll res = 0;
	vll seen(n, -1), path(n), par(n);
	seen[src] = src;
	vector<Edge> Q(n), in(n, {-1,-1}), comp;
	deque<tuple<ll, ll, vector<Edge>>> cycs;
	forn(s,0,n) {
		ll u = s, qi = 0, w;
		while (seen[u] < 0) {
			if (!heap[u]) return {-1,{}};
			Edge e = heap[u]->top();
			heap[u]->delta -= e.w, pop(heap[u]);
			Q[qi] = e, path[qi++] = u, seen[u] = s;
			res += e.w, u = uf.find(e.a);
			if (seen[u] == s) { /// found cycle, contract
				Node* cyc = 0;
				ll end = qi, time = uf.time();
				do cyc = merge(cyc, heap[w = path[--qi]]);
				while (uf.unite(u, w));
				u = uf.find(u), heap[u] = cyc, seen[u] = -1;
				cycs.push_front({u, time, {&Q[qi], &Q[end]}});
			}
		}
		forn(i,0,qi) in[uf.find(Q[i].b)] = Q[i];
	}

	for (auto& [u,t,com] : cycs) { // restore sol (optional)
		uf.rollback(t);
		Edge inEdge = in[u];
		for (auto& e : com) in[uf.find(e.b)] = e;
		in[uf.find(inEdge.b)] = inEdge;
	}
	forn(i,0,n) par[i] = in[i].a;
	return {res, par};
}
