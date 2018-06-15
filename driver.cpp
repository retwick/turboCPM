    //////////////-----------------------------------------------

    // Calendars
    // use C++ boost library for calendar arithmetic 

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
    if((g.get_name(u) == "off")) continue;
    Period p1(g.get_early_start(u)-1, Days);
    Period p2( g.get_early_finish(u)-1, Days);
    cout<<g.get_name(u)<<"\t"<<cal.advance(d1, p1) << "\t" << cal.advance(d1, p2)<<endl;
    //cout<<g.get_name(u)<<"\t"<<g.get_early_start(u) << "\t" <<g.get_early_finish(u) <<endl;
  }
}

int main() {

  CSVReader reader("tasks.csv");
  // Get the data from CSV File
  vector<vector<string> > dataList = reader.getData();
  cout<<dataList.size()-1;
  int n = (dataList.size()-1)*3;
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
  for(int i=0;i<n/3;++i){
    bool is_parent;
    cin>>is_parent;
    //g.set_variable(3*i+1, is_parent);
  }
*/
  g.topologicalSort();
  cout<<endl;
  print_graph(g);
  cout<<endl;
  g.critical_path();

  print_early_start(g);

  return 0;
}

