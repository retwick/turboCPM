//solve cpm for FS only

//convert to weighted graph with dummy nodes (Problem:size increases by 3)

//find longest path in DAG

#include <bits/stdc++.h>
using namespace std;
class Node{
	bool isDummy;
	//static data
	int id;
	int duration;
	//dependencies will be assigned on runtime

	//runtime data
	int early_start, early_finish;
	int late_start, late_finish;
public:
	Node(){
		isDummy=true;
		duration=0;
		//id = -1;
	}
	Node(int d){
		isDummy = false;
		duration = d;
	}
};

class DAG{
	vector <Node*> nodes;
	list <pair<Node,int> > *adj;
	int graph_size;
public:
	DAG(int n){
		graph_size = n;
		adj = new <list> [n];
		for (int i = 0; i < graph_size; ++i){
			nodes[i] = new Node;
		}
	}

	void add_edge(int u, int v, int w){
		adj[u-1].push_back(make_pair(nodes[v-1], w));
	}

};