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

  vector<int> top_order;
  
  struct attribute {        
    int early_start = INT_MIN, early_finish = INT_MIN, late_start=INT_MIN, late_finish=INT_MIN;
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
    Attributes.push_back(atb);                 
    //initialize graph
    for (int i = 0; i < Nvertices; i++) {
      AdjList_of_Vertices.insert(make_pair(i, s));                  //initialize adjaceny list
      Rev_AdjList.insert(make_pair(i,s));   
      Attributes.push_back(atb);                                    //store vertex data in attributes
    }
  }

  /* Private
  Add edge to graph between u and v
  arg:  u -- index of source vertex
        v -- index of terminal vertex
  */  
  void insert_edge(int u, int v) {    
    pair<int, int> edge = make_pair(u,v); 
    AdjList_of_Vertices[u].insert(v);
    Edges.insert(edge); 
  }
  
  /* Private
  Add vertex to Graph.
  arg:  u -- key of node
  */
  void insert_vertex(int u){
    Vertices.insert(u);
    set<int> s = {};
    attribute atb;    
    AdjList_of_Vertices.insert(make_pair(u, s));                  //initialize adjaceny list
    Rev_AdjList.insert(make_pair(u,s));   
    Attributes.push_back(atb);                                    //store vertex data in attributes
    Nvertices++;
  }

  /* Private
  Remove vertex from graph
  arg:  u -- key of node to be deleted
  */
  void remove_vertex(int u){
    set<int>::iterator it;
    it = Vertices.find(u);

    //check if node exists
    if(it != Vertices.end()){
      Vertices.erase(it);
      AdjList_of_Vertices[u].clear();
      Nvertices--;
    }
  }

  /* Private
  Remove edge from graph
  arg:  u -- key of source node
        v -- key of dest node
  */
  void remove_edge(int u, int v){
    set<pair<int,int> >::iterator it;
    it = Edges.find(make_pair(u,v));
    if(it != Edges.end()){
      Edges.erase(it);
    }
  }

  /* Public
  Add a new activity to graph
  Activity is represented by three nodes-  dummy start, actual activity, dummy terminal
  Each activity of ID=i, is mapped to 3i(dummy start, duration = 0), 
  3i+1(activity, duration = activity duration), 3i+2(dummy end, duration =0)
  
  arg:  index -- ID of activity
        dur -- duration of the activity
  */
  void add_activity(int index, int dur=0){
    
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

  /* Public
  Remove activity from graph
  arg:  u -- ID of activity to be removed
  */
  void remove_activity(int u){          
    remove_vertex(3*u);
    remove_vertex(3*u+1);
    remove_vertex(3*u+2);
    remove_edge(3*u, 3*u+1);
    remove_edge(3*u+1, 3*u+2);
  }

  /* Public
  Create a two sided link between node u and node v
  arg:  u -- key of source node
        v -- key of dest node
  */
  void connect(int u, int v){
    insert_edge(u, v);
    insert_reverse_adj(u, v); 
  }

  /* Private
  Maintains set for all incoming vertices 
  There is an edge from u to v in the graph
  arg:  u-- index of source vertex
        v-- index of dest vertex
  */ 
  void insert_reverse_adj(int u, int v){
    Rev_AdjList[v].insert(u);    
  }

  //----------------getter functions----------------
  
  /*  
  Returns set of vertices. 
  Activity of ID=i, is mapped to 
    3i(dummy start, duration = 0), 
    3i+1(activity, duration = activity duration), 
    3i+2(dummy end, duration =0)
  */
  set < int > vertices() {
    return Vertices;
  }

  /*
  Returns all edges in graph in pair format. 
  <u,v> when there is an edge between u and v
  */ 
  set <pair<int, int> > get_edges(){
    return Edges;
  }

  /*
  Returns the adjacency list (a set) of a vertex.
  arg:  vertex -- index of vertex whose neighbours is queried 
  */
  set < int > get_adjList(int vertex){ 
    return AdjList_of_Vertices[vertex];
  }

  /*
  Return Early Start date of a node
  arg:  v -- index of node
  */  
  int get_early_start(int v){
    return Attributes[v].early_start;
  }

  /*
  Return Early finish date of a node
  arg:  v -- index of node
  */
  int get_early_finish(int v){
    return Attributes[v].early_finish;
  }

  /*
  Return late start
  arg:  u -- index of node
  */
  int get_late_start(int u){
    return Attributes[u].late_start;
  }

  /*
  Return late finish
  arg:  u -- index of node
  */  
  int get_late_finish(int u){
    return Attributes[u].late_finish;
  }

  /*
  Return name of activity. for offset nodes, it is set as "offset"
  arg:  u -- index of node
  */
  string get_name(int node){
    return Attributes[node].name;
  }

  /*
  Retun duration of node, could be 0 also
  arg:  u -- index of node
  */
  int get_duration(int u){
    return Attributes[u].duration;
  }

  /*
  Check if given node is dummy start
  arg:  v -- index of node
  */
  bool is_initial(int v){
    return Attributes[v].is_initial;
  }

  /*
  Check if given node is dummy terminal
  arg:  v -- index of node
  */
  bool is_terminal(int v){
    return Attributes[v].is_terminal;
  }

  /*
  Return a topological order of the graph
  */
  vector<int> get_top_order(){
    return top_order;
  }

  //---------------end of getter functions-------------

  //------------------setter function-------------------

  /*
  Set name of activity
  arg:  node -- ID of the activity
        name -- name of the activity
  */
  void set_name(int node, string name){    
    node = 3*(node)+1;
    Attributes[node].name = name;
  }

  /*
  Set duration of node
  arg:  node -- index of the node
        duration -- duration of the node
  */
  void set_duration(int node, int duration){
    Attributes[node].duration = duration;
  }

  /*
  Set flag for dummy start node
  arg:  node -- index of the node        
  */
  void set_initial(int node){
    Attributes[node].is_initial = true;
  }

  /*
  Set flag for dummy terminal node
  arg:  node -- index of the node        
  */
  void set_terminal(int node){
    Attributes[node].is_terminal = true;
  }

  //---------------end of setter functions-------------------

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
      // of dequeued node u and decrease their in-degree by 1
                
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
  }  


  /*
  Function to maintain invariant property of nodes
  Invariant:ES(dummy start)=EF(dummy start)
            EF(dummy start)=ES(activity)
            EF(activity)=ES(activity)+duration(activity)
            EF(activity)=ES(dummy finish)
  
  arg:  node-- index of the node from which invariant has to corrected
        start-- distance between current node and last node from which invariant is broken                
  */
  void set_invariant_fwd_parse(int node, int start){
    /*
    Traverses backwards in the graph until a node with no predecessor is found.
    Start forward parse from that node,
    with start as distance between that node and the node from which invariant is broken.
    */
    if(Rev_AdjList[node].empty()){      
      forward_parse(node, start);
    }
    for(int v: Rev_AdjList[node]){      
      set_invariant_fwd_parse(v,start - Attributes[v].duration);
    }    
  }

  /*
  Forward parsing to find early start and early finish dates
  arg:  u-- index of the source node
        start-- baseline/start of the node
  */
  void forward_parse(int u, int start = 0){  
    Attributes[u].early_start = max(start,Attributes[u].early_start);
    Attributes[u].early_finish = max(start + Attributes[u].duration ,Attributes[u].early_finish);
    
    /*
    For every outgoing vertex of u,
    v.ES = max(v.ES, u.EF) and v.EF = max(v.EF, u.EF+v.duration)    
    */
    for(int v: AdjList_of_Vertices[u]){
      if(Attributes[v].early_start < Attributes[u].early_finish){

        Attributes[v].early_start = Attributes[u].early_finish;
        Attributes[v].early_finish = Attributes[u].early_finish + Attributes[v].duration;      
        
        /*
        if v is a dummy terminal node,(FF or SF dependency),
        invariant property could be broken.
        only dummy terminal is set proper start/finish and not actual activity and dummy start
        */
        if( Attributes[v].is_terminal ){
          if(Attributes[v-1].early_finish != Attributes[v].early_start){
            //vertex (v-1) must start with v.ES- (v-1).duration
            set_invariant_fwd_parse(v-1, Attributes[v].early_start - Attributes[v-1].duration);              
          }
        }

      }
      forward_parse(v,Attributes[v].early_start);
    }
  }
  
  /*
  Function to invoke forward parsing in topological order
  */
  void critical_path(){
    //source node is the first node in topological ordering
    Attributes[top_order[0]].early_start = 0;    
    Attributes[top_order[0]].early_finish = 0;
    
    for(int u: top_order ){
      forward_parse(u);    
    }
  }


  void set_invariant_bkwd_parse(int node, int LF){ 
  //cout<<"invariant:"<<node<<" "<<LF<<endl;   
    if(AdjList_of_Vertices[node].empty()){      
      backward_parse(node, LF);
    }
    for(int v: AdjList_of_Vertices[node]){      
      set_invariant_bkwd_parse(v,LF + Attributes[v].duration);
    }    
  }


  void backward_parse(int u, int end ){
    //cout<<u<<" "<<end<<endl;
    //cout<<Attributes[u].late_start<<" "<<Attributes[u].late_finish<<" "<<Attributes[u].duration<<endl;
    Attributes[u].late_start = min(end-Attributes[u].duration, Attributes[u].late_start);
    Attributes[u].late_finish = min(end, Attributes[u].late_finish);

    if( Attributes[u].is_initial ){
      if(Attributes[u].late_finish != Attributes[u+1].late_start){
        //cout<<"property broken at "<<u<<endl;
        set_invariant_bkwd_parse(u+1, Attributes[u].late_finish + Attributes[u+1].duration);              
      }
    }

    //v is connected to u in the original graph
    for(int v: Rev_AdjList[u]){        
      backward_parse(v, Attributes[u].late_start);
    }    
  }

  void compute_late_dates(int len){
    vector<int> rev_topo = top_order;
    reverse(rev_topo.begin(), rev_topo.end());

    Attributes[rev_topo[0]].late_finish = len;
    Attributes[rev_topo[0]].late_start = len - Attributes[rev_topo[0]].duration;

    for(int u: Vertices){
      Attributes[u].late_start = len- Attributes[u].duration;;
      Attributes[u].late_finish = len ;
    }

    //backward_parse(rev_topo[0], len);

    for(int u: rev_topo ){   
      backward_parse(u,len);
      //cout<<endl<<endl;
    }
  }


};
