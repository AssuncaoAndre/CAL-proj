#include <iostream>
#include "Graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string>
#include <fstream>
#include<map>
#include "graphviewer.h"

typedef struct encomenda
{
    unsigned long  origem;
    unsigned long  destino;
}encomenda;

class City_Map{
    public:
    Graph graph;
    vector<unsigned long> casas, lojas, carregadores;
    vector<encomenda> encomendas;
    unsigned long garagem;
    map<unsigned long, Vertex*> vertexes;

    void remove_non_visited();
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
