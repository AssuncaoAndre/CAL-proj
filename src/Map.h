#include <iostream>
#include "Graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string>
#include <fstream>
#include<map>
#include "Carrinha.h"
#include "graphviewer.h"

/*typedef struct encomenda
{
    unsigned long  origem;
    unsigned long  destino;
}encomenda;*/

class City_Map{
    public:
    vector<Carrinha> carrinhas;
    int n_carrinhas=3;
    Graph graph;
    vector<unsigned long> casas, lojas, carregadores;
    vector<unsigned long> encomendas;
    unsigned long garagem, loja;
    map<unsigned long, Vertex*> vertexes;

    vector<Vertex*> get_seeds();
    void fill_encomendas();
    void remove_non_visited();
    void plan_routes();
    int get_carrinha_menos_ocupada();
    void add_vertex_to_route(Vertex *dest, Carrinha *carrinha);
    list<Vertex*> get_list_from_path(Vertex *dest);
};

void City_Map::remove_non_visited() {
    unsigned long id_to_erase;
    int n=0;
    map<unsigned long, Vertex*>::iterator it;
    vector<Edge>::iterator it_e;

    for(it=vertexes.begin();it!=vertexes.end();it++)
    {
        vector<Edge> a=(*it).second->adj;
        for(it_e=(*it).second->adj.begin();it_e!=(*it).second->adj.end();)
        {
            if(!(*it_e).dest->visited)
                (*it).second->adj.erase(it_e);
            else ++it_e;
        }
    }

    vector<Vertex *>::iterator it_v;
    for (it_v=graph.vertexSet.begin();it_v!=graph.vertexSet.end();)
    {
        if(!(*it_v)->visited)
        {
            vertexes.erase((*it_v)->info.id);
            graph.vertexSet.erase(it_v);
        }
        else
            it_v++;
    }
}

void City_Map::fill_encomendas() {
    int i=0;
    encomendas.push_back(1198976574);
    encomendas.push_back(1199479598);
    encomendas.push_back(1199479624);
/*    for(auto v: vertexes)
    {
        if(v.second->info.is_casa)
        {

            encomendas.push_back(v.first);
            i++;
        }
    }*/
}

void City_Map::plan_routes() {

    //get_carrinha_menos_ocupada em de ser substituido por
    //uma carrinha que faça aumentar o minimo possivel o máximo do percurso

    for (int i=0;i<n_carrinhas;i++)
        carrinhas.push_back(Carrinha(vertexes.at(loja)));
    int carrinha_to_add;
    for (int i=0;i<encomendas.size();i++)
    {

        carrinha_to_add=get_carrinha_menos_ocupada();

        add_vertex_to_route(vertexes.at(encomendas[i]),&carrinhas[carrinha_to_add]);
    }
    /*get_seeds();*/
}

int City_Map::get_carrinha_menos_ocupada()
{
    double min=INF;
    int min_i;
    for(int i=0;i<n_carrinhas;i++)
    {
        if(carrinhas[i].dist==0)
            return i;
        if(carrinhas[i].dist<min)
        {
            min=carrinhas[i].dist;
            min_i=i;
        }
    }
    return min_i;
}

void City_Map::add_vertex_to_route(Vertex *dest, Carrinha *carrinha) {

    list<Vertex*>::iterator min_pos,pos,next, prev;
    list<Vertex*>::iterator first=carrinha->route.begin();
    first++;

    double min_dist=INF, astar1,astar2;
    list<Vertex*> temp_list,p1_dest,dest_p2;

    for (pos=first;pos!=carrinha->route.end();pos++)
    {

        prev=pos; prev--;
        astar1=graph.aStar(*prev,dest);
        temp_list=get_list_from_path(dest);

        astar2=graph.aStar(dest,*pos);
        if((astar1+astar2)<min_dist)
        {
            min_dist=astar1+astar2;
            min_pos=pos;
            p1_dest=temp_list;
            dest_p2=get_list_from_path(*pos);
        }
    }
    next=pos;next++;
    carrinha->route.splice(next,p1_dest);
    next++;
    carrinha->route.splice(next,dest_p2);


    carrinha->dist=carrinha->dist+min_dist;
}

list<Vertex*>City_Map::get_list_from_path(Vertex *dest) {
    list<Vertex*> l;

    for(auto v:vertexes)
        v.second->visited=false;

    for (Vertex *a = vertexes.at(dest->info.id); a->path!=NULL;a=a->path)
    {
        if(a->visited==true)
            break;
        l.emplace_front(a);
        a->visited=true;
    }
    return l;
}

/*vector<Vertex*>City_Map::get_seeds() {
    for(int i=0;i<n_carrinhas;i++)
    {

    }
}*/
