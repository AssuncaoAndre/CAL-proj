//
// Created by andre on 16/05/21.
//

#ifndef PROJ_CARRINHA_H
#define PROJ_CARRINHA_H


class Carrinha{
public:

    list<Vertex*> route;
    double dist=0;


    Carrinha(Vertex *loja);
};

Carrinha::Carrinha(Vertex *loja){
    route.emplace_back(loja);
    route.emplace_back(loja);
}


#endif //PROJ_CARRINHA_H
