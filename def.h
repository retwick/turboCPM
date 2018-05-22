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
    int parent = -1;

    int early_start, early_finish, late_start, late_finish;
  };
  vector < attribute > Attributes;

  void new_edge(int u, int v, pair < int, int > edge, int weight);

  void insert_edge(int u, int v, int weight = 1) ;  

  void insert_edge(pair < int, int > edge, int weight = 1) ;

  void insert_vertex(int u);

  set < int > vertices();

  set <pair<int, int> > get_edges();

  set < int > get_adjList(int vertex);

  int get_weight(int u, int v);

  int get_weight(pair < int, int > edge) ;

  bool is_visited(int u); //set attribute.visited to true

  bool get_distance(int u) ;

  bool is_edge(int u, int v); //check if (u,v)eE --slow 

  void topologicalSort();

  void critical_path();

  vector<int> get_top_order();


};