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
    vector< unsigned long> casas, lojas, carregadores;
    vector<encomenda> encomendas;
     unsigned long garagem;
    map< unsigned long, Vertex*> vextexes;

};