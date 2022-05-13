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
  struct Unit_assignation{
    int city;
    Pos pos;
    queue<Dir> route;
    Unit_assignation (int city, Pos pos, queue<Dir> route) : city(city), pos(pos), route(route) { };
  };
  vector<Unit_assignation> city_assigned; //City assigned to the units.


  int pos_diff(const Pos& pos1, const Pos& pos2){
     return abs(pos1.i - pos2.i) + abs(pos1.j - pos2.j);
  }

  // Return position of a random cell of city near by u_pos.
  int nearest_city(const Unit& u){
    //Run all cities

    int i = (0 == city_assigned[u.id].city); // If city assigned is 0, so it will start counting from 1.
    int diff = pos_diff(city(i)[0],u.pos);
    int id = i;

    while(++i < nb_cities()){
    //random(0,city_cells.size()-1) Randomly picking a cell of the city.
      int tmp_diff = pos_diff(city(i)[0], u.pos);
      if(tmp_diff < diff and city_assigned[u.id].city != i){
        diff = tmp_diff;
        id = i;
      }
    }
    return id;
  }

  Dir check_mask(const Pos& pos){
    Dir tmp_dir = NONE;
    for(unsigned i = 0; i < 4; ++i){
      Pos tmp_pos = pos + Dir(i);
      if(cell(tmp_pos).mask){
        tmp_dir = Dir(i);
        break;
      }
    }
    return tmp_dir;
  }

  /*Dir get_better_move(const Pos& pos, const Pos& target, const Dir& move1, const Dir& move2){
    bool found = false;
    bool wall1 = false;
    bool wall2 = false;
    Pos tmp_pos = pos;
    int n = 0;
    while(not found and tmp_pos.i != target.i and tmp_pos.j != target.j){
      tmp_pos += move1;
      Cell tmp_cell = cell(pos);
      if(tmp_cell.mask or tmp_cell.type == PATH or tmp_cell.type == CITY) found = true;
      if(tmp_cell.type == WALL) found = wall1 = true;
      ++n;
    }

    found = false;
    int m = 0;
    while(not found and tmp_pos.i != target.i and tmp_pos.j != target.j){
      tmp_pos += move2;
      Cell tmp_cell = cell(pos);
      if(tmp_cell.mask or tmp_cell.type == PATH or tmp_cell.type == CITY) found = true;
      if(tmp_cell.type == WALL) found = wall2 = true;
      ++m;
    }

    if(wall1 and wall2) return move1;
    if(wall1) return move2;
    if(wall2 or n < m) return move1;
    return move2;

  }*/

  Dir get_move_for_target(const Pos& pos, const Pos& target){
    vector<Dir> v(2, NONE);
    int n = -1;
    if(pos.i < target.i and pos_ok(pos.i+1,pos.j) and cell(pos.i+1,pos.j).type != WALL)
      v[++n] =  BOTTOM;
    else if(pos.i > target.i and pos_ok(pos.i-1,pos.j) and cell(pos.i-1,pos.j).type != WALL)
      v[++n] = TOP;
    else if(pos.j < target.j and pos_ok(pos.i,pos.j+1) and cell(pos.i,pos.j+1).type != WALL)
      v[++n] = RIGHT;
    else if(pos.j > target.j and pos_ok(pos.i,pos.j-1) and cell(pos.i,pos.j-1).type != WALL)
      v[++n] = LEFT;
    
    if(v[0] != NONE and v[1] != NONE){
      Pos tmp_pos1 = pos+v[0];
      Pos tmp_pos2 = pos+v[1];      
      if(cell(tmp_pos1).type == CITY or cell(tmp_pos1).type == PATH or cell(tmp_pos1).mask) return v[0];
      if(cell(tmp_pos2).type == CITY or cell(tmp_pos2).type == PATH or cell(tmp_pos2).mask) return v[0];

      while(tmp_pos1.i != target.i and tmp_pos1.j != target.j and
            tmp_pos2.i != target.i and tmp_pos2.j != target.j)
      {
        Cell tmp_cell1 = cell(tmp_pos1);
        Cell tmp_cell2 = cell(tmp_pos2);
        if(tmp_cell1.type == PATH or tmp_cell1.mask) return v[0];
        if(tmp_cell2.type == PATH or tmp_cell2.mask) return v[1];
        tmp_pos1 += v[0];
        tmp_pos2 += v[1];
      }

      if(tmp_pos1.i == target.i or tmp_pos1.j == target.j) return v[0];
      return v[1];
    }
    else if(v[0] != NONE) return v[0];
    else if(v[1] != NONE) return v[1];

    return NONE;
  }

  queue<Dir> path_to_cross_wall(const Pos& pos, const Pos& target){
    queue<Dir> d;
    return d;
  }

  Pos cell_city_path(int city_id){
    City city_cells = city(city_id);
    for(unsigned i = 0; i < city_cells.size(); ++i)
      for(unsigned j = 0; j < 4; ++j)
        if(cell(city_cells[i]+Dir(j)).type == PATH) return city_cells[i];
    
    return city_cells[0];
  }

  Dir movement(const Unit& u){
    Pos tmp_pos = u.pos;
    Dir tmp_dir = NONE;
    if(tmp_pos.i == 1 or tmp_pos.i == rows()-2 or tmp_pos.j == 1 or tmp_pos.j == cols()-2){
      int city_id = nearest_city(u);
      city_assigned[u.id].city = city_id;
      city_assigned[u.id].pos = cell_city_path(city_id);
      if(tmp_pos.i==1) tmp_dir =  BOTTOM;
      else if(tmp_pos.i==rows()-2) tmp_dir = TOP;
      else if(tmp_pos.j==1) tmp_dir = RIGHT;
      else if(tmp_pos.j==cols()-2) tmp_dir = LEFT;
    }
    else if(cell(tmp_pos).type == CITY){
      if(city_owner(cell(tmp_pos).city_id) == me()){
        int city_id = nearest_city(u);
        city_assigned[u.id].city = city_id;
        city_assigned[u.id].pos = cell_city_path(city_id);
      }

      for(unsigned i = 0; i < 4; ++i){
        CellType tmp_type = cell(tmp_pos+Dir(i)).type;
        if(tmp_type == GRASS) tmp_dir = Dir((i+1)%4);
        if(tmp_type == PATH){
          tmp_dir = Dir(i);
          break;
        }
      }
      if(tmp_dir == BOTTOM and (cell(tmp_pos + BOTTOM).type != CITY)) tmp_dir = RIGHT;
    }
    else if(cell(tmp_pos).type == PATH){
    }
    else if(cell(tmp_pos).type == GRASS){
    
    }
    if(tmp_dir == NONE) tmp_dir = get_move_for_target(u.pos, city_assigned[u.id].pos); 
    if(cell(u.pos + tmp_dir).unit_id != -1){
       for(unsigned i = 0; i < 4; ++i){
         if(cell(u.pos + Dir(i)).unit_id == -1) return Dir(i);
       }
    }

    return tmp_dir;
  }

  void controler(){
    VI U = my_units(me()); // Get the id's of my units.
    for (unsigned i = 0; i <  U.size(); ++i) {
      int id = U[i];
      if(not city_assigned[id].route.empty()){
        move(id, city_assigned[id].route.front());
        city_assigned[id].route.pop();
      }
      else{
        Dir tmp_move = check_mask(unit(id).pos);
        if(tmp_move == NONE) tmp_move = movement(unit(id));
        if(tmp_move != NONE){
          move(id, tmp_move);
        }
      }
    }

  }

  /*Pos bfs_find_pos(const Pos& start){
    
    vector <vector<char> > tmp_map(rows(), vector<char>(cols(), ' '));
    queue<Pos> q_pos;
    q_pos.push(start);
    bool found = false;
    while(not q_pos.empty()){
      Pos tmp_pos = q_pos.front();
      q_pos.pop();
      tmp_map[tmp_pos.i][tmp_pos.j] = 'x';
      Cell tmp_cell = cell(tmp_pos);
      if((tmp_cell.type == PATH and path_owner(tmp_cell.path_id) != me()) or
        (tmp_cell.type == CITY and city_owner(tmp_cell.city_id) != me())) return tmp_pos;
      for(int i = 0; i < 4; ++i){
        tmp_cell = cell(tmp_pos+Dir(i));
        if(tmp_cell.type != WALL and pos_ok(tmp_pos+Dir(i)) and tmp_map[(tmp_pos+Dir(i)).i][(tmp_pos+Dir(i)).j] != 'x'){
          q_pos.push(tmp_pos+Dir(i));
        }
      }
    }
    return start;
  }*/

  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
    if(round() == 0){
      city_assigned = vector<Unit_assignation>(60,Unit_assignation(-1,Pos(), queue<Dir>()));
    }
    controler();
  }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);


//------------------------------------------------------------//

 /* void n_units_assigned(){
    VI U = my_units(me());
    for (unsigned i = 0; i <  U.size(); ++i){
      n_units_assigned[city_assigned[U[i]].first]++; 
    }
  }
*/


  /*void find_nearest_path(int id, const Pos& target_pos, Pos inicial_pos){
    //Todo, si pongo random cell va mejor.
    bool found = false;
    //Loop to find the path from unit to city.
    while(not found){
      
      Dir tmp_move = get_move_for_target(inicial_pos, target_pos);

      if(tmp_move != NONE){
        inicial_pos +=  tmp_move;
        //v_units_paths[id].push(tmp_move);
      }
      else{found = true;}
      //else if(cell(pos).type == WALL){
        //path_to_cross_wall(u.pos, target_pos){

        //}
      }

    }
    */