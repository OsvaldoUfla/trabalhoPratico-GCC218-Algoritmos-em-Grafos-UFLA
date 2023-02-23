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

void preencherVerticesVisitados(bool verticesVisitados[]){
    for(int i = 0; i < instancia.size; i++){
        verticesVisitados[i] = false;
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
            return true;
    }
    return false;
}

/**
 * se da tempo de ir e fazer a entrega
 */
bool tempoAtualCoincide(int origem, int destino, int tempoAtual){
    int aberturaDestino = instancia.vertices.at(destino).etw;
    int fechamentoDestino = instancia.vertices.at(destino).ltw;
    return (((tempoAtual + MA[origem][destino]) >= aberturaDestino) && ((tempoAtual + MA[origem][destino]) <= fechamentoDestino));
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
        if((MA[caminhao.posicaoAtual][i] < caminhao.tempoRestante) && tempoAtualCoincide(caminhao.posicaoAtual, i, tempoAtual)){
            return i;
        }
    }
    return -1;
}

int main()
{    
    leitura();

    bool verticesVisitados[instancia.size];
    preencherVerticesVisitados(verticesVisitados);
    
    Caminhao caminhao;
    caminhao.cargaAtual = instancia.capacity;
    caminhao.tempoRestante = instancia.routeTime;

    int tempoAtual = 0;
    
    caminhao.posicaoAtual = 0;
    while(vizitouTodos(verticesVisitados)){
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
    }
    
    cout<<"Passou por todos os vertices"<<endl;

    for(int i = 0; i < N; i++)
        delete [] MA[i];
    delete [] MA;

    cout<<"Fechou o programa"<<endl;

    return 0;
}