#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME MrPanda1


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

  Dir get_move_for_target(const Pos& pos, const Pos& target, const Dir& tmp_dir){
    
    if(tmp_dir != NONE) return tmp_dir;
    if(pos.i < target.i and pos_ok(pos.i+1,pos.j) and cell(pos.i+1,pos.j).type != WALL)
      return BOTTOM;
    if(pos.i > target.i and pos_ok(pos.i-1,pos.j) and cell(pos.i-1,pos.j).type != WALL)
      return TOP;
    if(pos.j < target.j and pos_ok(pos.i,pos.j+1) and cell(pos.i,pos.j+1).type != WALL)
      return RIGHT;
    if(pos.j > target.j and pos_ok(pos.i,pos.j-1) and cell(pos.i,pos.j-1).type != WALL)
      return LEFT;
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
    return get_move_for_target(u.pos, city_assigned[u.id].pos, tmp_dir); 
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
        Dir tmp_move = movement(unit(id));
        if(tmp_move != NONE){
          move(id, tmp_move);
        }
      }
    }
    cerr << endl;
  }

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