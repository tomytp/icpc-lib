// Articulation Points / Bridges / Biconnected Components
//
// is_art[i] = true se i eh ponto de articulacao        (ART)
// bridges: lista de arestas ponte                      (BRIDGE)
// bcc: lista de componentes biconexas (em arestas) (BCC)
//
// O(n+m)


struct TJ {
    ll n, timer=0;
    vector<vll>& g;
    vll tin, low;
	
    vector<bool> is_art; // ART
    vector<pll> bridges; // BRIDGE
    vector<pll> st; // BCC
    vector<vector<pll>> bcc; // BCC

    TJ(ll n, vector<vll>& g): n(n), g(g){
        tin.assign(n,-1);
        low.resize(n);
        is_art.assign(n, false); // ART
		forn(i, 0, n) if(tin[i]==-1) dfs(i);
    }

    void dfs(ll v, ll p=-1){
        tin[v]=low[v]=timer++;
        ll ch=0;

        for(ll to:g[v]){
            if(to==p) continue;

            if(tin[to]==-1){
                st.push_back({v,to}); // BCC

                dfs(to,v);
                low[v] = min(low[v], low[to]);

                if(low[to]>tin[v]) bridges.push_back({v,to}); // BRIDGE
                if(low[to]>=tin[v] && p!=-1) is_art[v] = true; // ART

                if(low[to]>=tin[v]){ // BCC
                  vector<pll> c; // BCC
                  while(1){ // BCC
                    auto e=st.back(); st.pop_back(); // BCC
                    c.push_back(e); // BCC
                    if(e.first==v && e.second==to) break; // BCC
                  } // BCC
                  bcc.push_back(c); // BCC
                } // BCC

                ch++;
            }else{
                low[v]=min(low[v],tin[to]);
                if(tin[to]<tin[v]) st.push_back({v,to});   // BCC
            }
        }
        if(p==-1 && ch>1) is_art[v] = true;   // ART
    }
};