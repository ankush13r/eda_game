#ifndef Structs_hh
#define Structs_hh


#include "Utils.hh"


/** \file
 * Contains the Dir enumeration, the Pos struct,
 * the CellType enumeration, the Cell struct,
 * the UnitType enumeration, the Unit struct,
 * and some useful little functions.
 */


/**
 * Enum to encode directions.
 */
enum Dir {
  BOTTOM,    // South
  RIGHT,     // East
  TOP,       // North
  LEFT,      // West
  NONE,      // No direction
  DIR_SIZE   // Artificial value, for iterating.
};


/**
 * Returns whether dir is a valid direction.
 */
inline bool dir_ok (Dir dir) {
  return dir >= BOTTOM and dir <= NONE;
}


/**
 * Simple struct to handle positions.
 */
struct Pos {

  int i, j;

  /**
   * Default constructor (0, 0).
   */
  Pos ();
  /**
   * Given constructor.
   */
  Pos (int i, int j);

  /**
   * Print operator.
   */
  friend ostream& operator<< (ostream& os, const Pos& p);

  /**
   * Comparison operator.
   */
  friend bool operator== (const Pos& a, const Pos& b);

  /**
   * Comparison operator.
   */
  friend bool operator!= (const Pos& a, const Pos& b);

  /**
   * Comparison operator, mostly needed for sorting.
   */
  friend bool operator< (const Pos& a, const Pos& b);

  /**
   * Increment operator: moves a position according to a direction.
   */
  Pos& operator+= (Dir d);

  /**
   * Addition operator: Returns a position by adding a direction.
   */
  Pos operator+ (Dir d) const;

  /**
   * Increment operator: moves a position according to another position.
   */
  Pos& operator+= (Pos p);

  /**
   * Addition operator: Returns a position by adding another position.
   */
  Pos operator+ (Pos p) const;

};


/**
 * Defines if a cell is empty or it has any special feature on it.
 */
enum CellType {
  WALL,
  GRASS,
  CITY,
  PATH,
  CELL_TYPE_SIZE  // Artificial value, for iterating.
};



/**
 * Describes a cell in the board, and its contents.
 */
struct Cell {

  CellType type; // The type of cell.
  int   unit_id; // The id of the unit on the cell if any, -1 otherwise.
  int   city_id; // If type == CITY, the id of the city, -1 otherwise.
  int   path_id; // If type == PATH, the id of the path, -1 otherwise.
  int     virus; // The amount of virus on that cell.
  bool     mask; // Whether the cell has a mask on it.

  /**
   * Default constructor (CELL_TYPE_SIZE, -1, -1, -1).
   */
  Cell ();

  /**
   * Given constructor.
   */
  Cell (CellType t, int unit, int city, int path, int virus, bool mask);

};


/**
 * Describes a unit on the board and its properties.
 */
struct Unit {
  int id;      // The unique id for this unit during the game.
  int player;  // The player that owns this unit.
  Pos pos;     // The position on the board.
  int health;  // The health of the unit.
  int damage;  // The damage taken due to the virus (0 if not infected)
  int turns;   // The number of turns it has been infected by the virus
  bool immune; // Whether the unit is immune to the virus
  bool mask;   // Whether the unit has a mask on

  /**
   * Default constructor (-1, -1, (0, 0), 0).
   */
  Unit ();

  /**
   * Given constructor.
   */
  Unit (int id, int pl, Pos p = Pos(0, 0), int health = 0, int v_damage = 0,
		int v_turns = 0, bool immune = false, bool mask = false);

};


//////// STUDENTS DO NOT NEED TO READ BELOW THIS LINE ////////

inline Pos::Pos () : i(0), j(0) { }
inline Pos::Pos (int i, int j) : i(i), j(j) { }

inline ostream& operator<< (ostream& os, const Pos& p) {
  return os << "(" << p.i << ", " << p.j << ")";
}

inline bool operator== (const Pos& a, const Pos& b) {
  return a.i == b.i and a.j == b.j;
}

inline bool operator!= (const Pos& a, const Pos& b) {
  return not (a == b);
}

inline bool operator< (const Pos& a, const Pos& b) {
  if (a.i != b.i) return a.i < b.i;
  return a.j < b.j;
}

inline Pos& Pos::operator+= (Dir d) {
  switch (d) {
  case BOTTOM: ++i;  break;
  case RIGHT:  ++j;  break;
  case TOP:    --i;  break;
  case LEFT:   --j;  break;
  case NONE:         break;
  default: ; // do nothing
  }
  return *this;
}

inline Pos Pos::operator+ (Dir d) const {
  Pos p = *this;
  p += d;
  return p;
}

inline Pos& Pos::operator+= (Pos p) {
    this->i += p.i;
    this->j += p.j;
    return *this;
  }

inline Pos Pos::operator+ (Pos p) const {
    Pos p2 = *this;
    p2 += p;
    return p2;
  }


const char  wALL = 'W';
const char gRASS = '.';
const char  cITY = ';';
const char  pATH = ',';

/**
 * Returns the CellType defined by the char c.
 */
inline CellType char2CellType (char c) {
  switch (c) {
  case  wALL: return WALL;
  case gRASS: return GRASS;
  case  cITY: return CITY;
  case  pATH: return PATH;
  default: 
    _my_assert(false, string(1, c) + " in grid definition.");
    return CELL_TYPE_SIZE;
  }
}


/**
 * Returns the char representing Celltype t.
 */
inline char CellType2char (CellType t) {
  switch (t) {
  case WALL  : return   wALL;
  case GRASS : return  gRASS;
  case CITY  : return   cITY;
  case PATH  : return   pATH;
  default    : return    'X';
  }
}


inline Cell::Cell () :
  type(CELL_TYPE_SIZE),
  unit_id(-1),
  city_id(-1),
  path_id(-1),
  virus(0),
  mask(false) { }

inline Cell::Cell (CellType t, int unit, int city, int path, int vr, bool m) :
  type(t),
  unit_id(unit),
  city_id(city),
  path_id(path),
  virus(vr),
  mask(m) { }


inline Unit::Unit () :
  id(-1), player(-1), pos(0, 0), health(0), damage(0), turns(0), immune(0), mask(0) { }

inline Unit::Unit (int id, int pl, Pos p, int h, int vd, int vt, bool im, bool mk) :
  id(id), player(pl), pos(p), health(h), damage(vd), turns(vt), immune(im), mask(mk) { }

#endif
