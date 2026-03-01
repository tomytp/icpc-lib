// Test: structures/segtree_lazy
//
// Input:
//   N Q
//   A[0] ... A[N-1]
//   Q operations:
//     "1 l r v"  range add v to [l, r]
//     "2 l r v"  range set [l, r] to v
//     "3 l r"    range sum query [l, r]
//
// Output:
//   For each "3 l r": the sum

#include "src/extra/template.cpp"
#include "src/extra/random.cpp"

int main(int argc, char* argv[]) {
    rng.seed(atoll(argv[1]));
    ll n = uniform(1, 20);
    ll q = uniform(1, 40);
    cout << n << " " << q << ln;
    forn(i, 0, n) {
        if (i) cout << " ";
        cout << uniform(-100, 100);
    }
    cout << ln;
    forn(i, 0, q) {
        ll t = uniform(1, 3);
        ll l = uniform(0, n - 1);
        ll r = uniform(l, n - 1);
        if (t == 1) {
            cout << "1 " << l << " " << r << " " << uniform(-100, 100) << ln;
        } else if (t == 2) {
            cout << "2 " << l << " " << r << " " << uniform(-100, 100) << ln;
        } else {
            cout << "3 " << l << " " << r << ln;
        }
    }
}
