#include "Map.h"

City_Map read_file(string filename);
void print_gui(City_Map city_Map);
City_Map read_folder(char *folder);
void read_edges(string filename, City_Map &city_map);
void read_nodes(string filename, City_Map &city_map);
double distance(node_data v1, node_data v2);

int main() {

    City_Map city_map;
    char str[]={"../src/maps/PortugalMaps/Gondomar"};
    city_map=read_folder(str);
    city_map.garagem=0;
    print_gui(city_map);
    return 0;
}

City_Map read_folder(char* folder)
{
    City_Map city_map;
    string foldername(folder);
    char *token= strtok(folder, "/");

    while (strcmp("PortugalMaps",token)!=0)
    {
        token = strtok(NULL, "/");
    }
    token = strtok(NULL, "/");
    token[0]=token[0]+32;

    string city(token);

    string nodes_file=foldername+"/nodes_x_y_"+city+".txt";
    string edges_file=foldername+"/edges_"+city+".txt";
    cout<<nodes_file<<"\n";
    cout<<edges_file<<"\n";
    read_nodes(nodes_file,city_map);
    read_edges(edges_file,city_map);

    return city_map;
}

void read_edges(string filename, City_Map &city_map)
{
    fstream file;
    double weight;
    int n_edges;
    unsigned long  v1,v2;
    file.open(filename,ios::in);  // open a file to perform write operation using file object

    if (file.is_open()) {   //checking whether the file is open
        string line;

        getline(file, line);

        sscanf(line.c_str(), "%d", &n_edges);

        for (int i=0;i<n_edges;i++){
            getline(file, line);
            sscanf(line.c_str(),"(%lu, %lu)",&v1,&v2);

            weight=distance(city_map.vextexes.at(v1)->info,city_map.vextexes.at(v2)->info);

            city_map.vextexes.at(v1)->addEdge(city_map.vextexes.at(v2),v2,weight);

        }

    }
    else{
        printf("Error opening edges file\n");
        exit(1);
    }
    file.close();
}

void read_nodes(string filename, City_Map &city_map)
{
    fstream file;
    int n_nodes;
    unsigned long id;
    double x,y;
    Vertex *v;
    file.open(filename,ios::in);  // open a file to perform write operation using file object

    if (file.is_open()) {   //checking whether the file is open
        string line;

        getline(file, line);
        sscanf(line.c_str(), "%d", &n_nodes);
        for (int i = 0; i < n_nodes; i++) {
            getline(file, line);
            sscanf(line.c_str(), "(%lu, %lf, %lf)", &id, &x, &y);

            v=new Vertex(x, y);
            city_map.vextexes.insert({id,v});
            city_map.graph.addVertex(v);
        }
    }
    else{
        printf("Error opening nodes file\n");
        exit(1);
    }
    file.close();
}



double distance(node_data v1, node_data v2)
{
    return(sqrt(((v1.x+v2.x)*(v1.x+v2.x))+((v1.y+v2.y)*(v1.y+v2.y))));
}

void print_gui(City_Map city_map){

    unsigned long garagem=city_map.garagem;
    Graph graph=city_map.graph;
    GraphViewer gv;

    // Set coordinates of window center
    gv.setCenter(sf::Vector2f(539223.6710850116, 4554383.836588797));
    // Create window
    map<unsigned long, Vertex*>::iterator it=city_map.vextexes.begin();
    while (it!=city_map.vextexes.end())
    {
        node_data data=it->second->info;
        GraphViewer::Node &node = gv.addNode(it->first,sf::Vector2f(data.x,data.y));
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

        if(it->first==garagem)
            node.setColor(GraphViewer::GREEN);


        it++;
        //node.setSize(0.0001);
        //node.setOutlineThickness(0.1);
    }
    int k=0;
    it=city_map.vextexes.begin();
    while (it!=city_map.vextexes.end()) {
        for (int j=0;j<it->second->adj.size();j++)
        {

            //dest_id não é o id aqui no coiso
            GraphViewer::Edge &ed = gv.addEdge(k,gv.getNode(it->first),gv.getNode(it->second->adj[j].dest_id),GraphViewer::Edge::EdgeType::UNDIRECTED);

            ed.setColor(GraphViewer::BLACK);

            //ed.setThickness(0.0001);
            k++;
        }
        it++;
    }




    gv.createWindow(1600, 900);
    gv.join();

}

