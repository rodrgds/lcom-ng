#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#define FPS 60.0f
#define DELTA_TIME (1.0f / FPS)

// --- Screen Constants ---
#define SCREEN_W 800
#define SCREEN_H 600

// --- Keyboard Constants ---
#define ESC_BREAKCODE   0x81   /* ESC key released */
#define ENTER_BREAKCODE 0x9C   /* ENTER key released */
#define S_BREAKCODE     0x9F
#define SPACE_BREAKCODE 0xB9

#define KEY_1_BREAKCODE 0x82
#define KEY_2_BREAKCODE 0x83
#define KEY_3_BREAKCODE 0x84
#define KEY_4_BREAKCODE 0x85
#define KEY_5_BREAKCODE 0x86
#define KEY_6_BREAKCODE 0x87
#define KEY_7_BREAKCODE 0x88
#define KEY_8_BREAKCODE 0x89
#define KEY_9_BREAKCODE 0x8A
#define KEY_0_BREAKCODE 0x8B

#define EXTENDED_SCANCODE 0xE0
#define ARROW_UP_MAKE    0x48
#define ARROW_LEFT_MAKE  0x4B
#define ARROW_RIGHT_MAKE 0x4D
#define ARROW_DOWN_MAKE  0x50
#define Q_BREAKCODE     0x90   /* Q key released - Quit to main menu */
#define X_BREAKCODE     0xAD   /* X key released - Close game entirely */

/* Teclas 1-6 (break codes) — seleção rápida de plantas in-game */
#define KEY1_BREAKCODE  0x82
#define KEY2_BREAKCODE  0x83
#define KEY3_BREAKCODE  0x84
#define KEY4_BREAKCODE  0x85
#define KEY5_BREAKCODE  0x86
#define KEY6_BREAKCODE  0x87

/* Teclas de seta (extended break codes — enviadas após prefixo 0xE0) */
#define KBD_EXTENDED      0xE0
#define ARROW_UP_BREAK    0xC8
#define ARROW_DOWN_BREAK  0xD0
#define ARROW_LEFT_BREAK  0xCB
#define ARROW_RIGHT_BREAK 0xCD

// --- Mouse Constants ---
#define EN_DATA_REPORT    0xF4  // Enable Data Reporting
#define DIS_DATA_REPORT   0xF5  // Disable Data Reporting

// --- RTC Constants ---
#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

#define RTC_REG_SECONDS 0x00
#define RTC_REG_MINUTES 0x02
#define RTC_REG_HOURS   0x04
#define RTC_REG_A       0x0A
#define RTC_REG_B       0x0B
#define RTC_REG_DAY     0x07
#define RTC_REG_MONTH   0x08
#define RTC_REG_YEAR    0x09

#define RTC_UIP_MSK      (1 << 7)
#define RTC_24_HOUR_MSK  (1 << 1)
#define RTC_BINARY_MSK   (1 << 2)
#define RTC_PM_MSK       (1 << 7)
#define LAST_PLAYED_FILE "pvz-last-played.txt"

// --- Main Menu Button Constants ---
#define MENU_START_X 220
#define MENU_START_Y 260
#define MENU_START_W 360
#define MENU_START_H 50

#define MENU_EXIT_X 280
#define MENU_EXIT_Y 330
#define MENU_EXIT_W 240
#define MENU_EXIT_H 50

// --- HUD Constants ---
#define HUD_X 0
#define HUD_Y 0
#define HUD_W 539
#define HUD_H 123
#define HUD_VISUAL_W 500

// --- Seed Bank Constants ---
#define TOTAL_AVAILABLE_PLANTS 7
#define MAX_DECK_SIZE          6

#define SEED_BANK_X 108
#define SEED_BANK_Y 6
#define SEED_BANK_SLOT_W 56
#define SEED_BANK_SLOT_H 80
#define SEED_BANK_SLOT_GAP 6
#define SEED_BANK_SLOTS 6
#define SEED_BANK_W (SEED_BANK_SLOTS * SEED_BANK_SLOT_W + (SEED_BANK_SLOTS - 1) * SEED_BANK_SLOT_GAP)
#define SEED_PACKET_W 55
#define SEED_PACKET_H 80
#define SEED_PACKET_Y_OFFSET 0

// --- Shovel Constants ---
#define SHOVEL_W 80
#define SHOVEL_H 80
#define SHOVEL_ATTACH_OVERLAP 4
#define SHOVEL_X (HUD_X + HUD_VISUAL_W - SHOVEL_ATTACH_OVERLAP)
#define SHOVEL_Y HUD_Y

// --- Plant Selection Constants ---
#define SEL_CARD_W    75
#define SEL_CARD_H    95
#define SEL_CARD_GAP  20
#define SEL_COLS       4
#define SEL_ROWS       2
#define SEL_GRID_X   ((SCREEN_W - (SEL_COLS * SEL_CARD_W + (SEL_COLS - 1) * SEL_CARD_GAP)) / 2)
#define SEL_GRID_Y    95
#define SEL_DECK_GAP  12
#define SEL_DECK_X   ((SCREEN_W - (MAX_DECK_SIZE * SEL_CARD_W + (MAX_DECK_SIZE - 1) * SEL_DECK_GAP)) / 2)
#define SEL_DECK_Y   358
#define SEL_BTN_X    300
#define SEL_BTN_Y    465
#define SEL_BTN_W    200
#define SEL_BTN_H     45
#define DECK_FULL_FLASH_TICKS 12

// --- Keyboard Grid Constants ---
#define KEYBOARD_START_ROW 2
#define KEYBOARD_START_COL 4

// --- Zombies ---
#define MAX_ZOMBIES          20
#define ZOMBIE_SPAWN_X       120  /* valor inicial de zombie_spawn_rate */
#define ZOMBIE_INITIAL_OFFSET 200  /* initial position outside the screen */

// --- Ondas ---
#define MAX_WAVES             5
#define WAVE_SPAWN_MIN        60
#define WAVE_BREAK_TICKS     180

// --- Banners ---
#define WAVE_BANNER_TICKS    180   /* duration of any banner: 3s at 60 Hz */
#define WAVE_LONG_DELAY      360   /* delay before first spawn in special waves: 6s */
#define WAVE_SHORT_DELAY     120   /* delay normal entre waves */

// --- Tipos de zombie ---
#define CONEHEAD_CHANCE_PER_WAVE 10
#define CONEHEAD_CHANCE_MAX      50
#define POLE_VAULTING_CHANCE_PER_WAVE 8
#define POLE_VAULTING_CHANCE_MAX      24

#define REGULAR_ZOMBIE_HEALTH 100
#define CONEHEAD_ZOMBIE_HEALTH ((REGULAR_ZOMBIE_HEALTH * 5) / 2)
#define POLE_VAULTING_ZOMBIE_HEALTH REGULAR_ZOMBIE_HEALTH
#define ZOMBIE_SPEED 18
#define POLE_VAULTING_ZOMBIE_SPEED 22
#define ZOMBIE_DAMAGE 10
#define REGULAR_ZOMBIE_ATTACK_SPEED 30
#define CONEHEAD_ZOMBIE_ATTACK_SPEED 30
#define POLE_VAULTING_ZOMBIE_ATTACK_SPEED 30
#define POLE_VAULTING_JUMP_ANIM_SPEED 4
#define POLE_VAULTING_JUMP_TICKS 24
#define POLE_VAULTING_LANDING_OFFSET 45

// --- Grid Constants ---
// Tweak these two numbers once you find the exact starting point!
#define GRID_START_X      135
#define GRID_START_Y      80

#define GRID_COLS         9
#define GRID_ROWS         5
#define GRID_END_X        725  /* borda direita da relva (COL_START_X[8] + ~65) */
#define GRID_CELL_W       65
#define GRID_CELL_H       80

#define BOARD_ROWS GRID_ROWS
#define BOARD_COLS GRID_COLS
#define MAX_BULLETS 50
#define MAX_SUNS 20

// --- Lawnmower Constants ---
#define LAWNMOWER_START_X   62
#define LAWNMOWER_SPEED     300.0f
#define LAWNMOWER_W         58
#define LAWNMOWER_H         64
#define LAWNMOWER_EXIT_X    820
#define LAWNMOWER_ROW_Y_OFFSET 10

#define ZOMBIE_CENTER_X_OFFSET 30
#define PLANT_SHOOT_X_OFFSET 40
#define ZOMBIE_PLANT_COLLISION_W 60
#define ZOMBIE_HITBOX_LEFT_PADDING 10
#define ZOMBIE_HITBOX_RIGHT_PADDING 80

#define FALLING_SUN_START_X_MIN 150
#define FALLING_SUN_START_X_RANGE 551
#define FALLING_SUN_TARGET_Y_MIN 100
#define FALLING_SUN_TARGET_Y_RANGE 401
#define SUN_INITIAL_SPAWN_TIMER 120
#define SUN_SPAWN_TIMER_MIN 420
#define SUN_SPAWN_TIMER_RANGE 181
#define SUN_VALUE 25
#define INITIAL_SUN_COUNT 50
#define SUN_CLICK_CENTER_OFFSET 10
#define SUN_CLICK_RADIUS 25
#define SUN_GROUND_DURATION_TICKS 900

#define SUNFLOWER_IDLE_FRAMES 6
#define PEASHOOTER_IDLE_FRAMES 8
#define PEASHOOTER_SHOOTING_FRAMES 3
#define REPEATER_IDLE_FRAMES 5
#define REPEATER_SHOOTING_FRAMES 2
#define CABBAGE_IDLE_FRAMES 6
#define CABBAGE_SHOOTING_FRAMES 8
#define WALL_NUT_IDLE_FRAMES 3
#define POTATO_MINE_IDLE_FRAMES 5
#define POTATO_MINE_POP_FRAMES 3
#define POTATO_MINE_EXPLOSION_FRAMES 8
#define POTATO_MINE_EXPLOSION_SPEED 4
#define SPIKEWEED_IDLE_FRAMES 3
#define SPIKEWEED_ATTACK_FRAMES 4

#define REGULAR_WALKING_FRAMES 7
#define REGULAR_EATING_FRAMES 7
#define CONEHEAD_WALKING_FRAMES 7
#define CONEHEAD_EATING_FRAMES 7
#define POLE_VAULTING_WALKING_FRAMES 7
#define POLE_VAULTING_JUMP_FRAMES 6
#define POLE_VAULTING_NOPOLE_FRAMES 7
#define POLE_VAULTING_EATING_FRAMES 7

#define DAY_MAP_FRAMES 1

#define SUN_FRAMES 2
#define PEA_FRAMES 3
#define CABBAGE_PROJECTILE_FRAMES 4

// Exact coordinates
extern const int COL_START_X[GRID_COLS];
extern const int ROW_START_Y[GRID_ROWS];

extern const int sunflower_x_offsets[SUNFLOWER_IDLE_FRAMES];
extern const int sunflower_y_offsets[SUNFLOWER_IDLE_FRAMES];

extern const int peashooter_x_offsets[PEASHOOTER_IDLE_FRAMES];
extern const int peashooter_y_offsets[PEASHOOTER_IDLE_FRAMES];
extern const int peashooter_shooting_x_offsets[PEASHOOTER_SHOOTING_FRAMES];
extern const int peashooter_shooting_y_offsets[PEASHOOTER_SHOOTING_FRAMES];

extern const int repeater_x_offsets[REPEATER_IDLE_FRAMES];
extern const int repeater_y_offsets[REPEATER_IDLE_FRAMES];
extern const int repeater_shooting_x_offsets[REPEATER_SHOOTING_FRAMES];
extern const int repeater_shooting_y_offsets[REPEATER_SHOOTING_FRAMES];

extern const int cabbage_x_offsets[CABBAGE_IDLE_FRAMES];
extern const int cabbage_y_offsets[CABBAGE_IDLE_FRAMES];
extern const int cabbage_shooting_x_offsets[CABBAGE_SHOOTING_FRAMES];
extern const int cabbage_shooting_y_offsets[CABBAGE_SHOOTING_FRAMES];

extern const int wall_nut_x_offsets[WALL_NUT_IDLE_FRAMES];
extern const int wall_nut_y_offsets[WALL_NUT_IDLE_FRAMES];

extern const int potato_mine_idle_x_offsets[POTATO_MINE_IDLE_FRAMES];
extern const int potato_mine_idle_y_offsets[POTATO_MINE_IDLE_FRAMES];
extern const int potato_mine_pop_x_offsets[POTATO_MINE_POP_FRAMES];
extern const int potato_mine_pop_y_offsets[POTATO_MINE_POP_FRAMES];
extern const int potato_mine_explosion_x_offsets[POTATO_MINE_EXPLOSION_FRAMES];
extern const int potato_mine_explosion_y_offsets[POTATO_MINE_EXPLOSION_FRAMES];

extern const int spikeweed_idle_x_offsets[SPIKEWEED_IDLE_FRAMES];
extern const int spikeweed_idle_y_offsets[SPIKEWEED_IDLE_FRAMES];
extern const int spikeweed_attack_x_offsets[SPIKEWEED_ATTACK_FRAMES];
extern const int spikeweed_attack_y_offsets[SPIKEWEED_ATTACK_FRAMES];

extern const int regular_walking_x_offsets[REGULAR_WALKING_FRAMES];
extern const int regular_walking_y_offsets[REGULAR_WALKING_FRAMES];
extern const int regular_eating_x_offsets[REGULAR_EATING_FRAMES];
extern const int regular_eating_y_offsets[REGULAR_EATING_FRAMES];

extern const int conehead_walking_x_offsets[CONEHEAD_WALKING_FRAMES];
extern const int conehead_walking_y_offsets[CONEHEAD_WALKING_FRAMES];
extern const int conehead_eating_x_offsets[CONEHEAD_EATING_FRAMES];
extern const int conehead_eating_y_offsets[CONEHEAD_EATING_FRAMES];
extern const int pole_vaulting_walking_x_offsets[POLE_VAULTING_WALKING_FRAMES];
extern const int pole_vaulting_walking_y_offsets[POLE_VAULTING_WALKING_FRAMES];
extern const int pole_vaulting_jump_x_offsets[POLE_VAULTING_JUMP_FRAMES];
extern const int pole_vaulting_jump_y_offsets[POLE_VAULTING_JUMP_FRAMES];
extern const int pole_vaulting_nopole_x_offsets[POLE_VAULTING_NOPOLE_FRAMES];
extern const int pole_vaulting_nopole_y_offsets[POLE_VAULTING_NOPOLE_FRAMES];
extern const int pole_vaulting_eating_x_offsets[POLE_VAULTING_EATING_FRAMES];
extern const int pole_vaulting_eating_y_offsets[POLE_VAULTING_EATING_FRAMES];

extern const int pea_y_offsets[PEA_FRAMES];
extern const int cabbage_projectile_y_offsets[CABBAGE_PROJECTILE_FRAMES];

#define SUN_ANIM_SPEED 12
/* Sun fly-to-HUD animation */
#define SUN_HUD_TARGET_X 37   /* centre of the sun counter in the HUD */
#define SUN_HUD_TARGET_Y 60
#define SUN_FLY_SPEED    8    /* pixels per tick (~8 frames to cross the screen at 60 Hz) */

#define PEA_IMPACT_FIRST_FRAME 1
#define PEA_IMPACT_FRAME_TICKS 6
#define CABBAGE_PROJECTILE_FRAME_TICKS 8
#define CABBAGE_PROJECTILE_ARC_HEIGHT 70
#define CABBAGE_PROJECTILE_RELEASE_FRAME 4

#define PEASHOOTER_FIRE_RATE 90
#define PEASHOOTER_PROJECTILE_DAMAGE 20
#define PEASHOOTER_PROJECTILE_COUNT 1
#define REPEATER_FIRE_RATE PEASHOOTER_FIRE_RATE
#define REPEATER_PROJECTILE_DAMAGE PEASHOOTER_PROJECTILE_DAMAGE
#define REPEATER_PROJECTILE_COUNT 2
#define REPEATER_BURST_INTERVAL 8
#define CABBAGE_FIRE_RATE 120
#define CABBAGE_PROJECTILE_DAMAGE 40
#define CABBAGE_PROJECTILE_COUNT 1

#define SUNFLOWER_HEALTH 100
#define PEASHOOTER_HEALTH 100
#define REPEATER_HEALTH 120
#define CABBAGE_HEALTH 100
#define WALL_NUT_HEALTH 800
#define POTATO_MINE_HEALTH 300
#define SPIKEWEED_HEALTH 100

#define SUNFLOWER_COST 50
#define PEASHOOTER_COST 100
#define REPEATER_COST 200
#define CABBAGE_COST 100
#define WALL_NUT_COST 50
#define POTATO_MINE_COST 25
#define SPIKEWEED_COST 100

#define POTATO_MINE_ARM_TICKS (15 * 60)
#define POTATO_MINE_DAMAGE 1800
#define SPIKEWEED_DAMAGE 20
#define SPIKEWEED_ATTACK_RATE 45
#define SPIKEWEED_ATTACK_ANIM_TICKS 12

#define SUNFLOWER_PRODUCTION_RATE 480

/* Colours */
#define CLR_BG      0x1A1A2E
#define CLR_WHITE   0xFFFFFF
#define CLR_GREEN   0x27AE60
#define CLR_RED     0xE74C3C
#define CLR_GRAY    0x888888
#define CLR_OVERLAY 0x111122
#define CLR_DKRED   0x4A0E0E
#define CLR_DKGREEN 0x0E4A1A

/* 5x7 bitmap font (A-Z only + space) */
#endif /* _CONSTANTS_H_ */
