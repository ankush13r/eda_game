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

  struct Route_data{
    Pos pos;
    Dir dir;
    Route_data(Pos pos = Pos(0,0), Dir dir = NONE): pos(pos), dir(dir){}
  };

  struct PQ_data{
    Pos pos;
    queue<Route_data> route;
    int cost;
    PQ_data(Pos pos = Pos(0,0), queue<Route_data> route = queue<Route_data>(), int cost = 0):
    pos(pos), route(route), cost(cost){}
  
  };

  struct Unit_data{
    Pos pos;
    queue<Route_data> route;
    int city;
    int path;
    Unit_data():
    pos(Pos(0,0)), route(queue<Route_data>()), city(-1), path(-1){}
  };



  struct CustomCompare
  {
      bool operator()(const  PQ_data& x, const PQ_data& y){
          return x.cost > y.cost;
      }
  };

  vector<Unit_data> v_units;
  vector<int> units_to_paths;
  vector<int> units_to_cities;

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


  int pos_cost(int u_id, const Pos& p){
    Cell c = cell(p);
    Unit u = unit(u_id);
    if(not u.mask and not u.immune){
      if(c.mask) return 1;
      if(c.virus){
        if(c.type == PATH or c.type == CITY) return 12;
        else return 15;
      }
    }
    if(c.type == PATH or c.type == CITY){
      if(c.unit_id != -1 and unit(c.unit_id).player == me()) return 5;
      return 3;
    }
    return 10;
  }

  bool ok_city(int id){
    return city_owner(id) != me();
  }

  bool ok_path(int id){
    return path_owner(id) != me();
  }

  bool bfs_comp(int u_id, const Pos& start, const Pos& end){
    if(start == end ) return false;
    Cell c = cell(end) ;
    // if(c.mask and unit(u_id).health < 25 and not unit(u_id).mask and not unit(u_id).immune) return true;
    if(nb_cities() > 12 and c.virus and unit(u_id).damage == 0 and not unit(u_id).mask and not unit(u_id).immune) return false;

    return (c.type == PATH and ok_path(c.path_id)) or
            (c.type == CITY and ok_city(c.city_id));
  }


  
  //Bfs () to find nearest path. 
  /*pair<Pos, queue<Route_data>> bfs_find_pos(int u_id, const Pos& start){
    vector <vector<bool> > visited(rows(), vector<bool>(cols(), false));

    queue<Route_data> tmp_q = queue<Route_data>();
    queue<PQ_data> Q;
    Q.push(PQ_data(start,tmp_q));
    Pos tmp_pos;

    while(not Q.empty()){
      tmp_pos = Q.front().pos;
      tmp_q = Q.front().route;
      visited[tmp_pos.i][tmp_pos.j] = true;
      Q.pop();

      if(bfs_comp(u_id, start, tmp_pos)) break;
      for(int i = 0; i < 4; ++i){
        Cell tmp_cell = cell(tmp_pos+Dir(i));
        if(tmp_cell.type != WALL and pos_ok(tmp_pos+Dir(i)) and not visited[(tmp_pos+Dir(i)).i][(tmp_pos+Dir(i)).j]){
          queue<Route_data> q2 = tmp_q;
          q2.push(Route_data(tmp_pos, Dir(i)));
          Q.push(PQ_data(tmp_pos+Dir(i), q2));
          visited[(tmp_pos+Dir(i)).i][(tmp_pos+Dir(i)).j] = true;
        }
      }
    }
    return make_pair(tmp_pos, tmp_q);
  }
  */
  

  
  //BFS with priority_queue.
  //It's ordered by cout of the cost from staring position to current position.
  pair<Pos, queue<Route_data>> bfs_find_pos(int u_id, const Pos& start){
    vector <vector<bool> > visited(rows(), vector<bool>(cols(), false));
    queue<Route_data> tmp_q = queue<Route_data>();
    priority_queue<PQ_data, vector<PQ_data>, CustomCompare > PQ;
    PQ.push(PQ_data(start,tmp_q, 0));
    Pos tmp_pos;
    while(not PQ.empty()){
      tmp_pos = PQ.top().pos;
      tmp_q = PQ.top().route;
      int tmp_cost = PQ.top().cost;
      visited[tmp_pos.i][tmp_pos.j] = true;
      PQ.pop();

      if(bfs_comp(u_id, start, tmp_pos)) break;
      for(int i = 0; i < 4; ++i){
        Cell tmp_cell = cell(tmp_pos+Dir(i));
        if(tmp_cell.type != WALL and pos_ok(tmp_pos+Dir(i)) and not visited[(tmp_pos+Dir(i)).i][(tmp_pos+Dir(i)).j]){
          queue<Route_data> q2 = tmp_q;
          q2.push(Route_data(tmp_pos, Dir(i)));
          int t_cost = tmp_cost + pos_cost(u_id, tmp_pos+Dir(i));
          PQ.push(PQ_data(tmp_pos+Dir(i), q2, t_cost));
          visited[(tmp_pos+Dir(i)).i][(tmp_pos+Dir(i)).j] = true;
        }
      }
    }

    return make_pair(tmp_pos, tmp_q);
  }

  void assign_pos(const Unit& u){
    pair<Pos, queue<Route_data>> p = bfs_find_pos(u.id, u.pos);
    v_units[u.id].pos =  bfs_find_pos(u.id, u.pos).first;
    v_units[u.id].route = bfs_find_pos(u.id, u.pos).second;
    v_units[u.id].city = cell(v_units[u.id].pos).city_id;
    v_units[u.id].path = cell(v_units[u.id].pos).path_id;


  }

  Dir movement(const Unit& u){
    Dir tmp_move = find_enemy(u.pos);
    if(tmp_move != NONE) return tmp_move;
    if(not u.mask and not u.immune){
      tmp_move = find_mask(u.pos);
      if(tmp_move != NONE) return tmp_move;
    }
  
    Cell c = cell(u.pos);
    if(c.type == CITY and city(c.city_id).size() > 4){
        City cells = city(c.city_id);
        int n_me = 0;
        int n_enemy = 0;    
        for(unsigned i = 0; i < cells.size(); ++i){
          int u_id = cell(cells[i]).unit_id;
          if(u_id != -1 and u_id != u.id){
            if(unit(u_id).player == me() and v_units[u_id].city == c.city_id ) ++n_me;
            else if(unit(u_id).player != me()) ++n_enemy;
          }
        }
      if((n_enemy == 0  and n_me == 0) or (n_enemy > 0 and n_me <= 2)){
        tmp_move = NONE;
        for(unsigned i = 0; i < 4; ++i){
          CellType tmp_type = cell(u.pos+Dir(i)).type;
          if(tmp_type == GRASS) tmp_move = Dir((i+1)%4);
          if(tmp_type == PATH){
            tmp_move = Dir(i);
            break;
          }
        }
        if(tmp_move == BOTTOM and (cell(u.pos + BOTTOM).type != CITY)) tmp_move = RIGHT;
        return tmp_move;
      }
    }
  
    if(v_units[u.id].pos == Pos(0,0) or
    v_units[u.id].route.empty() or
    v_units[u.id].route.front().pos != u.pos
    or
    (cell(v_units[u.id].pos).city_id != -1 and city_owner(cell(v_units[u.id].pos).city_id) == me() ) 
    or
    (cell(v_units[u.id].pos).path_id != -1 and path_owner(cell(v_units[u.id].pos).path_id) == me())   
    ){
      assign_pos(u);
    }
    tmp_move = v_units[u.id].route.front().dir;
    v_units[u.id].route.pop();

    return tmp_move;
  }

  void control_units(){
    VI U = my_units(me()); // Get the id's of my units.
    for (unsigned i = 0; i <  U.size(); ++i) {
      Unit u = unit(U[i]);
      Dir tmp_move = NONE;
      tmp_move = movement(u);
      if(cell(u.pos + tmp_move).unit_id != -1 and unit(cell(u.pos + tmp_move).unit_id).player == me()){
        for(int i = 0; i < 4; ++i){
          if(cell(u.pos + Dir(i)).unit_id == -1 or unit(cell(u.pos + Dir(i)).unit_id).player != me()) {
            if(cell(u.pos + Dir(i)).type == CITY or cell(u.pos + Dir(i)).type == PATH){
              tmp_move = Dir(i);
              break;
            };
          }
        }
      }
      move(U[i], tmp_move);
    }
  }

  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
    if(round() == 0){
      v_units = vector<Unit_data>(300, Unit_data());
    }
    control_units();
  }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
