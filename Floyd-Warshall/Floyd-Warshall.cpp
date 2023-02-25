#include <iostream>
#include <fstream>

using namespace std;
/*
*   Floyd-Warshall Algorithm
*   Programa para encontrar o menor caminho entre todos os pares de vértices
*   em um grafo ponderado.
*   Complexidade: O(n³)
*   Vamos usar esse programa para verificar se os grafos do trabalho são Euclidianos
*/

void leitura(string nmArq){
    
    int tamanhoDaMatriz = 0;
    ifstream arquivo(nmArq);
    arquivo >> tamanhoDaMatriz;


    if(arquivo){
        while(arquivo >> tamanhoDaMatriz){
            cout<<tamanhoDaMatriz<<endl;
        }
        arquivo.close();
    }
    else{
        cout<<"Erro ao abrir o aquivo"<<endl;
        arquivo.clear();
    }
}

int main (){
 //   leitura("01.txt");
    return 0;
}