/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <list>
#include <limits>
#include <cmath>
#include <stack>
#include "MutablePriorityQueue.h"

using namespace std;

#define INF std::numeric_limits<double>::max()


class Edge;
class Graph;
class Vertex;

typedef struct node_data
{
    unsigned long id;
    double x;
    double y;
    bool is_casa=false;
    bool is_loja=false;
    bool is_carregador=false;
}node_data;

/************************* Vertex  **************************/


class Vertex {

    public:
    double aStar_value;
	node_data info;                        // content of the vertex
	vector<Edge> adj;        // outgoing edges
	double dist = 0;
	Vertex *path = NULL;
	int queueIndex = 0;        // required by MutablePriorityQueue
	bool visited = false;        // auxiliary field
	bool processing = false;    // auxiliary field
    int carrinha = -1;

    Vertex(double x, double y);
    Vertex(node_data in);
	void addEdge(Vertex *dest, unsigned long dest_id, double w);
	double weightTo(Vertex *dest);
	node_data getInfo() const;
	double getDist() const;
	Vertex *getPath() const;
	void remove_edges_to_id(unsigned long id);
	bool operator<(Vertex &vertex) const; // // required by MutablePriorityQueue
	void printVertex();
    friend class Graph;
	friend class MutablePriorityQueue<Vertex>;
};

Vertex::Vertex(double x, double y) {
   info.x=x;
   info.y=y;
}

Vertex::Vertex(node_data in) {
    info=in;
}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */


double euclidian_distance(Vertex *src, Vertex *dest)
{
    return (sqrt((src->info.x-dest->info.x)*(src->info.x-dest->info.x)+(src->info.y-dest->info.y)*(src->info.y-dest->info.y)));
}


bool Vertex::operator<(Vertex &vertex) const {
	return this->aStar_value < vertex.aStar_value;
}


node_data Vertex::getInfo() const {
	return this->info;
}


double Vertex::getDist() const {
	return this->dist;
}


Vertex *Vertex::getPath() const {
	return this->path;
}


/*double Vertex::weightTo(Vertex *dest) {
	for (Edge edge : this->adj)
		if (edge.dest == dest)
			return edge.weight;

	if (this == dest)
		return 0;

	return INF;
}*/

void Vertex::printVertex()
{
    printf("Vértice: ");
    if(info.is_loja)
        printf("loja ");

    if(info.is_carregador)
        printf("carregador ");
    if(info.is_casa)
        printf("casa ");

    printf("(%f, %f)\n",info.x,info.y);

}

/********************** Edge  ****************************/


class Edge {
public:
	Vertex *dest;      // destination vertex
    unsigned long dest_id;
	double weight;         // edge weight
	unsigned long id;

	Edge(Vertex *d, unsigned long dest_id, double w);

	friend class Graph;

	friend class Vertex;
};


Edge::Edge(Vertex *d, unsigned long dest_id, double w){
    dest=d;
    weight=w;
    this->dest_id=dest_id;
}


/*************************** Graph  **************************/


class Graph {
public:
	vector<Vertex *> vertexSet;    // vertex set
	vector<vector<double>> distancesMatrix;
	vector<vector<int>> pathMatrix;


	int findVertexIndex(Vertex *vertex) const;

	bool addVertex(Vertex *vertex);

	bool addEdge(Vertex *sourc, Vertex *dest, double w);

	int getNumVertex() const;

	vector<Vertex *> getVertexSet() const;

    vector<node_data> dfs() const;

    vector<node_data> bfs(Vertex *v) const;

    void dfsVisit(Vertex *v, vector<node_data> res) const;

    void erase_all_edges_to_vertex(Vertex *v, unsigned long id);

    double aStar(Vertex *src, Vertex *dest);
};


int Graph::getNumVertex() const {
	return vertexSet.size();
}


vector<Vertex *> Graph::getVertexSet() const {
	return vertexSet;
}

void Vertex::addEdge(Vertex *d, unsigned long dest_id, double w) {
    Edge edge(d, dest_id,w);
    adj.push_back(edge);
}

/*
 * Auxiliary function to find a vertex index.
 *
 * returns -1 if the vertex does not exist
 */

int Graph::findVertexIndex(Vertex *vertex) const {
	for (int i = 0; i < vertexSet.size(); ++i)
		if (vertexSet[i] == vertex)
			return i;
	return -1;
}

/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */

bool Graph::addVertex(Vertex *vertex) {

	vertexSet.push_back(vertex);
	return true;
}

vector<node_data> Graph::dfs() const {
    for (Vertex *v : vertexSet)
        v->visited = false;

    vector<node_data> res;

    for (Vertex *v : vertexSet)
        if(!v->visited)
            dfsVisit(v, res);

    return res;
}

void Graph::dfsVisit(Vertex *v, vector<node_data> res) const {
    v->visited = true;
    res.push_back(v->info);
    for (Edge w : v->adj)
        if (!w.dest->visited)
            dfsVisit(w.dest, res);
}

vector<node_data> Graph::bfs(Vertex *v) const {

    vector<node_data> res;
    int index = findVertexIndex(v);
    for (Vertex *v : vertexSet)
        v->visited = false;
    queue<Vertex*> temp;
    vertexSet[index]->visited = true;
    temp.push(vertexSet[index]);
    res.push_back(vertexSet[index]->info);
    while (!temp.empty()) {
        for (Edge w : temp.front()->adj)
            if (!w.dest->visited) {
                temp.push(w.dest);
                w.dest->visited = true;
                res.push_back(w.dest->info);
            }
        temp.pop();
    }
    return res;

}

void Vertex::remove_edges_to_id(unsigned long id)
{
    vector<Edge>::iterator it;
    for(it=adj.begin();it!=adj.end();it++)
    {
        if((*it).dest_id==id)
            adj.erase(it);
    }
}

void Graph::erase_all_edges_to_vertex(Vertex *v, unsigned long id) {
    for(auto edge: v->adj)
    {
        edge.dest->remove_edges_to_id(id);
    }
}


double Graph::aStar(Vertex *src, Vertex *dest)
{

    for (Vertex *v : vertexSet)
    {
        v->dist = INF;
        v->aStar_value = INF;
        v->visited = false;
        v->path = NULL;
    }
    src->dist=0;
    src->aStar_value=0;
    MutablePriorityQueue<Vertex> q;
    q.insert(src);


    while( !q.empty() ) {
        auto v = q.extractMin();

        if(v == dest){
            break;
        }

        v->visited = true;

        for(Edge e : v->adj) {
            Vertex * current = e.dest;
            double newDist =  v->dist + e.weight;

            if(current->visited) continue;

            if (newDist < current->dist) {

                current->path = v;
                current->dist = newDist;
                if (current->aStar_value == INF){
                    current->aStar_value = current->dist + euclidian_distance(current,dest);
                    q.insert(current);
                }
                else{
                    current->aStar_value = current->dist + euclidian_distance(current,dest);
                    q.decreaseKey(current);
                }
            }
        }

    }
    return dest->dist;
}
/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */

#endif