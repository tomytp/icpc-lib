#include "src/extra/template.cpp"
#include "src/extra/random.cpp"

int main(int argc, char* argv[]) {
    rng.seed(atoll(argv[1]));
    ll n = uniform(1, 7);
    cout << n << ln;
    forn(i, 0, n) {
        forn(j, 0, n) {
            if (j) cout << " ";
            cout << uniform(0, 100);
        }
        cout << ln;
    }
}
