/*
A simple C++ class - "Graph". 

The Graph class can be used to store and manipulate simple directed / non-directed graphs.

At this moment, multigraphs are not supported. Also, member functions for deletion 
of edges have not been introduced yet.

Excessive use of the STL (Standard Template Library) can be observed in the class.
*/


#include <bits/stdc++.h>
using namespace std;

class Graph {
  private:
  set < int > Vertices;                                           //vertex index starts from 1
  set < pair < int, int > > Edges;                                //<u,v>
  map < pair < int, int > , int > Weight_of_Edges;                //<(u,v) , w>
  map < int, set < int > > AdjList_of_Vertices;                   // <u, {v| (u,v)eE}

  public:
  int Nvertices, Nedges;                                          //n,m
  bool isDirected;

  vector<int> top_order;
  
struct attribute {
    bool visited = false;
    int distance = INT_MAX;
    int parent = 0;

    int early_start, early_finish, late_start, late_finish;
  };
  vector < attribute > Attributes;

  Graph(int Nvertices, int Nedges, bool isDirected = false) {
    this-> Nvertices = Nvertices;
    this-> Nedges = Nedges;
    this-> isDirected = isDirected;

    set < int > s = {};
    attribute atb;
    Attributes.push_back(atb);                                      //dummy atribute to start index from 1
    //initialize graph
    for (int i = 1; i <= Nvertices; i++) {
      AdjList_of_Vertices.insert(make_pair(i, s));                  //initialize adjaceny list
      Attributes.push_back(atb);                                    //store vertex data in attributes
    }
  }

  void new_edge(int u, int v, pair < int, int > edge, int weight) {
    AdjList_of_Vertices[u].insert(v);
    Edges.insert(edge);
    Weight_of_Edges.insert(make_pair(edge, weight));
  }

  void insert_edge(int u, int v, int weight = 1) {
    new_edge(u, v, make_pair(u, v), weight);

    if (!isDirected) {
      new_edge(v, u, make_pair(v, u), weight);
    }
  }

  void insert_edge(pair < int, int > edge, int weight = 1) {
    insert_edge(edge.first, edge.second, weight);
  }

  void insert_vertex(int u){
    Vertices.insert(u);
  }

  //------------getter functions---------------------
  set < int > vertices() {
    return Vertices;
  }

  set <pair<int, int> > get_edges(){
    return Edges;
  }

  // returns the adjacency list (a set) of a vertex.
  set < int > get_adjList(int vertex){ 
    return AdjList_of_Vertices[vertex];
  }

  // returns the weight of th edge {u, v}
  int get_weight(int u, int v){
    return Weight_of_Edges[make_pair(u, v)];
  }

  int get_weight(pair < int, int > edge) {
    return Weight_of_Edges[edge];
  }

  bool is_visited(int u) {
    return Attributes[u].visited;
  }

  bool get_distance(int u) {
    return Attributes[u].distance;
  }

  bool is_edge(int u, int v) {
    if (Edges.count(make_pair(u, v)) == 0) return false;

    return true;
  }

  int get_parent(int u) {
    return Attributes[u].parent;
  }

  vector<int> get_top_order(){
    return top_order;
  }

  //-----------------------setter functions---------------------

  void visit(int u) {
    Attributes[u].visited = true;
  }

  void set_distance(int node, int new_distance) {
    Attributes[node].distance = new_distance;
  }

  void set_parent(int child, int parent) {
    Attributes[child].parent = parent;
  }


  //-----------------------------------------
  //function from geeksforgeek

	// The function to do Topological Sort.
	void topologicalSort()
	{
	  int V = Nvertices;
    // Create a vector to store indegrees of all vertices. Initialize all indegrees as 0.
    vector<int> in_degree(V+1, 0);
 
    // Traverse adjacency lists to fill indegrees of vertices.  This step takes O(V+E) time

    //for every (u,v) e E, in_degree[v]++;
    for(pair<int, int> p : get_edges()){
      in_degree[p.second]++;
    }
    
    // Create an queue and enqueue all vertices with indegree 0
    queue<int> q;
    for(int u: vertices()){
      if(!in_degree[u]){
        q.push(u);
      }
    }
    
    // Initialize count of visited vertices
    int cnt = 0;
 	    
    // One by one dequeue vertices from queue and enqueue
    // adjacents if indegree of adjacent becomes 0
    while (!q.empty())
    {
      // Extract front of queue (or perform dequeue)
      // and add it to topological order
      int u = q.front();
      q.pop();
      top_order.push_back(u);

      // Iterate through all its neighbouring nodes
      // of dequeued node u and decrease their in-degree
      // by 1
      	        
      for(int v: get_adjList(u)){
        if(--in_degree[v] == 0)
          q.push(v);
      }
      
      cnt++;
	  }
	 
	    // Check if there was a cycle
    if (cnt != V){
      cout << "There exists a cycle in the graph\n";
      return;
    }
	 
    // Print topological order
    for (int i=0; i<top_order.size(); i++)
        cout << top_order[i] << " ";
    cout << endl;
	}  


  void critical_path(){
    /*

    Design

    find the topological order of the graph

    Initially all the nodes will be marked white denoting it’s not traversed.

    The method dfsVisit (Graph g, Vertex u) is implemented to check whether the graph is visited or not.

    The color is changed to Gray when the node is being visited, and marked black when the node is visited.

    Add this to a List of Vertices that in turn gives us the topological order.

    Enumerate all paths in a DAG from start to end enumeratePaths (Vertex u, index).

    HashMap is maintained for all paths along with the index.

    EC is calculated by comparing all the incoming edges to the vertex v and by taking the maximum value of all incoming edges.

    LC is calculated by comparing all the incoming edges to the vertex v and take the minimum value of all incoming edges for which, we need to reverse the topological order to traverse back

    For every edge present in the path, check for the critical edge (lc = ec) and add them to array of vertices.

    Iterator over them to find all critical paths, its length and print them.

    */

    //for every u e V, ES=0, EF=int min

    for(int u:vertices){
      Attributes[u].early_finish = 0;
    }


  }


};

//----------------------------------------------------------------------------------//

/*
The following BFS function is for the purpose of checking
that the Graph class and its members work correctly.
*/

void BFS(Graph & g, int source) {
  for (auto u: g.vertices()) {
    g.set_distance(u, INT_MAX);
    g.set_parent(u, 0);
  }

  queue < int > q;
  g.set_distance(source, 0);
  q.push(source);

  while (q.empty() == false) {
    auto u = q.front();
    q.pop();

    for (auto v: g.get_adjList(u)) {
      if (g.is_visited(v) == false) {
        g.visit(v);
        g.set_parent(v, u);
        q.push(v);
      }
    }
  }
}

void print_graph(Graph &g){
  cout<<"print\n";
  for (int i:g.vertices()) {
  	set < int > neigh = g.get_adjList(i);
    cout<<i<<" : ";
    for(auto v:neigh){
    	cout<<v<<"("<<g.get_weight(i,v)<<") ";
    }
    cout<<endl;
  }

}

//-----------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------//


int main() {

  int n, e;
  cout<<"Enter n, m:";
  cin >> n >> e;
  Graph g(n, e, true);

  while (e--) {
    int u, v, w;
    cout<<"enter u,v,w:";
    cin >> u >> v >> w;
    g.insert_vertex(u);
    g.insert_vertex(v);
    g.insert_edge(u, v, w);
  }

  g.topologicalSort();
  cout<<endl;
  print_graph(g);
  return 0;
}

