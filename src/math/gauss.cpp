template<typename T>
pair<ll, vector<T>> gauss(vector<vector<T>> a, vector<T> b) {
    const double eps = 1e-6;
    ll n = a.size(), m = a[0].size();
    forn(i, 0, n) a[i].push_back(b[i]);

    v64 where(m, -1);
    for (ll col = 0, row = 0; col < m and row < n; col++) {
        ll sel = row;
        forn(i, row, n)
            if(abs(a[i][col]) > abs(a[sel][col])) sel = i;
        if(abs(a[sel][col]) < eps) continue;
        forn(i, col, m+1)
            swap(a[sel][i], a[row][i]);
        where[col] = row;

        forn(i, 0, n) if (i != row) {
            T c = a[i][col] / a[row][col];
            forn(j, col, m+1)
                a[i][j] -= a[row][j] * c;
        }
        row++;
    }

    vector<T> ans(m, 0);
    forn(i, 0, m) if (where[i] != -1)
        ans[i] = a[where[i]][m]/a[where[i]][i];
    forn(i, 0, n) {
        T sum = 0;
        forn(j, 0, m)
            sum += ans[j]*a[i][j];
        if(abs(sum - a[i][m]) > eps)
            return pair(0, vector<T>());
    }

    forn(i, 0, m) if (where[i] == -1)
        return pair(INF, ans);
    return pair(1, ans);
}