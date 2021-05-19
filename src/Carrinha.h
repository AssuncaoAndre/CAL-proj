//
// Created by andre on 16/05/21.
//

#ifndef PROJ_CARRINHA_H
#define PROJ_CARRINHA_H


class Carrinha{
public:

    list<Vertex*> route;
    double dist=0;
    double bateria = 5000;
    vector<unsigned long> encomendas;
    int id;


    Carrinha(Vertex *loja, int id);
};

Carrinha::Carrinha(Vertex *loja, int id){
    route.emplace_back(loja);
    route.emplace_back(loja);
    this->id=id;
}


#endif //PROJ_CARRINHA_H
