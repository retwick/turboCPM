#include "graph_lib.cpp"
#include "read_input.cpp"
#include <ql/time/calendars/india.hpp>
#include <string>
using namespace QuantLib;


Date get_date(string line){
  
  int year = stoi(line.substr(0, 4));
  int month = stoi(line.substr(5, 2));
  int date = stoi(line.substr(8, 2));
    
  return Date(date, Month(month), year);
}

Calendar create_calendar(){
  
  Calendar cal = India();
  
  CSVReader reader("input/calendar.csv");
  // Get the data from CSV File
  vector<vector<string> > working_days = reader.getData();
    
  bool flag = false;
  for(auto row: working_days){
    if(row[0] =="holiday_name"){
      flag = true;
      continue;
    }
    if(flag){
      //start adding holidays
      Date d_start = get_date(row[1]);      
      Date d_end = get_date(row[2]);

      for(Date it= d_start; it<= d_end; ++it){
        cal.addHoliday(it);
      }      
    }
  }
  return cal;
}

void print_early_start(Graph &g, Calendar cal){
  Date d1(6, June, 2018);
  for(int u: g.vertices()){
    if((u+2)%3) continue;
    if(g.get_name(u) == "offset" || g.get_name(u) == "sink") continue;
    
    Period p1(g.get_early_start(u), Days);
    Period p2( g.get_early_finish(u)-1, Days);
    Date d_start = cal.advance(d1, p1), d_end = cal.advance(d1, p2);
    //date in yyyy-mm-dd format   
    cout<<g.get_name(u)<<"\t";
    cout<<d_start.year()<<"-"<<int(d_start.month())<<"-"<<d_start.dayOfMonth() << "\t";
    cout<<d_end.year()<<"-"<<int(d_end.month())<<"-"<<d_end.dayOfMonth() << endl;
    
  }
  cout<<endl<<endl; 
}

void print_late_dates(Graph &g, Calendar &cal){
  Date d1(6, June, 2018);
  for(int u: g.vertices()){
    if((u+2)%3) continue;
    if(g.get_name(u) == "offset" || g.get_name(u) == "sink") continue;

    Period p1(g.get_late_start(u), Days);
    Period p2( g.get_late_finish(u)-1, Days);    
    
    Date d_start = cal.advance(d1, p1), d_end = cal.advance(d1, p2);
    //date in yyyy-mm-dd format   
    cout<<g.get_name(u)<<"\t";
    cout<<d_start.year()<<"-"<<int(d_start.month())<<"-"<<d_start.dayOfMonth() << "\t";
    cout<<d_end.year()<<"-"<<int(d_end.month())<<"-"<<d_end.dayOfMonth() << endl;
    
  }
}

void write_output(Graph &g, Calendar &cal){
  ofstream output;
  output.open("output.txt");
  /*
  output format
  activity_name, early_start, early_finish, late_start, late_finish, slack
  */
  //all dates in yyyy-mm-dd format   

  Date d1(6, June, 2018);
  for(int u: g.vertices()){
    if((u+2)%3) continue;
    if(g.get_name(u) == "offset" || g.get_name(u) == "sink") continue;
    
    Period p1(g.get_early_start(u), Days);
    Period p2( g.get_early_finish(u)-1, Days);

    Period late_p1(g.get_late_start(u), Days);
    Period late_p2( g.get_late_finish(u)-1, Days);    
    
    Date d_e_start = cal.advance(d1, p1), d_e_end = cal.advance(d1, p2);
    int m;
    //date in yyyy-mm-dd format   
    output<<g.get_name(u)<<", ";
    //write early start
    output<<d_e_start.year()<<"-";
    m = int(d_e_start.month());
    if(m<10) { output<<"0";}
    output<<int(d_e_start.month())<<"-"<<d_e_start.dayOfMonth() << ", ";

    //wrte early finish
    output<<d_e_end.year()<<"-";
    m = int(d_e_end.month());
    if(m<10) { output<<"0";}
    output<<int(d_e_end.month())<<"-"<<d_e_end.dayOfMonth()<<", ";
    
    Date d_start = cal.advance(d1, late_p1), d_end = cal.advance(d1, late_p2);
    
    //write late start      
    output<<d_start.year()<<"-";
    m = int(d_start.month());
    if(m<10){ output<<"0";}
    output<<int(d_start.month());
    output<<"-"<<d_start.dayOfMonth() << ", ";
    
    //write late finish
    output<<d_end.year()<<"-";
    m = int(d_end.month());
    if(m<10){ output<<"0";}
    output<<int(d_end.month())<<"-"<<d_end.dayOfMonth()<<", ";

    //write slack
    output<<g.get_late_start(u) - g.get_early_start(u)<<endl;  
  }
}

/*
Find the length of the project.(largest late finish)
*/
int find_length(Graph &g){
  int proj_length = 0;
  for(int v: g.vertices()){
    proj_length = max(proj_length, g.get_early_finish(v));
  }
  return proj_length;
}

int main() {

  CSVReader reader("input/tasks.csv");
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

  CSVReader relationReader("input/relationships.csv");
  vector<vector<string> > relations = relationReader.getData();

  for(int i=1; i<relations.size(); ++i){
  
    vector<string> row = relations[i];
    
    //row[0] - task_id
    //row[1] - pred_id
    //row[2] - dependency type
    //row[3] - offset (string length == 1 if empty )
    
    int v=stoi(row[0]), u=stoi(row[1]);      
    //dummy 3u, actual 3u+1, dummy 3u+2
    //dummy 3v, actual 3v+1, dummy 3v+2
    
    if(row[2] == "FS"){          
      if(row[3].size()==1){
        //no offset

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
/*
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
*/
  
  g.topologicalSort();  
  g.critical_path();
  Calendar cal = create_calendar();    
  //print_early_start(g, cal);  
  
  int length = find_length(g);  

  g.compute_late_dates(length);
  //print_late_dates(g,cal);

  write_output(g,cal);
  return 0;
}

