#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "instancia.h"
#include "vertice.h"

#include <vector>

string campos [10];
int coutCampos = 0;

void preencheInstancia(string linha){
    coutCampos = 0;
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

void preencheNo(string linha){
    coutCampos = 0;
    string aux = "";
    for(int i = 0; i < linha.size(); i++){
        if(linha[i] != ' ')
            aux += linha[i];
        else {
            campos[coutCampos] = aux;
            aux = "";
        }
    }
    Vertice* v = new Vertice();
    for(int i = 0; i < 8; i++){
        switch (i)
        {
        case 0:
            v->set_id(stoi(campos[i]));
            break;
        case 1:
            v->set_lat(stod(campos[i]));
            break;
        case 2:
            v->set_longi(stod(campos[i]));
            break;
        case 3:
            v->set_dem(stoi(campos[i]));
            break;
        case 4:
            v->set_etw(stoi(campos[i]));
            break;
        case 5:
            v->set_ltw(stoi(campos[i]));
            break;
        case 6:
            v->set_dur(stoi(campos[i]));
            break;
        case 7:
            v->set_p(stoi(campos[i]));
            break;
        case 8:
            v->set_d(stoi(campos[i]));
            break;
        }
    }
    cout<<v->get_id() + v->get_lat() + v->get_longi() + v->get_dem() + v->get_etw() + v->get_ltw() + v->get_dur() + v->get_p() + v->get_d()<<endl;
}

int main()
{    
    string linha;
    bool t = true;
    int altLeitura = 0;
    ifstream arquivo("instances/bar-n100-1.txt");

    if(arquivo){
        while(getline(arquivo, linha)){
            if(altLeitura == 0){
                preencheInstancia(linha);
                
                if(coutCampos == 10)
                    altLeitura++;

            } else if(altLeitura == 1){
                if(linha != "NODES" && linha != "EDGES")
                    preencheNo(linha);
                if(linha == "EDGES")
                    altLeitura++;
            } else {
                cout<<"FIM! ";
            }
        }
        /*cout<<endl;
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

        delete i;*/
        arquivo.close();
    }
    
    else cout << "Erro ao abrir o arquivo!" << endl;

    return 0;
}