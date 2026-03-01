// Test: graphs/hungarian
//
// Input:
//   N
//   N lines of N integers: a[i][j] = cost of assigning worker i to job j
//
// Output:
//   min cost of a perfect matching

#include "src/extra/template.cpp"
#include "src/graphs/hungarian.cpp"

int main() {
    _;
    ll n; cin >> n;
    vector<v64> a(n, v64(n));
    forn(i, 0, n) forn(j, 0, n) cin >> a[i][j];
    auto [cost, assign] = hungarian(a);
    cout << cost << ln;
}
