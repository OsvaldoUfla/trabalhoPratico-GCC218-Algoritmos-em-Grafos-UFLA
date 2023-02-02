#include <iostream>
#include <fstream>
#include <string>
#include "instancia.h"
#include "vertice.h"

using namespace std;

string campos [10];
int coutCampos = 0;

Instancia* i;

int** MA;
int N;

/**
 * Preenche os dados da instancia
 * @param linha contendo os dados de uma linha especifica dos detalhes da instancia
 */
void preencheInstancia(string linha){
    bool addChar = false;
    for(int rl = 0; rl < linha.size(); rl++){
        if(addChar && coutCampos < 10)
            campos[coutCampos] += linha[rl];
        if(linha[rl] == ' ' && linha[rl - 1] == ':')
            addChar = true;
    }
    if(coutCampos < 10)
        coutCampos++;
}

/**
 * Preenche o vetor de vertices da classe Instancia. Objeto na varivael `i`
 * @param linha contendo os dados de cada vertice
 */
void preencheNo(string linha){
    coutCampos = 0;
    string aux = "";
    for(int i = 0; i < linha.size(); i++){
        if(linha[i] != ' ')
            aux += linha[i];
        else {
            campos[coutCampos] = aux;
            aux = "";
            coutCampos++;
        }
    }
    Vertice v;
    v.set_id(stoi(campos[0]));
    v.set_lat(stod(campos[1]));
    v.set_longi(stod(campos[2]));
    v.set_dem(stoi(campos[3]));
    v.set_etw(stoi(campos[4]));
    v.set_ltw(stoi(campos[5]));
    v.set_dur(stoi(campos[6]));
    v.set_p(stoi(campos[7]));
    v.set_d(stoi(campos[8]));
    i->setVertice(v);
}

/**
 * Inicia a matriz de adjacencia com os valores NULL
 * @param n numero de vertices do grafo
 */
void iniciarMatriz(int n){
    N = n;
    MA = new int*[N];
    
    for(int i = 0; i < N; i++)
        MA[i] = new int[N];
    
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            MA[i][j] = NULL;
}

/**
 * Insere uma nova aresta na matriz de adjacencia
 * Considera-se um grafo direcionado. Para grafo nao direcionado, adicionar a expressao MA[t][s] = w
 * @param s saida
 * @param t destino
 * @param s peso da aresta
 */
void insereAresta(int s, int t, int w){
    MA[s][t] = w;
}

/**
 * Le a linha recebida e salva os valores contidos nela na MA
 * @param linha de dados com os valores em string
 * @param s vertice de origem
 */
void lerInsere(string linha, int s){
    string aux = "";
    int auxCount = 0;
    for(int i = 0; i < linha.size(); i++){
        if(linha[i] != ' '){
            aux += linha[i];
        } else {
            insereAresta(s, auxCount, stoi(aux));
            auxCount++;
            aux = "";
        }
    }
}

/**
 * --- FUNCAO USADA EXCLUSIVAMENTE PARA TESTES ---
 * Faz o teste para os 10 primeiros vertices, mostrando os pesos para as 10 primeiras arestas as quais eles ligam
 * Para mostrar a matriz inteira, basta trocas o valor 9 para a variavel `N`
 */
void imprimirMatriz(){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++)
            cout << MA[i][j] << " ";
        cout << endl;
    }
}

int main()
{    
    string linha;
    bool t = true;
    int altLeitura = 0;
    ifstream arquivo("instances/bar-n100-1.txt");
    int s = 0;

    if(arquivo){
        while(getline(arquivo, linha)){
            if(altLeitura == 0){
                preencheInstancia(linha);
                
                if(coutCampos == 10){
                    altLeitura++;
                    i = new Instancia(campos[0],campos[1],campos[2],campos[3],stoi(campos[4]),campos[5],campos[6],stoi(campos[7]),stoi(campos[8]),stoi(campos[9]));
                }
            } else if(altLeitura == 1){
                if(linha[0] == '0' || linha[0] == '1' || linha[0] == '2' || linha[0] == '3' || linha[0] == '4' || linha[0] == '5' || linha[0] == '6' || linha[0] == '7' || linha[0] == '8' || linha[0] == '9')
                    preencheNo(linha);
                if(linha == "EDGES"){
                    iniciarMatriz(i->getSize());
                    altLeitura++;
                }
            } else {
                if(linha != "EOF"){
                    lerInsere(linha, s);
                    s++;
                }
            }
        }

        imprimirMatriz();// --- CHAMADA DE TESTE DA MATRIZ ---

        delete i;

        for(int i = 0; i < N; i++)
            delete [] MA[i];
        delete [] MA;

        arquivo.close();
    }
    else cout << "Erro ao abrir o arquivo!" << endl;
    
    return 0;
}