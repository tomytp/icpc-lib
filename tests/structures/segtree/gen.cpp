// Test: structures/segtree (sum variant)
//
// Input:
//   N Q
//   A[0] A[1] ... A[N-1]  (initial values, space-separated)
//   Q operations:
//     "1 i v"   point update: set a[i] = v (0-indexed)
//     "2 l r"   range query:  sum of a[l..r] inclusive
//
// Output:
//   For each "2 l r": the sum

#include "src/extra/template.cpp"
#include "src/extra/random.cpp"

int main(int argc, char* argv[]) {
    rng.seed(atoll(argv[1]));
    ll n = uniform(1, 20);
    ll q = uniform(1, 40);
    cout << n << " " << q << ln;
    forn(i, 0, n) {
        if (i) cout << " ";
        cout << uniform(-1000, 1000);
    }
    cout << ln;
    forn(i, 0, q) {
        ll t = uniform(1, 2);
        if (t == 1) {
            cout << "1 " << uniform(0, n - 1) << " " << uniform(-1000, 1000) << ln;
        } else {
            ll l = uniform(0, n - 1);
            ll r = uniform(l, n - 1);
            cout << "2 " << l << " " << r << ln;
        }
    }
}