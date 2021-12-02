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
  int half_cols = cols() >> 1;
  int half_rows = rows() >> 1;
  vector< queue<Dir> > v_units_paths = vector< queue<Dir> >(60);
  

  int pos_diff(const Pos& pos1, const Pos& pos2){
     return abs(pos1.i - pos2.i) + abs(pos1.j - pos2.j);
  }

  // Return position of a random cell of city near by u_pos.
  int nearest_city(const Pos& u_pos){
    //Run all cities
    int diff = pos_diff(city(0)[0],u_pos);
    int id = 0;

    for(int i= 1; i < nb_cities(); ++i){
    //random(0,city_cells.size()-1) Randomly picking a cell of the city.
      int tmp_diff = pos_diff(city(i)[0], u_pos);
      if(tmp_diff < diff){
        diff = tmp_diff;
        id = i;
      }
    }
    //cerr << "Debug: pos error: " << city(id)[0] << "..." << u_pos <<endl;

    return id;
  }

  Dir get_move_for_target(const Pos& pos, const Pos& target){
    if(pos.i < target.i and pos_ok(pos.i+1,pos.j) and cell(pos.i+1,pos.j).type != WALL)
      return RIGHT;
    if(pos.i > target.i and pos_ok(pos.i-1,pos.j) and cell(pos.i-1,pos.j).type != WALL)
      return LEFT;
    if(pos.j < target.j and pos_ok(pos.i,pos.j+1) and cell(pos.i,pos.j+1).type != WALL)
      return BOTTOM;
    if(pos.j > target.j and pos_ok(pos.i,pos.j-1) and cell(pos.i,pos.j-1).type != WALL)
      return TOP;
    return NONE;
  }

  queue<Dir> path_to_cross_wall(const Pos& pos, const Pos& target){
    queue<Dir> d;
    return d;
  }

  void find_nearest_path(const Unit& u, int city_id){
    queue<Dir> q_paths;
    //Todo, si pongo random cell va mejor.
    Pos target_pos = city(city_id)[city(city_id).size()-1]; //Getting first cell from city.
    Pos inicial_pos = u.pos;
    bool found = false;
    //Loop to find the path from unit to city.
    while(not found){
      
      Dir tmp_move = get_move_for_target(inicial_pos, target_pos);

      if(tmp_move != NONE){
        inicial_pos +=  tmp_move;
        q_paths.push(tmp_move);
      }
      else{found = true;}
      //else if(cell(pos).type == WALL){
        //path_to_cross_wall(u.pos, target_pos){

        //}
      }
      
      v_units_paths[u.id] = q_paths;

    }


  void control_unit(const Unit& u){
    if(u.pos.i == 1 or u.pos.i == rows()-2 or u.pos.j == 1 or u.pos.j == cols()-2){
      int city_id = nearest_city(u.pos);
      find_nearest_path(u, city_id);
      
    }
  }

  void controler(){
    VI U = my_units(me()); // Get the id's of my units.
    VI perm = random_permutation(U.size());
	  int n = U.size();
    for (int i = 0; i < n; ++i) {
      int id = U[i];
      Unit u = unit(id);
      control_unit(u);
      
      if(not v_units_paths[id].empty()){
        move(id, Dir(v_units_paths[id].front()));
        v_units_paths[id].pop();
      }
      else{
        //cerr << "Debug: Hola entro else: " << id <<"---" << v_units_paths[id].size() << endl;
        
      }
    }
  }


  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
    controler();
  }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
