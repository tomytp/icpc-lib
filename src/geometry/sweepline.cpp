// Segment Sweep Line Skeleton
//
// Maintains an active set of segments ordered for sweep-line processing over x; insertion and removal are typically logarithmic.

// observacoes sobre sweepline em segmentos:
// tomar cuidado com segmentos verticais se a sweepline e em x, nesse caso devemos ignorar esses casos sera que podemos fazer isso em outros problemas
// tomar cuidado para nao usar funcoes da biblioteca em lugares errados...
// a partir de agora, usar a funcao de comparacao de linhas como nesse arquivo
// colocar informacoes na struc de linha para retirar mapas

map<ll, set<line, cmp_sweepline>> sweepline_begin; // dado um x, diz quais linhas comecam naquele x
map<ll, set<line, cmp_sweepline>> sweepline_end;   // dado um x, diz quais linhas terminam naquele x

void process_beg(set<line, cmp_sweepline>& v, set<line, cmp_sweepline>& active_line, vector<ll>& parent){
    for(auto x : v){
        active_line.insert(x);
        // processar uma linha que esta sendo adicionada
    }
}

void process_end(set<line, cmp_sweepline>& v, set<line, cmp_sweepline>& active_line){
    for(auto x : v){
        active_line.erase(x);
    }
}

void sweepline(ll n){
    set<line, cmp_sweepline> active_line;

    while(!sweepline_begin.empty() or !sweepline_end.empty()){
        auto it_beg = sweepline_begin.begin();
        auto it_end = sweepline_end.begin();

        if(sweepline_end.empty()){
            process_beg(it_beg->second, active_line, parent);
            sweepline_begin.erase(it_beg);
            continue;
        }

        if(sweepline_begin.empty() or it_end->first <= it_beg->first){
            process_end(it_end->second, active_line);
            sweepline_end.erase(it_end);
            continue;
        }

        process_beg(it_beg->second, active_line, parent);
        sweepline_begin.erase(it_beg);
    }
}
