    //////////////-----------------------------------------------

    // Constraints -- Must finish before, must start after, must start before

    // change to particular point of time and recompute critical path
    
    // Accouting for actual fraction of work completed
    //-----------------------------//////////////////////////////

#include "graph_lib.cpp"
#include "read_input.cpp"
#include <ql/time/calendars/india.hpp>

using namespace QuantLib;

void print_early_start(Graph &g){

  Calendar cal = India();
  
  cal.addHoliday(Date(13, June, 2018));
  cal.addHoliday(Date(20, June, 2018));
  cal.addHoliday(Date(21, June, 2018));
  Date d1(6, June, 2018);
  cout<<"vertex\tES\tEF\n";
  for(int u: g.vertices()){
    if((u+2)%3) continue;
    if(g.get_name(u) == "off") continue;
    
    Period p1(g.get_early_start(u)-1, Days);
    Period p2( g.get_early_finish(u)-1, Days);
    
    cout<<g.get_name(u)<<"\t";
    //cout<<cal.advance(d1, p1) << "\t" << cal.advance(d1, p2)<<endl;
    cout<<g.get_early_start(u) << "\t" <<g.get_early_finish(u) <<endl;
  }
}

int main() {

  CSVReader reader("tasks.csv");
  // Get the data from CSV File
  vector<vector<string> > tasks = reader.getData();
  
  int n = (tasks.size()-1);
  Graph g(3*n);
  
  for(int i=1; i<=n; ++i){

    vector<string> row=tasks[i];

    /*
    row[0] - task_id
    row[1] - name
    row[2] - parent
    row[3] - duration
    */

    //cout<<"--"<<(row[0])<<" \n";  
    
    int index=stoi(row[0]);
    
    //create 3 nodes- dummy start, actual node, dummy end
    g.insert_vertex(3*index);
    g.insert_vertex(3*index+1);
    g.insert_vertex(3*index+2);
    //create link between dummy start and actual node
    g.insert_edge(3*index, 3*index+1);
    g.insert_reverse_adj(3*index, 3*index+1);
    //create link between actual node and dummy end
    g.insert_edge(3*index+1, 3*index+2);    
    g.insert_reverse_adj(3*index+1, 3*index+2);
    
    g.set_name(3*index+1, row[1]);

    //dummy start
    g.set_duration(3*index, 0);
    g.set_initial(3*index);

    //actual node
    g.set_duration(3*index+1,stoi(row[3]));

    //dummy end
    g.set_duration(3*index+2, 0);
    g.set_terminal(3*index+2);

    //delete any node marked as parent
    if(row[2] != "-"){
      int parent_index = stoi(row[2]);
      g.remove_activity(parent_index);
    }
  }

  int offset_count = 0;

  CSVReader relationReader("relationships.csv");
  vector<vector<string> > relations = relationReader.getData();
    
  for(int i=1; i<relations.size(); ++i){
  
    vector<string> row = relations[i];
    /*
    row[0] - task_id
    row[1] - pred_id
    row[2] - dependency type
    row[3] - offset (string length == 1 if empty )
    */
    int v=stoi(row[0]), u=stoi(row[1]), offset;      
    //cout<<row[3].size()<<endl;
    if (row[3].size() == 1) cout<<"empty\n";
    /*
    int j=0;
    for(auto word: row){
      cout<< j++ <<" "<<word<<endl;
    }
    cout<<endl;
    */

    if(row[2] == "FS"){
      cout<<"FS\n";
      //remove negation!
      //if(row[3].size()==1){
        //no offset
        //dummy 3u, actual 3u+1, dummy 3u+2
        //dummy 3v, actual 3v+1, dummy 3v+2

        //3u+2 --> 3v
        //cout<<3*u+2<<" FS--> "<<3*v<<endl;
        g.insert_edge(3*u+2, 3*v);
        g.insert_reverse_adj(3*u+2, 3*v); 
      //}
      /*
      else{
        offset = stoi(row[3]);
        offset_count++;
        //3u+2 --> offset -->3v
        g.insert_vertex(3*n+offset_count);
        g.insert_edge()
      }
      */
    }
    else if(row[2] == "SS"){
      //cout<<"SS\n";
      //if(row[3].size() == 1){
        //3u --> 3v
        g.insert_edge(3*u, 3*v);
        g.insert_reverse_adj(3*u, 3*v);   
      //}
    }
    else if(row[2] == "SF"){
      //if(row[3].size() == 1){
        //3u --> 3v+2
        g.insert_edge(3*u, 3*v+2);
        g.insert_reverse_adj(3*u, 3*v+2);   
      //}
    }
    else if(row[2] == "FF"){
      //3u+2 --> 3v+2
      //if(row[3].size() == 1){
        //3u --> 3v
        g.insert_edge(3*u+2, 3*v+2);
        g.insert_reverse_adj(3*u+2, 3*v+2);   
      //}
    }
  }

/*
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
  for(int i=0;i<n/3;++i){
    string s;
    cin>>s;
    g.set_name(3*i+1,s);
  }
  /*
  for(int i=0;i<n/3;++i){
    bool is_variable;
    cin>>is_variable;
    g.set_variable(3*i+1, is_variable);
  }
  for (int i = 0; i < 2; ++i)
  {
    int a,b;
    g.set_child(a,b);
  }
  */

  print_graph(g);
  cout<<endl;
  g.topologicalSort();
  cout<<endl;
  g.critical_path();

  print_early_start(g);

  return 0;
}

