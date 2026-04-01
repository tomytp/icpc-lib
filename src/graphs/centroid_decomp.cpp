// Centroid Decomposition - pares a distancia k
//
// Objetivo: contar #pares (u,v) com dist(u,v)=k na arvore.
// Ideia: em cada centroide c, para cada subarvore j:
//   - path = profundidades d partindo de j (sem nos removidos)
//   - somar cnt[k-d-1] ANTES de atualizar (evita dupla contagem entre subarvores)
//   - depois, cnt[d+1]++ (a aresta (c,j) soma 1)
//
// Funcoes: dfs_sz (tamanhos), centroid (acha centroide), dfs (coleta depths), decomp (divide e combina).
// Complexidade: O(n log n) tempo; O(n) memoria.
// Cuidados: limites de cnt, k==0 trata a parte se quiser contar (u,u); rem[c] permanece marcado.
// escrito pelo gpt, cuidado

const ll MAX = 100'005;
vll g[MAX];
ll siz[MAX], rem[MAX];

void dfs(vll& path, ll i, ll l=-1, ll d=0) {
    path.push_back(d);
    for (ll j : g[i]) if (j != l && !rem[j]) dfs(path, j, i, d+1);
}

ll dfs_sz(ll i, ll l=-1) {
    siz[i] = 1;
    for (ll j : g[i]) if (j != l && !rem[j]) siz[i] += dfs_sz(j, i);
    return siz[i];
}

ll centroid(ll i, ll l, ll size) {
    for (ll j : g[i]) if (j != l && !rem[j] && siz[j] > size / 2)
        return centroid(j, i, size);
    return i;
}

ll decomp(ll i, ll k) {
    ll c = centroid(i, i, dfs_sz(i));
    rem[c] = 1;

    // gasta O(n) aqui - dfs sem ir pros caras removidos
    ll ans = 0;
    vll cnt(siz[i]);
    cnt[0] = 1;
    for (ll j : g[c]) if (!rem[j]) {
        vll path;
        dfs(path, j);
        for (ll d : path) if (0 <= k-d-1 && k-d-1 < siz[i])
            ans += cnt[k-d-1];
        for (ll d : path) cnt[d+1]++;
    }

    for (ll j : g[c]) if (!rem[j]) ans += decomp(j, k);
    rem[c] = 0;
    return ans;
}
