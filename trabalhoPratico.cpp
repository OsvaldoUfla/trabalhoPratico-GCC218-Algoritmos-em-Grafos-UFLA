/*
Exercício Avaliativo 2
GCC218 - Algoritmos em Grafos
Professor: Mayron César de O. Moreira.
Alunos :
    Julia Teixeira
    Osvaldo Rodrigues de Faria Junior 201911203 14A
    Robson Ferreira dos Santos Junior 202120530 14A
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>

using namespace std;

struct Vertice{
    int id;
    string lat;
    string longi;
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

struct Estatisticas{
    int caminhoes;
    int tempoTotal;
};

string campos [10];
int coutCampos = 0;
Instancia instancia;
int** MA;

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
    vertice.lat = (campos[1]);
    vertice.longi = (campos[2]);
    vertice.dem = (stoi(campos[3]));
    vertice.etw = (stoi(campos[4]));
    vertice.ltw = (stoi(campos[5]));
    vertice.dur = (stoi(campos[6]));
    vertice.p = (stoi(campos[7]));
    vertice.d = (stoi(campos[8]));
    instancia.vertices.push_back(vertice);
}

void iniciarMatriz(){
    int N = instancia.size;
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

void leitura(string nmArq){
    string linha;
    int altLeitura = 0;
    ifstream arquivo(nmArq);
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
                    iniciarMatriz();
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
vector<int>trajeto;

void caminho(Caminhao caminhao, bool verticesVisitados[], Estatisticas& estatisticas) {
    int tempoAtual = 0;
    estatisticas.caminhoes++;
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
                        trajeto.push_back(instancia.vertices.at(s).id);
                        caminhao.tempoRestante -= MA[s][mc];
                        caminhao.posicaoAtual = mc;
                        s = mc;
                    }
                    else{
                        estatisticas.tempoTotal += (instancia.routeTime - caminhao.tempoRestante);
                        caminhao.tempoRestante = 0;
                        caminhao.posicaoAtual = 0;
                        trajeto.push_back(instancia.vertices.at(s).id);
                    }
                    if(s == mc){
                        estatisticas.tempoTotal += (instancia.routeTime - caminhao.tempoRestante);
                        caminhao.tempoRestante = 0;
                        caminhao.posicaoAtual = 0;
                        trajeto.push_back(instancia.vertices.at(s).id);
                        s = 0;
                    }
                }
                else{
                    estatisticas.tempoTotal += (instancia.routeTime - caminhao.tempoRestante);
                    caminhao.tempoRestante = 0;
                    caminhao.posicaoAtual = 0;
                    trajeto.push_back(instancia.vertices.at(s).id);
                }
            }else{
                verticesVisitados[s] = true;
                verticesVisitados[t] = true;
                caminhao.tempoRestante -= ((MA[s][t]) + espera);
                
                tempoAtual += ((MA[s][t]) + espera);
                trajeto.push_back(instancia.vertices.at(s).id);
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
                    estatisticas.tempoTotal += (instancia.routeTime - caminhao.tempoRestante);
                    caminhao.tempoRestante = 0;
                    caminhao.posicaoAtual = 0;
                    trajeto.push_back(instancia.vertices.at(s).id);
                }
                else{
                    caminhao.tempoRestante -= (MA[s][coletaMaisProx]);
                    trajeto.push_back(instancia.vertices.at(s).id);

                    s = coletaMaisProx;
                    caminhao.posicaoAtual = coletaMaisProx;
                }
            }
        }
        if(vizitouTodos(verticesVisitados) && (caminhao.tempoRestante > 20)){
            estatisticas.tempoTotal += (instancia.routeTime - caminhao.tempoRestante);
            caminhao.tempoRestante = 0;
            caminhao.posicaoAtual = 0;
            trajeto.push_back(instancia.vertices.at(s).id);
        }
    }
}

void gerarCsv(){
    ofstream arq;
    arq.open("dados.json");
    if(arq){
        int count = 0;
        arq << "{" << endl;
        arq << "\"rotas\": [" << endl;
        for(int vertice : trajeto){
            arq << "\"" << instancia.vertices.at(vertice).lat << " " << instancia.vertices.at(vertice).longi << "\"";
            if(count < trajeto.size() - 1){
                arq << "," << endl;
            } else {
                arq << endl;
            }
            count++;
        }
        arq << "]" << endl;
        arq << "}" << endl;
        
        arq.close();
    }
    system("start index.html");
}

int main()
{
    char comando = 'N';
    do{
        cout << "Deseja informar o nome do arquivo?\ns - sim\nn - nao" << endl << "> ";
        cin >> comando;

        if(comando == 's'){
            cout << "Por favor, informe o nome da instancia que deseja ler (sem o .txt):" << endl << "> ";
            string nomeArq;
            cin >> nomeArq;
            leitura("instances/" + nomeArq + ".txt");
        } else if(comando == 'n')
            leitura("instances/bar-n100-2.txt");

    } while (comando != 's' && comando != 'n');
    
    bool verticesVisitados[instancia.size];

    // marca todos os vertices como nao vizitados
    for(int i = 0; i < instancia.size; i++)
        verticesVisitados[i] = false;
    
    Caminhao caminhao;
    caminhao.cargaAtual = instancia.capacity;
    caminhao.tempoRestante = instancia.routeTime;
    
    caminhao.posicaoAtual = 0;

    Estatisticas estatisticas;
    estatisticas.caminhoes = 1;
    estatisticas.tempoTotal = 0;

    while(!vizitouTodos(verticesVisitados)){
        caminho(caminhao, verticesVisitados, estatisticas);
    }

    trajeto.push_back(0);

    cout<<estatisticas.caminhoes<<" | "<<estatisticas.tempoTotal<<endl;

    comando = 'N';
    do{
        cout << "Deseja visualizar as rotas no mapa?\ns - sim\nn - nao" << endl << "> ";
        cin >> comando;
        if(comando == 's'){
            cout << "Gerando o arquivo JSON..." << endl;
            gerarCsv();
            cout << "Arquivo JSON gerado com sucesso!" << endl << "Se encontrar um erro ao abrir o mapa, tente rodar a aplicacao em um servidor local." << endl;
            cout << "Como o arquivo JSON ja foi gerado, basta abrir o arquivo `index.html` usando o servidor local." << endl;
        }
    } while (comando != 's' && comando != 'n');
    
    instancia.vertices.clear();

    cout << "Fechando o programa..." << endl;

    return 0;
}