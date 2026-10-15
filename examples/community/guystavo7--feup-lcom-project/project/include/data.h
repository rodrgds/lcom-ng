#ifndef DATA
#define DATA

/** @file data.h
 *  @brief Parametros estaticos do jogo: caminhos de ficheiros de mapa, dimensoes do ecra, tags,
 *         stats de torres/monstros, posicoes de UI e macros de balanceamento.
 */

#include "../resources/maps/map1/map1.h"
#include "../resources/maps/map2/map2.h"
#include "utils/vec2d.h"

// Tags
#define WALLS_TAG 0
#define PLAYER_TAG 1
#define MONSTER_TAG 2
#define COLLECTABLE_TAG 3
#define PATH_TAG 4
#define UI_TAG 5
#define LAKE_TAG 6
#define TOWER_TAG 7

#define TAGS_NUM 8

// File Paths

/*
static const char* map1 = "/home/lcom/labs/project/resources/maps/map1/walls.csv";
static const char* path1 = "/home/lcom/labs/project/resources/maps/map1/path.csv";
static const char* lake1 = "/home/lcom/labs/project/resources/maps/map1/water.csv";

static const char* map2 = "/home/lcom/labs/project/resources/maps/map2/walls.csv";
static const char* path2 = "/home/lcom/labs/project/resources/maps/map2/path.csv";
static const char* lake2 = "/home/lcom/labs/project/resources/maps/map2/water.csv";


static const char* map3= "/home/lcom/labs/project/resources/maps/map3/walls.csv";
static const char* path3 = "/home/lcom/labs/project/resources/maps/map3/path.csv";
static const char* lake3 = "/home/lcom/labs/project/resources/maps/map3/water.csv";

*/

static const char* map1 = "/home/lcom/labs/grupo_2leic13_2/project/resources/maps/map1/walls.csv";
static const char* path1 = "/home/lcom/labs/grupo_2leic13_2/project/resources/maps/map1/path.csv";
static const char* lake1 = "/home/lcom/labs/grupo_2leic13_2/project/resources/maps/map1/water.csv";

static const char* map2 = "/home/lcom/labs/grupo_2leic13_2/project/resources/maps/map2/walls.csv";
static const char* path2 = "/home/lcom/labs/grupo_2leic13_2/project/resources/maps/map2/path.csv";
static const char* lake2 = "/home/lcom/labs/grupo_2leic13_2/project/resources/maps/map2/water.csv";

static const char* map3= "/home/lcom/labs/grupo_2leic13_2/project/resources/maps/map3/walls.csv";
static const char* path3 = "/home/lcom/labs/grupo_2leic13_2/project/resources/maps/map3/path.csv";
static const char* lake3 = "/home/lcom/labs/grupo_2leic13_2/project/resources/maps/map3/water.csv";

// Screen configs
#define MODE 0x115
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAP_TILES_X 25
#define MAP_TILES_Y 19
#define TILE_SIZE 32

#define PLACEMENT_QUERY_MAX 128
#define POPUP_ICON_MARGIN 14
#define TOWER_PICK_ICON_W 60

static const Vec2d EMPTY_VEC2D = {0, 0};
static const Vec2d PLAYER_POS = {100, 300};

// Level selector
#define MAX_LVL 3
#define FULL_IMAGE_INDEX 0
#define SMALL_IMAGE_INDEX 1

#define LVL_SELECT_PLAY_NORMAL 0
#define LVL_SELECT_PLAY_PRESSED 1
#define LVL_SELECT_BTN_NORMAL 2
#define LVL_SELECT_BTN_PRESSED 3
#define LVL_SELECT_LOGO 4

static const Vec2d LEVEL_SELECTOR_POS = {SCREEN_WIDTH / 2 - 318 / 2, 10};
static const Vec2d LEVEL_DISPLAY_POS = {SCREEN_WIDTH / 2 - 395 / 2, 150};

static const Vec2d PLAY_BUTTON_POS = {SCREEN_WIDTH / 2 - 283 / 2, 500};
static const Vec2d PLAY_BUTTON_SIZE = {283, 94};

static const Vec2d LEFT_SWITCH_BUTTON_POS = {10, SCREEN_HEIGHT / 2 - 96 / 2};
static const Vec2d RIGHT_SWITCH_BUTTON_POS = {SCREEN_WIDTH - 100, SCREEN_HEIGHT / 2 - 96 / 2};
static const Vec2d RED_BUTTON_SIZE = {100, 96};

// Fruit
static const Vec2d FRUIT_SIZE = {32, 32};
#define FRUIT_TIME_ON_SCREEN_SEC 15.0f
#define FRUIT_MONSTER_DROP_PERCENT 65
#define FRUIT_MONSTER_ROLL_BANANA_BELOW 50
#define FRUIT_MONSTER_ROLL_ABACATE_BELOW 80

#define FRUIT_CARROT_SPAWN_INTERVAL_SEC 22.0f
#define FRUIT_CARROT_SAMPLE_TRIES 48

// Player
#define PLAYER_MAX_VELOCITY 200
#define PLAYER_FRAME_SPEED 10
#define PLAYER_IDLE_FRAMES 8
#define PLAYER_RUNNING_FRAMES 6
static const Vec2d PLAYER_HIT_BOX_OFFSET = {10, 33};
static const Vec2d PLAYER_SIZE = {17, 13};
 

// Tower
#define NORMAL_DAMAGE 500
#define PIERCING_DAMAGE 800
#define FAST_DAMAGE 500
#define SKY_FALL_DAMAGE 3500
#define NEAR_DAMAGE 700

static const Vec2d BASIC_TOWER_SIZE = {43, 29};
static const Vec2d BASIC_TOWER_HIT_BOX_OFFSET = {9, 0};
static const Vec2d BASIC_UPPER_HALF_OFFSET = {0, -59};

// Basic tower
static const Vec2d BASIC_TOWER_RANGE = {250, 250};
static const Vec2d BASIC_TOWER_RANGE_OFFSET = {(60 - 250) / 2, (11 - 250) / 2};
#define BASIC_TOWER_COOLDOWN 0.5
#define BASIC_TOWER_SHOOT_SPEED 10
#define ARROW_FRAMES 8
#define ARROW_EPSILON 0.2f
#define ARROW_SPEED 1000
#define ARROW_FRAME_SPEED 10

#define ASTEROID_FRAMES 3
#define SKY_FALL_ARROW_SPEED 700
#define ASTEROID_FRAME_SPEED 10
static const Vec2d ASTEROID_SIZE = {140, 268};

#define LIGHTNING_FRAMES 4
#define LIGHTNING_SPEED 10
static const Vec2d LIGHTNING_SIZE = {122, 120};

// Fast Tower
static const Vec2d FAST_TOWER_RANGE = {250, 250};
static const Vec2d FAST_TOWER_RANGE_OFFSET = {(60 - 250) / 2, (11 - 250) / 2};
#define FAST_TOWER_COOLDOWN 0.05
#define FAST_TOWER_SHOOT_SPEED 30

// Piercing Tower
static const Vec2d PIERCING_TOWER_RANGE = {350, 350};
static const Vec2d PIERCING_TOWER_RANGE_OFFSET = {(60 - 350) / 2, (11 - 350) / 2};
#define PIERCING_TOWER_COOLDOWN 0.5
#define PIERCING_TOWER_SHOOT_SPEED 10

// Mortar Tower
static const Vec2d MORTAR_TOWER_RANGE = {500, 500};
static const Vec2d MORTAR_TOWER_RANGE_OFFSET = {(60 - 500) / 2, (11 - 500) / 2};
#define MORTAR_COOLDOWN 5
#define MORTAR_TOWER_SHOOT_SPEED 10

// Eletric Tower
static const Vec2d ELETRICT_TOWER_RANGE = {200, 200};
static const Vec2d ELETRICT_TOWER_RANGE_OFFSET = {(60 - 200) / 2, (11 - 200) / 2};
#define ELETRICT_COOLDOWN 0.3
#define ELETRICT_TOWER_SHOOT_SPEED 15

// Cost
static const int BASIC_TOWER_COST[4] = {4, 0, 0, 0};
static const int LONG_TOWER_COST[4] = {4, 0, 0, 0};
static const int FAST_TOWER_COST[4] = {2, 4, 0, 0};
static const int PIERCING_TOWER_COST[4] = {2, 0, 3, 0};
static const int MORTAR_TOWER_COST[4] = {0, 0, 2, 4};
static const int ELETRICT_TOWER_COST[4] = {0, 3, 0, 4};
static const int* TOWER_COST[6] = {BASIC_TOWER_COST, FAST_TOWER_COST, PIERCING_TOWER_COST, MORTAR_TOWER_COST, ELETRICT_TOWER_COST, LONG_TOWER_COST};

// Archer
#define ARCHER_FRAME_SPEED 10
#define ARCHER_IDLE_FRAMES 6
#define ARCHER_SHOOT_FRAMES 8
static const Vec2d ARCHER_OFFSET = {30 - 18, -27 - 44}; // x = tower center / 2 - archer / 2, y = upper tower some height - archer height

// Monk
#define MONK_FRAME_SPEED 6
#define MONK_IDLE_FRAMES 6
#define MONK_HEAL_FRAMES 11
static const Vec2d MONK_OFFSET = {30 - 14, -27 - 34};
static const Vec2d MONK_HEAL_OFFSET = {30 - 14 - 15, -27 - 34};

// Dust
#define DUST_FRAME_SPEED 10
#define DUST_FRAMES 9

// Cursor
#define CURSOR_FRAMES 4

// Tower upgrade menu
#define UPGRADE_SCROLL_W 376
#define UPGRADE_SCROLL_H 307
#define UPGRADE_SCROLL_GAP 16
#define UPGRADE_SCROLL_TOTAL_W (UPGRADE_SCROLL_W + UPGRADE_SCROLL_GAP + UPGRADE_SCROLL_W)

#define UPGRADE_MENU_XPM_BASIC 0
#define UPGRADE_MENU_XPM_PIERCING 1
#define UPGRADE_MENU_XPM_MORTAR 2
#define UPGRADE_MENU_XPM_ELECTRICT 3
#define UPGRADE_MENU_XPM_FAST 4
#define UPGRADE_MENU_XPM_LONG 5

#define UPGRADE_SCROLL_ORIGIN_X ((SCREEN_WIDTH - UPGRADE_SCROLL_TOTAL_W) / 2)
#define UPGRADE_SCROLL_ORIGIN_Y ((SCREEN_HEIGHT - UPGRADE_SCROLL_H) / 2)
#define UPGRADE_SCROLL_LEFT_X UPGRADE_SCROLL_ORIGIN_X
#define UPGRADE_SCROLL_RIGHT_X (UPGRADE_SCROLL_ORIGIN_X + UPGRADE_SCROLL_W + UPGRADE_SCROLL_GAP)

// Hammer button
#define UPGRADE_HAMMER_NORMAL 0
#define UPGRADE_HAMMER_PRESSED 1
#define UPGRADE_HAMMER_FRAMES 2
#define UPGRADE_HAMMER_FRAME_SPEED 10
#define UPGRADE_HAMMER_W 63
#define UPGRADE_HAMMER_H 60
#define UPGRADE_HAMMER_BOTTOM_MARGIN 50

#define UPGRADE_HAMMER_LEFT_X (UPGRADE_SCROLL_LEFT_X + (UPGRADE_SCROLL_W - UPGRADE_HAMMER_W) / 2)
#define UPGRADE_HAMMER_LEFT_Y (UPGRADE_SCROLL_ORIGIN_Y + UPGRADE_SCROLL_H - UPGRADE_HAMMER_H - UPGRADE_HAMMER_BOTTOM_MARGIN)
#define UPGRADE_HAMMER_RIGHT_X (UPGRADE_SCROLL_RIGHT_X + (UPGRADE_SCROLL_W - UPGRADE_HAMMER_W) / 2)
#define UPGRADE_HAMMER_RIGHT_Y UPGRADE_HAMMER_LEFT_Y

// Main Menu
#define NEW_GAME_BUTTON_NORMAL 0
#define NEW_GAME_BUTTON_PRESSED 1
#define EXIT_BUTTON_NORMAL 2
#define EXIT_BUTTON_PRESSED 3
#define LOGO 4

#define NEW_GAME_BUTTON_FRAMES 2
#define EXIT_BUTTON_FRAMES 2
#define BUTTONS_FRAME_SPEED 10

static const Vec2d LOGO_POSITION = {400 - 194, 100};

static const Vec2d NEW_GAME_BUTTON_POS = {400 - 282 / 2, 300};
static const Vec2d NEW_GAME_BUTTON_SIZE = {282, 94};
static const Vec2d EXIT_BUTTON_POS = {400 - 282 / 2, 450};
static const Vec2d EXIT_BUTTON_SIZE = {282, 94};

// Paused Menu
#define BUILD_NORMAL_BUTTON_INDEX 0
#define BUILD_PRESSED_BUTTON_INDEX 1
#define RESUME_NORMAL_BUTTON_INDEX 2
#define RESUME_PRESSED_BUTTON_INDEX 3
#define GO_BACK_NORMAl_BUTTON_INDEX 4
#define GO_BACK_PRESSED_BUTTON_INDEX 5
#define HELP_NORMAl_BUTTON_INDEX 6
#define HELP_PRESSED_BUTTON_INDEX 7

#define PAUSE_MENU_INDEX 0
static const Vec2d HOW_TO_PLAY_POSITION = {400 - 440 / 2, 100};

// Game over
#define GAME_OVER_EXIT_NORMAL 0
#define GAME_OVER_EXIT_PRESSED 1
#define GAME_OVER_LOGO 2
#define GAME_OVER_RESTART_NORMAL 3
#define GAME_OVER_RESTART_PRESSED 4

#define GAME_OVER_EXIT_FRAMES 2
#define GAME_OVER_RESTART_FRAMES 2
static const Vec2d GAME_OVER_POSITION = {400 - 283 / 2, 200};

static const Vec2d PAUSE_MENU_ICON_SIZE = {63, 60};
static const Vec2d PAUSE_MENU_BG_POS = {SCREEN_WIDTH / 2 - 295 / 2, SCREEN_HEIGHT / 2 - 252 / 2};
static const Vec2d RESUME_POS = {SCREEN_WIDTH / 2 - 259 / 2 + 20, SCREEN_HEIGHT / 2 - 252 / 2 + 124};
static const Vec2d HELP_POS = {SCREEN_WIDTH / 2 - 259 / 2 + 100, SCREEN_HEIGHT / 2 - 252 / 2 + 124};
static const Vec2d GO_BACK_POS = {SCREEN_WIDTH / 2 - 259 / 2 + 170, SCREEN_HEIGHT / 2 - 252 / 2 + 124};

// Win menu
#define WIN_BACKGROUND 0
#define WIN_BUTTON_NORMAL 2
#define WIN_BUTTON_PRESSED 1
static const Vec2d WIN_INFO_POSITION = {SCREEN_WIDTH / 2 - 450 / 2, 10};
static const Vec2d WIN_BUTTON_POSITION = {SCREEN_WIDTH / 2 - 283 / 2, 500};

// UI
#define HEALTH_BAR_INDEX 0
#define STATS_BAR_INDEX 1

static const Vec2d HEALTH_BAR_POSITION = {SCREEN_WIDTH - 162, 10};
static const Vec2d HEALTH_VALUE_POSITION = {SCREEN_WIDTH - 162 / 2, 20};

static const Vec2d STATS_BAR_POSITION = {SCREEN_WIDTH - 115, 20 + 52};
static const Vec2d BANANA_STATS_POSITION = {SCREEN_WIDTH - 115 + 60, 20 + 52 + 7};
static const Vec2d AVOCADO_STATS_POSITION = {SCREEN_WIDTH - 115 + 60, 20 + 52 + 39};
static const Vec2d CHERRY_STATS_POSITION = {SCREEN_WIDTH - 115 + 60, 20 + 52 + 74};
static const Vec2d CARROT_STATS_POSITION = {SCREEN_WIDTH - 115 + 60, 20 + 52 + 112};

// Numbers
#define NUMBER_WIDTH 14

// Monsters
// Pawn
#define PAWN_HEALTH 2000
#define PAWN_SPEED 100

// Alien
#define ALIEN_HEALTH 2000
#define ALIEN_SPEED 200

// Lancer
#define LANCER_HEALTH 7000
#define LANCER_SPEED 75 

// Boss
#define BOSS_HEALTH 99999
#define BOSS_SPEED 25

#endif
