#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME MrPanda


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */

  typedef vector<int> VI;
  vector<queue<Dir>> v_path_queue; 

  Dir dir_complement(const Dir& d){
    return Dir((d + 2)%4);
  }

  bool ok_city(int id){
    City cells = city(id);
    int count = 0;
    for(unsigned i = 0; i < cells.size(); ++i){
      int u_id = cell(cells[i]).unit_id;
      if(u_id != -1 and unit(u_id).player != me()) ++count; 
    }

    int n = 40/nb_cities();
    return count <= n;
  }

  // bool ok_path(int id){
  //   Path p = path(id);
  //   int count = 0;
  //   for(unsigned i = 0; i < p.second.size(); ++i){
  //     int u_id = cell(p.second[i]).unit_id;
  //     if(u_id != -1 and unit(u_id).player != me()) ++count; 
  //   }

  //   return count <= 50;
  // }

  pair<Pos, queue<Dir>> bfs_find_pos(const Pos& start, int u_id){
  
    vector <vector<bool> > visited(rows(), vector<bool>(cols(), false));
    vector <vector<int> > path(rows(), vector<int>(cols(), 0));

    queue<pair<Pos, int>> q_pos;
    q_pos.push(make_pair(start, 1));
    path[start.i][start.j] = 1;
    Pos tmp_pos;

    while(not q_pos.empty()){
      tmp_pos = q_pos.front().first;
      int n = q_pos.front().second;
      visited[tmp_pos.i][tmp_pos.j] = true;
      path[tmp_pos.i][tmp_pos.j] = n;
      q_pos.pop();

      Cell tmp_cell = cell(tmp_pos);
      if(n > 1 and (  
        (tmp_cell.type == PATH and path_owner(tmp_cell.path_id) != me()) or
        (tmp_cell.type == CITY and ok_city(tmp_cell.city_id) and city_owner(tmp_cell.city_id) != me())
        )) break;
      for(int i = 0; i < 4; ++i){
        tmp_cell = cell(tmp_pos+Dir(i));
        if(tmp_cell.type != WALL and pos_ok(tmp_pos+Dir(i)) and not visited[(tmp_pos+Dir(i)).i][(tmp_pos+Dir(i)).j]){
          q_pos.push(make_pair(tmp_pos+Dir(i),n+1));
          visited[(tmp_pos+Dir(i)).i][(tmp_pos+Dir(i)).j] = true;
        }
      }
    }

    bool found = false;
    Pos tmp = tmp_pos;
    stack<Dir> s_dir;

    while(tmp != start){
      int n = path[tmp.i][tmp.j];
      for(int i = 0; i < 4; ++i){
        if(path[(tmp+Dir(i)).i][(tmp+Dir(i)).j] == n-1){
          tmp = tmp+Dir(i);
          s_dir.push(Dir(i));
        }
      }
    }
    
    queue<Dir> q_dir;
    while(not s_dir.empty()){
     q_dir.push(dir_complement(s_dir.top()));
      s_dir.pop();
    }
    
    return make_pair(tmp_pos, q_dir);
  }

  bool is_secure_pos(const Pos& pos){
     for(int i = 0; i < 4; ++i){
        Cell c = cell(pos + Dir(i));
        if(c.unit_id != -1 and unit(c.unit_id).player != me()){
          return false;
        }
      }
      return true;
  }

 
  Dir get_move_for_target(const Pos& pos, const Pos& target){
    vector<Dir> v(2, NONE);
    int n = -1;
    if(pos.i < target.i and pos_ok(pos.i+1,pos.j) and cell(pos.i+1,pos.j).type != WALL)
      return  BOTTOM;
    if(pos.i > target.i and pos_ok(pos.i-1,pos.j) and cell(pos.i-1,pos.j).type != WALL)
      return TOP;
    if(pos.j < target.j and pos_ok(pos.i,pos.j+1) and cell(pos.i,pos.j+1).type != WALL)
      return RIGHT;
    if(pos.j > target.j and pos_ok(pos.i,pos.j-1) and cell(pos.i,pos.j-1).type != WALL)
      return LEFT;
    
    /*for(int i = 0; i < 4; ++i){
      Cell c = cell(pos + Dir(i));
      if(c.unit_id == -1 or unit(c.unit_id).player != me()){
        if(is_secure_pos(pos + Dir(i))) return Dir(i);
      }
    }
      
    return Dir(random(0,3));*/
    return NONE;
  }
  Dir movement(const Unit& u){
    pair<Pos, queue<Dir> > tmp_pair = bfs_find_pos(u.pos, u.id);
    Dir tmp_dir = get_move_for_target(u.pos, tmp_pair.first);
    
    if(tmp_dir != NONE){
      if(is_secure_pos(u.pos+tmp_dir)) return tmp_dir;
      /*for(int i = 0; i < 4; ++i){
      Cell c = cell(pos + Dir(i));
      if(c.unit_id == -1 or unit(c.unit_id).player != me()){
        if(is_secure_pos(pos + Dir(i))) return Dir(i);
      }
    }
      
    return Dir(random(0,3));*/

    }
    v_path_queue[u.id] = tmp_pair.second;
    v_path_queue[u.id].pop();
    return tmp_pair.second.front();
  }
  
  void controler(){
    VI U = my_units(me()); // Get the id's of my units.
    for (unsigned i = 0; i <  U.size(); ++i) {
      int id = U[i];
      Dir tmp_move = NONE;
      if(v_path_queue[id].empty()){
        tmp_move = movement(unit(id));
      }
      else{
        tmp_move = v_path_queue[id].front();
        v_path_queue[id].pop();
      }
      move(id, tmp_move);
    }
  }



  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
    if(round() == 0){
      v_path_queue = vector<queue<Dir>>(200); 
    }
    controler();
  }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);

