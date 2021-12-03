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
  
  void collect_info(){
    cerr << "Debug: units-" << nb_units() << " paths"  << nb_paths() << " city-" << nb_cities();

  }



  void control_units(){
    VI U = my_units(me()); // Get the id's of my units.
    VI perm = random_permutation(U.size());
	  int n = U.size();
    for (int i = 0; i < n; ++i) {
      int id = U[perm[i]];
      Unit u = unit(id);

      move(id, Dir(1));
    }

  }


  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
      collect_info();
    control_units();
    if(round() == 0) 
      cerr << "\nDebug: " << "Done collectiong info." << endl;

  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
