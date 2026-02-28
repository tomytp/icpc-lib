// Test: structures/dsu
//
// Input:
//   N Q
//   Q operations:
//     "1 u v"  union u and v (0-indexed)
//     "2 u v"  query: are u and v in the same component?
//
// Output:
//   For each "2 u v": "Yes" or "No"

#include "src/extra/template.cpp"
#include "src/extra/random.cpp"

int main(int argc, char* argv[]) {
    rng.seed(atoll(argv[1]));
    ll n = uniform(2, 20);
    ll q = uniform(1, 40);
    cout << n << " " << q << ln;
    forn(i, 0, q) {
        ll t = uniform(1, 2);
        ll u = uniform(0, n - 1);
        ll v = uniform(0, n - 1);
        cout << t << " " << u << " " << v << ln;
    }
}