#include "CursorController.h"

/**
 * @file CursorController.c
 * @brief Implements cursor movement, interactions (placing/removing blocks), and menu button handling.
 *
 * Controls how the mouse cursor behaves across different game states.
 * Includes logic for block placement/removal in GAME_STATE and UI interaction in MENU/PAUSE_STATE.
 */

static bool Remove_Block(Position p, Position o, int16_t Ow, int16_t Oh) {
  if ((p.x < o.x) || (p.x > o.x + Ow))
    return false;
  if ((p.y < o.y) || (p.y > o.y + Oh))
    return false;

  return true;
}

static bool AABB(Position p, int16_t Pw, int16_t Ph, Position o, int16_t Ow, int16_t Oh) {
  if ((p.x + Pw <= o.x) || (p.x >= o.x + Ow))
    return false;
  if ((p.y + Ph <= o.y) || (p.y >= o.y + Oh))
    return false;

  return true;
}

static bool distance(Position p1, Position p2) {
  int16_t dx = p1.x - p2.x;
  int16_t dy = p1.y - p2.y;
  return (dx * dx + dy * dy) > (128 * 128);
}

void Update_Mouse(Cursor *c, GameState gamestate) {

  MoveCursor(c, get_mouse_reading());
  ButtonPressed(c, get_mouse_reading());
}

void MoveCursor(Cursor *c, mouse_reading_t mouse_reading) {
  const int16_t dx = mouse_reading.elem.delta_x;
  const int16_t dy = mouse_reading.elem.delta_y;
  Position pos = c->position;

  // sense
  const int SENS_DIV = 2;
  // nova posição
  pos.x += dx / SENS_DIV;
  pos.y -= dy / SENS_DIV;

  bool screenCollision = false;
  if (pos.x < 0) {
    pos.x = 0;
    screenCollision = true;
  }
  else if (pos.x > mode_info.XResolution) {
    pos.x = mode_info.XResolution - 1;
    screenCollision = true;
  }

  if (pos.y < 0) {
    pos.y = 0;
    screenCollision = true;
  }
  else if (pos.y > mode_info.YResolution) {
    pos.y = mode_info.YResolution - 1;
    screenCollision = true;
  }

  if (!screenCollision) {
    c->position = pos;
  }
}

static CursorController check_collision(const Position pos, const MapController *mc) {
  CursorController cc = {.idx = 0, .collision = false};
  if(distance(pos, mc->map->player->position)) {
    return cc;
  }

  for (size_t i = 0; i < mc->map->count_elements; i++) {
    Element *e = mc->map->elements[i];
    if (Remove_Block(pos, e->position, e->width, e->height)) {
      cc.collision = true;
      cc.idx = i;
      return cc;
    }
  }
  return cc;
}

static bool delete_block(CursorController cc, MapController *mc) {
  if (cc.collision) {
    Element element = *mc->map->elements[cc.idx];
  
    if(delete (mc, cc.idx)){
      Backpack_add(get_player()->backpack, &element);
    }
  
    return true;
  }
  return false;
}



bool can_place_block(Position pos, MapController *mc, ItemType type) {
  if (pos.y + 32 >= mode_info.YResolution)
    return false;

  if(distance(pos, mc->map->player->position)) {
    return false;
  }

  for (size_t i = 0; i < mc->map->count_elements; i++) {
    Element *e = mc->map->elements[i];
    if (AABB(pos, 32, 32, e->position, e->width, e->height)) {
      return false;
    }
  }

  Player *pl = mc->map->player;
  if (AABB(pos, 32, 32, pl->position, pl->width, pl->height)) {
    return false;
  }

  if(type == ITEM_SAPPLING) {
    for (size_t i = 0; i < mc->map->count_elements; i++) {
      Element *e = mc->map->elements[i];
      bool adjacent_left = (pos.x + 32 == e->position.x &&
                          pos.y < e->position.y + e->height &&
                          pos.y + 32 > e->position.y && e->type == ELEMENT_EARTH);

    bool adjacent_right = (pos.x == e->position.x + e->width &&
                           pos.y < e->position.y + e->height &&
                           pos.y + 32 > e->position.y && e->type == ELEMENT_EARTH);

    bool adjacent_top = (pos.y + 32 == e->position.y &&
                         pos.x < e->position.x + e->width &&
                         pos.x + 32 > e->position.x && e->type == ELEMENT_EARTH);

    bool adjacent_bottom = (pos.y == e->position.y + e->height &&
                            pos.x < e->position.x + e->width &&
                            pos.x + 32 > e->position.x && e->type == ELEMENT_EARTH);

    if (adjacent_left || adjacent_right || adjacent_top || adjacent_bottom) {
      return true;
    }
  }
  return false;
  }
  for (size_t i = 0; i < mc->map->count_elements; i++) {
    Element *e = mc->map->elements[i];

    bool adjacent_left = (pos.x + 32 == e->position.x &&
                          pos.y < e->position.y + e->height &&
                          pos.y + 32 > e->position.y);

    bool adjacent_right = (pos.x == e->position.x + e->width &&
                           pos.y < e->position.y + e->height &&
                           pos.y + 32 > e->position.y);

    bool adjacent_top = (pos.y + 32 == e->position.y &&
                         pos.x < e->position.x + e->width &&
                         pos.x + 32 > e->position.x);

    bool adjacent_bottom = (pos.y == e->position.y + e->height &&
                            pos.x < e->position.x + e->width &&
                            pos.x + 32 > e->position.x);

    if (adjacent_left || adjacent_right || adjacent_top || adjacent_bottom) {
      return true;
    }
  }
  return false;
}

static void add_block(Backpack *b, MapController *mc, Cursor *c) {
  Position aligned_pos;
  aligned_pos.x = (c->position.x / 32) * 32;
  aligned_pos.y = (c->position.y / 32) * 32;

  int sel = b->selected;
  ItemType type = b->slots[sel];
  if (type == ITEM_NONE || b->quantity[sel] == 0)
    return;

  if (!can_place_block(aligned_pos, mc, type))
    return;

  ElementType elem_type = itemType_to_elementType(type);
  Element *e = NULL;
  if (elem_type == ELEMENT_TREE) {
    e = new_Element(elem_type, aligned_pos, 64, 120, false);
    e->data.tree.growth_stage = TREE_SAPPLING;
  }
  else if (elem_type == ELEMENT_EARTH) {
    e = new_Element(elem_type, aligned_pos, 32, 32, false);
  }
  else if (elem_type == ELEMENT_WOOD) {
    e = new_Element(elem_type, aligned_pos, 32, 32, false);
  }
  else {
    return;
  }

  backpack_use_selected(b);
  add(mc, e->type, e->position, e->width, e->height, e->block_up);
  delete_Element(e);
}

void ButtonPressed(Cursor *c, mouse_reading_t mouse_reading) {
  Position pos = c->position;
  if (gamestate == MENU_STATE) {
    if (mouse_reading.elem.lb) {
      Position playButtonPosition = {401, 330};
      Position optionsButtonPosition = {401, 460};
      Position exitButtonPosition = {401, 590};
      if (Remove_Block(pos, playButtonPosition, 350, 114)) {
        gamestate = GAME_STATE;
        MapInit();
      }
      else if (Remove_Block(pos, optionsButtonPosition, 300, 114)) {
        gamestate = OPTIONS_STATE;
        MenuInit();
      }
      else if (Remove_Block(pos, exitButtonPosition, 300, 114)) {
        gamestate = EXIT_STATE;
      }
    }
  }
  else if (gamestate == GAME_STATE) {
    MapController *mc = get_MapController();
    Position pauseButtonPosition = {20, 20};
    if (mouse_reading.elem.lb) {
      if (Remove_Block(pos, pauseButtonPosition, 92, 92)) {
        gamestate = PAUSE_STATE;
        MenuInit();
      }
      CursorController cc = check_collision(pos, mc);
      bool remove = delete_block(cc, mc);
      if (remove) {
        check_up(mc);
      }
    }
    else if (mouse_reading.elem.rb) {
      Backpack *b = get_player()->backpack;
      add_block(b, mc, c);
      check_up(mc);
    }
  }
  else if (gamestate == PAUSE_STATE) {
    if (mouse_reading.elem.lb) {
      Position playButtonPosition = {401, 220};
      Position exitButtonPosition = {401, 350};
      if (Remove_Block(pos, playButtonPosition, 300, 114)) {
        gamestate = GAME_STATE;
      }
      else if (Remove_Block(pos, exitButtonPosition, 300, 114)) {
        gamestate = MENU_STATE;
      }
    }
  }
  else if (gamestate == OPTIONS_STATE) {
    if (mouse_reading.elem.lb) {

      Position arrowPosition = {20, 20};
      Position mode1Position = {401, 330};
      Position mode2Position = {401, 460};
      Position mode3Position = {401, 590};
      if (Remove_Block (pos, arrowPosition, 92,92)){
        gamestate = MENU_STATE;
      }
      else if (Remove_Block(pos, mode1Position, 350,114)){
        change_video_mode(0x115);
      }
      else if (Remove_Block(pos, mode2Position, 350,114)){
        change_video_mode(0x14C);
      }
      else if (Remove_Block(pos, mode3Position, 350,114)){
        change_video_mode(0x11A);
      }
    }
  }
}

