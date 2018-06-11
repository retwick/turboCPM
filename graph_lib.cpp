

#include <bits/stdc++.h>
using namespace std;

class Graph {
  private:
  set < int > Vertices;                                           //vertex index starts from 1
  set < pair < int, int > > Edges;                                //<u,v>  
  map < int, set < int > > AdjList_of_Vertices;                   // <u, {v| (u,v)eE} >  
  map < int, set<int> > Rev_AdjList;                              // <v, {u| (u,v)eE} >
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
    bool is_initial = false, is_terminal=false;
  };
  vector < attribute > Attributes;

  //constructor
  Graph(int Nvertices, int Nedges) {
    this-> Nvertices = Nvertices;
    this-> Nedges = Nedges;    

    set < int > s = {};
    attribute atb;
    Attributes.push_back(atb);                                      //dummy atribute to start index from 1
    //initialize graph
    for (int i = 1; i <= Nvertices; i++) {
      AdjList_of_Vertices.insert(make_pair(i, s));                  //initialize adjaceny list
      Rev_AdjList.insert(make_pair(i,s));   
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

  void insert_reverse_adj(int u, int v){
    Rev_AdjList[v].insert(u);    
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

  bool is_terminal(int v){
    return Attributes[v].is_terminal;
  }

  bool is_initial(int v){
    return Attributes[v].is_initial;
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

  void set_initial(int node){
    Attributes[node].is_initial = true;
  }

  void set_terminal(int node){
    Attributes[node].is_terminal = true;
  }

  //-----------------------------------------

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
    cout<<"\ntopological order\n";
    for (int i=0; i<top_order.size(); i++)
        cout << top_order[i] << " ";
    cout << endl;
	}  

  void pull_back(int node, int ES){
    cout<<"pull_back on "<<node<<"\n";
    if(Rev_AdjList[node].empty()){
      cout<<"start forward_parse at :"<<node<<endl;
      forward_parse(node, ES);
    }
    for(int v: Rev_AdjList[node]){      
      pull_back(v,ES - Attributes[v].duration);
    }    
  }

  void forward_parse(int u, int start = 0){
    if(start != 0){
      Attributes[u].early_start = start;
      Attributes[u].early_finish = start + Attributes[u].duration - 1;
    }
    for(int v: AdjList_of_Vertices[u]){
      if(Attributes[v].early_start < Attributes[u].early_finish){

        Attributes[v].early_start = Attributes[u].early_finish+1;
        Attributes[v].early_finish = Attributes[v].early_start + Attributes[v].duration - 1;
        Attributes[v].parent = u;

        if( Attributes[u].is_terminal&&Attributes[v].is_terminal || 
           Attributes[u].is_initial &&Attributes[v].is_terminal ){
        //need to remove distance between dummy node and actual node

        //find farthest node(x) from current node(v) with edges reversed.
        //if many such x exists, do for all x
        //also find supposed to be distance from x to v,by adding all node weights in the path
        //set start date of x to start date of v - distance(x,v)

        pull_back(v, Attributes[v].early_start);      
        }
        forward_parse(v);
      }
    }
  }

  void critical_path(){

    //for every u in V, ES=int_min, EF=int min

    for(int u: Vertices){
      Attributes[u].early_finish = INT_MIN;
      Attributes[u].early_start = INT_MIN;

      Attributes[u].late_finish = INT_MAX;
      Attributes[u].late_start = INT_MAX;
    }
    
    Attributes[top_order[0]].early_start = 0;    
    Attributes[top_order[0]].early_finish = 0;
    for(int u: top_order ){
      forward_parse(u);    
    }

    //////////////-----------------------------------------------

    // Calendars
    // use C++ boost library for calendar arithmetic 

    // Constraints -- Must finish before, must start after, must start before

    // change to particular point of time and recompute critical path
    
    // Accouting for actual fraction of work completed
    //-----------------------------//////////////////////////////

    //print early starts
    cout<<"vertex\t\tES\tEF\n";
    for(int u: Vertices){
      if((u+2)%3) continue;
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
    cout<<i<<"("<<g.get_duration(i)<<") : "<< (g.is_initial(i)&&1) <<" "<<(g.is_terminal(i)&&1)<<"-";
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
  cout<<"Enter n, m:";
  cin >> n >> e;
  Graph g(n, e);

  while (e--) {
    int u, v;
    cout<<"enter u,v:";
    cin >> u >> v;
    g.insert_vertex(u);
    g.insert_vertex(v);
    g.insert_edge(u, v);
    g.insert_reverse_adj(u,v);
  }

  for(int i=0; i<n; ++i){
    int dur;
    if((i+2)%3 == 2){
      g.set_duration(i,0); 
      g.set_initial(i);
      continue;      
    }
    else if((i+2)%3 == 1){
      g.set_duration(i,0);
      g.set_terminal(i);
      continue;
    }
    cout<<"enter duration: ";
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

