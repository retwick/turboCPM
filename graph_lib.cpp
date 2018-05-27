

#include <bits/stdc++.h>
using namespace std;

class Graph {
  private:
  set < int > Vertices;                                           //vertex index starts from 1
  set < pair < int, int > > Edges;                                //<u,v>
  //map < pair < int, int > , int > Weight_of_Edges;                //<(u,v) , w>
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
    int duration;
  };
  vector < attribute > Attributes;

  Graph(int Nvertices, int Nedges) {
    this-> Nvertices = Nvertices;
    this-> Nedges = Nedges;    

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
    //Weight_of_Edges.insert(make_pair(edge, weight));
  }

  void insert_edge(int u, int v, int weight = 1) {
    new_edge(u, v, make_pair(u, v), weight);  
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

  bool is_visited(int u) {
    return Attributes[u].visited;
  }

  int get_distance(int u) {
    return Attributes[u].distance;
  }

  bool is_edge(int u, int v) {
    if (Edges.count(make_pair(u, v)) == 0) return false;

    return true;
  }

  int get_duration(int u){
    return Attributes[u].duration;
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

  void set_duration(int node, int duration){
    Attributes[node].duration = duration;
  }

  //-----------------------------------------
  //function from geeksforgeek


	// The function to do Topological Sort.
	void topologicalSort()
	{
	  int V = Nvertices;
    // Create a vector to store indegrees of all vertices. Initialize all indegrees as 0.
    vector<int> in_degree(V, 0);
 
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

    //for every u e V, ES=int_min, EF=int min

    for(int u: Vertices){
      Attributes[u].early_finish = INT_MIN;
      Attributes[u].early_start = INT_MIN;

      Attributes[u].late_finish = INT_MAX;
      Attributes[u].late_start = INT_MAX;
    }
    
    //find dest vertex --out_degree=0
    vector<int> out_degree(Nvertices, 0);
    vector<int> in_degree(Nvertices , 0);
    

    //for every (u,v) e E, in_degree[v]++;
    for(pair<int, int> p : Edges){
      out_degree[p.first]++;
      in_degree[p.second]++;      
    }

    //-------------------------------------------------
    //find source vertex -- in_degree = 0
    //finds first index s.t. in degree is zero
    auto it_source = find_if(in_degree.begin(), in_degree.end(), [](int i){ return !(i); });
    int souce_vertex = it_source - in_degree.begin();

    auto it_sink = find_if(out_degree.begin(), out_degree.end(), [](int i){ return !(i); });
    int sink_vertex = it_sink - out_degree.begin();


    cout<<"source vertex: "<<souce_vertex<<" sink_vertex: "<<sink_vertex<<endl;
    //---------------------------------------------------
    //could be removed if we can prove that source vertex is topo_order[0] and 
    //sink is topo_order[n-1];
    //---------------------------------------------------

    Attributes[top_order[0]].early_start = 0;
    //for(int v:AdjList_of_Vertices[0])
    Attributes[top_order[0]].early_finish = 0;
    for(int u: top_order ){
      //cout<<"compare: u "<<u<<"  ";
      for(int v: AdjList_of_Vertices[u] ){
        //cout<<"v: "<<v<<" V.ES ";        
        //cout<<Attributes[v].early_start <<" u.EF "<< Attributes[u].early_finish<<" v.D "<< Attributes[v].duration<<endl;
        if(Attributes[v].early_start < Attributes[u].early_finish){
          
           Attributes[v].early_start = Attributes[u].early_finish;
           Attributes[v].early_finish = Attributes[v].early_start + Attributes[v].duration;
           Attributes[v].parent = u;
        }

      }
    }

    //////////////-----------------------------------------------
    //TODO   
    // use sentinel nodes  and resultant nodes are of the form (0 -> duration -> 0)
    // Invariant: ES(initial_sentinel) + offset = EF(initial_sentinel) = ES(node)
    // EF(node) = ES(terminal_sentinel) + offset = EF(terminal_sentinel)

    // dummy node to have duration = offset (not implemented)

    // Dependencies -- Start to Start, FInish to Finish (Start to Finish)    
    // distinguish between initial and terminal sentinel nodes --> classify dependencies 
    // -->corner case FF dependency. (automatically adjusts, if invariant is maintained)    

    // Calendars
    // use C++ boost library for calendar arithmetic 

    // Constraints -- Must finish before, must start after, must start before

    //------

    // change to particular point of time and recompute critical path
    
    // Accouting for actual fraction of work completed
    //-----------------------------//////////////////////////////

    //print early starts
    cout<<"vertex\t\tES\tEF\n";
    for(int u: top_order){
      cout<<"vertex "<<u<<"\t"<<Attributes[u].early_start<<"\t"<<
      Attributes[u].early_finish<<endl;
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
    cout<<i<<"("<<g.get_duration(i)<<") : ";
    for(auto v:neigh){
    	cout<<v<<" ";
    }
    cout<<endl;
  }

}

//-----------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------//


int main() {

  int n, e;
  //cout<<"Enter n, m:";
  cin >> n >> e;
  Graph g(n, e);

  while (e--) {
    int u, v;
    //cout<<"enter u,v:";
    cin >> u >> v;
    g.insert_vertex(u);
    g.insert_vertex(v);
    g.insert_edge(u, v);
  }

  for(int i=0; i<n; ++i){
    int dur;
    cin>>dur;
    g.set_duration(i, dur);
  }

  g.topologicalSort();
  cout<<endl;
  print_graph(g);
  cout<<endl;
  g.critical_path();
  return 0;
}

