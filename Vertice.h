#ifndef VERTICE_H_INCLUDED
#define VERTICE_H_INCLUDED

class Vertice
{
    public:
        void setDistancias(int *distancias); //seta o vetor de distancias
        void set_id(int id);                 //seta o id do vertice
        void set_lat(double lat);            //seta a latitude do vertice
        void set_longi(double longi);        //seta a longitude do vertice
        void set_dem(int dem);               //seta a demanda do vertice
        void set_etw(int etw);               //seta o tempo mais cedo para iniciar o serviço
        void set_ltw(int ltw);               //seta o tempo mais tarde para iniciar o serviço
        void set_dur(int dur);               //seta a duração do serviço
        void set_p(int p);                   //seta o par de coleta
        void set_d(int d);                   //seta o par de entrega

        int get_id();                        //retorna o id do vertice
        double get_lat();                    //retorna a latitude do vertice
        double get_longi();                  //retorna a longitude do vertice
        int get_dem();                       //retorna a demanda do vertice
        int get_etw();                       //retorna o tempo mais cedo para iniciar o serviço
        int get_ltw();                       //retorna o tempo mais tarde para iniciar o serviço
        int get_dur();                       //retorna a duração do serviço
        int get_p();                         //retorna o par de coleta
        int get_d();                         //retorna o par de entrega

    private:
        int id;          //<id>: o identificador único do ponto (o ponto 0 é o depósito único);
        double lat;      //<lat>: latitude deste local;
        double longi;    //<long>: longitude deste local;
        int dem;         //<dem>: a demanda deste nó (dem > 0 para coleta, dem < 0 para entrega);
        int etw;         //<etw>: tempo mais cedo possível para iniciar o serviço (janela de tempo);
        int ltw;         //<ltw>: última hora possível para iniciar o serviço (janela de tempo);
        int dur;         //<dur>: a duração do serviço neste local;
        int p;           //<p>: o par de coleta se \<id> for uma entrega e 0 caso contrário;
        int d;           //<d>: o par de entrega se \<id> for uma coleta e 0 caso contrário;
        int *distancias; //distancia entre os vertices; números inteiros representam os tempos de viagem entre cada par de locais na instância, medidos em minutos
};

void Vertice::setDistancias(int *distancias){ this->distancias = distancias; }
void Vertice::set_id(int id){ this->id = id; }
void Vertice::set_lat(double lat){ this->lat = lat; }
void Vertice::set_longi(double longi){ this->longi = longi; }
void Vertice::set_dem(int dem){ this->dem = dem; }
void Vertice::set_etw(int etw){ this->etw = etw; }
void Vertice::set_ltw(int ltw){ this->ltw = ltw; }
void Vertice::set_dur(int dur){ this->dur = dur; }
void Vertice::set_p(int p){ this->p = p; }
void Vertice::set_d(int d){ this->d = d; }

double Vertice::get_lat() { return this->lat; }
double Vertice::get_longi() { return this->longi; }
int Vertice::get_id() { return this->id;  }
int Vertice::get_dem() { return this->dem; }
int Vertice::get_etw() { return this->etw; }
int Vertice::get_ltw() { return this->ltw; }
int Vertice::get_dur() { return this->dur; }
int Vertice::get_p() { return this->p;   }
int Vertice::get_d() { return this->d;   }


#endif // VERTICE_H_INCLUDED 