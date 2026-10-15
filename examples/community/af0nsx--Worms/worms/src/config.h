#ifndef CONFIG_H
#define CONFIG_H

#include "vbe.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Available modes (defined in vbe.h):
//   MODE_105 → 1024×768,  indexed,  8 bpp
//   MODE_110 → 640×480,   direct,  15 bpp
//   MODE_115 → 800×600,   direct,  24 bpp
//   MODE_11A → 1280×1024, direct,  16 bpp (use XPM_5_6_5)
//   MODE_14C → 1152×864,  direct,  32 bpp (use XPM_8_8_8_8)
#define GAME_VIDEO_MODE   MODE_115
#define GAME_XPM_TYPE     XPM_8_8_8_8 /* Sprite storage uses uint32_t pixels; renderer writes 24bpp. */

#define SCREEN_W  800
#define SCREEN_H  600

#define GAME_FPS          60
#define TIMER_FREQ_HZ     60
#define TURN_DURATION_TICKS (30 * GAME_FPS)
#define EVENT_QUEUE_CAPACITY 67

#define PROJECTILE_GRAVITY 0.15
#define PROJECTILE_SPEED   8

/* Weapons */
#define BAZOOKA_DAMAGE           50
#define BAZOOKA_EXPLOSION_RADIUS 40
#define BAZOOKA_SPEED            15.0
#define BAZOOKA_PULL_TICKS       2
#define BAZOOKA_RETRACT_TICKS    2
#define BAZOOKA_TEAM_INVULNERABLE_TICKS 4
#define WEAPON_MUZZLE_OFFSET_X   30
#define WEAPON_MUZZLE_OFFSET_Y   (WORM_HITBOX_HEIGHT / 2)
#define WEAPON_AIM_MAX_POWER     100
#define WEAPON_AIM_PREVIEW_STEPS 20
#define WEAPON_AIM_DOT_INTERVAL  4
#define WEAPON_AIM_DOT_SIZE      3
#define WEAPON_AIM_BAR_W         40
#define WEAPON_AIM_BAR_H         6
#define WEAPON_AIM_BAR_OFFSET_X  20
#define WEAPON_AIM_BAR_OFFSET_Y  14
#define BAZOOKA_MUZZLE_OFFSET_X  WEAPON_MUZZLE_OFFSET_X
#define BAZOOKA_MUZZLE_OFFSET_Y  WEAPON_MUZZLE_OFFSET_Y
#define MISSILE_ANGLE_COUNT      32
#define MISSILE_SPRITE_SIZE      60
#define MISSILE_HITBOX_SIZE      14

#define GRENADE_DAMAGE                     50
#define GRENADE_EXPLOSION_RADIUS           40
#define GRENADE_SPEED                      15.0
#define GRENADE_SPRITE_SIZE                60
#define GRENADE_HITBOX_SIZE                14
#define GRENADE_EXPLOSION_TICKS            180
#define GRENADE_RESTITUTION                0.6
#define GRENADE_FRICTION                   0.95
#define GRENADE_SELF_COLLISION_GRACE_TICKS 10

#define WORM_MOVE_SPEED 0.2
#define WORM_JUMP_SPEED 7.0
#define WORM_GRAVITY 0.6
#define WORM_MAX_STEP_HEIGHT 4
#define WORM_ANIMATION_TICKS_PER_FRAME 3
#define WORM_INITIAL_HEALTH 100

#define MAX_TEAMS 2
#define WORMS_PER_TEAM 5
#define MAX_WORMS (MAX_TEAMS * WORMS_PER_TEAM)

#define TEAM0_WORM0_START_X 120
#define TEAM0_WORM1_START_X 220
#define TEAM1_WORM0_START_X 580
#define TEAM1_WORM1_START_X 680
#define WORM_START_Y 400

#define WORM_HITBOX_WIDTH   32
#define WORM_HITBOX_HEIGHT  32

#define WORM_DAMAGE_HITBOX_WIDTH    18
#define WORM_DAMAGE_HITBOX_HEIGHT   24
#define WORM_DAMAGE_HITBOX_OFFSET_X  7   /* (32-18)/2 — keeps it centered */
#define WORM_DAMAGE_HITBOX_OFFSET_Y  8   /*  32-24    — keeps it bottom-aligned */

#define TERRAIN_WIDTH   800   /* must match horizontal resolution of GAME_VIDEO_MODE */
#define TERRAIN_HEIGHT  600   /* must match vertical resolution of GAME_VIDEO_MODE */
#define TERRAIN_BASE_HEIGHT      360
#define TERRAIN_VARIATION        120
#define TERRAIN_MIN_HEIGHT       300
#define TERRAIN_MAX_HEIGHT       430
#define TERRAIN_SMOOTH_PASSES      3
#define TERRAIN_SEGMENT_MIN_WIDTH 35
#define TERRAIN_SEGMENT_MAX_WIDTH 110
#define TERRAIN_NOISE_AMPLITUDE   4
#define TERRAIN_PLATFORM_COUNT     5
#define TERRAIN_PLATFORM_MIN_WIDTH 34
#define TERRAIN_PLATFORM_MAX_WIDTH 70
#define TERRAIN_CLIFF_COUNT        7
#define TERRAIN_CLIFF_MIN_HEIGHT  22
#define TERRAIN_CLIFF_MAX_HEIGHT  58
#define TERRAIN_CRATER_MIN_COUNT   6
#define TERRAIN_CRATER_MAX_COUNT  11
#define TERRAIN_CRATER_MIN_RADIUS 18
#define TERRAIN_CRATER_MAX_RADIUS 42
#define TERRAIN_CRATER_DEPTH       8
#define TERRAIN_GRASS_DEPTH         7
#define TERRAIN_GRASS_TEXTURE_X   101
#define TERRAIN_GRASS_TEXTURE_W   309

#define WATER_LEVEL_Y   500   /* Y coordinate where the water surface begins */
#define WATER_KILL_DAMAGE 10000 /* damage dealt to worms touching water (instant kill) */

#define WORM_IDLE_TICKS_PER_FRAME   8
#define WORM_WALK_TICKS_PER_FRAME   3
#define WORM_JUMP_TICKS_PER_FRAME   4
#define WORM_DIE_TICKS_PER_FRAME    2
#define WORM_IDLE_ANIM_COUNT        4

#define MENU_NAV_COOLDOWN_TICKS     6
#define PAUSE_COOLDOWN_TICKS        12
#define EXPLOSION_DURATION_TICKS    12

#define COLOR_BLACK       0xFF000000
#define COLOR_WHITE       0xFFFFFFFF
#define COLOR_RED         0xFFFF0000
#define COLOR_GREEN       0xFF00FF00
#define COLOR_BLUE        0xFF0000FF
#define COLOR_DARK_BLUE   0xFF00008B
#define COLOR_LIME        0xFF7CFC00
#define COLOR_GREY        0xFF333333
#define COLOR_TERRAIN_GRASS 0xFF3F9B2F
#define COLOR_TERRAIN_DIRT  0xFF7A3F20

#define FONT_CHAR_SPACING  2    /* px gap between consecutive glyphs */
#define FONT_SPACE_WIDTH   8    /* px advance for a space character */
#define FONT_GLYPH_HEIGHT  16   /* expected height of all glyph sprites */

/* ── Splash ──────────────────────────────────────────────────────────── */
#define SPLASH_DURATION_TICKS  180   /* 3 s at 60 Hz */
#define GAME_TITLE_TEXT        "LCOMBAT"

/* ── Generic menu layout ─────────────────────────────────────────────── */
#define MENU_LOGO_GAP     20   /* gap between logo and button block in main menu */
#define MENU_TITLE_H      48
#define MENU_BTN_W       220
#define MENU_BTN_H        56
#define MENU_BTN_GAP      16
#define MENU_BTN_BORDER    2

/* ── Game Over / Win screen ──────────────────────────────────────────── */
#define GAME_OVER_PANEL_W             350
#define GAME_OVER_PANEL_H             240
#define GAME_OVER_PANEL_GAP            20
#define GAME_OVER_HEADER_H             50
#define GAME_OVER_WIN_LINE1_OFFSET_Y  126
#define GAME_OVER_WIN_LINE2_OFFSET_Y  150
#define GAME_OVER_LOSE_MSG_OFFSET_Y   138
#define GAME_OVER_PROMPT_OFFSET_Y      20
#define GAME_OVER_PROMPT_PAD_X         20
#define GAME_OVER_PROMPT_PAD_Y          6

/* ── Controls screen ─────────────────────────────────────────────────── */
#define CONTROLS_TITLE_X   160
#define CONTROLS_TITLE_Y    30
#define CONTROLS_TITLE_W   480
#define CONTROLS_TITLE_H    50
#define CONTROLS_COL1_X    150
#define CONTROLS_COL2_X    450
#define CONTROLS_ROWS_Y    120
#define CONTROLS_ROW_H      32
#define CONTROLS_BTN_Y     490

/* ── HUD — timer (bottom-left square) ───────────────────────────────── */
#define HUD_TIMER_W   60
#define HUD_TIMER_H   60
#define HUD_TIMER_X    8
#define HUD_TIMER_Y   (SCREEN_H - HUD_TIMER_H - 8)

/* ── HUD — team health bars (top corners, fixed) ─────────────────────── */
#define HUD_TEAM_BAR_W       150
#define HUD_TEAM_BAR_H        14
#define HUD_TEAM_BAR_BORDER    2
#define HUD_TEAM_BAR0_X        8
#define HUD_TEAM_BAR1_X       (SCREEN_W - HUD_TEAM_BAR_W - 8)
#define HUD_TEAM_BAR_Y        26
#define HUD_TEAM_NAME_Y        8
#define HUD_TEAM_BAR_LOW_PCT  30

/* ── HUD — per-worm health bar (floating, above sprite) ──────────────── */
#define HUD_WORM_BAR_W        60
#define HUD_WORM_BAR_H         8
#define HUD_WORM_BAR_BORDER    1
#define HUD_WORM_BAR_OFFSET_Y 20

/* ── HUD — worm name (floating, above worm bar) ──────────────────────── */
#define HUD_NAME_OFFSET_Y     36

/* ── HUD — hotbar (bottom-center) ───────────────────────────────────── */
#define HUD_HOTBAR_SLOT_COUNT   2
#define HUD_HOTBAR_SLOT_W      48
#define HUD_HOTBAR_SLOT_H      48
#define HUD_HOTBAR_GAP          6
#define HUD_HOTBAR_Y           (SCREEN_H - HUD_HOTBAR_SLOT_H - 8)
#define HUD_HOTBAR_X           ((SCREEN_W \
    - HUD_HOTBAR_SLOT_COUNT * HUD_HOTBAR_SLOT_W \
    - (HUD_HOTBAR_SLOT_COUNT - 1) * HUD_HOTBAR_GAP) / 2)
#define HUD_HOTBAR_ICON_W      32
#define HUD_HOTBAR_ICON_H      32

/* ── Colors (additions) ──────────────────────────────────────────────── */
#define COLOR_HEALTH_OK   0xFF00CC44
#define COLOR_HEALTH_LOW  0xFFFF3333
#define COLOR_TEAM_RED    0xFFCC2200
#define COLOR_TEAM_BLUE   0xFF0055CC
#define COLOR_HUD_BG      0xFF222222
#define COLOR_HUD_BORDER  0xFF888888

#define COLOR_MENU_BTN       0xFF320D18
#define COLOR_MENU_BTN_SEL   0xFFBB0422
#define COLOR_MENU_BTN_BDR   0xFFD68D45
#define COLOR_MENU_TITLE_BG  0xFF200A12
#define COLOR_MENU_BACKDROP  0xFF0D0208
#define MENU_BACKDROP_PAD    14

#endif /* CONFIG_H */
