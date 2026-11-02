// Divisor Decomposition / Square Root Trick
//
// Itera sobre os intervalos [l, r] onde floor(n / i) e constante.
//
// complexity: O(sqrt(n))

void divisor_decomposition(ll n) {
    for (ll l = 1, r; l <= n; l = r + 1) {
        ll val = n / l;
        r = n / val;
        // n/i == val para todo i em [l, r]
        // Exemplo: res += (r - l + 1) * val;
    }
}
