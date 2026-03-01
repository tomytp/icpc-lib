// Test: structures/perseg (persistent segment tree)
//
// Input:
//   N Q
//   A[0] ... A[N-1]  (initial values, version 0)
//   Q operations:
//     "1 t i v"  point set: new version from t with position i = v
//     "2 t l r"  range sum query on version t
//
// Output:
//   For each "2 t l r": the sum

#include "src/extra/template.cpp"
#include "src/extra/random.cpp"

int main(int argc, char* argv[]) {
    rng.seed(atoll(argv[1]));
    ll n = uniform(1, 15);
    ll q = uniform(1, 30);
    cout << n << " " << q << ln;
    forn(i, 0, n) {
        if (i) cout << " ";
        cout << uniform(-100, 100);
    }
    cout << ln;
    ll nver = 1; // version 0 exists after build
    forn(i, 0, q) {
        ll t = uniform(1, 2);
        ll ver = uniform(0, nver - 1);
        if (t == 1) {
            ll idx = uniform(0, n - 1);
            ll val = uniform(-100, 100);
            cout << "1 " << ver << " " << idx << " " << val << ln;
            nver++;
        } else {
            ll l = uniform(0, n - 1);
            ll r = uniform(l, n - 1);
            cout << "2 " << ver << " " << l << " " << r << ln;
        }
    }
}
