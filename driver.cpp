#include "graph_lib.cpp"
#include "read_input.cpp"
#include <ql/time/calendars/india.hpp>

using namespace QuantLib;

Calendar create_calendar(){
  //take argument- start date of project;
  Calendar cal = India();
  
  CSVReader reader("dummy_input/Calendar.csv");
  // Get the data from CSV File
  vector<vector<string> > working_days = reader.getData();

  cal.addHoliday(Date(13, June, 2018));
  cal.addHoliday(Date(20, June, 2018));
  cal.addHoliday(Date(21, June, 2018));
    
  return cal;
}

void print_early_start(Graph &g, Calendar cal){
  Date d1(6, June, 2018);
  for(int u: g.vertices()){
    if((u+2)%3) continue;
    if(g.get_name(u) == "offset" || g.get_name(u) == "sink") continue;
    
    Period p1(g.get_early_start(u), Days);
    Period p2( g.get_early_finish(u)-1, Days);
        
    cout<<g.get_name(u)<<"\t";
    cout<<cal.advance(d1, p1) << "\t" << cal.advance(d1, p2)<<endl;
    
  }
}

int main() {

  CSVReader reader("dummy_input/tasks.csv");
  // Get the data from CSV File
  vector<vector<string> > tasks = reader.getData();
  
  int n = tasks.size();
  Graph g(3*n);
  
  for(int i=1; i<=n-1; ++i){
    vector<string> row=tasks[i];

    //row[0] - task_id
    //row[1] - name
    //row[2] - parent
    //row[3] - duration
             
    int index=stoi(row[0]);
    
    //create 3 nodes- dummy start, actual node, dummy end
    g.add_activity(index, stoi(row[3]));    
    
    g.set_name(index, row[1]);

    //delete any node marked as parent
    if(row[2] != "-"){
      int parent_index = stoi(row[2]);
      g.remove_activity(parent_index);
    }
  }
    
  int offset_count = 0;

  CSVReader relationReader("dummy_input/relationships.csv");
  vector<vector<string> > relations = relationReader.getData();

  for(int i=1; i<relations.size(); ++i){
  
    vector<string> row = relations[i];
    
    //row[0] - task_id
    //row[1] - pred_id
    //row[2] - dependency type
    //row[3] - offset (string length == 1 if empty )
    
    int v=stoi(row[0]), u=stoi(row[1]);      
    
    if(row[2] == "FS"){          
      if(row[3].size()==1){
        //no offset
        //dummy 3u, actual 3u+1, dummy 3u+2
        //dummy 3v, actual 3v+1, dummy 3v+2

        //3u+2 --> 3v
        //cout<<3*u+2<<" FS--> "<<3*v<<endl;
        g.connect(3*u+2, 3*v);
      }
      
      else{
        int offset = stoi(row[3]);
        
        offset_count++;
        //3u+2 --> offset -->3v
        g.add_activity(n+offset_count, offset);
        g.set_name(n+offset_count, "offset");

        g.connect(3*u+2,3*(offset_count+n));
        g.connect(3*(offset_count+n)+2, 3*v);
      }
    }
    else if(row[2] == "SS"){
      
      if(row[3].size() == 1){
        //3u --> 3v
        g.insert_edge(3*u, 3*v);
        g.insert_reverse_adj(3*u, 3*v);   
      }
      else{
        int offset = stoi(row[3]);

        offset_count++;
        //3u --> offset -->3v+2
        g.add_activity(n+offset_count, offset);
        g.set_name(n+offset_count, "offset");

        g.connect(3*u,3*(offset_count+n));
        g.connect(3*(offset_count+n)+2, 3*v);
      }
    }
    else if(row[2] == "SF"){
      
      if(row[3].size() == 1){
        //3u --> 3v+2
        g.insert_edge(3*u, 3*v+2);
        g.insert_reverse_adj(3*u, 3*v+2);   
      }
      else{
        int offset = stoi(row[3]);
        
        offset_count++;
        //3u --> offset -->3v+2
        g.add_activity(n+offset_count, offset);
        g.set_name(n+offset_count, "offset");

        g.connect(3*u,3*(offset_count+n));
        g.connect(3*(offset_count+n)+2, 3*v+2);
      }
    }
    else if(row[2] == "FF"){
      //3u+2 --> 3v+2      
      if(row[3].size() == 1){
        //3u --> 3v
        g.insert_edge(3*u+2, 3*v+2);
        g.insert_reverse_adj(3*u+2, 3*v+2);   
      }
      else{
        int offset = stoi(row[3]);

        offset_count++;
        //3u+2 --> offset -->3v+2
        g.add_activity(n+offset_count, offset);
        g.set_name(n+offset_count, "offset");

        g.connect(3*u+2,3*(offset_count+n));
        g.connect(3*(offset_count+n)+2, 3*v+2);
      }
    }
  }

  //add dummy sink
  offset_count += 1;
  g.add_activity(n+offset_count, 0);
  g.set_name(n+offset_count, "sink");
  for(int v: g.vertices()){    
    if((g.get_adjList(v)).empty()){
      if(v!= 3*(offset_count+n)+2){        
        g.connect(v,3*(offset_count+n));   
      }
    }
  }

  
  print_graph(g);
  //cout<<endl;
  g.topologicalSort();
  cout<<endl;
  g.critical_path();
  Calendar cal = create_calendar();
    
  print_early_start(g, cal);
  cout<<endl;
  
  return 0;
}

