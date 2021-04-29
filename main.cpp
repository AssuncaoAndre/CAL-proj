#include <iostream>
#include "Graph.h"

int main() {
    Graph<int> g;

    g.readFile("test1.txt");

    vector<Vertex<int> *> vect = g.getVertexSet();
    vector<Order<int> *> orders = g.getOrders();

    for(int i = 0; i < vect.size(); i++)
        cout << vect[i]->getType() << "\n";

    vector<Edge<int> *> edge = vect[0]->getOutgoing();

    for (int i = 0; i < edge.size(); i++)
        cout << edge[i]->getWeight() << "\n";

    for(int i = 0; i < orders.size(); i++)
        cout << orders[i]->getOrig()->getInfo() << " " << orders[i]->getDest()->getInfo() << "\n";

    return 0;
}
