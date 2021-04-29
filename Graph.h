/*
 * Graph.h.
 * For implementation of the minimum cost flow algorithm.
 * See TODOs for code to add/adapt.
 * FEUP, CAL, 2017/18.
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <limits>
#include <iostream>
#include <string>
#include <fstream>
#include "MutablePriorityQueue.h"

using namespace std;

constexpr auto INF = std::numeric_limits<double>::max();

template <class T> class Vertex;
template <class T> class Edge;
template <class T> class Order;
template <class T> class Graph;

/*
 * ================================================================================================
 * Class Vertex
 * ================================================================================================
 */

template <class T>
class Vertex {
	T info;
	vector<Edge<T> *> outgoing;
	vector<Edge<T> *> incoming;
	string type;

	bool visited;  // for path finding
	Edge<T> *path; // for path finding
	double dist;   // for path finding
	int queueIndex = 0; // required by MutablePriorityQueue

	Vertex(T in, string type);
	void addEdge(Edge<T> *e);
	bool operator<(Vertex<T> & vertex) const; // required by MutablePriorityQueue

public:
	T getInfo() const;
	vector<Edge<T> *> getIncoming() const;
	vector<Edge<T> *> getOutgoing() const;
	string getType() const;
	friend class Graph<T>;
	friend class MutablePriorityQueue<Vertex<T>>;
};


template <class T>
Vertex<T>::Vertex(T in, string type): info(in), type(type) {}

template <class T>
void Vertex<T>::addEdge(Edge<T> *e) {
	outgoing.push_back(e);
	e->dest->incoming.push_back(e);
}

template <class T>
bool Vertex<T>::operator<(Vertex<T> & vertex) const {
	return this->dist < vertex.dist;
}

template <class T>
T Vertex<T>::getInfo() const {
	return this->info;
}

template <class T>
vector<Edge<T> *>  Vertex<T>::getIncoming() const {
	return this->incoming;
}

template <class T>
vector<Edge<T> *>  Vertex<T>::getOutgoing() const {
	return this->outgoing;
}

template <class T>
string Vertex<T>::getType() const { return type; }


/* ================================================================================================
 * Class Edge
 * ================================================================================================
 */

template <class T>
class Edge {
	Vertex<T> * orig;
	Vertex<T> * dest;
	double weight;

	Edge(Vertex<T> *o, Vertex<T> *d, double weight);

public:
    double getWeight();
	friend class Graph<T>;
	friend class Vertex<T>;

};

template <class T>
Edge<T>::Edge(Vertex<T> *o, Vertex<T> *d, double weight):
	orig(o), dest(d), weight(weight) {}

template <class T>
double Edge<T>::getWeight() { return weight; }

/* ================================================================================================
 * Class Order
 * ================================================================================================
 */

template <class T>
class Order {
    Vertex<T> *orig;
    Vertex<T> *dest;

public:
    Order(Vertex<T> *o, Vertex<T> *d) { this->orig = o; this->dest = d; }
    Vertex<T> *getOrig() const;
    Vertex<T> *getDest() const;

};

template <class T>
Vertex<T> * Order<T>::getOrig() const { return orig; }

template <class T>
Vertex<T> * Order<T>::getDest() const { return dest; }

/* ================================================================================================
 * Class Graph
 * ================================================================================================
 */

template <class T>
class Graph {
	vector<Vertex<T> *> vertexSet;
	vector<Order<T> *> orders;
	void subNode(int id, string node);
	void subEdge(string edge);
	void subOrder(string order);

public:
	Vertex<T>* findVertex(const T &inf) const;
	vector<Vertex<T> *> getVertexSet() const;
	vector<Order<T> *> getOrders() const;
	Vertex<T> *addVertex(const T &in, string type);
	Edge<T> *addEdge(const T &sourc, const T &dest, double weight);
	Order<T> *addOrder(const T &orig, const T &dest);
	void readFile(string filename);
};

template <class T>
Vertex<T> * Graph<T>::addVertex(const T &in, string type) {
	Vertex<T> *v = findVertex(in);
	if (v != nullptr)
		return v;
	v = new Vertex<T>(in, type);
	vertexSet.push_back(v);
	return v;
}

template <class T>
Edge<T> * Graph<T>::addEdge(const T &sourc, const T &dest, double weight) {
	auto s = findVertex(sourc);
	auto d = findVertex(dest);
	if (s == nullptr || d == nullptr)
		return nullptr;
	Edge<T> *e = new Edge<T>(s, d, weight);
	s->addEdge(e);
	return e;
}

template <class T>
Order<T> * Graph<T>::addOrder(const T &orig, const T &dest) {
    auto o = findVertex(orig);
    auto d = findVertex(dest);
    if (o == nullptr || d == nullptr)
        return nullptr;
    Order<T> *ord = new Order<T>(o, d);
    orders.push_back(ord);
    return ord;
}

template <class T>
Vertex<T>* Graph<T>::findVertex(const T & inf) const {
	for (auto v : vertexSet)
		if (v->info == inf)
			return v;
	return nullptr;
}

template <class T>
vector<Vertex<T> *> Graph<T>::getVertexSet() const {
	return vertexSet;
}

template <class T>
vector<Order<T> *> Graph<T>::getOrders() const {
    return orders;
}

template <class T>
void Graph<T>::readFile(string filename) {
    ifstream file;
    string line;
    file.open(filename);
    if(file.fail()) {
        cerr << "The file failed to open.";
    }
    for(int n = 0; n < 3; n++){
        int oldStop = 0;
        int id = -1;
        getline(file, line);
        for(int i = 0; i < line.length(); i++){
            if (line[i] == ';') {
                switch (n) {
                    case 0:
                        id++;
                        subNode(id, line.substr(oldStop, i - oldStop));
                        break;
                    case 1:
                        subEdge(line.substr(oldStop, i - oldStop));
                        break;
                    case 2:
                        subOrder(line.substr(oldStop, i - oldStop));
                }

                oldStop = i + 1;
            }
        }
    }
    file.close();
}

template <class T>
void Graph<T>::subNode(int id, string node) {
    addVertex(id, node);
}

template <class T>
void Graph<T>::subEdge(string edge) {
    int lim1 = 0;
    int lim2 = 0;

    for (int i = 0; i < edge.length(); i++) {
        if (edge[i] == ',') {
            if(lim1 == 0)
                lim1 = i;
            else if(lim2 == 0)
                lim2 = i;
        }
    }

    int idO = stoi( edge.substr(0, lim1) );
    int idD = stoi( edge.substr(lim1 + 1, lim2 - lim1 - 1) );
    double weight = stod( edge.substr(lim2 + 1, edge.length() - 1));

    addEdge(idO, idD, weight);
}

template <class T>
void Graph<T>::subOrder(string order) {
    int lim = order.find(',');

    int idO = stoi( order.substr(0, lim) );
    int idD = stoi( order.substr(lim+1, order.length() - 1) );

    addOrder(idO, idD);
}

#endif //GRAPH_H_