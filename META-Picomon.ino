#include <Gamebuino-Meta.h>
#include "GSFX.h"
GSFX gsfx;

#define MAP_WIDTH   128
#define MAP_HEIGHT  64

#define PLAYER_X          36
#define PLAYER_Y          7
#define PLAYER_DIRECTION  3

#define TILE_WIDTH              8
#define TILE_HEIGHT             8
#define TILE_SIZE               8
#define TILES_PASSABLE_END      16
#define TILE_GRASS              3
#define TILE_CAVE               7
#define TILE_WATER_START        17
#define TILE_WATER_END          26
#define TILE_CLIFF              28
#define TILE_APPLE_TREE_TOP     29
#define TILE_APPLE_TREE_BOTTOM  30
#define TILE_PICOSTOP_TOP       38
#define TILE_PICOSTOP_BOTTOM    39
#define TILE_PICOMONSTERS       80
#define TILE_ELEMENT            101

// delays in ms
#define WALK_DELAY              10
#define BUTTON_DELAY            20
#define ARROW_DELAY             5
#define TEXT_DELAY              40
#define TEXT_BLINK_DELAY        70
#define TEXT_BUTTON_DELAY       1
#define FADE_DELAY              70
#define BATTLE_TEXT_BLINK_DELAY 40
#define BATTLE_DAMAGES_DELAY    30
#define DAMAGE_BLINK_DELAY      20

#define DAMAGE_BLINK_LENGHT     8

#define BATTLE_FREQUENCY  20 // percent

#define CORKS_PRIZE_THRESHOLD 50

#define NO_RESET    true
#define WITH_RESET  false

#define STATS_MENU_MAP    0
#define STATS_MENU_BATTLE 1
#define STATS_MENU_ITEM   2

Color pico_8_palette[] = {
  (Color)0x0000,  // BLACK
  (Color)0x194a,  // DARK-BLUE
  (Color)0x792a,  // DARK-PURPLE
  (Color)0x042a,  // DARK-GREEN
  (Color)0xaa86,  // BROWN
  (Color)0x5aa9,  // DARK-GRAY
  (Color)0xc618,  // LIGHT-GRAY
  (Color)0xff9d,  // WHITE
  (Color)0xf809,  // RED
  (Color)0xfd00,  // ORANGE
  (Color)0xff64,  // YELLOW
  (Color)0x0726,  // GREEN
  (Color)0x2d7f,  // BLUE
  (Color)0x83b3,  // INDIGO
  (Color)0xfbb5,  // PINK
  (Color)0xfe75   // PEACH
};

Color pico_8_palette_dark[] = {
  (Color)0x0000,  // BLACK
  (Color)0x0004,  // DARK-BLUE
  (Color)0x4804,  // DARK-PURPLE
  (Color)0x02a3,  // DARK-GREEN
  (Color)0x7900,  // BROWN
  (Color)0x2923,  // DARK-GRAY
  (Color)0x9492,  // LIGHT-GRAY
  (Color)0xcdf6,  // WHITE
  (Color)0xc803,  // RED
  (Color)0xCB80,  // ORANGE
  (Color)0xcdc0,  // YELLOW
  (Color)0x0580,  // GREEN
  (Color)0x03d9,  // BLUE
  (Color)0x522d,  // INDIGO
  (Color)0xCa2e,  // PINK
  (Color)0xcccf   // PEACH
};

Color pico_8_palette_darker[] = {
  (Color)0x0000,  // BLACK
  (Color)0x0000,  // DARK-BLUE
  (Color)0x1800,  // DARK-PURPLE
  (Color)0x0000,  // DARK-GREEN
  (Color)0x4000,  // BROWN
  (Color)0x0000,  // DARK-GRAY
  (Color)0x5aec,  // LIGHT-GRAY
  (Color)0x9C70,  // WHITE
  (Color)0x9800,  // RED
  (Color)0x99e0,  // ORANGE
  (Color)0x9c40,  // YELLOW
  (Color)0x0400,  // GREEN
  (Color)0x0253,  // BLUE
  (Color)0x1887,  // INDIGO
  (Color)0x9888,  // PINK
  (Color)0x9b48   // PEACH
};

// debug
byte player_x;
byte player_y;
byte player_direction;
byte player_animation = 0;
short player_kills;
int camera_x;
int camera_y;
byte tmp;
int music = -1;

#include "maps.h"
#include "sprites.h"
#include "strings.h"
#include "stats.h"
#include "scripts.h"
#include "graphics.h"

const byte zone00[] = {
  SWOOTY, PIGGY, GROLDO, FEUXDINO, WATAWAMP, ROWPIM, BUZZCOR, PURBIRB, TINKERELLE, BOBERL,
  PLARADI, HATCELL, MOGMINE, CRUB, FLIPA, SNEG, PILO, LIMEGOO, DOGSHADE, PUKESUN, HUEVO
};
const byte zone01[] = { ROWPIM, PIGGY, PURBIRB };
const byte zone02[] = { GROLDO, FEUXDINO, TINKERELLE };
const byte zone03[] = { LIMEGOO };
const byte zone04[] = { MOGMINE, HATCELL, PLARADI };
const byte zone05[] = { DOGSHADE, BUZZCOR, SNEG };
const byte zone06[] = { WATAWAMP, PILO };
const byte zone07[] = { FLIPA, CRUB };

const byte map_menu[] = {
  // x, y, w, h, option id
  45, 0, 35, 27,
  M_MONSTER, M_ITEM, M_SAVE, M_RESET
};

const byte first_release_menu[] = {
  // x, y, w, h, option id
  22, 24, 35, 15,
  M_FIRST, M_RELEASE
};

const byte use_drop_menu[] = {
  // x, y, w, h, option id
  28, 24, 23, 15,
  M_USE, M_DROP
};

const byte no_yes_menu[] = {
  // x, y, w, h, option id
  14, 29, 15, 11,
  M_NO, M_YES
};

const SaveDefault savefileDefaults[] = {
  {0, SAVETYPE_INT, PLAYER_X, 0},
  {1, SAVETYPE_INT, PLAYER_Y, 0},
  {2, SAVETYPE_INT, PLAYER_DIRECTION, 0},
  {3, SAVETYPE_BLOB, {.ptr = default_flag}, sizeof(default_flag)},
  {4, SAVETYPE_BLOB, {.ptr = default_player_picomons}, sizeof(default_player_picomons)},
  {5, SAVETYPE_BLOB, {.ptr = default_player_items}, sizeof(default_player_items)}
};

void setup() {
  gb.begin();
  gb.sound.playOK();
  gsfx.init();
  gb.setFrameRate(40);
  gb.save.config(savefileDefaults);
  gb.display.colorIndex = pico_8_palette;
  player_sprite_set.setTransparentColor((Color)0xff9d);
}

void loop() {
reset:
  load_game();
  player_kills = 0;
  title_screen();
  fade_in(FADE_DELAY);
  choose_music();
  if (player_x == PLAYER_X && player_y == PLAYER_Y && player_direction == PLAYER_DIRECTION) {
    gb.display.fill(BLACK);
    fade_out(FADE_DELAY);
    text_frame();
    text(STR_INTRO, NO_RESET);
    fade_in(FADE_DELAY);
  }
  update_camera();
  update_map();
  fade_out_map(FADE_DELAY);
  boolean object_collision = false;
  while (true) {
    if (gb.update()) {
      int x_direction = -gb.buttons.repeat(BUTTON_LEFT, 1) * (player_x > 0) + gb.buttons.repeat(BUTTON_RIGHT, 1) * (player_x < MAP_WIDTH - 1);
      int y_direction = -gb.buttons.repeat(BUTTON_UP, 1) * (player_y > 0) + gb.buttons.repeat(BUTTON_DOWN, 1) * (player_y < MAP_HEIGHT - 1);
      if (!x_direction != !y_direction) {
        player_direction = (1 + x_direction) * (x_direction != 0);
        player_direction = (2 + y_direction) * (y_direction != 0 || player_direction == 0);

        if (map_[(player_y + y_direction)*MAP_WIDTH + player_x + x_direction] <= TILES_PASSABLE_END || (search_item(I_SWIM_SUIT) != NO_ITEM && map_[(player_y + y_direction)*MAP_WIDTH + player_x + x_direction] <= TILE_WATER_END)) {
          // check for objects to collide
          object_collision = false;
          for (byte i = 0; i < sizeof(action_triggered_scripts); i += ACTION_SCRIPTS_LENGTH) {
            if (action_triggered_scripts[i + 3] > 0 &&
                flag[action_triggered_scripts[i + 2]] == 0 &&
                player_x + x_direction == action_triggered_scripts[i] &&
                player_y + y_direction == action_triggered_scripts[i + 1]) {
              object_collision = true;
              break;
            }
          }
          if (!object_collision) {
            for (byte i = 1; i <= TILE_SIZE; i++) {
              player_animation += i % 2;
              player_animation *= player_animation < 2 && i < TILE_SIZE;
              camera_x = (player_x * TILE_WIDTH - gb.display.width() / 2 + 4 + i * x_direction);
              camera_x = camera_x * (camera_x > 0) + (MAP_WIDTH * TILE_WIDTH - gb.display.width() - camera_x) * (camera_x > MAP_WIDTH * TILE_WIDTH - gb.display.width());
              camera_y = player_y * TILE_HEIGHT - gb.display.height() / 2 + 4 + i * y_direction;
              camera_y = camera_y * (camera_y > 0) + (MAP_HEIGHT * TILE_HEIGHT - gb.display.height() - camera_y) * (camera_y > MAP_HEIGHT * TILE_HEIGHT - gb.display.height());
              draw_map(camera_x, camera_y);
              draw_player(player_x * TILE_WIDTH - camera_x + i * x_direction, player_y * TILE_WIDTH - camera_y + i * y_direction);
              while (!gb.update());
              //delay(WALK_DELAY);
            }
            player_x += x_direction;
            player_y += y_direction;

            // check for warps
            for (byte i = 0; i < sizeof(warps); i += WARPS_LENGTH) {
              if (player_x == warps[i] && player_y == warps[i + 1]) {
                gb.sound.play("sounds/warp.wav");
                fade_in_map(FADE_DELAY);
                if (!(player_x > 32 && player_x < 56 && player_y < 10)) {
                  player_x = warps[i + 2];
                  player_y = warps[i + 3];
                  choose_music();
                }
                player_x = warps[i + 2];
                player_y = warps[i + 3];
                update_camera();
                fade_out_map(FADE_DELAY);
                break;
              }
            }

            // check for walk-triggered events
            for (byte i = 0; i < sizeof(walk_triggered_scripts); i += WALK_SCRIPTS_LENGTH) {
              if (player_x == walk_triggered_scripts[i] & player_y == walk_triggered_scripts[i + 1]) {
                script(walk_triggered_scripts[i + 2]);
                break;
              }
            }

            // check for battles
            if (rand() % 100 <= BATTLE_FREQUENCY) {
              if (map_[player_y * MAP_WIDTH + player_x] == TILE_GRASS) {
                if (player_x < 19 && player_y < 31) {
                  battle(zone01, sizeof(zone01));
                } else if (player_x < 35 && player_y < 56) {
                  battle(zone02, sizeof(zone02));
                } else if (player_x < 35) {
                  battle(zone03, sizeof(zone03));
                } else if (player_x > 108 && player_y > 38) {
                  battle(zone04, sizeof(zone04));
                } else if (player_x > 60 && player_y > 40) {
                  battle(zone05, sizeof(zone05));
                } else {
                  battle(zone00, sizeof(zone00));
                }
                choose_music();
              } else if (map_[player_y * MAP_WIDTH + player_x] == TILE_CAVE) {
                battle(zone06, sizeof(zone06));
                choose_music();
              } else if (map_[player_y * MAP_WIDTH + player_x] >= TILE_WATER_START && map_[player_y * MAP_WIDTH + player_x] <= TILE_WATER_END) {
                battle(zone07, sizeof(zone07));
                choose_music();
              }
            }
          }
          x_direction = y_direction = 0;
        }

        // check for cliffs
        if (map_[(player_y + y_direction)*MAP_WIDTH + player_x] == TILE_CLIFF && player_direction == 3) {
          delay(WALK_DELAY * 2);
          for (byte i = 1; i <= TILE_SIZE * 2; i++) {
            camera_y = player_y * TILE_HEIGHT - gb.display.height() / 2 + 4 + i;
            camera_y = camera_y * (camera_y > 0) + (MAP_HEIGHT * TILE_HEIGHT - gb.display.height() - camera_y) * (camera_y > MAP_HEIGHT * TILE_HEIGHT - gb.display.height());
            draw_map(camera_x, camera_y);
            draw_player(player_x * TILE_WIDTH - camera_x, player_y * TILE_WIDTH - camera_y + i);
            while (!gb.update());
            //delay(WALK_DELAY/2);
          }
          player_y += 2;
        }

        update_map();
      } else if (gb.buttons.repeat(BUTTON_A, BUTTON_DELAY)) {
        for (byte i = 0; i < sizeof(action_triggered_scripts); i += ACTION_SCRIPTS_LENGTH) {
          if (flag[action_triggered_scripts[i + 2]] == 0 &&
              player_x + (player_direction == 0) - (player_direction == 2) == action_triggered_scripts[i] &&
              player_y + (player_direction == 3) - (player_direction == 1) == action_triggered_scripts[i + 1]) {
            switch (action_triggered_scripts[i + 4]) {
              case TEXT :
                text((char*) strings[action_triggered_scripts[i + 5]], WITH_RESET);
                break;
              case SCRIPT :
                script(action_triggered_scripts[i + 5]);
                break;
            }
          }
        }

        // check for picostops & apple trees
        tmp = map_[(player_y + (player_direction == 3) - (player_direction == 1)) * MAP_WIDTH + player_x + (player_direction == 0) - (player_direction == 2)];
        if (tmp == TILE_PICOSTOP_TOP || tmp == TILE_PICOSTOP_BOTTOM) {
          script(S_PICOSTOP);
        } else if (tmp == TILE_APPLE_TREE_TOP || tmp == TILE_APPLE_TREE_BOTTOM) {
          if (search_item(I_APPLE) == NO_ITEM && search_item(I_SWIM_SUIT) == NO_ITEM) {
            text(STR_PICK_APPLE, WITH_RESET);
            player_items[search_item(NO_ITEM)] = I_APPLE;
          } else {
            text(STR_LEAVE_APPLE, WITH_RESET);
          }
        }

      } else if (gb.buttons.repeat(BUTTON_MENU, BUTTON_DELAY)) {
        gb.sound.playOK();
        tmp = 0;
map_menu_loop:
        while (!gb.update());
        update_map();
        switch (menu(map_menu, sizeof(map_menu), true, tmp)) {
          case M_MONSTER:
            stats_menu(STATS_MENU_MAP);
            tmp = 0;
            goto map_menu_loop;
          case M_ITEM:
            item_menu(false);
            tmp = 1;
            goto map_menu_loop;
          case M_SAVE:
            save_game();
            text(STR_GAME_SAVED, WITH_RESET);
            break;
          case M_RESET:
            text(STR_RESET, WITH_RESET);
            gb.display.setColor(WHITE);
            gb.display.fillRect(12, 21, 55, 21);
            gb.display.setColor(BLACK);
            gb.display.drawRect(12, 21, 55, 21);
            gb.display.setCursor(14, 23);
            text(STR_SURE, NO_RESET);
            while (!gb.update());
            if (menu(no_yes_menu, sizeof(no_yes_menu), false, 0) == M_YES) {
              gb.sound.stop(music);
              reset_game();
              //setup();
              goto reset;
            }
            break;
        }
        update_map();
      }

    }
  }
}

void title_screen() {
  gb.sound.stop(music);
  music = gb.sound.play("musics/battle.wav", true);
  byte picomon_offset = rand() % (21 - 6);
  long loop = 0;
  do {
    if (gb.update()) {
      loop++;
      gb.display.fill(WHITE);
      for (byte i = 0; i < 7 ; i++) {
        tile_set.setFrame(TILE_PICOMONSTERS + picomon_offset + i);
        gb.display.drawImage(gb.display.width() / 2 + cos(radians(i * (360 / 7) + loop * 2)) * (2.5 * TILE_SIZE) - TILE_WIDTH / 2, gb.display.height() / 2 + sin(radians(i * (360 / 7) + loop * 2)) * (2.5 * TILE_SIZE) - TILE_HEIGHT / 2, tile_set);
      }
      for (byte i = 0; i < 4 ; i++) {
        tile_set.setFrame(TILE_ELEMENT + i);
        gb.display.drawImage(gb.display.width() / 2 - cos(radians(i * (360 / 4) + loop * 2))*TILE_SIZE - TILE_WIDTH / 2, gb.display.height() / 2 + sin(radians(i * (360 / 4) + loop * 2))*TILE_SIZE - TILE_HEIGHT / 2, tile_set);
      }
      gb.display.setColor(BLACK);
      gb.display.setCursor(14, 0);
      gb.display.print(STR_TITLE);
      gb.display.setCursor(6 - 2 * (loop / 100 % 2 > 0), 59);
      if (loop / 100 % 2 > 0) {
        gb.display.print(STR_AUTHOR_2);
      } else {
        gb.display.print(STR_AUTHOR_1);
      }
    }
  } while (!gb.buttons.repeat(BUTTON_A, BUTTON_DELAY));
}

void load_game() {
  player_x = gb.save.get(0);
  player_y = gb.save.get(1);
  player_direction = gb.save.get(2);
  gb.save.get(3, flag);
  gb.save.get(4, player_picomons);
  gb.save.get(5, player_items);
}

void save_game() {
  gb.save.set(0, player_x);
  gb.save.set(1, player_y);
  gb.save.set(2, player_direction);
  gb.save.set(3, flag);
  gb.save.set(4, player_picomons);
  gb.save.set(5, player_items);
}

void reset_game() {
  gb.save.del(0);
  gb.save.del(1);
  gb.save.del(2);
  gb.save.del(3);
  gb.save.del(4);
  gb.save.del(5);
}

void choose_music() {
  gb.sound.stop(music);
  if (player_x > 32 && player_x < 56 && player_y < 10) {
    music = gb.sound.play("musics/inside.wav", true);
  } else if (player_x > 68 && player_y < 22) {
    music = gb.sound.play("musics/cave.wav", true);
  } else {
    music = gb.sound.play("musics/outside.wav", true);
  }
}

byte text(const char string[], boolean no_reset) {
  if (!no_reset) text_frame();
  for (byte i = 0; i < strlen(string); i++) {
    char c = (char) string[i];
    switch ((byte) c) {
      case 10:
        gb.display.setCursor(2, gb.display.getCursorY() + 6);
        break;
      case 112:
        text_pause();
        text_frame();
        break;
      default:
        gb.display.print(c);
        if (c > 64 && c < 91) gsfx.play(GSFX::FX{GSFX::WaveType::SQUARE, 10000, 0, 3000 + 500 * (c - 64), 0, 2000});
        while (!gb.update());
        break;
    }
  }
  if (!no_reset) {
    text_pause();
    update_map();
  }
}

void text_frame() {
  gb.display.setColor(WHITE);
  gb.display.fillRect(0, 37, 80, 27);
  gb.display.setColor(BLACK);
  gb.display.drawRect(0, 37, 80, 27);
  gb.display.setCursor(2, 39);
}

void text_pause() {
  byte i = 0, quit = 0;
  while (!quit) {
    if (gb.update()) {
      i++;
      if (i % 2) {
        gb.display.setColor(WHITE);
      }
      gb.display.drawChar(gb.display.getCursorX(), gb.display.getCursorY(), 21, 1);
      gb.display.setColor(BLACK);
      while (!gb.update());
      delay(TEXT_BLINK_DELAY);
      if (gb.buttons.repeat(BUTTON_A, TEXT_BUTTON_DELAY)) quit = 1;
    }
  }
  gb.sound.playOK();
}

word menu(const byte data[], byte size, boolean frame, byte cursor_pos) {
  if (frame) {
    gb.display.setColor(WHITE);
    gb.display.fillRect((int) data[0], (int) data[1], (int) data[2], (int) data[3]);
    gb.display.setColor(BLACK);
    gb.display.drawRect((int) data[0], (int) data[1], (int) data[2], (int) data[3]);
  }
  for (byte i = 4; i < size; i++) {
    gb.display.setCursor(data[0] + 2 * frame + 4, data[1] + 2 * frame + 6 * (i - 4));
    gb.display.print(menu_strings[data[i]]);
  }
  byte option = handle_vertical_cursor(size - 4, data[0] + 2 * frame, data[1] + 2 * frame, cursor_pos);
  if (option == M_CANCEL) return option;
  return data[4 + option];
}

byte handle_vertical_cursor(byte options_number, byte x, byte y, byte cursor_pos) {
  byte prev_pos = (cursor_pos == 0);
  do {
    if (gb.update()) {
      if (cursor_pos != prev_pos) {
        //gb.sound.playTick();
        gb.display.setColor(WHITE);
        gb.display.drawChar(x, y + 6 * (prev_pos), 16, 1);
        gb.display.setColor(BLACK);
        gb.display.drawChar(x, y + 6 * (cursor_pos), 16, 1);
        prev_pos = cursor_pos;
      }
      cursor_pos -= gb.buttons.repeat(BUTTON_UP, ARROW_DELAY) * (cursor_pos > 0) - (options_number - 1) * (gb.buttons.repeat(BUTTON_UP, ARROW_DELAY) && cursor_pos == 0);
      cursor_pos += gb.buttons.repeat(BUTTON_DOWN, ARROW_DELAY) * (cursor_pos < options_number - 1) - (options_number - 1) * (gb.buttons.repeat(BUTTON_DOWN, ARROW_DELAY) && cursor_pos == options_number - 1);;
      if (gb.buttons.repeat(BUTTON_B, BUTTON_DELAY)) {
        gb.sound.playCancel();
        return M_CANCEL;
      }
    }
  } while (!gb.buttons.repeat(BUTTON_A, BUTTON_DELAY));
  gb.sound.playOK();
  return cursor_pos;
}

byte script(byte script_id) {
  byte i;
  switch (script_id) {
    case S_PHONE:
      if (!flag[F_PICOBALL]) {
        text(str_phone, WITH_RESET);
        break;
      }
      text(str_no_messages, WITH_RESET);
      break;
    case S_PICOBALL:
      flag[F_PICOBALL] = true;
      gb.sound.play("sounds/take.wav");
      add_picomon(0, SWOOTY, 2, calc_stat(SWOOTY, 2, HP), 0, MO_TACKLE, MO_LEER, NO_MOVE, NO_MOVE);
      text(str_starter, WITH_RESET);
      break;
    case S_LEAVE_HOUSE:
      if (!flag[F_PICOBALL]) {
        text(str_take_picoball, WITH_RESET);
        break;
      }
      fade_in_map(FADE_DELAY);
      player_x = 12;
      player_y = 7;
      update_camera();
      fade_out_map(FADE_DELAY);
      break;
    case S_PICOSTOP:
      gb.sound.play("sounds/picostop.wav");
      for (i = 0; i < sizeof(player_picomons) / 2; i += PLAYER_PICOMONS_LENGTH) {
        if (player_picomons[i + ID] == MISSING_NO) break;
        player_picomons[i + CUR_HP] = calc_stat(player_picomons[i + ID], player_picomons[i + LVL], HP);
      }
      save_game();
      text(STR_GAME_SAVED, WITH_RESET);
      text(STR_HEALED, WITH_RESET);
      if (player_kills < 2) {
        text(STR_DEFEAT_WILD, WITH_RESET);
      } else {
        player_kills -= 2;
        i = search_item(NO_ITEM);
        if (i != NO_ITEM) {
          player_items[i] = rand() % 3;
          if (rand() % 100 < 5) player_items[i] = I_RESURRECT;
          text(STR_PICOSTOP, WITH_RESET);
          gb.sound.play("sounds/take.wav");
          text_frame();
          text(STR_POINTS, NO_RESET);
          text(item_names[player_items[i]], NO_RESET);
          text(STR_EXCLAMATION, NO_RESET);
          update_map();
        } else {
          text(STR_FULL_INVENTORY, WITH_RESET);
        }
      }
      break;
    case S_GIRL:
      if (rand() % 10 < 8) {
        text(STR_LOST_CITY, WITH_RESET);
      } else if (rand() % 2) {
        text(STR_SAME_STREETS, WITH_RESET);
      } else {
        text(STR_LAKE_HINT, WITH_RESET);
      }
      break;
    case S_CORKS_ASSISTANT:
      if (search_item(I_GARDEN_KEY) == NO_ITEM) {
        text(STR_CORKS_ASSISTANT, WITH_RESET);
        if (search_item(NO_ITEM) != NO_ITEM) {
          gb.sound.play("sounds/take.wav");
          text(STR_GARDEN_KEY, WITH_RESET);
          player_items[search_item(NO_ITEM)] = I_GARDEN_KEY;
        } else {
          text(STR_FULL_INVENTORY, WITH_RESET);
        }
      } else {
        text(STR_ASSISTANT_CATCH, WITH_RESET);
      }
      break;
    case S_GATE_1:
      if (search_item(I_GARDEN_KEY) != NO_ITEM) {
        text(STR_UNLOCK, WITH_RESET);
        flag[F_GATE_1] = true;
        update_map();
      } else {
        text(STR_LOCKED, WITH_RESET);
      }
      break;
    case S_GATE_2:
      if (search_item(I_GARDEN_KEY) != NO_ITEM) {
        text(STR_UNLOCK, WITH_RESET);
        flag[F_GATE_2] = true;
        update_map();
      } else {
        text(STR_LOCKED, WITH_RESET);
      }
      break;
    case S_GATE_3:
      if (search_item(I_GARDEN_KEY) != NO_ITEM) {
        text(STR_UNLOCK, WITH_RESET);
        flag[F_GATE_3] = true;
        update_map();
      } else {
        text(STR_LOCKED, WITH_RESET);
      }
      break;
    case S_APPLE_DUDE:
      if (search_item(I_SWIM_SUIT) == NO_ITEM) {
        if (search_item(I_APPLE) == NO_ITEM) {
          text(STR_APPLE_DUDE, WITH_RESET);
        } else {
          gb.sound.play("sounds/take.wav");
          text(STR_APPLE_DUDE_THX, WITH_RESET);
          player_items[search_item(I_APPLE)] = I_SWIM_SUIT;
        }
      } else {
        text(STR_MONSTERS_LAKE, WITH_RESET);
      }
      break;
    case S_CANDY:
      if (search_item(NO_ITEM) == NO_ITEM) {
        text(STR_FULL_INVENTORY, WITH_RESET);
        break;
      }
      gb.sound.play("sounds/take.wav");
      text(STR_CANDY, WITH_RESET);
      player_items[search_item(NO_ITEM)] = I_CANDY;
      flag[F_CANDY] = true;
      update_map();
      break;
    case S_PROF_CORKS:
      if (search_item(I_HAMMER) == NO_ITEM) {
        text(STR_PROF_CORKS, WITH_RESET);
        text_frame();
        text(STR_RATING, NO_RESET);
        tmp = player_picomons[LVL] * (player_picomons[ID] != MISSING_NO) + player_picomons[PLAYER_PICOMONS_LENGTH + LVL] * (player_picomons[PLAYER_PICOMONS_LENGTH + ID] != MISSING_NO) + player_picomons[2 * PLAYER_PICOMONS_LENGTH + LVL] * (player_picomons[2 * PLAYER_PICOMONS_LENGTH + ID] != MISSING_NO) + player_picomons[3 * PLAYER_PICOMONS_LENGTH + LVL] * (player_picomons[3 * PLAYER_PICOMONS_LENGTH + ID] != MISSING_NO);
        gb.display.print(tmp);
        text(STR_EXCLAMATION, NO_RESET);
        if (tmp >= CORKS_PRIZE_THRESHOLD) {
          text(STR_TAKE_HAMMER, WITH_RESET);
          if (search_item(NO_ITEM) != NO_ITEM) {
            gb.sound.play("sounds/take.wav");
            text(STR_GET_HAMMER, WITH_RESET);
            player_items[search_item(NO_ITEM)] = I_HAMMER;
          } else {
            text(STR_FULL_INVENTORY, WITH_RESET);
          }
        } else {
          text(STR_STRONGER_MONSTERS, WITH_RESET);
        }
      } else {
        text(STR_CHECK_CAVE, WITH_RESET);
      }
      break;
    case S_ROCK_1:
      if (search_item(I_HAMMER) != NO_ITEM) {
        text(STR_USE_HAMMER, WITH_RESET);
        flag[F_ROCK_1] = true;
        update_map();
      }
      break;
    case S_ROCK_2:
      if (search_item(I_HAMMER) != NO_ITEM) {
        text(STR_USE_HAMMER, WITH_RESET);
        flag[F_ROCK_2] = true;
        update_map();
      }
      break;
    case S_ROCK_3:
      if (search_item(I_HAMMER) != NO_ITEM) {
        text(STR_USE_HAMMER, WITH_RESET);
        flag[F_ROCK_3] = true;
        update_map();
      }
      break;
    case S_ROCK_4:
      if (search_item(I_HAMMER) != NO_ITEM) {
        text(STR_USE_HAMMER, WITH_RESET);
        flag[F_ROCK_4] = true;
        update_map();
      }
      break;
    case S_ROCK_5:
      if (search_item(I_HAMMER) != NO_ITEM) {
        text(STR_USE_HAMMER, WITH_RESET);
        flag[F_ROCK_5] = true;
        update_map();
      }
      break;
    case S_ROCK_6:
      if (search_item(I_HAMMER) != NO_ITEM) {
        text(STR_USE_HAMMER, WITH_RESET);
        flag[F_ROCK_6] = true;
        update_map();
      }
      break;
  }
}

void add_picomon(byte position, byte id, byte lvl, unsigned short hp, unsigned short xp, byte move_1, byte move_2, byte move_3, byte move_4) {
  player_picomons[position + ID] = (unsigned short) id;
  player_picomons[position + LVL] = (unsigned short) lvl;
  player_picomons[position + CUR_HP] = hp;
  player_picomons[position + XP] = xp;
  player_picomons[position + MOVE_1] = (unsigned short) move_1;
  player_picomons[position + MOVE_2] = (unsigned short) move_2;
  player_picomons[position + MOVE_3] = (unsigned short) move_3;
  player_picomons[position + MOVE_4] = (unsigned short) move_4;
}

unsigned short tmp_array[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
void swap_picomons(byte first_id, byte second_id) {
  memcpy(&tmp_array, &player_picomons[first_id * PLAYER_PICOMONS_LENGTH], PLAYER_PICOMONS_LENGTH * 2);
  memcpy(&player_picomons[first_id * PLAYER_PICOMONS_LENGTH], &player_picomons[second_id * PLAYER_PICOMONS_LENGTH], PLAYER_PICOMONS_LENGTH * 2);
  memcpy(&player_picomons[second_id * PLAYER_PICOMONS_LENGTH], &tmp_array, PLAYER_PICOMONS_LENGTH * 2);
}

void sort_picomons() {
  for (byte i = 0; i < sizeof(player_picomons) / 2 / PLAYER_PICOMONS_LENGTH - 1; i++) {
    if (player_picomons[i * PLAYER_PICOMONS_LENGTH + ID] == MISSING_NO && player_picomons[(i + 1)*PLAYER_PICOMONS_LENGTH + ID] != MISSING_NO) {
      swap_picomons(i, i + 1);
    }
  }
}

unsigned short calc_stat(byte id, byte lvl, byte stat) {
  return floor((picomon_stats[id * PICOMON_STATS_LENGTH + stat] * 2 * lvl / 100) + lvl + 10);
}

byte stats_menu(byte option) {
  if (player_picomons[ID] == MISSING_NO) {
    gb.sound.playCancel();
    return M_CANCEL;
  }
  byte picomon_count, cursor_pos = 0, prev_pos = 1;
  while (true) {
    if (gb.update()) {
      do {
        if (gb.update()) {
          if (cursor_pos != prev_pos) {
            //gb.sound.playTick();
            gb.display.fill(WHITE);
            gb.display.setColor(BLACK);
            gb.display.drawRect(0, 0, 43, 64);
            gb.display.drawRect(37, 37, 43, 27);
            gb.display.setColor(WHITE);
            gb.display.fillRect(38, 38, 41, 25);
            for (picomon_count = 0; picomon_count < sizeof(player_picomons) / 2; picomon_count += PLAYER_PICOMONS_LENGTH) {
              if (player_picomons[picomon_count + ID] == MISSING_NO)  break;
              tile_set.setFrame(picomon_stats[player_picomons[picomon_count + ID]*PICOMON_STATS_LENGTH + SPRITE_ID]);
              gb.display.drawImage(48, 1 + picomon_count / PLAYER_PICOMONS_LENGTH * 9, tile_set);
              draw_HP_box(57, 4 + picomon_count / PLAYER_PICOMONS_LENGTH * 9, 22, 4, (float) player_picomons[picomon_count + CUR_HP] / calc_stat(player_picomons[picomon_count + ID], player_picomons[picomon_count + LVL], HP));
            }
            picomon_count = picomon_count / PLAYER_PICOMONS_LENGTH - 1;

            gb.display.setColor(WHITE);
            gb.display.drawChar(44, 3 + 9 * (prev_pos), 16, 1);
            gb.display.setColor(BLACK);
            gb.display.drawChar(44, 3 + 9 * (cursor_pos), 16, 1);
            prev_pos = cursor_pos;

            gb.display.setCursor(2, 2);
            gb.display.print(picomon_names[player_picomons[(cursor_pos * PLAYER_PICOMONS_LENGTH) + ID]]);
            tile_set.setFrame(TILE_ELEMENT + picomon_stats[player_picomons[(cursor_pos * PLAYER_PICOMONS_LENGTH) + ID]*PICOMON_STATS_LENGTH + ELEMENT]);
            gb.display.drawImage(2, 8, tile_set);
            gb.display.setCursor(11, 10);
            gb.display.print(element_names[picomon_stats[player_picomons[(cursor_pos * PLAYER_PICOMONS_LENGTH) + ID]*PICOMON_STATS_LENGTH + ELEMENT]]);
            gb.display.setCursor(2, 17);
            gb.display.print(STR_LVL);
            gb.display.setCursor(2, 23);
            gb.display.print(STR_XP);
            gb.display.setCursor(2, 29);
            gb.display.print(STR_HP);
            gb.display.setCursor(2, 35);
            gb.display.print(STR_SPEED);
            gb.display.setCursor(2, 41);
            gb.display.print(STR_DEFENSE);
            gb.display.setCursor(2, 47);
            gb.display.print(STR_ATTACK);
            gb.display.setCursor(26, 23);
            gb.display.print(STR_SLASH);
            gb.display.setCursor(26, 29);
            gb.display.print(STR_SLASH);
            gb.display.setCursor(18, 17);
            gb.display.print(player_picomons[(cursor_pos * PLAYER_PICOMONS_LENGTH) + LVL]);
            tmp = player_picomons[(cursor_pos * PLAYER_PICOMONS_LENGTH) + XP];
            gb.display.setCursor(14 + 4 * (tmp < 100) + 4 * (tmp < 10), 23);
            gb.display.print(tmp);
            tmp = pow(player_picomons[(cursor_pos * PLAYER_PICOMONS_LENGTH) + LVL] * 10, 1.2);
            gb.display.setCursor(30 + 4 * (tmp < 100) + 4 * (tmp < 10), 23);
            gb.display.print(tmp);
            tmp = player_picomons[(cursor_pos * PLAYER_PICOMONS_LENGTH) + CUR_HP];
            gb.display.setCursor(14 + 4 * (tmp < 100) + 4 * (tmp < 10), 29);
            gb.display.print(tmp);
            tmp = calc_stat(player_picomons[cursor_pos * PLAYER_PICOMONS_LENGTH + ID], player_picomons[cursor_pos * PLAYER_PICOMONS_LENGTH + LVL], HP);
            gb.display.setCursor(30 + 4 * (tmp < 100) + 4 * (tmp < 10), 29);
            gb.display.print(tmp);
            gb.display.setCursor(18, 35);
            gb.display.print(calc_stat(player_picomons[cursor_pos * PLAYER_PICOMONS_LENGTH + ID], player_picomons[cursor_pos * PLAYER_PICOMONS_LENGTH + LVL], SPEED));
            gb.display.setCursor(18, 41);
            gb.display.print(calc_stat(player_picomons[cursor_pos * PLAYER_PICOMONS_LENGTH + ID], player_picomons[cursor_pos * PLAYER_PICOMONS_LENGTH + LVL], DEFENSE));
            gb.display.setCursor(18, 47);
            gb.display.print(calc_stat(player_picomons[cursor_pos * PLAYER_PICOMONS_LENGTH + ID], player_picomons[cursor_pos * PLAYER_PICOMONS_LENGTH + LVL], ATTACK));
            for (byte i = 0; i < 4; i++) {
              tmp = player_picomons[cursor_pos * PLAYER_PICOMONS_LENGTH + MOVE_1 + i];
              if (tmp == NO_MOVE) break;
              gb.display.setCursor(39, 39 + i * 6);
              gb.display.print(move_names[tmp]);
            }
          }
          cursor_pos -= gb.buttons.repeat(BUTTON_UP, ARROW_DELAY) * (cursor_pos > 0) - picomon_count * (gb.buttons.repeat(BUTTON_UP, ARROW_DELAY) && cursor_pos == 0);
          cursor_pos += gb.buttons.repeat(BUTTON_DOWN, ARROW_DELAY) * (cursor_pos < picomon_count) - picomon_count * (gb.buttons.repeat(BUTTON_DOWN, ARROW_DELAY) && cursor_pos == picomon_count);
          if (gb.buttons.repeat(BUTTON_B, BUTTON_DELAY)) {
            gb.sound.playCancel();
            return M_CANCEL;
          }
        }
      } while (!gb.buttons.repeat(BUTTON_A, BUTTON_DELAY));
      gb.sound.playOK();
      if (option == STATS_MENU_ITEM) return cursor_pos;
      while (!gb.update());
      switch (menu(first_release_menu, sizeof(first_release_menu), true, 0)) {
        case M_FIRST:
          swap_picomons(0, cursor_pos);
          prev_pos = (cursor_pos == 0);
          if (option == STATS_MENU_BATTLE && cursor_pos != 0) return 1;
          break;
        case M_RELEASE:
          if (picomon_count > 1) {
            gb.display.setColor(WHITE);
            gb.display.fillRect(12, 21, 55, 21);
            gb.display.setColor(BLACK);
            gb.display.drawRect(12, 21, 55, 21);
            gb.display.setCursor(14, 23);
            text(STR_SURE, NO_RESET);
            while (!gb.update());
            if (menu(no_yes_menu, sizeof(no_yes_menu), false, 0) == M_YES) {
              player_picomons[cursor_pos * PLAYER_PICOMONS_LENGTH + ID] = MISSING_NO;
              sort_picomons();
              cursor_pos = (cursor_pos - 1) * (cursor_pos > 0);
            }
          } else {
            text(STR_CANT_RELEASE, WITH_RESET);
          }
          if (option == STATS_MENU_BATTLE) return 1;
        default:
          prev_pos = (cursor_pos == 0);
          break;
      }
    }
  }
}

byte item_menu(boolean during_battle) {
  byte items_count, cursor_pos = 0, prev_pos = 1;
  while (true) {
    if (gb.update()) {
      if (player_items[0] == NO_ITEM) {
        gb.sound.playCancel();
        return M_CANCEL;
      }
      do {
        if (gb.update()) {
          if (cursor_pos != prev_pos) {
            //gb.sound.playTick();
            gb.display.fill(WHITE);
            gb.display.setColor(BLACK);
            gb.display.drawRect(0, 0, 80, 64);
            for (items_count = 0; items_count < sizeof(player_items); items_count ++) {
              if (player_items[items_count] == NO_ITEM) break;
              gb.display.setCursor(7, 2 + 6 * items_count);
              gb.display.print(item_names[player_items[items_count]]);
            }
            items_count--;
            gb.display.setColor(WHITE);
            gb.display.drawChar(2, 2 + 6 * (prev_pos), 16, 1);
            gb.display.setColor(BLACK);
            gb.display.drawChar(2, 2 + 6 * (cursor_pos), 16, 1);
            prev_pos = cursor_pos;
          }
          cursor_pos -= gb.buttons.repeat(BUTTON_UP, ARROW_DELAY) * (cursor_pos > 0) - (items_count) * (gb.buttons.repeat(BUTTON_UP, ARROW_DELAY) && cursor_pos == 0);
          cursor_pos += gb.buttons.repeat(BUTTON_DOWN, ARROW_DELAY) * (cursor_pos < items_count) - (items_count) * (gb.buttons.repeat(BUTTON_DOWN, ARROW_DELAY) && cursor_pos == items_count);
          if (gb.buttons.repeat(BUTTON_B, BUTTON_DELAY)) {
            gb.sound.playCancel();
            return M_CANCEL;
          }
        }
      } while (!gb.buttons.repeat(BUTTON_A, BUTTON_DELAY));
      gb.sound.playOK();
      while (!gb.update());
      switch (menu(use_drop_menu, sizeof(use_drop_menu), true, 0)) {
        case M_USE:
          switch (player_items[cursor_pos]) {
            case I_POTION:
              while (!gb.update());
              prev_pos = stats_menu(STATS_MENU_ITEM);
              if (prev_pos != M_CANCEL) {
                text_frame();
                if (player_picomons[prev_pos * PLAYER_PICOMONS_LENGTH + CUR_HP > 0]) {
                  tmp = calc_stat(player_picomons[prev_pos * PLAYER_PICOMONS_LENGTH + ID], player_picomons[prev_pos * PLAYER_PICOMONS_LENGTH + LVL], HP);
                  if (player_picomons[prev_pos * PLAYER_PICOMONS_LENGTH + CUR_HP] == tmp) {
                    gb.sound.play("sounds/cantheal.wav");
                    text(picomon_names[player_picomons[prev_pos * PLAYER_PICOMONS_LENGTH + ID]], NO_RESET);
                    text(STR_ALREADY_HEALED, NO_RESET);
                    break;
                  }
                  gb.sound.play("sounds/heal.wav");
                  player_picomons[prev_pos * PLAYER_PICOMONS_LENGTH + CUR_HP] += 20;
                  if (player_picomons[prev_pos * PLAYER_PICOMONS_LENGTH + CUR_HP] > tmp) {
                    player_picomons[prev_pos * PLAYER_PICOMONS_LENGTH + CUR_HP] = tmp;
                    text(STR_HEALING, NO_RESET);
                    text(picomon_names[player_picomons[prev_pos * PLAYER_PICOMONS_LENGTH + ID]], NO_RESET);
                    text(STR_FULL_HP, NO_RESET);
                  } else {
                    text(STR_HEALING, NO_RESET);
                    text(picomon_names[player_picomons[prev_pos * PLAYER_PICOMONS_LENGTH + ID]], NO_RESET);
                    text(STR_20_HP, NO_RESET);
                  }
                  player_items[cursor_pos] = NO_ITEM;
                  sort_items();
                  cursor_pos = (cursor_pos - 1) * (cursor_pos > 0);
                } else {
                  text(picomon_names[player_picomons[prev_pos * PLAYER_PICOMONS_LENGTH + ID]], NO_RESET);
                  text(STR_DEFEATED, NO_RESET);
                }
                if (during_battle) return ITEM_HANDLED;
                break;
              }
              break;
            case I_RESURRECT:
              while (!gb.update());
              prev_pos = stats_menu(STATS_MENU_ITEM);
              if (prev_pos != M_CANCEL) {
                text_frame();
                if (player_picomons[prev_pos * PLAYER_PICOMONS_LENGTH + CUR_HP] > 0) {
                  gb.sound.play("sounds/cantheal.wav");
                  text(picomon_names[player_picomons[prev_pos * PLAYER_PICOMONS_LENGTH + ID]], NO_RESET);
                  text(STR_NOT_KO, NO_RESET);
                  break;
                }
                gb.sound.play("sounds/heal.wav");
                player_picomons[prev_pos * PLAYER_PICOMONS_LENGTH + CUR_HP] = calc_stat(player_picomons[prev_pos * PLAYER_PICOMONS_LENGTH + ID], player_picomons[prev_pos * PLAYER_PICOMONS_LENGTH + LVL], HP) / 2;
                text(picomon_names[player_picomons[prev_pos * PLAYER_PICOMONS_LENGTH + ID]], NO_RESET);
                text(STR_HAS_BEEN_HEALED, NO_RESET);
                player_items[cursor_pos] = NO_ITEM;
                sort_items();
                if (during_battle) return ITEM_HANDLED;
                cursor_pos = (cursor_pos - 1) * (cursor_pos > 0);
              }
              break;
            case I_CANDY:
              while (!gb.update());
              prev_pos = stats_menu(STATS_MENU_ITEM);
              if (prev_pos != M_CANCEL) {
                gb.sound.play("sounds/heal.wav");
                text_frame();
                if (rand() % 2) {
                  player_picomons[prev_pos * PLAYER_PICOMONS_LENGTH + DOUBLE_XP] = 1;
                  text(picomon_names[player_picomons[prev_pos * PLAYER_PICOMONS_LENGTH + ID]], NO_RESET);
                  text(STR_LOVE_CANDY, NO_RESET);
                } else {
                  text(picomon_names[player_picomons[prev_pos * PLAYER_PICOMONS_LENGTH + ID]], NO_RESET);
                  text(STR_HATE_CANDY, NO_RESET);
                }
                player_items[cursor_pos] = NO_ITEM;
                sort_items();
                if (during_battle) return ITEM_HANDLED;
                cursor_pos = (cursor_pos - 1) * (cursor_pos > 0);
              }
              break;
            case I_PICOBALL:
              if (during_battle) return I_PICOBALL;
            case I_SMOKEBALL:
              if (during_battle) return I_SMOKEBALL;
            default:
              text_frame();
              text(STR_CANT_USE, NO_RESET);
              break;
          }
          break;
        case M_DROP:
          if (player_items[cursor_pos] < NO_DROPPABLE_ITEM) {
            gb.display.setColor(WHITE);
            gb.display.fillRect(12, 21, 55, 21);
            gb.display.setColor(BLACK);
            gb.display.drawRect(12, 21, 55, 21);
            gb.display.setCursor(14, 23);
            text(STR_SURE, NO_RESET);
            while (!gb.update());
            if (menu(no_yes_menu, sizeof(no_yes_menu), false, 0) == M_YES) {
              player_items[cursor_pos] = NO_ITEM;
              sort_items();
              if (during_battle) return ITEM_HANDLED;
              cursor_pos = (cursor_pos - 1) * (cursor_pos > 0);
            }
          } else {
            text_frame();
            text(STR_CANT_DROP, NO_RESET);
          }
        default:
          break;
      }
      prev_pos = (cursor_pos == 0);
    }
  }
}

byte search_item(byte item) {
  for (byte i = 0; i < PLAYER_ITEMS_LENGTH; i++) {
    if (player_items[i] == item) return i;
  }
  return NO_ITEM;
}

void sort_items() {
  for (byte i = 0; i < sizeof(player_items) - 1; i++) {
    if (player_items[i] == NO_ITEM && player_items[i + 1] != NO_ITEM) {
      player_items[i] = player_items[i + 1];
      player_items[i + 1] = NO_ITEM;
    }
  }
  return;
}

#define B_FIGHT   1
#define B_MONSTER 2
#define B_ITEM    3
#define B_RUN     4

unsigned short opponent_stats[] = {
  // picomon id, lvl, current hp, xp, double XP, move 1 id, move 2 id, move 3 id, move 4 id
  MISSING_NO, 0, 0, 0, 0, NO_MOVE, NO_MOVE, NO_MOVE, NO_MOVE
};

void battle(const byte zone[], byte size) {
  gb.sound.stop(music);
  gb.sound.play("sounds/battle_intro.wav");
  for (byte i = 0; i < 3; i++) {
    fade_in_map(BATTLE_TEXT_BLINK_DELAY);
    fade_out_map(BATTLE_TEXT_BLINK_DELAY);
  }
  music = gb.sound.play("musics/battle.wav", true);

  byte opponent_move_count = 1, escape_tries = 0, move_accuracy = 95, effectiveness, i, opponent_malus_def = 0, player_malus_def = 0;
  boolean critical;
  short damages;

  opponent_stats[ID] = zone[rand() % size];
  int tmp = player_picomons[LVL] - 3 + rand() % 5;
  if (tmp < 1) tmp = 1;
  if (tmp > 100) tmp = 100;
  opponent_stats[LVL] = tmp;
  opponent_stats[CUR_HP] = calc_stat(opponent_stats[ID], opponent_stats[LVL], HP);

  opponent_stats[MOVE_1] = NO_MOVE;
  opponent_stats[MOVE_2] = NO_MOVE;
  opponent_stats[MOVE_3] = NO_MOVE;
  opponent_stats[MOVE_4] = NO_MOVE;
  opponent_stats[MOVE_1] = rand() % (MO_FIRST_MAX + 1);
  if (opponent_stats[MOVE_1] == MO_LEER || rand() % 2) {
    opponent_move_count++;
    do {
      opponent_stats[MOVE_2] = rand() % (MO_SECOND_MAX + 1);
    } while (opponent_stats[MOVE_2] == opponent_stats[MOVE_1]);
    if (rand() % 2 && opponent_stats[LVL] > 5) {
      opponent_move_count++;
      do {
        opponent_stats[MOVE_3] = rand() % (MO_THIRD_MAX + 1);
      } while (opponent_stats[MOVE_3] == opponent_stats[MOVE_2]);
      if (rand() % 2 && opponent_stats[LVL] > 10) {
        opponent_move_count++;
        do {
          opponent_stats[MOVE_4] = rand() % (MO_FOURTH_MAX + 1);
        } while (opponent_stats[MOVE_4] == opponent_stats[MOVE_3]);
      }
    }
  }

  draw_battle_screen();
  battle_text(STR_WILD_MONSTER_1);
  battle_text(picomon_names[opponent_stats[ID]]);
  battle_text(STR_WILD_MONSTER_2);

  while (true) {
    if (gb.update()) {
battle_loop:
      byte cursor_pos = 1;
battle_loop_skip_cursor:
      draw_battle_screen();
      gb.display.setCursor(23, 51);
      gb.display.print(STR_FIGHT);
      gb.display.setCursor(51, 51);
      gb.display.print(STR_MONSTER);
      gb.display.setCursor(23, 57);
      gb.display.print(STR_ITEM);
      gb.display.setCursor(51, 57);
      gb.display.print(STR_RUN);
      switch (handle_battle_cursor(19, 28, cursor_pos, 4, false)) {
        case B_FIGHT:
          battle_text_frame();
          gb.display.setCursor(6, 51);
          gb.display.print(move_names[player_picomons[MOVE_1]]);
          i = 1;
          if (player_picomons[MOVE_2] != NO_MOVE) {
            gb.display.setCursor(45, 51);
            gb.display.print(move_names[player_picomons[MOVE_2]]);
            i ++;
          }
          if (player_picomons[MOVE_3] != NO_MOVE) {
            gb.display.setCursor(6, 57);
            gb.display.print(move_names[player_picomons[MOVE_3]]);
            i ++;
          }
          if (player_picomons[MOVE_4] != NO_MOVE) {
            gb.display.setCursor(45, 57);
            gb.display.print(move_names[player_picomons[MOVE_4]]);
            i ++;
          }
          while (!gb.update());
          i = handle_battle_cursor(2, 39, 1, i, true);
          switch (i) {
            case M_CANCEL:
              goto battle_loop;
            default:
              battle_text_frame();
              battle_text(picomon_names[player_picomons[ID]]);
              battle_text(STR_USED);
              battle_text(move_names[player_picomons[MOVE_1 + i - 1]]);
              battle_text(STR_EXCLAMATION);
              if (rand() % 100 > move_accuracy - 1) {
                battle_text(move_names[player_picomons[MOVE_1 + i - 1]]);
                battle_text(STR_MISSED);
                break;
              }
              if (player_picomons[MOVE_1 + i - 1] == MO_LEER) {
                battle_text(STR_LOWERED);
                battle_text(picomon_names[opponent_stats[ID]]);
                battle_text(STR_S_DEF);
                opponent_malus_def++;
              } else {
                effectiveness = calc_effectiveness(picomon_stats[player_picomons[ID] * PICOMON_STATS_LENGTH + ELEMENT], picomon_stats[opponent_stats[ID] * PICOMON_STATS_LENGTH + ELEMENT]);
                critical = rand() % 100 < (calc_stat(player_picomons[ID], player_picomons[LVL], SPEED) / 512) * 100;
                damages = calc_damage(player_picomons[MOVE_1 + i - 1], player_picomons[LVL], calc_stat(player_picomons[ID], player_picomons[LVL], ATTACK), calc_stat(opponent_stats[ID], opponent_stats[LVL], DEFENSE) - opponent_malus_def, effectiveness, critical);
                tmp = opponent_stats[CUR_HP] - damages;
                tmp *= (tmp > 0);

                gb.sound.play("sounds/damage.wav");
                for (i = 0; i < DAMAGE_BLINK_LENGHT; i++) {
                  if (i % 2) {
                    tile_set.setFrame(picomon_stats[opponent_stats[ID]*PICOMON_STATS_LENGTH + SPRITE_ID]);
                    gb.display.drawImage(55, 1, tile_set, -24, 24);
                  } else {
                    gb.display.setColor(WHITE);
                    gb.display.fillRect(55, 1, 24, 24);
                  }
                  while (!gb.update());
                  //delay(DAMAGE_BLINK_DELAY);
                }

                for (i = opponent_stats[CUR_HP]; i > tmp; i--) {
                  draw_HP_box(15, 14, 38, 4, (float) i / calc_stat(opponent_stats[ID], opponent_stats[LVL], HP));
                  while (!gb.update());
                  //delay(BATTLE_DAMAGES_DELAY);
                }
                draw_HP_box(15, 14, 38, 4, (float) tmp / calc_stat(opponent_stats[ID], opponent_stats[LVL], HP));
                opponent_stats[CUR_HP] = tmp;
                if (effectiveness == 2) battle_text(STR_EFFECTIVE);
                if (critical) battle_text(STR_CRITICAL_HIT);
                if (tmp == 0) {
                  battle_text(STR_YOU_DEFEATED);
                  battle_text(picomon_names[opponent_stats[ID]]);
                  battle_text(STR_EXCLAMATION);
                  player_kills++;
                  tmp = (5 * opponent_stats[LVL] + rand() % (5 * opponent_stats[LVL])) * (player_picomons[DOUBLE_XP] + 1);
                  player_picomons[XP] += tmp;
                  battle_text(STR_YOUR);
                  battle_text(picomon_names[player_picomons[ID]]);
                  battle_text(STR_EARNED);
                  gb.display.print(tmp);
                  battle_text(STR_XP_EXCLAMATION);
                  if (player_picomons[XP] > pow(player_picomons[LVL] * 10, 1.2) && player_picomons[LVL] < 100) {
                    gb.sound.play("sounds/take.wav");
                    player_picomons[LVL]++;
                    player_picomons[CUR_HP] = calc_stat(player_picomons[ID], player_picomons[LVL], HP);
                    player_picomons[XP] = 0;
                    battle_text(STR_YOUR);
                    battle_text(picomon_names[player_picomons[ID]]);
                    battle_text(STR_NOW_LEVEL);
                    gb.display.print(player_picomons[LVL]);
                    battle_text(STR_EXCLAMATION);
                  }
                  return;
                }
              }
              break;
          }
          break;
        case B_MONSTER:
          if (stats_menu(STATS_MENU_BATTLE) != M_CANCEL) {
            player_malus_def = 0;
            break;
          }
          cursor_pos = 2;
          goto battle_loop_skip_cursor;
        case B_ITEM:
          i = item_menu(true);
          if (i != M_CANCEL) {
            draw_battle_screen();
            switch (i) {
              case I_PICOBALL:
                for (i = 0; i < sizeof(player_picomons) / PLAYER_PICOMONS_LENGTH / 2 + 1; i++) {
                  if (i == sizeof(player_picomons) / PLAYER_PICOMONS_LENGTH / 2) {
                    battle_text(STR_TOO_MANY_MONSTERS);
                    goto battle_loop_skip_cursor;
                  }
                  if (player_picomons[i * PLAYER_PICOMONS_LENGTH + ID] == MISSING_NO) break;
                }
                player_items[cursor_pos] = NO_ITEM;
                sort_items();
                if (rand() % 10 < 2 + opponent_stats[CUR_HP] / calc_stat(opponent_stats[ID], opponent_stats[LVL], HP) * 2) {
                  battle_text(STR_MISSED_PICOBALL);
                  battle_text(picomon_names[opponent_stats[ID]]);
                  battle_text(STR_EXCLAMATION);
                } else {
                  gb.sound.play("sounds/take.wav");
                  battle_text(STR_CAUGHT);
                  battle_text(picomon_names[opponent_stats[ID]]);
                  battle_text(STR_EXCLAMATION);
                  memcpy(&player_picomons[i * PLAYER_PICOMONS_LENGTH + ID], &opponent_stats[ID], PLAYER_PICOMONS_LENGTH * 2);
                  return;
                }
                break;
              case I_SMOKEBALL:
                battle_text(STR_SMOKEBALL);
                player_items[cursor_pos] = NO_ITEM;
                sort_items();
                return;
              default:
                break;
            }
            break;
          }
          cursor_pos = 3;
          goto battle_loop_skip_cursor;
        case B_RUN:
          battle_text_frame();
          if (rand() % 255 < calc_stat(player_picomons[ID], player_picomons[LVL], SPEED) * 32 / ((calc_stat(opponent_stats[ID], opponent_stats[LVL], SPEED) / 4) % 256) + escape_tries * 30) {
            battle_text(STR_RAN_AWAY);
            return;
          } else {
            escape_tries ++;
            battle_text(STR_ESCAPE_FAIL);
          }
          break;
      }
      // opponent turn
      i = opponent_stats[MOVE_1 + rand() % opponent_move_count];
      draw_battle_screen();
      battle_text(picomon_names[opponent_stats[ID]]);
      battle_text(STR_USED);
      battle_text(move_names[i]);
      battle_text(STR_EXCLAMATION);
      if (rand() % 100 > move_accuracy - 1) {
        battle_text(move_names[i]);
        battle_text(STR_MISSED);
      } else {
        if (i == MO_LEER) {
          battle_text(STR_LOWERED);
          battle_text(picomon_names[player_picomons[ID]]);
          battle_text(STR_S_DEF);
          player_malus_def++;
        } else {
          effectiveness = calc_effectiveness(picomon_stats[opponent_stats[ID] * PICOMON_STATS_LENGTH + ELEMENT], picomon_stats[player_picomons[ID] * PICOMON_STATS_LENGTH + ELEMENT]);
          critical = rand() % 100 < (calc_stat(opponent_stats[ID], opponent_stats[LVL], SPEED) / 512) * 100;
          damages = calc_damage(i, opponent_stats[LVL], calc_stat(opponent_stats[ID], opponent_stats[LVL], ATTACK), calc_stat(player_picomons[ID], player_picomons[LVL], DEFENSE) - player_malus_def, effectiveness, critical);
          tmp = player_picomons[CUR_HP] - damages;
          tmp *= (tmp > 0);

          gb.sound.play("sounds/damage.wav");
          for (i = 0; i < DAMAGE_BLINK_LENGHT; i++) {
            if (i % 2) {
              tile_set.setFrame(picomon_stats[player_picomons[ID]*PICOMON_STATS_LENGTH + SPRITE_ID]);
              gb.display.drawImage(1, 24, tile_set, 24, 24);
            } else {
              gb.display.setColor(WHITE);
              gb.display.fillRect(1, 24, 24, 24);
            }
            while (!gb.update());
            //delay(DAMAGE_BLINK_DELAY);
          }

          for (i = player_picomons[CUR_HP]; i > tmp; i--) {
            draw_HP_box(39, 42, 38, 4, (float) i / calc_stat(player_picomons[ID], player_picomons[LVL], HP));
            while (!gb.update());
            //delay(BATTLE_DAMAGES_DELAY);
          }
          draw_HP_box(39, 42, 38, 4, (float) i / calc_stat(player_picomons[ID], player_picomons[LVL], HP));
          player_picomons[CUR_HP] = tmp;
          if (effectiveness == 2) battle_text(STR_EFFECTIVE);
          if (critical) battle_text(STR_CRITICAL_HIT);
          if (tmp == 0) {
            battle_text(picomon_names[player_picomons[ID]]);
            battle_text(STR_IS_KO);
            for (i = 0; i < sizeof(player_picomons) / PLAYER_PICOMONS_LENGTH / 2 + 1; i++) {
              if (i == sizeof(player_picomons) / PLAYER_PICOMONS_LENGTH / 2) {
                battle_text(STR_YOU_ARE_DEFEATED);
                battle_text(STR_AWAKE);
                fade_in_map(FADE_DELAY);
                load_game();
                update_camera();
                update_map();
                fade_out_map(FADE_DELAY);
                return;
              }
              if (player_picomons[i * PLAYER_PICOMONS_LENGTH + CUR_HP] > 0) break;
            }
            do {
              while (!gb.update());
              i = stats_menu(STATS_MENU_ITEM);
              if (player_picomons[i * PLAYER_PICOMONS_LENGTH + CUR_HP] == 0) i = M_CANCEL;
            } while (i == M_CANCEL);
            swap_picomons(0, i);
          }
        }
      }
    }
  }
}

void draw_battle_screen() {
  gb.display.fill(WHITE);
  gb.display.setColor(BLACK);
  gb.display.drawFastVLine(1, 0, 20);
  gb.display.drawFastHLine(2, 19, 52);
  gb.display.setCursor(3, 1);
  gb.display.print(picomon_names[opponent_stats[ID]]);
  gb.display.setCursor(3, 7);
  gb.display.print(STR_L);
  gb.display.print(opponent_stats[LVL]);
  gb.display.setCursor(3, 13);
  gb.display.print(STR_HP_);
  draw_HP_box(15, 14, 38, 4, (float) opponent_stats[CUR_HP] / calc_stat(opponent_stats[ID], opponent_stats[LVL], HP));
  tile_set.setFrame(picomon_stats[opponent_stats[ID]*PICOMON_STATS_LENGTH + SPRITE_ID]);
  gb.display.drawImage(55, 1, tile_set, -24, 24);
  gb.display.drawFastVLine(78, 28, 20);
  gb.display.drawFastHLine(26, 47, 52);
  gb.display.setCursor(78 - strlen(picomon_names[player_picomons[ID]]) * 4, 30);
  gb.display.print(picomon_names[player_picomons[ID]]);
  gb.display.setCursor(66 - 4 * (player_picomons[LVL] > 9) - 4 * (player_picomons[LVL] > 99), 36);
  gb.display.print(STR_L);
  gb.display.print(player_picomons[LVL]);
  gb.display.setCursor(27, 41);
  gb.display.print(STR_HP_);
  draw_HP_box(39, 42, 38, 4, (float) player_picomons[CUR_HP] / calc_stat(player_picomons[ID], player_picomons[LVL], HP));
  tile_set.setFrame(picomon_stats[player_picomons[ID]*PICOMON_STATS_LENGTH + SPRITE_ID]);
  gb.display.drawImage(1, 24, tile_set, 24, 24);
  battle_text_frame();
}

void draw_HP_box(byte x, byte y, byte w, byte h, float hp_ratio) {
  gb.display.setColor(WHITE);
  gb.display.fillRect(x, y, w, h);
  gb.display.setColor((Color)0xf809);
  gb.display.fillRect(x + 1, y + 1, (w - 2)*hp_ratio, h - 2);
  gb.display.setColor(BLACK);
  gb.display.drawRect(x, y, w, h);
}

void battle_text_frame() {
  gb.display.setColor(WHITE);
  gb.display.fillRect(1, 50, 78, 13);
  gb.display.setColor(BLACK);
  gb.display.drawRect(0, 49, 80, 15);
  gb.display.setCursor(2, 51);
}

byte battle_text(const char string[]) {
  for (byte i = 0; i < strlen(string); i++) {
    char c = (char) string[i];
    switch ((byte) c) {
      case 10:
        gb.display.setCursor(2, gb.display.getCursorY() + 6);
        break;
      case 112:
        text_pause();
        battle_text_frame();
        break;
      default:
        gb.display.print(c);
        if (c > 64 && c < 91) gsfx.play(GSFX::FX{GSFX::WaveType::SQUARE, 10000, 0, 3000 + 500 * (c - 64), 0, 2000});
        while (!gb.update());
        break;
    }
  }
}

byte handle_battle_cursor(byte x, byte w, byte cursor_pos, byte options_number, boolean allow_cancel) {
  byte prev_pos = 4;
  do {
    if (gb.update()) {
      if (cursor_pos != prev_pos) {
        //gb.sound.playTick();
        gb.display.setColor(WHITE);
        gb.display.drawChar(x + w * !(prev_pos % 2), 51 + 6 * (prev_pos > 2), 16, 1);
        gb.display.setColor(BLACK);
        gb.display.drawChar(x + w * !(cursor_pos % 2), 51 + 6 * (cursor_pos > 2), 16, 1);
        prev_pos = cursor_pos;
      }
      cursor_pos -= 1 * gb.buttons.repeat(BUTTON_LEFT, ARROW_DELAY) * (cursor_pos > 1 && cursor_pos != 3);
      cursor_pos += 1 * gb.buttons.repeat(BUTTON_RIGHT, ARROW_DELAY) * ((cursor_pos == 1 && options_number > 1) || (cursor_pos == 3 && options_number > 3));
      cursor_pos -= 2 * gb.buttons.repeat(BUTTON_UP, ARROW_DELAY) * (cursor_pos > 2);
      cursor_pos += 2 * gb.buttons.repeat(BUTTON_DOWN, ARROW_DELAY) * ((cursor_pos == 1 && options_number > 2) || (cursor_pos == 2 && options_number > 3));
      if (allow_cancel && gb.buttons.repeat(BUTTON_B, BUTTON_DELAY)) {
        gb.sound.playCancel();
        return M_CANCEL;
      }
    }
  } while (!gb.buttons.repeat(BUTTON_A, BUTTON_DELAY));
  gb.sound.playOK();
  return cursor_pos;
}

byte calc_effectiveness(byte from_element, byte target_element) {
  switch (from_element) {
    case T_AIR:
      if (target_element == T_WATER) return 2;
      break;
    case T_FIRE:
      if (target_element == T_AIR || target_element == T_EARTH) return 2;
      break;
    //case T_EARTH:
    case T_WATER:
      if (target_element == T_FIRE) return 2;
      break;
  }
  return 1;
}

short calc_damage(byte from_move, byte from_level, byte from_attack, byte target_defense, byte effectiveness, boolean critical) {
  //short damage = (((2 * from_level + 10) / 300) * (from_attack / target_defense) * 3 * moves_base[from_move] + 2) * effectiveness;
  short damage = (from_level * 0.4 + 2) * from_attack * moves_base[from_move] / (target_defense * 50) + 2 * effectiveness;
  if (critical) damage *= 2;
  return damage;
}
