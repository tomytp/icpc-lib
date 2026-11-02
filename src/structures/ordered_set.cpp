// Order-Statistic Tree (PBDS)
//
// Set ordenado com acesso por posicao.
// s.find_by_order(k): iterador pro k-esimo (0-indexado); end() se k >= sz(s).
// s.order_of_key(x): quantidade de elementos < x.
// Para multiset, use ordered_set<pll> com {valor, id unico}.
//
// complexity: O(log N) per op, O(N)

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace __gnu_pbds;

template<class T> using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;