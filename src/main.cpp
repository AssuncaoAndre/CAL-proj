#include "Map.h"

Map read_file(string filename);
void print_gui(Graph graph, int garagem);

int main() {

    Map map;
    map=read_file("../src/teste2.txt");

    print_gui(map.graph,map.garagem);
    return 0;
}


Map read_file(string filename)
{
    Map map;
    node_data temp;
    float x,y;
    int n_nodes, n_edges, n_encomendas, n_casas, n_lojas, n_carregadores,v1,v2,weight, id;
    encomenda enc;
    vector<Vertex*> vector;
    fstream file;

    file.open(filename,ios::in);  // open a file to perform write operation using file object

    if (file.is_open()){   //checking whether the file is open
        string line;


        getline(file, line);
        sscanf(line.c_str(), "%d", &n_nodes);
        for (int i=0;i<n_nodes;i++){
            getline(file, line);
            sscanf(line.c_str(), "%f %f",&x,&y);
            map.graph.addVertex(new Vertex(x,y));
        }

        getline(file, line);
        sscanf(line.c_str(), "%d", &n_edges);
        for (int i=0;i<n_edges;i++){
            getline(file, line);
            sscanf(line.c_str(),"%d %d %d",&v1,&v2, &weight);
            map.graph.vertexSet[v1]->addEdge(map.graph.vertexSet[v2],v2,weight);
        }

        getline(file, line);
        sscanf(line.c_str(), "%d", &n_casas);
        for (int i=0;i<n_casas;i++){
            getline(file, line);
            sscanf(line.c_str(),"%d",&id);
            map.casas.push_back(id);
            map.graph.vertexSet[id]->info.is_casa= true;
        }

        getline(file, line);
        sscanf(line.c_str(), "%d", &n_lojas);
        for (int i=0;i<n_lojas;i++){
            getline(file, line);
            sscanf(line.c_str(),"%d",&id);
            map.lojas.push_back(id);
            map.graph.vertexSet[id]->info.is_loja= true;
        }

        getline(file, line);
        sscanf(line.c_str(), "%d", &n_carregadores);
        for (int i=0;i<n_carregadores;i++){
            getline(file, line);
            sscanf(line.c_str(),"%d",&id);
            map.carregadores.push_back(id);
            map.graph.vertexSet[id]->info.is_carregador= true;
        }


        getline(file, line);
        sscanf(line.c_str(), "%d", &n_encomendas);
        for (int i=0;i<n_encomendas;i++){
            getline(file, line);
            sscanf(line.c_str(),"%d %d",&enc.origem,&enc.destino);
            map.encomendas.push_back(enc);
        }

        getline(file, line);
        sscanf(line.c_str(), "%d", &map.garagem);

        file.close(); //close the file object.
    }
    else printf("Wrong file!\n");
    return map;
}

void print_gui(Graph graph, int garagem){

    GraphViewer gv;

    // Set coordinates of window center
    gv.setCenter(sf::Vector2f(300, 300));
    // Create window
    gv.createWindow(600, 600);

    for (int i=0;i<graph.vertexSet.size();i++)
    {

        node_data data=graph.vertexSet[i]->info;
        GraphViewer::Node &node = gv.addNode(i,sf::Vector2f(data.x,data.y));
        if(data.is_carregador)
            node.setColor(GraphViewer::GREEN);

        if(data.is_carregador )
            node.setColor(GraphViewer::YELLOW);

        if(data.is_loja)
            node.setColor(GraphViewer::BLUE);

        if(data.is_casa)
            node.setColor(GraphViewer::LIGHT_GRAY);

        if(data.is_casa && data.is_loja)
            node.setColor(GraphViewer::PINK);

        if(i==garagem)
            node.setColor(GraphViewer::GREEN);

    }
    int k=0;
    for (int i=0;i<graph.vertexSet.size();i++)
    {
        for (int j=0;j<graph.vertexSet[i]->adj.size();j++)
        {
            GraphViewer::Edge &ed = gv.addEdge(k,gv.getNode(i),gv.getNode(graph.vertexSet[i]->adj[j].dest_id),GraphViewer::Edge::EdgeType::UNDIRECTED);
            ed.setColor(GraphViewer::BLACK);
            k++;
        }
    }
    gv.join();
}

