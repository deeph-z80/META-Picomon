#define WARPS_LENGTH 4
const byte warps[] = {
  // origin X, origin Y, destination X, destination Y
  40, 3, 55, 3,
  55, 3, 40, 3,
  12, 6, 51, 8,
  5, 52, 124, 2,
  124, 2, 5, 52,
  121, 21, 87, 48,
  87, 48, 121, 21,
  72, 3, 74, 33,
  74, 33, 72, 3
};

#define F_PICOBALL  1
#define F_GATE_1    2
#define F_GATE_2    3
#define F_GATE_3    4
#define F_CANDY     5
#define F_ROCK_1    6
#define F_ROCK_2    7
#define F_ROCK_3    8
#define F_ROCK_4    9
#define F_ROCK_5    10
#define F_ROCK_6    11
boolean flag[] = {
  false, false, false, false, false, false, false, false, false, false, false, false
};
const boolean default_flag[] = {
  false, false, false, false, false, false, false, false, false, false, false, false
};

#define S_LEAVE_HOUSE     0
#define S_PHONE           1
#define S_PICOBALL        2
#define S_PICOSTOP        3
#define S_GIRL            4
#define S_CORKS_ASSISTANT 5
#define S_GATE_1          6
#define S_GATE_2          7
#define S_GATE_3          8
#define S_APPLE_DUDE      9
#define S_CANDY           10
#define S_PROF_CORKS      11
#define S_ROCK_1          12
#define S_ROCK_2          13
#define S_ROCK_3          14
#define S_ROCK_4          15
#define S_ROCK_5          16
#define S_ROCK_6          17

#define WALK_SCRIPTS_LENGTH 3
const byte walk_triggered_scripts[] = {
  // X, Y, script id
  50, 9, S_LEAVE_HOUSE,
  51, 9, S_LEAVE_HOUSE
};

#define NO_FLAG  0
#define NO_TILE  0
#define TEXT    0
#define SCRIPT  1
#define ACTION_SCRIPTS_LENGTH 6
const byte action_triggered_scripts[] = {
  // X, Y, flag id (0 = no flag : constant event), tile id (0 = no tile), script type, script id
  36, 6, NO_FLAG, NO_TILE, TEXT, STR_CARTRIDGE,
  33, 3, NO_FLAG, NO_TILE, TEXT, STR_COMPUTER,
  48, 3, NO_FLAG, NO_TILE, TEXT, STR_BOOKS,
  49, 3, NO_FLAG, NO_TILE, TEXT, STR_BOOKS,
  51, 3, NO_FLAG, NO_TILE, TEXT, STR_TV,
  52, 3, NO_FLAG, NO_TILE, SCRIPT, S_PHONE,
  48, 6, F_PICOBALL, 35, SCRIPT, S_PICOBALL,
  10, 6, NO_FLAG, NO_TILE, TEXT, STR_HINT,
  6, 43, NO_FLAG, NO_TILE, TEXT, STR_PICOTOWN_SIGN,
  13, 48, NO_FLAG, NO_TILE, TEXT, STR_CORKS_SIGN,
  6, 52, NO_FLAG, NO_TILE, TEXT, STR_CAVE_SIGN,
  11, 38, NO_FLAG, NO_TILE, TEXT, STR_MAILMAN,
  13, 43, NO_FLAG, NO_TILE, TEXT, STR_GROLDO_MAP,
  14, 43, NO_FLAG, NO_TILE, TEXT, STR_GROLDO_OWNER,
  6, 39, NO_FLAG, NO_TILE, TEXT, STR_OBVIOUS_GUY,
  121, 49, NO_FLAG, NO_TILE, TEXT, STR_RUNNER,
  108, 56, NO_FLAG, NO_TILE, SCRIPT, S_GIRL,
  32, 34, NO_FLAG, NO_TILE, TEXT, STR_GIRL2,
  121, 19, NO_FLAG, NO_TILE, SCRIPT, S_CORKS_ASSISTANT,
  20, 56, F_GATE_1, 34, SCRIPT, S_GATE_1,
  23, 56, F_GATE_2, 34, SCRIPT, S_GATE_2,
  26, 59, F_GATE_3, 34, SCRIPT, S_GATE_3,
  26, 60, F_GATE_3, 34, SCRIPT, S_GATE_3,
  106, 42, NO_FLAG, NO_TILE, SCRIPT, S_APPLE_DUDE,
  104, 11, F_CANDY, 36, SCRIPT, S_CANDY,
  49, 56, NO_FLAG, NO_TILE, SCRIPT, S_PROF_CORKS,
  102, 5, F_ROCK_1, 37, SCRIPT, S_ROCK_1,
  102, 1, F_ROCK_2, 37, SCRIPT, S_ROCK_2,
  90, 13, F_ROCK_3, 37, SCRIPT, S_ROCK_3,
  90, 5, F_ROCK_4, 37, SCRIPT, S_ROCK_4,
  90, 6, F_ROCK_4, 37, SCRIPT, S_ROCK_4,
  90, 7, F_ROCK_4, 37, SCRIPT, S_ROCK_4,
  90, 8, F_ROCK_4, 37, SCRIPT, S_ROCK_4,
  84, 4, F_ROCK_5, 37, SCRIPT, S_ROCK_5,
  72, 8, F_ROCK_5, 37, SCRIPT, S_ROCK_6,
};
