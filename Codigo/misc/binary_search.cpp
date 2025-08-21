// Binary Search Helpers
//
// Template functions to find first or last index satisfying a monotonic predicate over a sorted search space.
//
// complexity: O(log N), O(1)

ll find_last_valid(ll val) {
    ll left = 0;
    ll right = n - 1;
    ll result = -1;
    
    while (left <= right) {
        ll mid = left + (right - left) / 2;
        if (condition) {
            result = mid;
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return result;
}

ll find_first_valid(ll val) {
    ll left = 0;
    ll right = n - 1;
    ll result = n;
    
    while (left <= right) {
        ll mid = left + (right - left) / 2;
        if (condition) {
            result = mid;
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    return result;
}
