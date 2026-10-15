#include "app.h"

app_t *app_init() {
  app_t *app = (app_t *) malloc(sizeof(app_t));
  if (app == NULL)
    return NULL;

  app->state = MAIN_MENU;
  app->game.cards = NULL;
  app->game.main_player.cards = NULL;
  app->game.other_player.cards = NULL;
  app->game.dealer = NULL;
  app->game.curr_anim = NULL;

  if (cursor_init(&app->cursor))
    return NULL;

  app->background = sprite_create((xpm_map_t) background_xpm);
  if (app->background == NULL)
    return NULL;

  app->button_bet = sprite_create((xpm_map_t) bet_xpm);
  if (app->button_bet == NULL)
    return NULL;
  sprite_move(app->button_bet, 620, vg_get_height() - 93);

  if (app_buttons_main_menu_init(&app->buttons_main_menu))
    return NULL;

  if (app_buttons_game_playing_init(&app->buttons_game_playing))
    return NULL;

  if (app_buttons_game_over_init(&app->buttons_game_over))
    return NULL;

  font_init(&app->font);
  banner_init(&app->banner);

  return app;
}

int app_buttons_game_playing_init(queue_t **queue) {
  sprite_t *sprite = NULL;

  if (queue == NULL)
    return 1;
  *queue = queue_create(4);
  if (*queue == NULL)
    return 1;

  sprite = sprite_create((xpm_map_t) hit_xpm);
  if (sprite == NULL)
    return 1;
  queue_push(*queue, sprite);

  sprite = sprite_create((xpm_map_t) stand_xpm);
  if (sprite == NULL)
    return 1;
  queue_push(*queue, sprite);

  sprite = sprite_create((xpm_map_t) double_xpm);
  if (sprite == NULL)
    return 1;
  queue_push(*queue, sprite);

  sprite = sprite_create((xpm_map_t) surrender_xpm);
  if (sprite == NULL)
    return 1;
  queue_push(*queue, sprite);

  return 0;
}

int app_buttons_main_menu_init(queue_t **queue) {
  sprite_t *sprite = NULL;

  if (queue == NULL)
    return 1;
  *queue = queue_create(2);
  if (*queue == NULL)
    return 1;

  sprite = sprite_create((xpm_map_t) start_xpm);
  if (sprite == NULL)
    return 1;
  queue_push(*queue, sprite);

  sprite = sprite_create((xpm_map_t) exit_xpm);
  if (sprite == NULL)
    return 1;
  queue_push(*queue, sprite);

  return 0;
}

int app_buttons_game_over_init(queue_t **queue) {
  sprite_t *sprite = NULL;

  if (queue == NULL)
    return 1;
  *queue = queue_create(2);
  if (*queue == NULL)
    return 1;

  sprite = sprite_create((xpm_map_t) rebet_xpm);
  if (sprite == NULL)
    return 1;
  queue_push(*queue, sprite);

  sprite = sprite_create((xpm_map_t) exit_xpm);
  if (sprite == NULL)
    return 1;
  queue_push(*queue, sprite);

  return 0;
}

void app_destroy(app_t *app) {
  if (app == NULL)
    return;

  queue_destroy(&app->buttons_main_menu, sprite_queue_destroy);
  queue_destroy(&app->buttons_game_playing, sprite_queue_destroy);
  queue_destroy(&app->buttons_game_over, sprite_queue_destroy);
  sprite_destroy(app->background);
  sprite_destroy(app->button_bet);
  game_destroy(&app->game);
  cursor_destroy(&app->cursor);
  font_destroy(&app->font);

  free(app);
}
