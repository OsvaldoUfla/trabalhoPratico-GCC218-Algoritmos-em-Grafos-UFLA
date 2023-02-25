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

int dist(Caminhao caminhao, int tempoAtual){
    int espera = 5000;
    int s = caminhao.posicaoAtual;
    int result = -1;

    for(int i = 0; i < instancia.size; i++){
        if(instancia.vertices.at(i).dem > 0){
            int ti = i + ((instancia.size - 1) / 2);
            int e = 5000;

            if(instancia.vertices.at(i).etw > (tempoAtual + MA[s][i] + MA[i][ti])){
                e = instancia.vertices.at(i).etw - (tempoAtual + MA[s][i] + MA[i][ti]);
            }
            if(e < espera){
                result = i;
            }
        }
    }

    return result;
}
int cnt = 1;
void caminho(Caminhao caminhao, bool verticesVisitados[]) {
    cout<<"Caminhao "<<cnt<<endl;cnt++;
    int tempoAtual = 0;
    while ((!vizitouTodos(verticesVisitados)) && caminhao.tempoRestante > 20) {
        int s = caminhao.posicaoAtual;
        int t = s + ((instancia.size - 1) / 2);

        if ((instancia.vertices.at(s).dem > 0) && (!verticesVisitados[s])) {
            int espera = 0;

            if((MA[s][t] + tempoAtual) < instancia.vertices.at(t).etw){
                espera = (instancia.vertices.at(t).etw + (MA[s][t] + tempoAtual));
            }

            if((caminhao.tempoRestante - (MA[s][t] + espera)) < MA[t][0]){
                int mc = dist(caminhao, tempoAtual);
                if(mc > 0){
                    int destinoMc = mc + ((instancia.size - 1) / 2);
                    
                    espera = 0;
                    if((MA[s][mc] + MA[mc][destinoMc] + tempoAtual) < instancia.vertices.at(destinoMc).etw)
                        espera = instancia.vertices.at(destinoMc).etw - (MA[s][mc] + MA[mc][destinoMc] + tempoAtual);
                    
                    if((caminhao.tempoRestante - (MA[s][mc] + MA[mc][destinoMc] + espera)) > MA[t][0]){
                        cout << "-> " << instancia.vertices.at(s).id << " - " << instancia.vertices.at(mc).id << endl;
                        caminhao.tempoRestante -= MA[s][mc];
                        caminhao.posicaoAtual = mc;
                        s = mc;
                    }
                    else{
                        caminhao.tempoRestante = 0;
                        caminhao.posicaoAtual = 0;
                        cout << "-> " << instancia.vertices.at(s).id << " - " << instancia.vertices.at(0).id << endl;
                    }
                    if(s == mc){
                        caminhao.tempoRestante = 0;
                        caminhao.posicaoAtual = 0;
                        cout << "-> " << instancia.vertices.at(s).id << " - " << instancia.vertices.at(0).id << endl;
                        s = 0;
                    }
                }
                else{
                    caminhao.tempoRestante = 0;
                    caminhao.posicaoAtual = 0;
                    cout << "-> " << instancia.vertices.at(s).id << " - " << instancia.vertices.at(0).id << endl;
                }
            }else{
                verticesVisitados[s] = true;
                verticesVisitados[t] = true;
                caminhao.tempoRestante -= ((MA[s][t]) + espera);
                
                tempoAtual += ((MA[s][t]) + espera);
                cout << "-> " << instancia.vertices.at(s).id << " - " << instancia.vertices.at(t).id << endl;
                s = t;
                caminhao.posicaoAtual = t;
            }           
        }
        else{
            int distanciaColeta = 5000;
            int espera = 5000;
            int coletaMaisProx = 0;
            if(s == 0)
                verticesVisitados[s] = true;

            for(int i = 1; i < instancia.size; i++){
                int ti = i + ((instancia.size - 1) / 2);
                if((MA[s][i] < distanciaColeta) && (instancia.vertices.at(i).dem > 0) && (!verticesVisitados[i])){
                    int esp; 
                    if((tempoAtual + MA[s][i]) > instancia.vertices.at(i).etw){
                        esp = instancia.vertices.at(i).etw - (tempoAtual + MA[s][i]);
                    }
                    if(esp < espera){
                        espera = esp;
                        distanciaColeta = MA[s][i];
                        coletaMaisProx = i;
                    }
                }
            }

            if(coletaMaisProx > 0){
                int destinoColetaMaisProx = coletaMaisProx + ((instancia.size - 1) / 2);
                if((caminhao.tempoRestante - (MA[s][coletaMaisProx] + MA[coletaMaisProx][0] + MA[coletaMaisProx][destinoColetaMaisProx])) < MA[s][0]){
                    caminhao.tempoRestante = 0;
                    caminhao.posicaoAtual = 0;
                    cout << "-> " << instancia.vertices.at(s).id << " - " << instancia.vertices.at(0).id << endl;
                }
                else{
                    caminhao.tempoRestante -= (MA[s][coletaMaisProx]);
                    cout << "-> " << instancia.vertices.at(s).id << " - " << instancia.vertices.at(coletaMaisProx).id << endl;

                    s = coletaMaisProx;
                    caminhao.posicaoAtual = coletaMaisProx;
                }
            }
        }
        if(vizitouTodos(verticesVisitados) && (caminhao.tempoRestante > 20)){
            caminhao.tempoRestante = 0;
            caminhao.posicaoAtual = 0;
            cout << "-> " << instancia.vertices.at(s).id << " - " << instancia.vertices.at(0).id << endl;
        }
    }
    cout<<"Terminando o exercicio"<<endl;
}

int main()
{    
    leitura();

    bool verticesVisitados[instancia.size];

    // marca todos os vertices como nao vizitados
    for(int i = 0; i < instancia.size; i++)
        verticesVisitados[i] = false;
    
    Caminhao caminhao;
    caminhao.cargaAtual = instancia.capacity;
    caminhao.tempoRestante = instancia.routeTime;
    
    caminhao.posicaoAtual = 0;

    while(!vizitouTodos(verticesVisitados)){
        caminho(caminhao, verticesVisitados);
    }

    cout<<"Deve ser -1: " << proxVerticeNaoVizitado(verticesVisitados) << endl;

    for(int i = 0; i < N; i++)
        delete [] MA[i];
    delete [] MA;

    cout<<"Fechou o programa"<<endl;

    return 0;
}
