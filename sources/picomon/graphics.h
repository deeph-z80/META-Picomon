void draw_map(int camera_x, int camera_y) {
  for (byte y = 0; y <= 9; y++) {
    for (byte x = 0; x <= 11; x++) {
      int tile_x = camera_x / TILE_WIDTH + x;
      int tile_y = camera_y / TILE_HEIGHT + y;
      if (tile_x >= 0 && tile_x < MAP_WIDTH && tile_y >= 0 && tile_y < MAP_HEIGHT) {
        tile_set.setFrame(map_[tile_y * MAP_WIDTH + tile_x]);
        gb.display.drawImage(x * TILE_WIDTH - camera_x % TILE_WIDTH, y * TILE_HEIGHT - camera_y % TILE_HEIGHT, tile_set);

        // check for objects to draw
        for (byte i = 0; i < sizeof(action_triggered_scripts); i += ACTION_SCRIPTS_LENGTH) {
          byte tile_id = action_triggered_scripts[i + 3];
          if (tile_id > 0 &&
              flag[action_triggered_scripts[i + 2]] == 0 &&
              tile_x == action_triggered_scripts[i] &&
              tile_y == action_triggered_scripts[i + 1]) {
            tile_set.setFrame(tile_id);
            gb.display.drawImage(x * TILE_WIDTH - camera_x % TILE_WIDTH, y * TILE_HEIGHT - camera_y % TILE_HEIGHT, tile_set);
            break;
          }
        }

      }
    }
  }
}

void draw_player(int x, int y) {
  if(map_[player_y * MAP_WIDTH + player_x] >= TILE_WATER_START && map_[player_y * MAP_WIDTH + player_x] <= TILE_WATER_END){
    player_sprite_set.setFrame(player_direction + 8);
  }else{
    player_sprite_set.setFrame(player_direction * 2 + player_animation );
  }
  gb.display.drawImage(x, y, player_sprite_set);
}

void update_map() {
  draw_map(camera_x, camera_y);
  draw_player(player_x * TILE_WIDTH - camera_x, player_y * TILE_WIDTH - camera_y);

  if(map_[player_y * MAP_WIDTH + player_x] == TILE_GRASS){
    tile_set.setTransparentColor((Color)0xff9d);
    tile_set.setFrame(TILE_GRASS);
    gb.display.drawImage(player_x * TILE_WIDTH - camera_x, player_y * TILE_WIDTH - camera_y, tile_set);
    tile_set.clearTransparentColor();
  }
}

void update_camera() {
  camera_x = player_x * TILE_WIDTH - gb.display.width() / 2 + 4;
  camera_x = camera_x * (camera_x > 0) + (MAP_WIDTH * TILE_WIDTH - gb.display.width() - camera_x) * (camera_x > MAP_WIDTH * TILE_WIDTH - gb.display.width());
  camera_y = player_y * TILE_HEIGHT - gb.display.height() / 2 + 4;
  camera_y = camera_y * (camera_y > 0) + (MAP_HEIGHT * TILE_HEIGHT - gb.display.height() - camera_y) * (camera_y > MAP_HEIGHT * TILE_HEIGHT - gb.display.height());
}

void fade_in(byte delay_){
  gb.display.colorIndex = pico_8_palette_dark;
  while(!gb.update());
  delay(delay_);
  gb.display.colorIndex = pico_8_palette_darker;
  while(!gb.update());
  delay(delay_);
  gb.display.fill(BLACK);
  while(!gb.update());
  delay(delay_);
}

void fade_out(byte delay_){
  gb.display.colorIndex = pico_8_palette_darker;
  while(!gb.update());
  delay(delay_);
  gb.display.colorIndex = pico_8_palette_dark;
  while(!gb.update());
  delay(delay_);
  gb.display.colorIndex = pico_8_palette;
  while(!gb.update());
  delay(delay_);
}

void fade_in_map(byte delay_){
  gb.display.colorIndex = pico_8_palette_dark;
  update_map();
  while(!gb.update());
  delay(delay_);
  gb.display.colorIndex = pico_8_palette_darker;
  update_map();
  while(!gb.update());
  delay(delay_);
  gb.display.fill(BLACK);
  while(!gb.update());
  delay(delay_);
}

void fade_out_map(byte delay_){
  gb.display.colorIndex = pico_8_palette_darker;
  update_map();
  while(!gb.update());
  delay(delay_);
  gb.display.colorIndex = pico_8_palette_dark;
  update_map();
  while(!gb.update());
  delay(delay_);
  gb.display.colorIndex = pico_8_palette;
  update_map();
  while(!gb.update());
  delay(delay_);
}
