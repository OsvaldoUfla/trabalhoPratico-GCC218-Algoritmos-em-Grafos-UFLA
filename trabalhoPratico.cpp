#include <iostream>
#include <fstream>

using namespace std;

int main()
{

    string name;                //NAME: <identificação de instância única>  
    string location;            //LOCATION: <cidade onde foi gerado>  
    string comment;             //COMMENT: : <uma referência geral>  
    string type;               //TYPE: <o tipo da instância>  
    int size;                  //SIZE: <número de vértices incluindo o depósito>  
    string distribution;     //DISTRIBUTION: <distribuição a qual a instância foi gerada>  
    string depot;               //DEPOT: <tipo de localização do depósito: 'central' ou 'aleatório'>  
    int routeTime;             //ROUTE-TIME: <horizonte de tempo da roteirização>  
    int timeWindow;            //TIME-WINDOW: <largura da janela de tempo>  
    int capacity;               //CAPACITY: <capacidade máxima do veículo> 
    
    ifstream arquivo("instances/bar-n100-1.txt");
    if(!arquivo){
        cout << "Erro ao abrir o arquivo!" << endl;
    }
    else{
        string linha;
        while(getline(arquivo, linha)){
            cout << linha << endl;
        }
    }
    return 0;
}