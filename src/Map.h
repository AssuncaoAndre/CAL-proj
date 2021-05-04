#include <iostream>
#include "Graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include "graphviewer.h"

typedef struct encomenda
{
    int origem;
    int destino;
}encomenda;

class Map{
    public:
    Graph graph;
    vector<int> casas, lojas, carregadores;
    vector<encomenda> encomendas;
    int garagem;
};