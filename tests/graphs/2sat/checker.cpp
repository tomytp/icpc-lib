// Checker for graphs/2sat
//
// Called as: ./checker <input_file> <ignored> <output_file>
//
// If output is "YES": verify the assignment satisfies all clauses.
// If output is "NO":  verify unsatisfiability by brute-forcing all 2^N assignments.

#include "src/extra/template.cpp"

int main(int argc, char* argv[]) {
    // Read input
    ifstream inf(argv[1]);
    int n, m; inf >> n >> m;
    vector<pair<int,int>> clauses(m);
    for (auto& [a, b] : clauses) inf >> a >> b;

    // Read contestant output
    ifstream ouf(argv[3]);
    string verdict; ouf >> verdict;

    // Evaluate literal (1-indexed) against a bitmask
    auto eval = [&](int lit, int mask) -> bool {
        return lit > 0 ? (mask >> (lit - 1)) & 1 : !((mask >> (-lit - 1)) & 1);
    };
    auto all_sat = [&](int mask) -> bool {
        for (auto [a, b] : clauses)
            if (!eval(a, mask) && !eval(b, mask)) return false;
        return true;
    };

    if (verdict == "NO") {
        for (int mask = 0; mask < (1 << n); mask++) {
            if (all_sat(mask)) {
                cerr << "WA: output is NO but mask " << mask << " satisfies all clauses\n";
                return 1;
            }
        }
        return 0; // correctly UNSAT

    } else if (verdict == "YES") {
        int mask = 0;
        for (int i = 0; i < n; i++) {
            int xi; ouf >> xi;
            if (xi != 0 && xi != 1) {
                cerr << "WA: invalid assignment value " << xi << " for x" << i << "\n";
                return 1;
            }
            mask |= (xi << i);
        }
        if (!all_sat(mask)) {
            cerr << "WA: assignment does not satisfy all clauses\n";
            return 1;
        }
        return 0; // valid assignment

    } else {
        cerr << "PE: expected YES or NO, got '" << verdict << "'\n";
        return 1;
    }
}
