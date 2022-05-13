#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME MrPanda7


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
  vector<queue<Dir>> v_queue; 

  Pos bfs_find_pos(const Pos& start){
  
    vector <vector<char> > tmp_map(rows(), vector<char>(cols(), ' '));
    queue<Pos> q_pos;
    q_pos.push(start);
    tmp_map[start.i][start.j] = 'x';

    bool found = false;
    while(not q_pos.empty()){
      Pos tmp_pos = q_pos.front();
      q_pos.pop();
      tmp_map[tmp_pos.i][tmp_pos.j] = 'x';
      Cell tmp_cell = cell(tmp_pos);
      if(tmp_pos != start and ((tmp_cell.type == PATH and path_owner(tmp_cell.path_id) != me()) or
        (tmp_cell.type == CITY and city_owner(tmp_cell.city_id) != me()))) return tmp_pos;
      for(int i = 0; i < 4; ++i){
        tmp_cell = cell(tmp_pos+Dir(i));
        if(tmp_cell.type != WALL and pos_ok(tmp_pos+Dir(i)) and tmp_map[(tmp_pos+Dir(i)).i][(tmp_pos+Dir(i)).j] != 'x'){
          q_pos.push(tmp_pos+Dir(i));
          tmp_map[(tmp_pos+Dir(i)).i][(tmp_pos+Dir(i)).j] = 'x';
        }
      }
    }
    return start;
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
    
    

    for(int i = 0; i < 4; ++i){
      Cell c = cell(pos + Dir(i));
      if(c.unit_id == -1 or unit(c.unit_id).player != me()){
        if(is_secure_pos(pos + Dir(i))) return Dir(i);
      }
    }
    
  
    return Dir(random(0,3));
  }
  Dir movement(const Unit& u){
    Pos pos = bfs_find_pos(u.pos);
    return get_move_for_target(u.pos, pos); 
  }
  
  void controler(){
    VI U = my_units(me()); // Get the id's of my units.
    for (unsigned i = 0; i <  U.size(); ++i) {
      int id = U[i];
      Dir tmp_move = movement(unit(id));
      move(id, tmp_move);
    }
  }



  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
    if(round() == 0){
      v_queue = vector<queue<Dir>>(200); 
    }
    controler();
  }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);

