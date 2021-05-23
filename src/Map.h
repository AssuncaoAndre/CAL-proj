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
    void add_vertex_to_route(Vertex *dest, Carrinha *carrinha);
    list<Vertex*> get_list_from_path(Vertex *dest);
    bool closest_carregador(Vertex* src, double limit, Carrinha* carrinha);
    void add_garagem(Carrinha *carrinha);
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
    for(auto v: vertexes)
    {
        if(v.second->info.is_casa)
        {
            encomendas.push_back(v.first);
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


    for (int i=0;i<n_carrinhas;i++)
        carrinhas.push_back(Carrinha(vertexes.at(loja),i));

    get_seeds();

    int carrinha_to_add;
    for (int i=0;i<encomendas.size();i++)
    {

        carrinha_to_add=get_carrinha_menos_ocupada(vertexes.at(encomendas[i]));

        if(vertexes.at(encomendas[i])->info.is_casa && vertexes.at(encomendas[i])->carrinha==-1)
            add_vertex_to_route(vertexes.at(encomendas[i]),&carrinhas[carrinha_to_add]);
    }

    for (int i = 0; i < n_carrinhas; i++) {
        add_garagem(&carrinhas[i]);
        add_carregadores(&carrinhas[i]);
    }
}

int City_Map::get_carrinha_menos_ocupada(Vertex *dest)
{

    int min_i;
    double min_all = INF;

    for (int i = 0; i < n_carrinhas; i++) {
        list<Vertex*>::iterator pos, prev;
        list<Vertex*>::iterator first = carrinhas[i].route.begin();
        first++;
        double min_dist = INF, astar1, astar2;

        for (pos = first; pos != carrinhas[i].route.end(); pos++) {
            prev = pos; prev--;
            astar1 = euclidian_distance(*prev, dest);
            astar2 = euclidian_distance(dest, *pos);

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




void City_Map::add_vertex_to_route(Vertex *dest, Carrinha *carrinha) {

    list<Vertex*>::iterator min_pos,pos,next, prev, min_prev;
    list<Vertex*>::iterator first=carrinha->route.begin();
    first++;

    double min_dist=INF, astar1,astar2;
    list<Vertex*> temp_list,p1_dest,dest_p2;

    for (pos=first;pos!=carrinha->route.end();pos++)
    {

        prev=pos; prev--;

        astar1 = euclidian_distance(*prev, dest);
        astar2 = euclidian_distance(dest, *pos);
        if((astar1+astar2)<min_dist)
        {
            min_dist=astar1+astar2;
            min_pos=pos;
            min_prev = prev;
        }
    }
    carrinha->encomendas.push_back(dest->info.id);
    dest->carrinha=carrinha->id;
    graph.aStar(*min_prev, dest);
    p1_dest = get_list_from_path(dest);
    graph.aStar(dest, *min_pos);
    dest_p2= get_list_from_path(*min_pos);


    for(auto v: p1_dest)
    {
        if(v->info.is_casa)
            v->carrinha=carrinha->id;
    }

    for(auto v: dest_p2)
    {
        if(v->info.is_casa)
            v->carrinha=carrinha->id;

    }

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

bool City_Map::closest_carregador(Vertex *src, double limit, Carrinha *carrinha) {
    double min = INF;
    int min_i;
    double dist = 0;
    for (int i = 0; i < postos.size(); i++) {
        double dist = euclidian_distance(vertexes.at(postos[i]), src);
        if (dist < min) {
            min = dist;
            min_i = i;
        }
    }

    dist = graph.aStar(src, vertexes.at(postos[min_i]));
    double score = dist * 0.00001;

    if (score < limit) {
        add_vertex_to_route(vertexes.at(postos[min_i]), carrinha);
        carrinha->carregadores.push_back(postos[min_i]);
        return true;
    }

    else return false;
}

void City_Map::add_carregadores(Carrinha *carrinha) {
    list<Vertex*>::iterator it = carrinha->route.begin();
    list<Vertex*>::iterator prev = carrinha->route.begin();
    ++it;

    while (it != carrinha->route.end()) {
        if (carrinha->bateria <= 100) {
            while (!closest_carregador(*it, carrinha->bateria, carrinha)) {
                --it;
                --prev;
                for (Edge e : (*prev)->adj) {
                    if (e.dest->info.id == (*it)->info.id) {
                        carrinha->bateria += e.weight * 0.00001;
                    }
                }
            }

            carrinha->bateria = 5000;
        }

        else  {
            for (Edge e : (*prev)->adj) {
                if (e.dest->info.id == (*it)->info.id) {
                    carrinha->bateria -= e.weight * 0.00001;
                }
            }
        }

        ++it;
        ++prev;
    }
}

void City_Map::get_seeds() {
    double max, dist;
    vector<vector<unsigned long>::iterator> vec;

    for (int i = 0; i < n_carrinhas; i++) {
        max = 0;
        vector<unsigned long>::iterator max_i;
        for (auto it = encomendas.begin(); it != encomendas.end(); ++it) {

            if (i == 0) {
                dist = euclidian_distance(vertexes.at(loja), vertexes.at(*it));
                if (dist > max) {
                    max_i = it;
                    max = dist;
                }
            }

            else {
                int j = 0;
                while (j < vec.size()) {
                    dist = euclidian_distance(vertexes.at(*vec[j]), vertexes.at(*it));
                    if (dist < max) {
                        break;
                    }
                    j++;
                }

                if (j == vec.size()) {
                    max_i = it;
                    max = dist;
                }
            }
        }

        add_vertex_to_route(vertexes.at(*max_i), &carrinhas[i]);
        vec.push_back(max_i);
    }
}

void City_Map::add_garagem(Carrinha *carrinha) {
    graph.aStar(vertexes.at(garagem), vertexes.at(loja));
    list<Vertex*> l = get_list_from_path(vertexes.at(loja));
    list<Vertex*>::iterator it = carrinha->route.begin();
    carrinha->route.splice(it, l);
}
