#include "../include/state.h"

static app_state_t app_state;
static action_t action = NOTHING;
static struct packet* pp = NULL;

app_state_t 
get_app_state()
{
  return app_state;
}

int 
set_app_state_menu()
{
  if (app_state == MENU) return 1;
  app_state = MENU;
  return 0;
}

int 
set_app_state_game()
{
  if (app_state == GAME) return 1;
  app_state = GAME;
  return 0;
}

int 
set_app_state_null()
{
  if (app_state == SNULL) return 1;
  app_state = SNULL;
  return 0;
}

void 
state_step()
{
  switch (app_state) {
    case GAME: game_state_step(action); break;
    case MENU: menu_state_step(action); break;
    case SNULL: break;
  }
}

void
state_set_action(action_t act)
{
  action = act;
}

action_t
state_get_action()
{
  return action;
}

void 
state_set_packet(struct packet* packet)
{
  pp = packet;
}

struct packet*
state_get_packet()
{
  return pp;
}
