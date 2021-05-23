#include <unistd.h>
#include "Map.h"

#define DIST_TO_KM 0.00000001
City_Map read_file(string filename);
void print_gui(City_Map city_Map);
City_Map read_folder(string folder);
void read_edges(string filename, City_Map &city_map);
void read_nodes(string filename, City_Map &city_map);
void read_tags(string filename, City_Map &city_map, string type);
double distance(node_data v1, node_data v2);

int main() {

    string city;
    int n;
    printf("Enter the city to load the map from: ");
    cin>>city;
    City_Map city_map;

    city_map=read_folder(city);

    printf("Enter the node id of the garage: ");
    cin>>city_map.garagem;

    printf("Enter the node id of the shop: ");
    cin>>city_map.loja;

    printf("Enter the number of available trucks: ");
    cin>>city_map.n_carrinhas;

    city_map.graph.bfs(city_map.vertexes.at(city_map.garagem));

    city_map.remove_non_visited();

    city_map.fill_encomendas();
    city_map.fill_postos();
    city_map.plan_routes();



    print_gui(city_map);

    return 0;
}

City_Map read_folder(string city)
{
    City_Map city_map;
    string original = city;
    city[0] = city[0] + 32;

    string nodes_file = "../src/maps/PortugalMaps/" + original + "/nodes_x_y_" + city + ".txt";
    string edges_file = "../src/maps/PortugalMaps/" + original + "/edges_" + city + ".txt";
    string houses_file = "../src/maps/TagExamples/" + original + "/t10_tags_" + city + ".txt";
    string stores_file = "../src/maps/TagExamples/" + original + "/t04_tags_" + city + ".txt";
    string rechargers_file = "../src/maps/TagExamples/" + original + "/t08_tags_" + city + ".txt";

    read_nodes(nodes_file,city_map);
    read_edges(edges_file,city_map);
    read_tags(houses_file, city_map, "casa");
    read_tags(stores_file, city_map, "loja");
    read_tags(rechargers_file, city_map, "carregador");


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

            weight=distance(city_map.vertexes.at(v1)->info,city_map.vertexes.at(v2)->info);

            city_map.vertexes.at(v1)->addEdge(city_map.vertexes.at(v2),v2,weight);
            city_map.vertexes.at(v2)->addEdge(city_map.vertexes.at(v1),v1,weight);

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
            v->info.id=id;
            city_map.vertexes.insert({id,v});
            city_map.graph.addVertex(v);
        }
    }
    else{
        printf("That city does not exist\n");
        exit(1);
    }
    file.close();
}

void read_tags(string filename, City_Map &city_map, string type) {
    fstream file;
    int n_tags, n_nodes;
    unsigned long id;
    Vertex *v;
    file.open(filename,ios::in);  // open a file to perform write operation using file object

    if (file.is_open()) {   //checking whether the file is open
        string line;

        getline(file, line);
        sscanf(line.c_str(), "%d", &n_tags);


        for (int i = 0; i < n_tags; i++) {
            getline(file, line);
            getline(file, line);
            sscanf(line.c_str(), "%d", &n_nodes);

            for (int j = 0; j < n_nodes; j++){
                getline(file, line);
                sscanf(line.c_str(), "%lu", &id);


                if(type == "casa") {
                    city_map.casas.push_back(id);
                    city_map.vertexes.at(id)->info.is_casa = true;
                }

                else if(type == "loja") {
                    city_map.lojas.push_back(id);
                    city_map.vertexes.at(id)->info.is_loja = true;
                }

                else {
                    city_map.carregadores.push_back(id);
                    city_map.vertexes.at(id)->info.is_carregador = true;
                }
            }

        }
    }
    else{
        printf("Error opening tags file\n");
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
    map<unsigned long, Vertex*>::iterator it=city_map.vertexes.begin();
    gv.setCenter(sf::Vector2f(city_map.vertexes.at(city_map.garagem)->info.x, city_map.vertexes.at(city_map.garagem)->info.y));

    // Create window
    while (it!=city_map.vertexes.end())
    {
        node_data data=it->second->info;
        GraphViewer::Node &node = gv.addNode(it->first,sf::Vector2f(data.x,data.y));
        node.setColor(GraphViewer::BLACK);

        if(it->first==garagem)
            node.setColor(GraphViewer::GREEN);
        it++;

    }

    int k=0;
    it=city_map.vertexes.begin();
    while (it!=city_map.vertexes.end()) {

        for (int j=0;j<it->second->adj.size();j++)
        {
            it->second->adj[j].id=k;
            GraphViewer::Edge &ed = gv.addEdge(k,gv.getNode(it->first),gv.getNode(it->second->adj[j].dest_id),GraphViewer::Edge::EdgeType::UNDIRECTED);

            ed.setColor(GraphViewer::BLACK);

            k++;
        }

        it++;
    }


/*    printf("Distância percorrida pela carrinha 1: %.2lf km\n",city_map.carrinhas[0].dist/1000);
    printf("Distância percorrida pela carrinha 2: %.2lf km\n",city_map.carrinhas[1].dist/1000);
    printf("Distância percorrida pela carrinha 3: %.2lf km\n",city_map.carrinhas[2].dist/1000);
    list<Vertex*>::iterator it_l;
    list<Vertex*>::iterator next_l;
    vector<unsigned long>::iterator it_v;*/

    gv.createWindow(1600, 900);

   /* int i = 1;

    while(gv.isWindowOpen()) {
        for(int i=0;i<city_map.n_carrinhas;i++)
        {
            gv.lock();
            int prev=i-1;
            if(prev<0)
                prev=city_map.n_carrinhas-1;
            for(it_l=city_map.carrinhas[prev].route.begin();it_l!=city_map.carrinhas[prev].route.end();it_l++)
            {
                gv.getNode((*it_l)->info.id).setColor(GraphViewer::BLACK);



                next_l=it_l;
                next_l++;

                if(next_l==city_map.carrinhas[prev].route.end())
                    continue;

                for(auto e:(*it_l)->adj)
                {
                    if(e.dest_id==(*next_l)->info.id)
                    {
                        gv.getEdge(e.id).setColor(GraphViewer::BLACK);
                    }
                }
            }
            for(it_l=city_map.carrinhas[i].route.begin();it_l!=city_map.carrinhas[i].route.end();it_l++)
            {


                next_l=it_l;
                next_l++;
                gv.getNode((*it_l)->info.id).setColor(GraphViewer::RED);
                if(next_l==city_map.carrinhas[i].route.end())
                    continue;
                for(auto e:(*it_l)->adj)
                {

                    if(e.dest_id==(*next_l)->info.id)
                    {
                        gv.getEdge(e.id).setColor(GraphViewer::RED);
                    }
                }

            }
            for (it_v=city_map.carrinhas[i].encomendas.begin();it_v!=city_map.carrinhas[i].encomendas.end();it_v++)
            {
                gv.getNode((*it_v)).setColor(GraphViewer::CYAN);
            }

            for (it_v=city_map.carrinhas[i].carregadores.begin();it_v!=city_map.carrinhas[i].carregadores.end();it_v++)
            {
                gv.getNode((*it_v)).setColor(GraphViewer::MAGENTA);
            }
            gv.unlock();

            sleep(2);
        }
    }*/

    gv.join();

}

