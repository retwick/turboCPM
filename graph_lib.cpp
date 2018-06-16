

#include <bits/stdc++.h>
using namespace std;

class Graph {
  private:
  set < int > Vertices;                                           //vertex index starts from 1
  set < pair < int, int > > Edges;                                //<u,v>  
  map < int, set < int > > AdjList_of_Vertices;                   // <u, {v| (u,v)eE} >  
  map < int, set<int> > Rev_AdjList;                              // <v, {u| (u,v)eE} >
  public:
  int Nvertices;                                          //n
  bool isDirected;  
  vector<int> top_order;
  
  struct attribute {
    bool visited = false;
    int distance = INT_MAX;
    int parent = 0;
    int early_start = INT_MIN, early_finish = INT_MIN, late_start=INT_MAX, late_finish=INT_MAX;
    int duration;
    bool is_initial = false, is_terminal=false, is_variable=false;
    string name; 
  };
  vector < attribute > Attributes;

  //constructor
  Graph(int Nvertices) {
    this-> Nvertices = Nvertices;
    
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

  void new_edge(int u, int v, pair < int, int > edge) {    
    AdjList_of_Vertices[u].insert(v);
    Edges.insert(edge);    
  }

  void insert_edge(int u, int v) {
    new_edge(u, v, make_pair(u, v));  
  }
  
  void insert_vertex(int u){
    Vertices.insert(u);
    Nvertices++;
  }

  void connect(int u, int v){
    insert_edge(u, v);
    insert_reverse_adj(u, v); 
  }

  void remove_vertex(int u){
    set<int>::iterator it;
    it = Vertices.find(u);
    if(it != Vertices.end()){
      Vertices.erase(it);
      AdjList_of_Vertices[u].clear();
      Nvertices--;
    }
  }

  void remove_edge(int u, int v){
    set<pair<int,int> >::iterator it;
    it = Edges.find(make_pair(u,v));
    if(it != Edges.end()){
      Edges.erase(it);
    }
  }

  void add_activity(int index, int dur){
    
    insert_vertex(3*index);
    insert_vertex(3*index+1);
    insert_vertex(3*index+2);
    //create link between dummy start and actual node
    insert_edge(3*index, 3*index+1);
    insert_reverse_adj(3*index, 3*index+1);
    //create link between actual node and dummy end
    insert_edge(3*index+1, 3*index+2);    
    insert_reverse_adj(3*index+1, 3*index+2);

    //dummy start
    set_duration(3*index, 0);
    set_initial(3*index);

    //actual node
    set_duration(3*index+1,dur);

    //dummy end
    set_duration(3*index+2, 0);
    set_terminal(3*index+2);
  }

  void remove_activity(int u){          
    remove_vertex(3*u);
    remove_vertex(3*u+1);
    remove_vertex(3*u+2);
    remove_edge(3*u, 3*u+1);
    remove_edge(3*u+1, 3*u+2);
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

  string get_name(int node){
    return Attributes[node].name;
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

  int get_early_start(int v){
    return Attributes[v].early_start;
  }

  int get_early_finish(int v){
    return Attributes[v].early_finish;
  }

  //-----------------------setter functions---------------------

  void visit(int u) {
    Attributes[u].visited = true;
  }

  void set_name(int node, string name){    
    node = 3*(node)+1;
    Attributes[node].name = name;
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
    vector<int> in_degree(V, 0);
     
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
   /*
    // Check if there was a cycle
    if (cnt != V){
      cout << "There exists a cycle in the graph\n";
      return;
    }
    */
   /*
    // Print topological order
    cout<<"\ntopological order\n";
    for (int i=0; i<top_order.size(); i++)
        cout << top_order[i] << " ";
    cout << endl;
    */
  }  


  //function to remove distance between dummy node and actual node
  void pull_back(int node, int ES){
    //cout<<"pull_back on "<<node<<"arg: "<<ES<<"\n";
    if(Rev_AdjList[node].empty()){      
      forward_parse(node, ES);
    }
    for(int v: Rev_AdjList[node]){      
      pull_back(v,ES - Attributes[v].duration);
    }    
  }

  void forward_parse(int u, int start = 0){
  
    Attributes[u].early_start = max(start,Attributes[u].early_start);
    Attributes[u].early_finish = max(start + Attributes[u].duration ,Attributes[u].early_finish);
    
    for(int v: AdjList_of_Vertices[u]){
      if(Attributes[v].early_start < Attributes[u].early_finish){

        Attributes[v].early_start = Attributes[u].early_finish;
        Attributes[v].early_finish = Attributes[u].early_finish + Attributes[v].duration;        
        
        if( Attributes[v].is_terminal ){
          if(Attributes[v-1].early_finish != Attributes[v].early_start){
              pull_back(v-1, Attributes[v].early_start - Attributes[v-1].duration);              
            }
        }

      }
      forward_parse(v,Attributes[v].early_start);
    }
  }
  
  void critical_path(){
    Attributes[top_order[0]].early_start = 0;    
    Attributes[top_order[0]].early_finish = 0;
    
    for(int u: top_order ){
      forward_parse(u);    
    }
  }
};

//----------------------------------------------------------------------------------//
void print_graph(Graph &g){
  cout<<"print\n";
  for (int i:g.vertices()) {
    set < int > neigh = g.get_adjList(i);
    cout<<i<<"("<<g.get_duration(i)<<") : "<<g.get_name(i)<<" : "<< (g.is_initial(i)) <<" "<<(g.is_terminal(i))<<"-";
    for(auto v:neigh){
      cout<<v<<" ";
    }
    cout<<endl;
  }

}
