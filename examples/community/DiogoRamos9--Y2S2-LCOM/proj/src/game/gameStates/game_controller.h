#include <lcom/lcf.h>

typedef enum {
    MENU,
    GAME,
    SETTINGS,
    INSTRUCTIONS,
    EXIT,
    GAME_OVER,
    LEADERBOARD,
    PAUSE
} GameState;

void draw_lasers();

void draw_cannon();

void draw_asteroids();

void draw_game();

void create_laser();

void create_asteroid();

void update_lasers();

void update_asteroids();

void check_collisions();

void reset_game();

void draw_text(char text[], int x, int y);

void draw_score();

void draw_ammo();

void create_powerup();

void draw_powerups();
