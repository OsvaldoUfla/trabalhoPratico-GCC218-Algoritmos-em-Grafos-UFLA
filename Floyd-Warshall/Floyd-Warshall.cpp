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

#define INF 1000000000

/*
 * Variaveis globais
 */

// matriz de adjacencia
int** MA;
int** MA2;

// quantidade de vertices
int n;

// quantidade de arestas
int m;

// quantidade de pares origem-destino a ser calculado
int k;

int floyd_warshall()
{
    for(int k = 0; k < n; k++)
      for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
          /*
           * Adaptacao: originalmente, o calculo eh MA[i][j] = min(MA[i][j], MA[i][k] + MA[k][j]);
           */
          MA[i][j] = min(MA[i][j], max(MA[i][k], MA[k][j]));
}

/* Função para ler o arquivo
*  Recebe o nome do arquivo como parâmetro, Matriz de adjacência, Matriz de adjacência 2
*  Preenche as matrizes com os valores do arquivo
*/
void leitura(string nmArq, int** MA, int** MA2){
    
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

int main()
{
    int instancia = 1;
    cin >> n >> m;
    while(n != 0 && m != 0)
    {     
        MA = new int*[n];
        int u, v, h;
     
        for(int i = 0; i < n; i++)
        {
            MA[i] = new int[n];
            for(int j = 0; j < n; j++)
                MA[i][j] = INF;
        }

        for(int i = 0; i < m; i++)
        {
            cin >> u >> v >> h;
            u--;
            v--;
            MA[u][v] = MA[v][u] = h;
        }
     
        floyd_warshall();
     
        int org, dst, k;
        cin >> k;
        cout << "Instancia " << instancia << endl;
        for(int i = 0; i < k; i++)
        {
            cin >> org >> dst;
            org--;
            dst--;
            if(org != dst)
              cout << MA[org][dst] << endl;
         
            else
              cout << 0 << endl;
        }
        instancia++;
        cout << endl;
     
        cin >> n >> m;
    }
 
    return 0;
}
