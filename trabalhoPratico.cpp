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
            MA[i][j] = NULL;
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