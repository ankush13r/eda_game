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
  void control_units(){
    VI U = my_units(me()); // Get the id's of my units.
    VI perm = random_permutation(U.size());
	  int n = U.size();
    int commands = 0;
    while(commands < 100){
      for (int i = 0; i < n and commands < 100; ++i) {
        int id = U[perm[i]];
        Unit u = unit(id);

        move(id, Dir(1));
        ++commands;
      }
    }
  }




  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
    control_units();
  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
