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
    vector<unsigned long> encomendas, postos;
    unsigned long garagem, loja;
    map<unsigned long, Vertex*> vertexes;

    void get_seeds();
    void fill_encomendas();
    void fill_postos();
    void remove_non_visited();
    void plan_routes();
    void add_carregadores(Carrinha *carrinha);
    int get_carrinha_menos_ocupada(Vertex *dest);
    int get_min_dist(Vertex *dest);
    bool has_encomenda(Vertex *dest, Carrinha *carrinha);
    void add_vertex_to_route(Vertex *dest, Carrinha *carrinha);
    list<Vertex*> get_list_from_path(Vertex *dest);
    Vertex* closest_carregador(Vertex* dest);
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
    /*int i=0;
    encomendas.push_back(1198976574);
    encomendas.push_back(1199479598);
    encomendas.push_back(1199479624);*/
    for(auto v: vertexes)
    {
        if(v.second->info.is_casa)
        {
            encomendas.push_back(v.first);
            //i++;
        }
    }
}

void City_Map::fill_postos() {
    for (auto v: vertexes) {
        if (v.second->info.is_carregador) {
            postos.push_back(v.first);
        }
    }
}

void City_Map::plan_routes() {

    //get_carrinha_menos_ocupada em de ser substituido por
    //uma carrinha que faça aumentar o minimo possivel o máximo do percurso

    for (int i=0;i<n_carrinhas;i++)
        carrinhas.push_back(Carrinha(vertexes.at(loja)));

    get_seeds();

    int carrinha_to_add;
    for (int i=0;i<encomendas.size();i++)
    {

        carrinha_to_add=get_carrinha_menos_ocupada(vertexes.at(encomendas[i]));

        if (carrinha_to_add != -1)
            add_vertex_to_route(vertexes.at(encomendas[i]),&carrinhas[carrinha_to_add]);
    }

    for (int i = 0; i < n_carrinhas; i++) {
        add_carregadores(&carrinhas[i]);
    }
}

int City_Map::get_carrinha_menos_ocupada(Vertex *dest)
{
    /*double min=INF;
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
    return get_min_dist(dest);*/
    for (int i = 0; i < n_carrinhas; i++) {
        if (has_encomenda(dest, &carrinhas[i]))
            return -1;
    }

    return get_min_dist(dest);
}

int City_Map::get_min_dist(Vertex *dest) {
    int min_i;
    double min_all = INF;

    for (int i = 0; i < n_carrinhas; i++) {
        list<Vertex*>::iterator pos, prev;
        list<Vertex*>::iterator first = carrinhas[i].route.begin();
        first++;
        double min_dist = INF, astar1, astar2;

        for (pos = first; pos != carrinhas[i].route.end(); pos++) {
            prev = pos; prev--;
            //astar1 = graph.aStar(*prev, dest);
            astar1 = euclidian_distance(*prev, dest);
            astar2 = euclidian_distance(dest, *pos);
            //astar2 = graph.aStar(dest, *pos);

            if ((astar2 + astar1) < min_dist) {
                min_dist = astar1 + astar2;
            }
        }

        min_dist += carrinhas[i].dist;

        if (min_dist < min_all) {
            min_all = min_dist;
            min_i = i;
        }
    }

    return min_i;
}

bool City_Map::has_encomenda(Vertex *dest, Carrinha *carrinha) {
    for (auto it = carrinha->route.begin(); it != carrinha->route.end(); ++it) {
        if (*it == dest)
            return true;
    }

    return false;
}

void City_Map::add_vertex_to_route(Vertex *dest, Carrinha *carrinha) {

    list<Vertex*>::iterator min_pos,pos,next, prev, min_prev;
    list<Vertex*>::iterator first=carrinha->route.begin();
    first++;

    double min_dist=INF, astar1,astar2;
    list<Vertex*> temp_list,p1_dest,dest_p2;

    for (pos=first;pos!=carrinha->route.end();pos++)
    {

        prev=pos; prev--;
        //astar1=graph.aStar(*prev,dest);
        //temp_list=get_list_from_path(dest);
        astar1 = euclidian_distance(*prev, dest);
        astar2 = euclidian_distance(dest, *pos);
        //astar2=graph.aStar(dest,*pos);
        if((astar1+astar2)<min_dist)
        {
            min_dist=astar1+astar2;
            min_pos=pos;
            min_prev = prev;
            /*graph.aStar(*prev, dest);
            p1_dest=get_list_from_path(dest);
            dest_p2=get_list_from_path(*pos);*/
        }
    }

    graph.aStar(*min_prev, dest);
    p1_dest = get_list_from_path(dest);
    dest_p2 = get_list_from_path(*min_pos);
    next=min_pos;next++;
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

Vertex * City_Map::closest_carregador(Vertex *dest) {
    double min = INF;
    int min_i;
    for (int i = 0; i < postos.size(); i++) {
        double dist = euclidian_distance(vertexes.at(postos[i]), dest);
        if (dist < min) {
            min = dist;
            min_i = i;
        }
    }
    return vertexes.at(postos[min_i]);
}

void City_Map::add_carregadores(Carrinha *carrinha) {
    list<Vertex*>::iterator it = carrinha->route.begin();
    list<Vertex*>::iterator prev = carrinha->route.begin();
    ++it;

    while (it != carrinha->route.end()) {
        if (carrinha->bateria <= 0) {
            Vertex* v = closest_carregador(*it);
            add_vertex_to_route(v, carrinha);
            carrinha->bateria = 5000;
        }

        else  {
            double dist = euclidian_distance(*prev, *it);
            carrinha->bateria -= dist * 0.00001;
        }

        ++it;
        ++prev;
    }
}

void City_Map::get_seeds() {
    double max = 0;
    vector<unsigned long>::iterator max_i1, max_i2, max_i3;

    for (auto it = encomendas.begin(); it != encomendas.end(); it++) {
        double dist = euclidian_distance(vertexes.at(loja), vertexes.at(*it));

        if (dist > max) max_i1 = it;
    }

    add_vertex_to_route(vertexes.at(*max_i1), &carrinhas[0]);
    max = 0;

    for (auto it = encomendas.begin(); it != encomendas.end(); it++) {
        double dist = euclidian_distance(vertexes.at(*max_i1), vertexes.at(*it));

        if (dist > max) max_i2 = it;
    }

    add_vertex_to_route(vertexes.at(*max_i2), &carrinhas[1]);
    max = 0;

    for (auto it = encomendas.begin(); it != encomendas.end(); it++) {
        double dist1 = euclidian_distance(vertexes.at(*max_i1), vertexes.at(*it));
        double dist2 = euclidian_distance(vertexes.at(*max_i2), vertexes.at(*it));

        if (dist1 > max && dist2 > max) max_i3 = it;
    }

    add_vertex_to_route(vertexes.at(*max_i3), &carrinhas[2]);
    /*encomendas.erase(max_i1);
    encomendas.erase(max_i2);
    encomendas.erase(max_i3);*/
}
