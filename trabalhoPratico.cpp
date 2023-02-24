#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct Vertice{
    int id;
    double lat;
    double longi;
    int dem;
    int etw;
    int ltw;
    int dur;
    int p;
    int d;
    int *distancias;
};

struct Instancia{
    string name;
    string location;
    string comment;
    string type;
    int size;
    string distribution;
    string depot;
    int routeTime;
    int timeWindow;
    int capacity;
    vector <Vertice> vertices;
};

struct Caminhao{
    int cargaAtual;
    int tempoRestante;
    int posicaoAtual;
};

string campos [10];
int coutCampos = 0;

Instancia instancia;

int** MA;
int N;

void preencheInstancia(string linha){
    bool addChar = false;
    for(int rl = 0; rl < (int)linha.size(); rl++){
        if(addChar && coutCampos < 10)
            campos[coutCampos] += linha[rl];
        if(linha[rl] == ' ' && linha[rl - 1] == ':')
            addChar = true;
    }
    if(coutCampos < 10)
        coutCampos++;
}

void preencheNo(string linha){
    coutCampos = 0;
    string aux = "";
    for(int i = 0; i < (int)linha.size(); i++){
        if(linha[i] != ' ')
            aux += linha[i];
        else {
            campos[coutCampos] = aux;
            aux = "";
            coutCampos++;
        }
    }
    Vertice vertice;
    vertice.id = (stoi(campos[0]));
    vertice.lat = (stod(campos[1]));
    vertice.longi = (stod(campos[2]));
    vertice.dem = (stoi(campos[3]));
    vertice.etw = (stoi(campos[4]));
    vertice.ltw = (stoi(campos[5]));
    vertice.dur = (stoi(campos[6]));
    vertice.p = (stoi(campos[7]));
    vertice.d = (stoi(campos[8]));
    instancia.vertices.push_back(vertice);
}

void iniciarMatriz(int n){
    N = n;
    MA = new int*[N];
    
    for(int i = 0; i < N; i++)
        MA[i] = new int[N];
    
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            MA[i][j] = 0;
}

void insereAresta(int s, int t, int w){
    MA[s][t] = w;
}

void lerInsere(string linha, int s){
    string aux = "";
    int auxCount = 0;
    for(int i = 0; i < (int)linha.size(); i++){
        if(linha[i] != ' '){
            aux += linha[i];
        } else {
            insereAresta(s, auxCount, stoi(aux));
            auxCount++;
            aux = "";
        }
    }
}

void imprimirMatriz(){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++)
            cout << MA[i][j] << " ";
        cout << endl;
    }
}

void leitura(){
    string linha;
    int altLeitura = 0;
    ifstream arquivo("instances/bar-n100-1.txt");
    int s = 0;


    if(arquivo){
        while(getline(arquivo, linha)){
            if(altLeitura == 0){
                preencheInstancia(linha);
                
                if(coutCampos == 10){
                    altLeitura++;
                    instancia.name = campos[0];
                    instancia.location = campos[1];
                    instancia.comment = campos[2];
                    instancia.type = campos[3];
                    instancia.size = stoi(campos[4]);
                    instancia.distribution = campos[5];
                    instancia.depot = campos[6];
                    instancia.routeTime = stoi(campos[7]);
                    instancia.timeWindow = stoi(campos[8]);
                    instancia.capacity = stoi(campos[9]);
                }
            } else if(altLeitura == 1){
                if(linha[0] == '0' || linha[0] == '1' || linha[0] == '2' || linha[0] == '3' || linha[0] == '4' || linha[0] == '5' || linha[0] == '6' || linha[0] == '7' || linha[0] == '8' || linha[0] == '9')
                    preencheNo(linha);
                if(linha == "EDGES"){
                    iniciarMatriz(instancia.size);
                    altLeitura++;
                }
            } else {
                if(linha != "EOF"){
                    lerInsere(linha, s);
                    s++;
                }
            }
        }

        arquivo.close();
    }
    else{
        cout<<"Erro ao abrir o aquivo"<<endl;
        arquivo.clear();
    } 
}

int proxVerticeNaoVizitado(bool verticesVisitados[]){
    for(int i = 0; i < instancia.size; i++){
        if((!verticesVisitados[i]) && (instancia.vertices.at(i).dem > 0))//alem de nao tes sido vizitado, usar ele soh se for um vertice de demanda
            return i;
    }
    return -1;
}

bool vizitouTodos(bool verticesVisitados[]){
    for(int i = 0; i < instancia.size; i++){
        if(!verticesVisitados[i])
            return false;
    }
    return true;
}

void fpc(bool verticesVisitados[], Caminhao& caminhao, int& tempoAtual){
    int origem = caminhao.posicaoAtual;
    
    // Se o vertice atual ainda nao foi vizitado
    if(!verticesVisitados[caminhao.posicaoAtual]){

        // Se se tratar de um vertice de origem
        if(instancia.vertices.at(origem).dem > 0){
            int destino = origem + (( instancia.size - 1 ) / 2);

            int aberturaDestino = instancia.vertices.at(origem).etw;
            int fechamentoDestino = instancia.vertices.at(origem).ltw;
            int espera = -1;

            // se o vertice destino vai estar aberto
            if(((tempoAtual + MA[origem][destino]) >= aberturaDestino) && ((tempoAtual + MA[origem][destino]) <= fechamentoDestino)){
                espera = 0;
            }
            else{
                // Da para esperar o vertice abrir
                if(((tempoAtual + MA[origem][destino]) <= fechamentoDestino)){
                    espera = aberturaDestino - (tempoAtual + MA[origem][destino]);
                }
                // O vertice ja fechou
                else{
                    verticesVisitados[caminhao.posicaoAtual] = true;
                    int mc=5000;
                    int pmc;
                    for(int i = 1; i < instancia.size; i++){
                        if((MA[caminhao.posicaoAtual][i] < mc) && !verticesVisitados[i]){
                            mc = MA[caminhao.posicaoAtual][i];
                            pmc = i;
                        }
                    }
                    caminhao.posicaoAtual = pmc;
                    //considerar o tempo de viagem ate pmc
                    fpc(verticesVisitados, caminhao, tempoAtual);
                }
            }
            verticesVisitados[origem] = true;
            verticesVisitados[destino] = true;
            tempoAtual += (MA[origem][destino] + espera);
            caminhao.tempoRestante -= (MA[origem][destino] + espera);
        }
        // Se trata-se de um vertice de destino
        else if(instancia.vertices.at(origem).dem < 0){
            origem = origem - (( instancia.size - 1 ) / 2);
            int destino = caminhao.posicaoAtual;
            verticesVisitados[origem] = true;
            verticesVisitados[destino] = true;
            tempoAtual += MA[origem][destino];
            caminhao.tempoRestante -= MA[origem][destino];
        }
        // Se eh um vertice com demanda 0, buscar o vertice mais proximo que eh um vertice de demanda e nao foi vizitado 
        else{
            verticesVisitados[caminhao.posicaoAtual] = true;
            int mc=5000;
            int pmc;
            for(int i = 1; i < instancia.size; i++){
                if((MA[caminhao.posicaoAtual][i] < mc) && !verticesVisitados[i]){
                    mc = MA[caminhao.posicaoAtual][i];
                    pmc = i;
                }
            }
            caminhao.posicaoAtual = pmc;
            //considerar o tempo de viagem ate pmc
            fpc(verticesVisitados, caminhao, tempoAtual);
        }
    } // Se o vertice ja tiver sido vizitado, buscar o vertice mais proximo que eh um vertice de demanda e nao foi vizitado 
    else{
        int mc=5000;
        int pmc;
        for(int i = 0; i < instancia.size; i++){
            if((MA[caminhao.posicaoAtual][i] < mc) && !verticesVisitados[i]){
                mc = MA[caminhao.posicaoAtual][i];
                pmc = i;
            }
        }
        caminhao.posicaoAtual = pmc;
        //considerar o tempo de viagem ate pmc
        fpc(verticesVisitados, caminhao, tempoAtual);
    }
}

int tempoRestanteEhSuficiente(Caminhao caminhao, int tempoAtual){
    for(int i = 1; i < instancia.size; i++){
        if((MA[caminhao.posicaoAtual][i] < caminhao.tempoRestante)){
            return i;
        }
    }
    return -1;
}

Caminhao caminho(Caminhao caminhao, bool verticesVisitados[]){
    int s = caminhao.posicaoAtual;
    int t = s + (( instancia.size - 1 ) / 2);

    if((instancia.vertices.at(s).dem > 0) && (!verticesVisitados[s])){
        for(int i = 0; i < instancia.size; i++){
            if(instancia.vertices.at(i).dem > 0){
                if(MA[s][i] < MA[s][t]){
                    int ti = i + (( instancia.size - 1 ) / 2);
                    if((MA[s][i] + MA[i][ti] + MA[ti][t]) < (MA[s][i] + MA[i][t] + MA[t][ti])){
                        if((MA[s][i] + MA[i][ti] + MA[ti][t]) < caminhao.tempoRestante){
                            verticesVisitados[s] = true;
                            verticesVisitados[i] = true;
                            verticesVisitados[t] = true;
                            verticesVisitados[ti] = true;

                            cout << instancia.vertices.at(s).id <<  " - " <<  instancia.vertices.at(i).id << endl;
                            cout << instancia.vertices.at(i).id <<  " - " <<  instancia.vertices.at(ti).id << endl;
                            cout << instancia.vertices.at(ti).id <<  " - " <<  instancia.vertices.at(t).id << endl;
                            s = t;
                            caminhao.posicaoAtual = t;
                            caminhao.tempoRestante -= (MA[s][i] + MA[i][ti] + MA[ti][t]);
                        }
                    }
                    else if((MA[s][i] + MA[i][t] + MA[t][ti]) < caminhao.tempoRestante){
                        verticesVisitados[s] = true;
                        verticesVisitados[i] = true;
                        verticesVisitados[t] = true;
                        verticesVisitados[ti] = true;

                        cout << instancia.vertices.at(s).id <<  " - " <<  instancia.vertices.at(i).id << endl;
                        cout << instancia.vertices.at(i).id <<  " - " <<  instancia.vertices.at(t).id << endl;
                        cout << instancia.vertices.at(t).id <<  " - " <<  instancia.vertices.at(ti).id << endl;
                        s = ti;
                        caminhao.posicaoAtual = ti;
                        caminhao.tempoRestante -= (MA[s][i] + MA[i][t] + MA[t][ti]);
                    }
                }
            }
        }
    }
    else{
        int distanciaColeta = 5000;
        int coletaMaisProx = 0;
        if(s == 0)
            verticesVisitados[0] = true;

        for(int i = 0; i < instancia.size; i++){
            if((MA[s][i] < distanciaColeta) && (instancia.vertices.at(i).dem > 0) && (!verticesVisitados[i])){
                distanciaColeta = MA[s][i];
                coletaMaisProx = i;
            }
        }
        if(coletaMaisProx > 0){
            cout << instancia.vertices.at(s).id <<  " - " <<  instancia.vertices.at(coletaMaisProx).id << endl;
            caminhao.posicaoAtual = coletaMaisProx;
            caminhao.tempoRestante -= (MA[s][coletaMaisProx]);
            caminho(caminhao, verticesVisitados);
        }
    }
    return caminhao;
}

void cmn(Caminhao caminhao, bool verticesVisitados[]) {
    while (!vizitouTodos(verticesVisitados) && caminhao.tempoRestante > 0) {
        int s = caminhao.posicaoAtual;
        int t = s + ((instancia.size - 1) / 2);

        if ((instancia.vertices.at(s).dem > 0) && (!verticesVisitados[s])) {
            for (int i = 0; i < instancia.size; i++) {
                if (instancia.vertices.at(i).dem > 0) {
                    if (MA[s][i] < MA[s][t]) {
                        int ti = i + ((instancia.size - 1) / 2);
                        if ((MA[s][i] + MA[i][ti] + MA[ti][t]) < (MA[s][i] + MA[i][t] + MA[t][ti])) {
                            if ((MA[s][i] + MA[i][ti] + MA[ti][t]) < caminhao.tempoRestante) {
                                verticesVisitados[s] = true;
                                verticesVisitados[i] = true;
                                verticesVisitados[t] = true;
                                verticesVisitados[ti] = true;

                                cout << instancia.vertices.at(s).id << " - " << instancia.vertices.at(i).id << endl;
                                cout << instancia.vertices.at(i).id << " - " << instancia.vertices.at(ti).id << endl;
                                cout << instancia.vertices.at(ti).id << " - " << instancia.vertices.at(t).id << endl;
                                s = t;
                                caminhao.posicaoAtual = t;
                                caminhao.tempoRestante -= (MA[s][i] + MA[i][ti] + MA[ti][t]);
                            }
                        }
                        else if ((MA[s][i] + MA[i][t] + MA[t][ti]) < caminhao.tempoRestante) {
                            verticesVisitados[s] = true;
                            verticesVisitados[i] = true;
                            verticesVisitados[t] = true;
                            verticesVisitados[ti] = true;

                            cout << instancia.vertices.at(s).id <<  " - " <<  instancia.vertices.at(i).id << endl;
                            cout << instancia.vertices.at(i).id <<  " - " <<  instancia.vertices.at(t).id << endl;
                            cout << instancia.vertices.at(t).id <<  " - " <<  instancia.vertices.at(ti).id << endl;
                            s = ti;
                            caminhao.posicaoAtual = ti;
                            caminhao.tempoRestante -= (MA[s][i] + MA[i][t] + MA[t][ti]);
                        }
                    }
                }
            }
        }
        else{
            int distanciaColeta = 5000;
            int coletaMaisProx = 0;
            //if(s == 0)
                verticesVisitados[caminhao.posicaoAtual] = true;

            for(int i = 0; i < instancia.size; i++){
                if((MA[s][i] < distanciaColeta) && (instancia.vertices.at(i).dem > 0) && (!verticesVisitados[i])){
                    distanciaColeta = MA[s][i];
                    coletaMaisProx = i;
                }
            }
            if(coletaMaisProx > 0){
                cout << instancia.vertices.at(s).id <<  " - " <<  instancia.vertices.at(coletaMaisProx).id << endl;
                caminhao.posicaoAtual = coletaMaisProx;
                caminhao.tempoRestante -= (MA[s][coletaMaisProx]);
            }
        }
    }
}

int main()
{    
    leitura();

    bool verticesVisitados[instancia.size];

    // marca todos os vertices como nao vizitados
    for(int i = 0; i < instancia.size; i++){
        verticesVisitados[i] = false;
    }
    
    Caminhao caminhao;
    caminhao.cargaAtual = instancia.capacity;
    caminhao.tempoRestante = instancia.routeTime;

    int tempoAtual = 0;
    
    caminhao.posicaoAtual = 0;

    cout<<"vai entrar no while"<<endl;
    cmn(caminhao, verticesVisitados);
    /*while(!vizitouTodos(verticesVisitados)){
        fpc(verticesVisitados, caminhao, tempoAtual);
        int proxVertice = tempoRestanteEhSuficiente(caminhao, tempoAtual);

        // o turno ja vai acabar e nao ha tempo para uma nova entrega
        // nesse caso, procuro o proximo vertice para um novo caminhao
        if(proxVertice == -1){
            caminhao.cargaAtual = instancia.capacity;
            caminhao.tempoRestante = instancia.routeTime;
            caminhao.posicaoAtual = proxVerticeNaoVizitado(verticesVisitados);
            tempoAtual = 0;
        } // se ainda ouver tempo, vou para o prox vertice
        else {
            int distanciaColeta = 5000;
            int coletaMaisProx = 0;
            for(int i = 0; i < instancia.size; i++){
                // busca pelo ponto de coleta mains proximo
                if((MA[caminhao.posicaoAtual][i] < distanciaColeta) && (instancia.vertices.at(i).dem > 0) && (!verticesVisitados[i])){
                    distanciaColeta = MA[caminhao.posicaoAtual][i];
                    coletaMaisProx = i;
                }
            }
            caminhao.posicaoAtual = coletaMaisProx;
        }
        caminhao = caminho(caminhao, verticesVisitados);
        caminhao.cargaAtual = instancia.capacity;
        caminhao.tempoRestante = instancia.routeTime;
        caminhao.posicaoAtual = 0;
    }*/
    
    cout<<"Passou por todos os vertices"<<endl;

    for(int i = 0; i < N; i++)
        delete [] MA[i];
    delete [] MA;

    cout<<"Fechou o programa"<<endl;

    return 0;
}
/*
    /**
     * @attribute MA matriz de adjacencias
     * @attribute Vertice objeto do vertice que esta sendo explorado
     * @attribute caminhao objeto do tipo caminhao que garda os dados como carga e posicao atual
     * @attribute tempo variavel que guarda o tempo atual do codigo
    verificaPontoEntrega(MA, Vertice, caminhao, tempo*){
        // Se demanda > 0 (coleta), verificar a possivilidade de fazer o trajeto
        if(Vertice.dem > 0){

            // retorna true se a carga couber no caminhao
            if(calcularCarga(caminhao)){

                // Verifica se é possível chegar no destino a tempo. Se sim, retorna true e atualiza o tempo
                if(calcularTempoTrajeto(tempo)){

                    // Variavel que armazena o id do destino
                    destino = (<id>+((SIZE-1)/2));

                    // Tenta fazer a entraga e retorna false se nao conseguir
                    if(!(fazerEntrega(MA, Vertice.id, destino, tempo)){
                    
                        // Se nao for possivel fazer a etrega dentro do tempo do vertice, adicionar a pilha de destinos do caminhao
                        caminhao.destinos += destino;
                    }
                } else {

                    // Se nao houver tempo para fazer a entraga, incluir a entrega na pilha de destinos do caminhao
                    caminhao.destinos += destino;
                }
            } else {

                // Chama a funcao verificaPontoEntrega() ate que apilha de destinos do caminhao esteja vazia
                esvaziarCaminhao(MA, Vertice, caminhao, tempo*);
            }
        } else {

            // Se se trata de um vertice de destino, verificar o ponto de origem dele e entao a possibilidade de fazer a entrega
            origem = (<id>-((SIZE-1)/2));
            verificaPontoEntrega(origem);
        }
    }

    main(){
        <leitura do arquivo e preenchimento das variaveis> (parte que ja foi feita)
        
        // Enquanto houver vertices nao vizitados, 
        while(haVerticeNaoVizitado()){

            // Chamar a funcao verificaPontoEntrega() para os vertices nao vizitados
            verificaPontoEntrega(MA, VerticeNaoVizitado, caminhao, tempo*);
        }
    }
*/