#ifndef INSTANCIA_H_INCLUDED
#define INSTANCIA_H_INCLUDED
#include <iostream>
#include "Vertice.h"

using namespace std;

class Instancia
{
    public:
        Instancia(string name, string location, string comment, string type, int size, string distribution, string depot, int routeTime, int timeWindow, int capacity);
    //private:
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
        Vertice *vertices;    //vetor de vertices
};

//Construtor
Instancia::Instancia(string name, string location, string comment, string type, int size, string distribution, string depot, int routeTime, int timeWindow, int capacity)
{
    this->name = name;
    this->location = location;
    this->comment = comment;
    this->type = type;
    this->size = size;
    this->distribution = distribution;
    this->depot = depot;
    this->routeTime = routeTime;
    this->timeWindow = timeWindow;
    this->capacity = capacity;
    vertices = new Vertice[size];
}

#endif // INSTANCIA_H_INCLUDED