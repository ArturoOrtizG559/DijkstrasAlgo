// dijkstras.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <map>
#include <vector>
#include <deque>

using namespace std;


struct Node
{
	int val;
	int d;
	int w;
	Node* pie = NULL;
	
	Node(int v){
		val = v;
	}
	Node(int v, int weight) {
		val = v;
		w = weight;
	}
	Node(int v, int we, int di) {
		val = v;
		w = we;
		d = di;
	}
};



struct Graph {
	vector<Node*> vertices;
	
	map<pair<int,int>, int> edges;
	Node* adj[4][4];
	Graph();
	void addEdges(int,int,int);
	void makeAdjMatrix();
	void print();
	void addVert(vector<Node*>&);

};
// custom minHeap
void minHeapify(deque<Node*> &a, int i, int hs) {
	int l = (i * 2) + 1;
	int r = (i * 2) + 2;
	int smallest;
	Node* temp = NULL;
	Node* ptrL = NULL;
	Node* ptrR = NULL;
	Node* ptrI = NULL;
	Node* small = NULL;

	if (l <= hs && i <= hs && r <= hs+1) {
		ptrL = a.at(l);
		if (r <=hs) {
			ptrR = a.at(r);
		}
		ptrI = a.at(i);
	}
	else {
		return;
	}

	if (ptrL != NULL && l <= hs && ptrL->d <= ptrI->d) {
		smallest = l;
		small = a.at(smallest);
	}

	else {
		smallest = i;
		small = a.at(smallest);
	}

	if (ptrL != NULL && small != NULL && ptrR != NULL && r <= hs && ptrR->d <= small->d) {
		smallest = r;
		small = a.at(smallest);
	}

	if (smallest != i && ptrL != NULL) {
		temp = a.at(smallest);
		a.at(smallest) = a.at(i);
		a.at(i) = temp;
		minHeapify(a, smallest, hs);
	}

}

//creates the minQueue
void minQueue(deque<Node*> &v, int size) {
	int hSize = size;
	for (int i = (size / 2); i > -1; i--) {
		minHeapify(v, i, hSize);
	}
}

//get top value from queue
Node* extractMin(deque<Node*> &a) {
	Node* temp = NULL;
	temp = a.at(0);
	a.at(0) = a.back();
	a.pop_back();
	minQueue(a, a.size() - 1);
	return temp;
}

//insert into queue
void insertMin(deque<Node*> &a, Node* n) {
	a.push_back(n);
	minQueue(a, a.size() - 1);
}



Graph::Graph() {
	
	for (int i = 0; i < 4;i++) {
		for (int j = 0; j < 4;j++) {
			adj[i][j] = NULL;
		}
	}
}

//adds edges to graph
void Graph::addVert(vector<Node*> &v) {
	for (int i = 0;i < v.size();i++) {
		vertices.push_back(v[i]);
	}
}

//prints value in graph
void Graph::print() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4;j++) {
			if (adj[i][j] != NULL) {
				cout << "value for i = " << i << "value for j = " << j << adj[i][j]->w << endl;
			}
		}
	}
}

//add to graph an edge from u to v with weight w
void Graph::addEdges(int u, int v, int w) {
	edges.insert(make_pair(make_pair(u, v), w));
}


//makes an adjacency matrix for faster look ups in the graph
void Graph::makeAdjMatrix() {
	map<pair<int, int>, int>::iterator eit;
	for (eit = edges.begin(); eit != edges.end(); eit++) {
		adj[(*eit).first.first][(*eit).first.second] = new Node((*eit).first.first, (*eit).second);
		//cout << "in adj = " <<  adj[(*eit).first.first][(*eit).first.second]->w<< endl;
	}

}


//relaxes vertex u and v if u.d is greater than u.d + w
void relax(Node* u, Node* v, int w) {
	if (v->d > u->d + w) {
		v->d = u->d + w;
		v->pie = u;
		//cout << "in here v =" << v->d <<"v val = "<< v->val<< " u = " << u->d << endl;
	}
}


//intitialize all values to infinity or 0
void initialize_SS(Graph &g, Node* s) {
	for (int i = 0; i < g.vertices.size();i++) {
		g.vertices[i]->d = 1000000;
		g.vertices[i]->pie = NULL;
	}
	s->d = 0;
}

//dijkstras algo
void Dijkstra(Graph g, Node* s, vector<Node*> &v, deque<Node*> &q) {
	Node* pointer = NULL;
	
	initialize_SS(g, s);
	minQueue(q,q.size()-1);

	while(!q.empty()) {
		pointer = extractMin(q);
		v.push_back(pointer);
		//cout << pointer->d << endl;
		for (int i = 1; i < 4;i++) {
			
			if (g.adj[pointer->val][i] != NULL && pointer->val != i) {

				relax(pointer, g.vertices[i], g.adj[pointer->val][i]->w);
			}
		}
		minQueue(q,q.size()-1);
	}

	return;
}

int main()
{
	
	Graph graph;
	vector<Node*> ver;
	deque<Node*> queue;
	vector<Node*> sPath;
	int x;
	int y;
	int z;

	for (int i = 0; i < 4; i++) {
		
		ver.push_back(new Node(i));
		queue.push_back(ver[i]);
		//cout << queue.size() <<endl;
	}
	minQueue(queue, queue.size() - 1);
	for (int i = 0; i < 4; i++)
	{
		cin >> x;
		cin >> y;
		cin >> z;
		graph.addEdges(x, y, z);
	}

	graph.makeAdjMatrix();
	graph.addVert(ver);
	//graph.print();
	Dijkstra(graph, ver[0], sPath,queue);

	for (int l = 0; l < sPath.size();l++) {
		if (sPath[l]->pie != NULL) {
			cout << sPath[l]->d<< endl;
		}
	}

	for (int i = 0; i < 4;i++) {
		for (int j = 0; j < 4;j++) {
			if (graph.adj[i][j] != NULL) {
				delete graph.adj[i][j];
			}
		}
	}

	for (int j = 0; j < ver.size(); j++) {
		delete ver[j];
	}
	
    return 0;
}

