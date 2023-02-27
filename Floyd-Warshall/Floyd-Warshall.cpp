#include <iostream>
#include <fstream>
#include<vector>
#include<utility>
#include<functional>

using namespace std;
/*
*   Floyd-Warshall Algorithm
*   Programa para encontrar o menor caminho entre todos os pares de vértices
*   em um grafo ponderado.
*   Complexidade: O(n³)
*   Vamos usar esse programa para verificar se os grafos do trabalho são Euclidianos
*/

#define INF 1000000000

void floyd_warshall(int** MA, int n)
{
    for(int k = 0; k < n; k++){
      for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
          /*
           * Adaptacao: originalmente, o calculo eh MA[i][j] = min(MA[i][j], MA[i][k] + MA[k][j]);
           */
          //MA[i][j] = min(MA[i][j], max(MA[i][k], MA[k][j]));
          MA[i][j] = min(MA[i][j], MA[i][k] + MA[k][j]);
        }
      }
    }
}

/*
*   Método para comparar duas matrizes returna True se forem iguais e false senão
*/
bool comparaMatrizes(int **MA, int **MA2, int n){
    bool igual = true;
    for(int i = 0; i < n and igual; i++){
            for(int j = 0; j < n and igual; j++){
                //cout << "i:" << i <<" -> j:" << j << "    " << MA2[i][j] << "     " << MA[i][j] << endl;
                if(MA[i][j] != MA2[i][j]){
                    igual = false;
                }
                
            }
        }
    return igual;
}

// Método para imprimir uma matriz
void printMatriz(int **MA, int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cout << MA[i][j] << " ";
        }
        cout << endl;
    }
}

int main()
{
    // matriz de adjacencia
    int** MA;
    int** MA2;
    int n;
     
    ifstream arquivo("04.txt");
    if(arquivo){
        arquivo >> n;
        while (n != 0){
            
            // Aloca a matriz de adjacencia
            MA = new int*[n];
            MA2 = new int*[n];
            
            // Inicializa a matriz de adjacencia
            for(int i = 0; i < n; i++){
                MA[i] = new int[n];
                MA2[i] = new int[n];
                for(int j = 0; j < n; j++){
                    MA[i][j] = INF;
                    MA2[i][j] = INF;
                }
            }

            // Le a matriz de adjacencia
            for(int i = 0; i < n; i++){
                for(int j = 0; j < n; j++){
                    arquivo >> MA[i][j];
                    MA2[i][j] = MA[i][j];
                }
            }
            
            // Aplicando o algoritmo de Floyd-Warshall na matriz de adjacencia MA
            floyd_warshall(MA, n);
            // Exibe se as matrizes são iguais
            cout << endl << comparaMatrizes(MA, MA2, n) << endl ;

            //cout << endl << endl;
            //printMatriz(MA, n);
            //cout << endl << endl;
            //printMatriz(MA2, n);

            // desalocando a matriz
            for(int i = 0; i < n; i++){
                delete[] MA[i];
                delete[] MA2[i];
            }
            delete[] MA;
            delete[] MA2;

            // Inicia uma nova instancia do arquivo
            arquivo >> n;
        }
    }
    else{
        cout<<"Erro ao abrir o aquivo"<<endl;
        arquivo.clear();
    }
    // Fecha o arquivo
    arquivo.close();
    

    
    //for(int i = 1 ; i < (n - 1)/2 ; i++){
    //    cout << "i:" << i <<" -> j:" << i + ((n-1)/2) << "      " << MA[i][i + ((n-1)/2)] << "   " << MA2[i][i + ((n-1)/2)] << endl;
    //}
    return 0;
}
