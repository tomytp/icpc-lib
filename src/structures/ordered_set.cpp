// Order-Statistic Tree (PBDS)
//
// Wraps __gnu_pbds tree to support order_of_key and find_by_order operations on a sorted set.
//
// complexity: O(log N) per op, O(N)

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace __gnu_pbds;

#define ordered_set tree<p64, null_type,less<p64>, rb_tree_tag,tree_order_statistics_node_update> 

int main() {
    ordered_set s;
    s.find_by_order(position);
    s.order_of_key(value);
}
