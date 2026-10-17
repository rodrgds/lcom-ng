#include "gui.h"

static sprite_t *cursor1 = NULL;
static sprite_t *W_Dirt = NULL;
static sprite_t *Dirt = NULL;
static sprite_t *Sun_Sprite = NULL;
static sprite_t *Moon_Sprite = NULL;
static sprite_t *Cloud_Sprite = NULL;
static sprite_t *Cloud2_Sprite = NULL;
static sprite_t *BgDay = NULL;
static sprite_t *BgNight = NULL;
static sprite_t *BgSet = NULL;
static sprite_t *BgRise = NULL;
static sprite_t *HotbarSprite = NULL;
static sprite_t *HotbarSelectedSprite = NULL;
static sprite_t *Tree_sappling = NULL;
static sprite_t *Tree_Growing = NULL;
static sprite_t *Tree_Adult = NULL;
static sprite_t *menuBackground = NULL;
static sprite_t *pauseBackground = NULL;
static sprite_t *optionsBackground = NULL;
static sprite_t *mode1 = NULL;
static sprite_t *mode2 = NULL;
static sprite_t *mode3 = NULL;
static sprite_t *Wood = NULL;
static sprite_t *Number[10] = {NULL};
static sprite_t *Bedrock = NULL;
static sprite_t *arrow = NULL;
static sprite_t *Attack[6] = {NULL};
static sprite_t *Idle[7] = {NULL};
static sprite_t *Jump[3] = {NULL};
static sprite_t *Walk[8] = {NULL};

sprite_t *playButton = NULL;
sprite_t *optionsButton = NULL;
sprite_t *exitButton = NULL;
sprite_t *pauseButton = NULL;

void setup_sprites(uint16_t mode) {

  char *suffix = NULL;

  switch (mode) {
    case 0x11A:
      suffix = "_16";
      break;
    case 0x115:
      suffix = "_24";
      break;
    case 0x14C:
      suffix = "_32";
      break;
    default:
      return;
  }

  char filepath[100];

  sprintf(filepath, "/home/lcom/labs/grupo_2leic05_6/proj/src/assets/cursor/Cursor%s.bmp", suffix);

  cursor1 = bitmap_load(filepath, mode);
  get_cursor()->width = cursor1->width;
  get_cursor()->height = cursor1->height;

  sprintf(filepath, "/home/lcom/labs/grupo_2leic05_6/proj/src/assets/menu_background/menu_back%s.bmp", suffix);
  menuBackground = bitmap_load(filepath, mode);

  sprintf(filepath, "/home/lcom/labs/grupo_2leic05_6/proj/src/assets/menu_background/menuPause%s.bmp", suffix);
  pauseBackground = bitmap_load(filepath, mode);

  sprintf(filepath, "/home/lcom/labs/grupo_2leic05_6/proj/src/assets/menu_background/options_background%s.bmp", suffix);
  optionsBackground = bitmap_load(filepath, mode);

  sprintf(filepath, "/home/lcom/labs/grupo_2leic05_6/proj/src/assets/buttons/play%s.bmp", suffix);
  playButton = bitmap_load(filepath, mode);

  sprintf(filepath, "/home/lcom/labs/grupo_2leic05_6/proj/src/assets/menu_background/seta%s.bmp", suffix);
  arrow = bitmap_load(filepath, mode);

  sprintf(filepath, "/home/lcom/labs/grupo_2leic05_6/proj/src/assets/buttons/options%s.bmp", suffix);
  optionsButton = bitmap_load(filepath, mode);

  sprintf(filepath, "/home/lcom/labs/grupo_2leic05_6/proj/src/assets/buttons/exit%s.bmp", suffix);
  exitButton = bitmap_load(filepath, mode);

  sprintf(filepath, "/home/lcom/labs/grupo_2leic05_6/proj/src/assets/buttons/pause%s.bmp", suffix);
  pauseButton = bitmap_load(filepath, mode);

  sprintf(filepath, "/home/lcom/labs/grupo_2leic05_6/proj/src/assets/buttons/800x600%s.bmp", suffix);
  mode1 = bitmap_load(filepath, mode);

  sprintf(filepath, "/home/lcom/labs/grupo_2leic05_6/proj/src/assets/buttons/1152x864%s.bmp", suffix);
  mode2 = bitmap_load(filepath, mode);

  sprintf(filepath, "/home/lcom/labs/grupo_2leic05_6/proj/src/assets/buttons/1280x1024%s.bmp", suffix);
  mode3 = bitmap_load(filepath, mode);

  sprintf(filepath, "/home/lcom/labs/grupo_2leic05_6/proj/src/assets/Hotbar/Hotbar%s.bmp", suffix);
  HotbarSprite = bitmap_load(filepath, mode);

  sprintf(filepath, "/home/lcom/labs/grupo_2leic05_6/proj/src/assets/Hotbar/Hotbar_S%s.bmp", suffix);
  HotbarSelectedSprite = bitmap_load(filepath, mode);

  sprintf(filepath, "/home/lcom/labs/grupo_2leic05_6/proj/src/assets/Map/Wood/Wood%s.bmp", suffix);
  Wood = bitmap_load(filepath, mode);

  sprintf(filepath, "/home/lcom/labs/grupo_2leic05_6/proj/src/assets/Map/sun/sun%s.bmp", suffix);
  Sun_Sprite = bitmap_load(filepath, mode);

  sprintf(filepath, "/home/lcom/labs/grupo_2leic05_6/proj/src/assets/Map/moon/moon%s.bmp", suffix);
  Moon_Sprite = bitmap_load(filepath, mode);

  sprintf(filepath, "/home/lcom/labs/grupo_2leic05_6/proj/src/assets/Map/cloud/cloud%s.bmp", suffix);
  Cloud_Sprite = bitmap_load(filepath, mode);

  sprintf(filepath, "/home/lcom/labs/grupo_2leic05_6/proj/src/assets/Map/cloud/cloud2%s.bmp", suffix);
  Cloud2_Sprite = bitmap_load(filepath, mode);

  sprintf(filepath, "/home/lcom/labs/grupo_2leic05_6/proj/src/assets/Map/background/bgday/bg-day%s.bmp", suffix);
  BgDay = bitmap_load(filepath, mode);

  sprintf(filepath, "/home/lcom/labs/grupo_2leic05_6/proj/src/assets/Map/background/bgset/bg-set%s.bmp", suffix);
  BgSet = bitmap_load(filepath, mode);

  sprintf(filepath, "/home/lcom/labs/grupo_2leic05_6/proj/src/assets/Map/background/bgnight/bg-night%s.bmp", suffix);
  BgNight = bitmap_load(filepath, mode);

  sprintf(filepath, "/home/lcom/labs/grupo_2leic05_6/proj/src/assets/Map/background/bgrise/bg-sunrise%s.bmp", suffix);
  BgRise = bitmap_load(filepath, mode);

  sprintf(filepath, "/home/lcom/labs/grupo_2leic05_6/proj/src/assets/Map/Dirt/w-grass%s.bmp", suffix);
  W_Dirt = bitmap_load(filepath, mode);

  sprintf(filepath, "/home/lcom/labs/grupo_2leic05_6/proj/src/assets/Map/Dirt/dirt%s.bmp", suffix);
  Dirt = bitmap_load(filepath, mode);

  sprintf(filepath, "/home/lcom/labs/grupo_2leic05_6/proj/src/assets/Map/Tree/Tree_sappling%s.bmp", suffix);
  Tree_sappling = bitmap_load(filepath, mode);

  sprintf(filepath, "/home/lcom/labs/grupo_2leic05_6/proj/src/assets/Map/Tree/Tree%s.bmp", suffix);
  Tree_Growing = bitmap_load(filepath, mode);

  sprintf(filepath, "/home/lcom/labs/grupo_2leic05_6/proj/src/assets/Map/Tree/Tree_adult%s.bmp", suffix);
  Tree_Adult = bitmap_load(filepath, mode);

  sprintf(filepath, "/home/lcom/labs/grupo_2leic05_6/proj/src/assets/Map/Bedrock/Stone%s.bmp", suffix);
  Bedrock = bitmap_load(filepath, mode);

  for (int i = 0; i < 10; i++) {
    sprintf(filepath, "/home/lcom/labs/grupo_2leic05_6/proj/src/assets/numbers/%d/%d%s.bmp", i, i, suffix);
    Number[i] = bitmap_load(filepath, mode);
  }

  for (int i = 0; i < 8; i++) {
    sprintf(filepath, "/home/lcom/labs/grupo_2leic05_6/proj/src/assets/Character/walking/walk%d%s.bmp", i, suffix);
    Walk[i] = bitmap_load(filepath, mode);
  }

  for (int i = 0; i < 7; i++) {
    sprintf(filepath, "/home/lcom/labs/grupo_2leic05_6/proj/src/assets/Character/idle/idle%d%s.bmp", i, suffix);
    Idle[i] = bitmap_load(filepath, mode);
  }

  for (int i = 0; i < 3; i++) {
    sprintf(filepath, "/home/lcom/labs/grupo_2leic05_6/proj/src/assets/Character/jump/jump%d%s.bmp", i, suffix);
    Jump[i] = bitmap_load(filepath, mode);
  }

  for (int i = 0; i < 6; i++) {
    sprintf(filepath, "/home/lcom/labs/grupo_2leic05_6/proj/src/assets/Character/attack/ataque%d%s.bmp", i, suffix);
    Attack[i] = bitmap_load(filepath, mode);
  }

}

void draw() {
  switch (gamestate) {
    case MENU_STATE:
      draw_menu();
      break;
    case GAME_STATE:
      get_player()->height = Idle[1]->height;
      get_player()->width = Idle[1]->width;
      draw_game();
      break;

    case OPTIONS_STATE:
      draw_options();
      break;

    case PAUSE_STATE:
      draw_pause();
      break;

    case EXIT_STATE:
      break;
  }

  draw_cursor(get_cursor()->position);
  swap_buffers();
}

void draw_character(Position position) {
  sprite_t *sprite = Jump[0];
  static Position last_pos = {0, 0};
  static int idle_frame = 0, walk_frame = 0, jump_frame = 0;
  static int frame_counter = 0;
  static bool facing_left = false;

  if (position.y != last_pos.y) {
    sprite = Jump[jump_frame];
    frame_counter++;
    if (frame_counter >= 10) {
      jump_frame = (jump_frame + 1) % 3;
      frame_counter = 0;
    }
  }
  else if (position.x != last_pos.x) {
    sprite = Walk[walk_frame];
    frame_counter++;
    if (frame_counter >= 10) {
      walk_frame = (walk_frame + 1) % 8;
      frame_counter = 0;
    }
  }

  else {
    sprite = Idle[idle_frame];
    frame_counter++;
    if (frame_counter >= 10) {
      idle_frame = (idle_frame + 1) % 7;
      frame_counter = 0;
    }
  }

  if (left_pressed && !right_pressed) {
    facing_left = true;
  }
  else if (right_pressed && !left_pressed) {
    facing_left = false;
  }

  sprite->position = position;

  if (facing_left) {
    vg_draw_sprite_flipped(sprite);
  }
  else {
    vg_draw_sprite(sprite);
  }

  last_pos = position;
}


void draw_menu() {
  VG_clear();
  vg_draw_sprite(menuBackground);
  draw_mainMenu_buttons();
}

void draw_game() {
  VG_clear();
  draw_Map();
  draw_hotbar(get_player()->backpack);
  draw_character(get_player()->position);
  Position position = {20, 20};
  draw_Button(position, pauseButton);
}

void draw_pause() {
  VG_clear();
  vg_draw_sprite(pauseBackground);
  draw_pauseMenu_buttons();
}

void draw_options() {
  VG_clear();
  vg_draw_sprite(optionsBackground);
  draw_optionsMenu_buttons();
}

void draw_hotbar(Backpack *b) {
  int start_x = mode_info.XResolution - (HotbarSprite->width * BACKPACK_SIZE) - 16;
  int start_y = 8;

  for (int i = 0; i < BACKPACK_SIZE; i++) {
    sprite_t *sprite = (i == b->selected) ? HotbarSelectedSprite : HotbarSprite;
    sprite->position.x = start_x + (i * HotbarSprite->width);
    sprite->position.y = start_y;
    vg_draw_sprite(sprite);
  }

  for (int i = 0; i < BACKPACK_SIZE; i++) {
    int item_x = start_x + (i * HotbarSprite->width) + 8;
    int item_y = start_y + 8;
    switch (b->slots[i]) {
      case ITEM_EARTH:
        Dirt->position.x = item_x;
        Dirt->position.y = item_y;
        Number[b->quantity[i]]->position.x = Dirt->position.x + 16;
        Number[b->quantity[i]]->position.y = Dirt->position.y + 16;
        vg_draw_sprite(Dirt);
        vg_draw_sprite(Number[b->quantity[i]]);
        break;
      case ITEM_WOOD:

        Wood->position.x = item_x;
        Wood->position.y = item_y;
        Number[b->quantity[i]]->position.x = Wood->position.x + 16;
        Number[b->quantity[i]]->position.y = Wood->position.y + 16;
        vg_draw_sprite(Wood);
        vg_draw_sprite(Number[b->quantity[i]]);
        break;
      case ITEM_SAPPLING:
        Tree_sappling->position.x = item_x;
        Tree_sappling->position.y = item_y;
        Number[b->quantity[i]]->position.x = Tree_sappling->position.x + 16;
        Number[b->quantity[i]]->position.y = Tree_sappling->position.y + 16;
        vg_draw_sprite(Tree_sappling);
        vg_draw_sprite(Number[b->quantity[i]]);
        break;
      default:
        break;
    }
  }
}

void draw_cursor(Position position) {
  cursor1->position = position;
  vg_draw_sprite_clipped(cursor1);
}

void draw_mainMenu_buttons() {
  Position playButtonPosition = {401, 330};
  Position optionsButtonPosition = {401, 460};
  Position exitButtonPosition = {401, 590};
  draw_Button(playButtonPosition, playButton);
  draw_Button(optionsButtonPosition, optionsButton);
  draw_Button(exitButtonPosition, exitButton);
}

void draw_pauseMenu_buttons() {
  Position playButtonPosition = {401, 220};
  Position exitButtonPosition = {401, 350};
  draw_Button(playButtonPosition, playButton);
  draw_Button(exitButtonPosition, exitButton);
}

void draw_optionsMenu_buttons() {
  Position arrowPosition = {20, 20};
  Position mode1Position = {401, 330};
  Position mode2Position = {401, 460};
  Position mode3Position = {401, 590};
  draw_Button(mode1Position, mode1);
  draw_Button(mode2Position, mode2);
  draw_Button(mode3Position, mode3);
  draw_Button(arrowPosition, arrow);
}

void draw_Button(Position position, sprite_t *sprite) {
  sprite->position = position;
  vg_draw_sprite(sprite);
}

void draw_Map() {
  MapController *mc = get_MapController();
  
  switch (bg) {
    case BG_DAY:
      vg_draw_sprite(BgDay);
      break;
    case BG_NIGHT:
      vg_draw_sprite(BgNight);
      break;
    case BG_SET:
      vg_draw_sprite(BgSet);
      break;
    case BG_RISE:
      vg_draw_sprite(BgRise);
      break;
  }
  
  Sun_Sprite->position = mc->map->sun.position;
  vg_draw_sprite_clipped(Sun_Sprite);
  Moon_Sprite->position = mc->map->moon.position;
  vg_draw_sprite_clipped(Moon_Sprite);
  Cloud_Sprite->position = mc->map->cloud.position;
  vg_draw_sprite_clipped(Cloud_Sprite);

  Cloud2_Sprite->position = mc->map->cloud2.position;
  vg_draw_sprite_clipped(Cloud2_Sprite);

  
  for (size_t i = 0; i < mc->map->count_elements; i++) {
    Element *e = mc->map->elements[i];
    
    switch (e->type) {
      case ELEMENT_EARTH:
      if (!e->block_up) {
        W_Dirt->position = e->position;
        vg_draw_sprite(W_Dirt);
      }
      else {
        Dirt->position = e->position;
        vg_draw_sprite(Dirt);
      }
      break;
      case ELEMENT_TREE:
      if (e->block_up) {
        Tree_Growing->position = e->position;
        vg_draw_sprite(Tree_Growing);
      }
      else if (e->data.tree.growth_stage == TREE_SAPPLING) {
         Tree_sappling->position = e->position;
         vg_draw_sprite(Tree_sappling);
       }
      else if (e->data.tree.growth_stage == TREE_MEDIUM) {
        Tree_Growing->position = e->position;
        vg_draw_sprite(Tree_Growing);
      }
      else {
        Tree_Adult->position = e->position;
        vg_draw_sprite(Tree_Adult);
      }
        break;
      case ELEMENT_WOOD:
        Wood->position = e->position;
        vg_draw_sprite(Wood);
        break;
      case ELEMENT_BEDROCK:
        Bedrock->position = e->position;
        vg_draw_sprite(Bedrock);
        break;
      default:
        break;
      }
    }
  }
