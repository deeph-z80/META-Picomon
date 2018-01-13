#define T_AIR   0
#define T_FIRE  1
#define T_EARTH 2
#define T_WATER 3

#define SWOOTY      0
#define PIGGY       1
#define GROLDO      2
#define FEUXDINO    3
#define WATAWAMP    4
#define ROWPIM      5
#define BUZZCOR     6
#define PURBIRB     7
#define TINKERELLE  8
#define BOBERL      9
#define PLARADI     10
#define HATCELL     11
#define MOGMINE     12
#define CRUB        13
#define FLIPA       14
#define SNEG        15
#define PILO        16
#define LIMEGOO     17
#define DOGSHADE    18
#define PUKESUN     19
#define HUEVO       20
#define MISSING_NO  255

#define PICOMON_STATS_LENGTH 6
#define SPRITE_ID 0
#define ELEMENT   1
#define HP        2
#define ATTACK    3
#define DEFENSE   4
#define SPEED     5
const byte picomon_stats[] = {
  //sprite id, element id, hp, attack, defense, speed
  81, T_AIR, 65, 70, 75, 45,    // swooty
  80, T_EARTH, 25, 55, 50, 20,  // piggy
  82, T_EARTH, 60, 55, 50, 20,  // groldo
  83, T_FIRE, 35, 55, 50, 20,   // feuxdino
  84, T_AIR, 30, 40, 70, 20,    // watawamp
  85, T_EARTH, 45, 5, 60, 20,   // rowpim
  86, T_AIR, 45, 55, 50, 20,    // buzzcor
  87, T_AIR, 45, 55, 50, 20,    // purbirb
  88, T_EARTH, 45, 55, 50, 20,  // tinkerelle
  89, T_EARTH, 100, 5, 15, 20,  // boberl
  90, T_EARTH, 75, 55, 50, 20,  // plaradi
  91, T_EARTH, 95, 55, 50, 20,  // hatcell
  92, T_FIRE, 110, 25, 50, 20,  // mogmine
  93, T_WATER, 62, 55, 50, 20,  // crub
  94, T_WATER, 88, 55, 50, 20,  // flipa
  95, T_EARTH, 25, 55, 50, 20,  // sneg
  96, T_EARTH, 15, 25, 80, 20,  // pilo
  97, T_WATER, 78, 22, 25, 40,  // limegoo
  98, T_AIR, 45, 55, 50, 60,    // dogshade
  99, T_FIRE, 25, 95, 5, 100,   // pukesun
  100, T_AIR, 99, 25, 55, 5     // huevo
};

#define MO_SCRATCH  0
#define MO_LEER     1
#define MO_TACKLE   2
#define MO_PUNCH    3
#define MO_COUNTER  4
#define MO_BUBBLE   5
#define MO_BASH     6
#define MO_RAGE     7
#define MO_BURN     8
#define MO_TAKEDOWN 9
#define MO_FIRST_MAX  MO_TACKLE
#define MO_SECOND_MAX MO_BUBBLE
#define MO_THIRD_MAX  MO_RAGE
#define MO_FOURTH_MAX MO_TAKEDOWN
const byte moves_base[] = {
  15, 0, 20, 30, 35, 40, 50, 55, 65, 70
};

#define PLAYER_PICOMONS_LENGTH 9
#define ID        0
#define LVL       1
#define CUR_HP    2
#define XP        3
#define DOUBLE_XP 4
#define MOVE_1    5
#define MOVE_2    6
#define MOVE_3    7
#define MOVE_4    8
unsigned short player_picomons[] = {
  // picomon id, lvl, current hp, xp, double xp, move 1 id, move 2 id, move 3 id, move 4 id
  MISSING_NO, 0, 0, 0, 0, NO_MOVE, NO_MOVE, NO_MOVE, NO_MOVE,
  MISSING_NO, 0, 0, 0, 0, NO_MOVE, NO_MOVE, NO_MOVE, NO_MOVE,
  MISSING_NO, 0, 0, 0, 0, NO_MOVE, NO_MOVE, NO_MOVE, NO_MOVE,
  MISSING_NO, 0, 0, 0, 0, NO_MOVE, NO_MOVE, NO_MOVE, NO_MOVE
};

const unsigned short default_player_picomons[] = {
  MISSING_NO, 0, 0, 0, 0, NO_MOVE, NO_MOVE, NO_MOVE, NO_MOVE,
  MISSING_NO, 0, 0, 0, 0, NO_MOVE, NO_MOVE, NO_MOVE, NO_MOVE,
  MISSING_NO, 0, 0, 0, 0, NO_MOVE, NO_MOVE, NO_MOVE, NO_MOVE,
  MISSING_NO, 0, 0, 0, 0, NO_MOVE, NO_MOVE, NO_MOVE, NO_MOVE
};

#define PLAYER_ITEMS_LENGTH 10
#define I_POTION          0
#define I_PICOBALL        1
#define I_SMOKEBALL       2
#define I_RESURRECT       3
#define I_CANDY           4
#define I_GARDEN_KEY      5
#define I_SWIM_SUIT       6
#define I_APPLE           7
#define I_HAMMER          8
//#define I_RUNNING_SHOES   9
#define ITEM_HANDLED      254
#define NO_ITEM           255
#define NO_DROPPABLE_ITEM I_GARDEN_KEY
byte player_items[] = {
  I_POTION, I_PICOBALL, I_PICOBALL, I_SMOKEBALL, I_RESURRECT, I_CANDY, I_SWIM_SUIT, NO_ITEM, NO_ITEM, NO_ITEM
};
const byte default_player_items[] = {
  NO_ITEM, NO_ITEM, NO_ITEM, NO_ITEM, NO_ITEM, NO_ITEM, NO_ITEM, NO_ITEM, NO_ITEM, NO_ITEM
};
