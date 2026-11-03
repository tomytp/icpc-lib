// Gauss-Jordan
//
// Resolve um sistema linear Ax = b.
//
// Retorna:
// {0, {}}       se nao existe solucao;
// {1, ans}      se existe solucao unica;
// {INF, ans}    se existem infinitas solucoes.
//
// Para tipos de ponto flutuante (double, long double).
// eps e uma tolerancia absoluta.
//
// Complexity: O(n^2 * m) time, O(n*m) memory.

template<typename T>
pair<ll, vector<T>> gauss(
    vector<vector<T>> a,
    vector<T> b,
    long double eps = 1e-9L
) {
    ll n = sz(a);
    assert(n > 0 && sz(b) == n);

    ll m = sz(a[0]);
    for (auto &row : a) assert(sz(row) == m);

    forn(i,0,n) a[i].push_back(b[i]);

    vll where(m, -1);

    for (ll col = 0, row = 0; col < m && row < n; col++) {
        ll sel = row;

        forn(i,row,n)
            if (abs(a[i][col]) > abs(a[sel][col]))
                sel = i;

        if (abs(a[sel][col]) <= eps) continue;

        swap(a[sel], a[row]);
        where[col] = row;

        forn(i,0,n) if (i != row) {
            T c = a[i][col] / a[row][col];

            forn(j,col,m+1)
                a[i][j] -= a[row][j]*c;
        }

        row++;
    }

    vector<T> ans(m, 0);

    forn(i,0,m) if (where[i] != -1)
        ans[i] = a[where[i]][m]/a[where[i]][i];

    forn(i,0,n) {
        T sum = 0;

        forn(j,0,m)
            sum += ans[j]*a[i][j];

        if (abs(sum-a[i][m]) > eps)
            return {0, {}};
    }

    forn(i,0,m) if (where[i] == -1)
        return {INF, ans};

    return {1, ans};
}