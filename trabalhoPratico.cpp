#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "instancia.h"

#include <vector>

int main()
{
    string name;          //NAME: <identificação de instância única>  
    string location;      //LOCATION: <cidade onde foi gerado>  
    string comment;       //COMMENT: : <uma referência geral>  
    string type;          //TYPE: <o tipo da instância>  
    int size;             //SIZE: <número de vértices incluindo o depósito>  
    string distribution;  //DISTRIBUTION: <distribuição a qual a instância foi gerada>  
    string depot;         //DEPOT: <tipo de localização do depósito: 'central' ou 'aleatório'>  
    int routeTime;        //ROUTE-TIME: <horizonte de tempo da roteirização>  
    int timeWindow;       //TIME-WINDOW: <largura da janela de tempo>  
    int capacity;         //CAPACITY: <capacidade máxima do veículo> 
    
    ifstream arquivo("instances/bar-n100-1.txt");
    if(!arquivo){
        cout << "Erro ao abrir o arquivo!" << endl;
    }
    else{
        string linha;
        bool t = true;//
        string campos [10];//
        int coutCampos = 0;
        while(getline(arquivo, linha)){
            if(t){
                //cout << linha << endl;
                bool addChar = false;
                for(int rl = 0; rl < linha.size(); rl++){
                    if(addChar && coutCampos < 10)
                        campos[coutCampos] += linha[rl];
                    if(linha[rl] == ' ' && linha[rl - 1] == ':')
                        addChar = true;
                }
                if(coutCampos < 10)
                    coutCampos++;

                if(linha == "NODES")
                    t = false;
            }
        }
        Instancia* i = new Instancia(campos[0],campos[1],campos[2],campos[3],stoi(campos[4]),campos[5],campos[6],stoi(campos[7]),stoi(campos[8]),stoi(campos[9]));

        cout << "name: " << i->name << endl;
        cout << "location: " << i->location << endl;
        cout << "comment: " << i->comment << endl;
        cout << "type: " << i->type << endl;
        cout << "size: " << i->size << endl;
        cout << "distribution: " << i->distribution << endl;
        cout << "depot: " << i->depot << endl;
        cout << "routeTime: " << i->routeTime << endl;
        cout << "timeWindow: " << i->timeWindow << endl;
        cout << "capacity: " << i->capacity << endl;

        arquivo.close();
    }
    return 0;
}
