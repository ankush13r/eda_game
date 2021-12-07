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
  vector<Pos> pos_assigned;

  Dir find_mask(const Pos& start){
    for(int i = 0; i < 4; ++i){
      if(cell(start + Dir(i)).mask) return Dir(i);
    }
    return NONE;
  }

  Dir find_enemy(const Pos& start){
    for(int i = 0; i < 4; ++i){
      int u_id = cell(start + Dir(i)).unit_id;
      if(u_id != -1 and unit(u_id).player != me()) return Dir(i);
    }
    return NONE;
  }

  bool ok_city(int id){
    City cells = city(id);
    int count = 0;
    for(unsigned i = 0; i < cells.size(); ++i){
      int u_id = cell(cells[i]).unit_id;
      if(u_id != -1 and unit(u_id).player == me()) ++count; 
    }
 
    return count <= 3;
  }

  bool ok_path(int id){
    vector<Pos> cells = path(id).second;
    int count = 0;
    for(unsigned i = 0; i < cells.size(); ++i){
      int u_id = cell(cells[i]).unit_id;
      if(u_id != -1 and unit(u_id).player == me()) ++count; 
    }
    return count <= 1 and cells.size() > 5;
  }

  bool bfs_comp(const Pos& start, const Pos& end){
      if(start == end) return false;
      Cell tmp_cell = cell(end);
      return (tmp_cell.type == PATH and ok_path(tmp_cell.path_id) and path_owner(tmp_cell.path_id) != me()) or
             (tmp_cell.type == CITY and ok_city(tmp_cell.city_id) and city_owner(tmp_cell.city_id) != me());

  }

  pair<Pos, queue<Dir>> bfs_find_pos(const Pos& start){
  
    vector <vector<bool> > visited(rows(), vector<bool>(cols(), false));

    queue<Dir> tmp_q = queue<Dir>();
    queue<pair<Pos, queue<Dir> >> q_pos;
    q_pos.push(make_pair(start,tmp_q));
    Pos tmp_pos;

    while(not q_pos.empty()){
      tmp_pos = q_pos.front().first;
      tmp_q = q_pos.front().second;
      visited[tmp_pos.i][tmp_pos.j] = true;
      q_pos.pop();

      if(bfs_comp(start,tmp_pos)) break;
      for(int i = 0; i < 4; ++i){
        Cell tmp_cell = cell(tmp_pos+Dir(i));
        if(tmp_cell.type != WALL and pos_ok(tmp_pos+Dir(i)) and not visited[(tmp_pos+Dir(i)).i][(tmp_pos+Dir(i)).j]){
          queue<Dir> q2 = tmp_q;
          q2.push(Dir(i));
          q_pos.push(make_pair(tmp_pos+Dir(i), q2));
          visited[(tmp_pos+Dir(i)).i][(tmp_pos+Dir(i)).j] = true;
        }
      }
    }
    return make_pair(tmp_pos, tmp_q);
  }

  Dir movement(const Unit& u){
    Dir tmp_move = find_enemy(u.pos);
    if(tmp_move == NONE and not u.mask and not u.immune) tmp_move = find_mask(u.pos);
    if(tmp_move != NONE) return tmp_move;

    pair<Pos, queue<Dir>> tmp_pair = bfs_find_pos(u.pos);
    Pos target = tmp_pair.first;
    vector<Dir> moves(2,NONE);
    int n = -1;
    if(target.i < u.pos.i and pos_ok(u.pos+TOP) and cell(u.pos+TOP).type != WALL) moves[++n] = TOP;
    if(target.i > u.pos.i and pos_ok(u.pos+BOTTOM) and cell(u.pos+BOTTOM).type != WALL) moves[++n] = BOTTOM;
    if(target.j < u.pos.j and pos_ok(u.pos+LEFT) and cell(u.pos+LEFT).type != WALL) moves[++n] = LEFT;
    if(target.j > u.pos.j and pos_ok(u.pos+RIGHT) and cell(u.pos+RIGHT).type != WALL) moves[++n] = RIGHT;
    
    Dir better_move = tmp_pair.second.front();
    if(moves[0] != NONE and moves[1] != NONE){
      Cell cell1 = cell(u.pos + moves[0]);
      Cell cell2 = cell(u.pos + moves[1]);
      if(cell1.type == CITY or cell1.type == PATH) better_move = moves[0];
      else if(cell2.type == CITY or cell2.type == PATH)  better_move = moves[1];
    }
    else if(moves[0] != NONE) better_move = moves[0];
    else if(moves[1] != NONE) better_move = moves[1];

    if(tmp_pair.second.front() != better_move){
    
      Cell cell1 = cell(u.pos + tmp_pair.second.front());
      Cell cell2 = cell(u.pos + better_move);
      if(cell1.type == CITY or cell1.type == PATH) return tmp_pair.second.front();
      else if(cell2.type == CITY or cell2.type == PATH)  return better_move;
    }
    return tmp_pair.second.front();
  }

  void move_units(){
    VI U = my_units(me()); // Get the id's of my units.
    for (unsigned i = 0; i <  U.size(); ++i) {
      int id = U[i];
       Dir tmp_move = NONE;
       Unit u = unit(id);
       tmp_move = movement(u);
       int tmp_uid = cell(u.pos + tmp_move).unit_id; 
       if(tmp_uid != -1 and unit(tmp_uid).player == me()){
         for(int i = 0; i < 4; ++i){
           Cell tmp_cell = cell(u.pos + Dir(i)); 
           if(tmp_cell.type != WALL and pos_ok(u.pos + Dir(i)) and 
              (tmp_cell.unit_id == -1 or unit(tmp_cell.unit_id).player != me()))
            {
              tmp_move = Dir(i);
              break;
           }
         }
       }
       move(id, tmp_move);
    }
  }

  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
    if(round() == 0){
      pos_assigned = vector<Pos>(200, Pos());
    }
    move_units();
  }

//---------------------//
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);

