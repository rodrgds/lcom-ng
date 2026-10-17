#include "game.h"

/**
 * @brief Represents a single Game instance.
 *
 * The Game struct is opaque.
 */
struct Game {
    State state;          /**< Current state of the game */
    uint8_t level_number; /**< Current level number */
    uint8_t score;        /**< Max score of the player */
    Level *level;         /**< Pointer to the current level */
    Cursor *cursor;       /**< Pointer to the cursor instance */
    union {
        struct MainMenu *main_menu; /**< Pointer to the main menu instance */
        struct Victory *victory;    /**< Pointer to the victory menu instance */
        struct GameOver *game_over; /**< Pointer to the game over menu instance */
    } menu;                         /**< Union to hold different menu instances */
    bool shooting;                  /**< Indicates if the player is shooting */
};

/* Create and destroy section */

static void set_cursor(Game *game, CursorMode mode) {
    if (game == NULL) {
        return;
    }
    uint16_t x = cursor_get_x(game->cursor);
    uint16_t y = cursor_get_y(game->cursor);
    if (destroy_cursor(game->cursor)) {
        printf("set_cursor: destroy_cursor failed\n");
        return;
    }
    game->cursor = create_cursor(mode, x, y);
}

static void menu_init(Game *game) {
    if (game == NULL) {
        return;
    }
    set_cursor(game, CURSOR_DEFAULT);
    game->menu.main_menu = create_main_menu();
}

static void level_init(Game *game) {
    if (game == NULL) {
        return;
    }
    set_cursor(game, CURSOR_CROSSHAIR);
    game->level = create_level(game->level_number);
}

static void victory_init(Game *game) {
    if (game == NULL) {
        return;
    }
    set_cursor(game, CURSOR_DEFAULT);
    game->score = game->level_number;
    game->level_number++;

    game->menu.victory = create_victory();
}

static void game_over_init(Game *game) {
    if (game == NULL) {
        return;
    }
    set_cursor(game, CURSOR_DEFAULT);
    game->score = game->level_number;
    game->level_number = 0;

    game->menu.game_over = create_game_over();
}

static void exit_init(Game *game) {
    if (game == NULL) {
        return;
    }
    printf("exiting game\n");
}

static void (*game_init[])(Game *game) = {
    menu_init,
    level_init,
    victory_init,
    game_over_init,
    exit_init};

static void state_init(Game *game) {
    if (game == NULL) {
        return;
    }
    game_init[game->state](game);
}

Game *create_game() {
    Game *game = (Game *) malloc(sizeof(Game));
    if (!game)
        return NULL;

    game->level_number = 0;
    game->score = 0;
    game->state = MENU;
    game->cursor = create_cursor(CURSOR_DEFAULT, x_res / 2, y_res / 2);
    game->shooting = false;
    state_init(game);

    return game;
}

/* Statics section */

// Destroy
static void menu_destroy(Game *game) {
    if (game == NULL) {
        return;
    }
    if (destroy_main_menu(game->menu.main_menu)) {
        printf("menu_destroy: destroy_main_menu failed\n");
        return;
    }
}

static void level_destroy(Game *game) {
    if (game == NULL) {
        return;
    }
    destroy_level(game->level);
}

static void victory_destroy(Game *game) {
    if (game == NULL) {
        return;
    }
    if (destroy_victory(game->menu.victory)) {
        printf("victory_destroy: destroy_victory failed\n");
        return;
    }
}

static void game_over_destroy(Game *game) {
    if (game == NULL) {
        return;
    }
    if (destroy_game_over(game->menu.game_over)) {
        printf("game_over_destroy: destroy_game_over failed\n");
        return;
    }
}

static void exit_destroy(Game *game) {
    if (game == NULL) {
        return;
    }
    printf("This function should never be called\n");
}

static void (*game_destroy[])(Game *game) = {
    menu_destroy,
    level_destroy,
    victory_destroy,
    game_over_destroy,
    exit_destroy};

static void state_destroy(Game *game) {
    if (game == NULL) {
        return;
    }
    game_destroy[game->state](game);
}

void destroy_game(Game *game) {
    if (game == NULL) {
        printf("destroy_game: NULL pointer provided\n");
        return;
    }

    state_destroy(game);
    destroy_cursor(game->cursor);
    free(game);
}

static void set_state(Game *game, State new_state) {
    if (game == NULL) {
        return;
    }
    state_destroy(game);
    game->state = new_state;
    state_init(game);
}

// Timer handlers

static void menu_timer_handler(Game *game) {
    if (game == NULL) {
        return;
    }
    clear_frame_buffer(secondary_frame_buffer, 0);

    if (draw_main_menu(game->menu.main_menu, secondary_frame_buffer)) {
        printf("menu_timer_handler: draw_main_menu failed\n");
        return;
    }
    if (draw_cursor(game->cursor, secondary_frame_buffer)) {
        printf("menu_timer_handler: draw_cursor failed\n");
        return;
    }

    vga_flip_pages();
}
static void level_timer_handler(Game *game) {
    if (game == NULL) {
        return;
    }
    clear_frame_buffer(secondary_frame_buffer, IN_FOV_COLOR);

    if (draw_level(game->level, pp)) {
        printf("level_timer_handler: draw_level failed\n");
        return;
    }

    if (draw_cursor(game->cursor, secondary_frame_buffer)) {
        printf("level_timer_handler: draw_cursor failed\n");
        return;
    }

    vga_flip_pages();

    if (player_get_state(get_player(game->level)) == PLAYER_DEAD) {
        set_state(game, GAME_OVER);
        return;
    }

    if (level_get_player_state(game->level) == PLAYER_WIN) {
        set_state(game, VICTORY);
        return;
    }
}

static void victory_timer_handler(Game *game) {
    if (game == NULL) {
        return;
    }
    clear_frame_buffer(secondary_frame_buffer, 0);

    if (draw_victory(game->menu.victory, secondary_frame_buffer)) {
        printf("victory_timer_handler: draw_victory failed\n");
        return;
    }
    if (draw_cursor(game->cursor, secondary_frame_buffer)) {
        printf("victory_timer_handler: draw_cursor failed\n");
        return;
    }

    vga_flip_pages();
}

static void game_over_timer_handler(Game *game) {
    if (game == NULL) {
        return;
    }
    clear_frame_buffer(secondary_frame_buffer, 0);

    if (draw_game_over(game->menu.game_over, secondary_frame_buffer)) {
        printf("game_over_timer_handler: draw_game_over failed\n");
        return;
    }
    if (draw_cursor(game->cursor, secondary_frame_buffer)) {
        printf("game_over_timer_handler: draw_cursor failed\n");
        return;
    }

    vga_flip_pages();
}

static void exit_timer_handler(Game *game) {
    if (game == NULL) {
        printf("exit_timer_handler: NULL pointer provided.\n");
        return;
    }
    printf("exiting\n");
}

static void (*game_timer_handlers[])(Game *game) = {
    menu_timer_handler,
    level_timer_handler,
    victory_timer_handler,
    game_over_timer_handler,
    exit_timer_handler};

void game_timer_handler(Game *game) {
    if (game == NULL) {
        return;
    }
    game_timer_handlers[game->state](game);
}

// Keyboard handlers

static void menu_keyboard_handler(Game *game) {
    if (game == NULL) {
        return;
    }
    if (scan_code == ESC_BREAK_CODE) {
        set_state(game, EXIT);
        return;
    }
    if (scan_code == KEY_W || scan_code == KEY_S || scan_code == KEY_A || scan_code == KEY_D) {
        if (main_menu_change_button(game->menu.main_menu)) {
            printf("menu_keyboard_handler: main_menu_change_button failed\n");
            return;
        }
    }
    
    if (scan_code == KEY_ENTER) {
        ButtonType button = main_menu_get_button(game->menu.main_menu);
        if (button == BUTTON_PLAY) {
            set_state(game, LEVEL);
            return;
        } else if (button == BUTTON_EXIT) {
            set_state(game, EXIT);
            return;
        }
    }
}

static void level_keyboard_handler(Game *game) {
    if (game == NULL) {
        return;
    }
    if (level_update_position(game->level, scan_code)) {
        printf("level_keyboard_handler: level_update_position failed\n");
        return;
    }
    if (scan_code == ESC_BREAK_CODE) {
        set_state(game, MENU);
        return;
    }
}

static void game_over_keyboard_handler(Game *game) {
    if (game == NULL) {
        return;
    }
    if (scan_code == ESC_BREAK_CODE) {
        set_state(game, EXIT);
        return;
    }
    if (scan_code == KEY_W || scan_code == KEY_S || scan_code == KEY_A || scan_code == KEY_D)
        if (game_over_change_button(game->menu.game_over)) {
            printf("game_over_keyboard_handler: game_over_change_button failed\n");
            return;
        }
    if (scan_code == KEY_ENTER) {
        ButtonType button = game_over_get_button(game->menu.game_over);
        if (button == BUTTON_MENU) {
            set_state(game, MENU);
            return;
        } else if (button == BUTTON_EXIT) {
            set_state(game, EXIT);
            return;
        }
    }
}

static void victory_keyboard_handler(Game *game) {
    if (game == NULL) {
        return;
    }
    if (scan_code == ESC_BREAK_CODE) {
        set_state(game, EXIT);
        return;
    }
    if (scan_code == KEY_W || scan_code == KEY_S || scan_code == KEY_A || scan_code == KEY_D)
        if (victory_change_button(game->menu.victory)) {
            printf("victory_keyboard_handler: victory_change_button failed\n");
            return;
        }
    if (scan_code == KEY_ENTER) {
        ButtonType button = victory_get_button(game->menu.victory);
        if (button == BUTTON_NEXT) {
            set_state(game, LEVEL);
            return;
        } else if (button == BUTTON_EXIT) {
            set_state(game, EXIT);
            return;
        }
    }
}

static void exit_keyboard_handler(Game *game) {
    if (game == NULL) {
        return;
    }
    printf("This function should never be called\n");
}

static void (*game_keyboard_handlers[])(Game *game) = {
    menu_keyboard_handler,
    level_keyboard_handler,
    victory_keyboard_handler,
    game_over_keyboard_handler,
    exit_keyboard_handler};

void game_keyboard_handler(Game *game) {
    if (game == NULL) {
        return;
    }
    game_keyboard_handlers[game->state](game);
}

// Mouse handlers

static void menu_mouse_handler(Game *game) {
    if (game == NULL) {
        return;
    }
    ButtonType button = BUTTON_NONE;
    if (pp.lb)
        button = main_menu_click_handler(game->menu.main_menu, cursor_get_x(game->cursor), cursor_get_y(game->cursor));
    if (button == BUTTON_PLAY) {
        set_state(game, LEVEL);
        return;
    } else if (button == BUTTON_EXIT) {
        set_state(game, EXIT);
        return;
    }
}

static void level_mouse_handler(Game *game) {
    if (game == NULL) {
        return;
    }
    if (level_update_delta(game->level, cursor_get_x(game->cursor), cursor_get_y(game->cursor))) {
        printf("level_mouse_handler: level_update_delta failed\n");
        return;
    }
    if (pp.lb) {
        if (!game->shooting)
            if (level_shoot(game->level)) {
                printf("level_mouse_handler: level_shoot failed\n");
                return;
            }
        game->shooting = true;
    } else {
        game->shooting = false;
    }
}

static void victory_mouse_handler(Game *game) {
    if (game == NULL) {
        return;
    }
    ButtonType button = BUTTON_NONE;
    if (pp.lb)
        button = victory_click_handler(game->menu.victory, cursor_get_x(game->cursor), cursor_get_y(game->cursor));
    if (button == BUTTON_NEXT) {
        set_state(game, LEVEL);
        return;
    } else if (button == BUTTON_EXIT) {
        set_state(game, EXIT);
        return;
    }
}

static void game_over_mouse_handler(Game *game) {
    if (game == NULL) {
        return;
    }
    ButtonType button = BUTTON_NONE;
    if (pp.lb)
        button = game_over_click_handler(game->menu.game_over, cursor_get_x(game->cursor), cursor_get_y(game->cursor));
    if (button == BUTTON_MENU) {
        set_state(game, MENU);
        return;
    } else if (button == BUTTON_EXIT) {
        set_state(game, EXIT);
        return;
    }
}

static void exit_mouse_handler(Game *game) {
    if (game == NULL) {
        return;
    }
    printf("This function should never be called\n");
}

static void (*game_mouse_handlers[])(Game *game) = {
    menu_mouse_handler,
    level_mouse_handler,
    victory_mouse_handler,
    game_over_mouse_handler,
    exit_mouse_handler};

void game_mouse_handler(Game *game) {
    if (game == NULL) {
        return;
    }
    if (cursor_update(game->cursor, pp.delta_x, -pp.delta_y)) {
        printf("game_mouse_handler: cursor_update failed\n");
        return;
    }
    game_mouse_handlers[game->state](game);
}

/* Getter and setter section */

State get_state(Game *game) {
    if (game == NULL) {
        return EXIT;
    }
    return game->state;
}
