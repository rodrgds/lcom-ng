#include <lcom/lcf.h>
#include "dino.h"
#include "../cursor.h"
#include "../video.h"
#include "../sprite.h"
#include "../font.h"
#include "../dictionary.h"
#include "../scancode.h"
#include "../ui.h"

#include "../../xpm/dino/cactus_large.xpm"
#include "../../xpm/dino/cactus_large1.xpm"
#include "../../xpm/dino/cactus_large2.xpm"
#include "../../xpm/dino/cactus_small.xpm"
#include "../../xpm/dino/cactus_small1.xpm"
#include "../../xpm/dino/cactus_small2.xpm"
#include "../../xpm/dino/dino_dead.xpm"
#include "../../xpm/dino/dino_duck.xpm"
#include "../../xpm/dino/dino_duck1.xpm"
#include "../../xpm/dino/dino_duck2.xpm"
#include "../../xpm/dino/dino_life.xpm"
#include "../../xpm/dino/dino_run1.xpm"
#include "../../xpm/dino/dino_run2.xpm"
#include "../../xpm/dino/ptero_frame1.xpm"
#include "../../xpm/dino/ptero_frame2.xpm"

#define GROUND_Y 560
#define DINO_X 160
#define DINO_SPRITE_W 80
#define DINO_SPRITE_H 80
#define DUCK_SPRITE_W 96
#define DUCK_SPRITE_H 56
#define CACTUS_W_SMALL 17
#define CACTUS_H_SMALL 35
#define CACTUS_W_LARGE 34
#define CACTUS_H_LARGE 70
#define PTERO_W 80
#define PTERO_H 60

// Physics
#define JUMP_VELOCITY -12
#define GRAVITY 1
#define GRAVITY_PERIOD 2
#define DUCK_HOLD_TICKS 25

// Game speed
#define INITIAL_SPEED 5
#define SPEED_INCREMENT 1
#define SPEED_TICKS 600

// Obstacles
#define MAX_OBSTACLES 4
#define OBSTACLE_SPAWN_MIN 80
#define OBSTACLE_SPAWN_MAX 220

// Lives
#define STARTING_LIVES 3
#define INVINCIBLE_TICKS 120

// Colors
#define COL_BG 0xA89A7E
#define COL_GROUND 0x8D6E63
#define COL_SCORE 0x4E342E
#define COL_WORD_NORMAL 0x424242
#define COL_WORD_OK 0x2E7D32
#define COL_WORD_ERR 0xC62828
#define COL_LIFE_ACTIVE 0xE53935
#define COL_LIFE_EMPTY 0xBCAAA4
#define COL_POWERUP 0xFBC02D
#define COL_OVERLAY 0x212121

// Types
typedef enum {
    OBS_NONE = 0,
    OBS_CACTUS_SMALL,
    OBS_CACTUS_LARGE,
    OBS_PTERODACTYL
} ObstacleType;

typedef struct {
    ObstacleType type;
    int x;
    int y;
    int w, h;
    bool active;
    char word[MAX_WORD_LEN + 1];
    int word_len;
    int typed_len;
    char typed_word[MAX_WORD_LEN + 1];
    bool word_correct;
    bool word_done;
    bool destroyed;
    int destroy_timer;
} Obstacle;

typedef enum {
    DINO_RUN,
    DINO_JUMP,
    DINO_DUCK,
    DINO_DEAD
} DinoState;

// Static State
static Sprite *spr_run[2];
static Sprite *spr_duck[3];
static Sprite *spr_cactus_small[3];
static Sprite *spr_cactus_large[3];
static Sprite *spr_dead;
static Sprite *spr_life;
static Sprite *spr_ptero_frame[2];

static DinoState dino_state;
static int dino_y;
static int dino_vy;
static int gravity_timer;
static int duck_timer;
static int duck_for_ptero;
static int invincible_timer;

static int lives;
static bool game_over;
static unsigned long score;
static int speed;
static int speed_timer;

static Obstacle obstacles[MAX_OBSTACLES];
static int spawn_timer;

// Animation
static int anim_frame;
static int anim_timer;

// Active word target
static int target_obs;

// Jump Word
#define JUMP_WORD "jump"
static char jump_buf[8];
static int jump_buf_len;

// Helpers

static int dino_ground_y(void) {
    return GROUND_Y - DINO_SPRITE_H;
}

static int duck_ground_y(void) {
    return GROUND_Y - DUCK_SPRITE_H;
}

static bool dino_collides(Obstacle *o) {
    if (!o->active || o->destroyed) return false;

    int dw = (dino_state == DINO_DUCK) ? DUCK_SPRITE_W : DINO_SPRITE_W;
    int dh = (dino_state == DINO_DUCK) ? DUCK_SPRITE_H : DINO_SPRITE_H;
    int dy = (dino_state == DINO_DUCK) ? duck_ground_y() : dino_y;

    int margin = 4;
    if (DINO_X + dw - margin <= o->x) return false;
    if (DINO_X + margin >= o->x + o->w) return false;
    if (dy + dh - margin <= o->y) return false;
    if (dy + margin >= o->y + o->h) return false;
    return true;
}

static void pick_next_target(void) {
    target_obs = -1;
    int closest_x = 0x7fffffff;
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (!obstacles[i].active || obstacles[i].destroyed || obstacles[i].word_done) continue;
        if (obstacles[i].x < closest_x) {
            closest_x = obstacles[i].x;
            target_obs = i;
        }
    }
}

static void spawn_obstacle(void) {
    int slot = -1;
    int r, base;
    Obstacle *o;

    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (!obstacles[i].active) {
            slot = i;
            break;
        }
    }
    if (slot < 0) return;

    o = &obstacles[slot];
    memset(o, 0, sizeof(*o));

    r = rand() % 10;
    if (r < 5) o->type = OBS_CACTUS_SMALL;
    else if (r < 7) o->type = OBS_CACTUS_LARGE;
    else o->type = OBS_PTERODACTYL;

    switch (o->type) {
        case OBS_CACTUS_SMALL:
            o->w = CACTUS_W_SMALL; o->h = CACTUS_H_SMALL;
            o->y = GROUND_Y - o->h;
            break;
        case OBS_CACTUS_LARGE:
            o->w = CACTUS_W_LARGE; o->h = CACTUS_H_LARGE;
            o->y = GROUND_Y - o->h;
            break;
        case OBS_PTERODACTYL:
            o->w = PTERO_W; o->h = PTERO_H;
            o->y = dino_ground_y() - 55;
            break;
        default: break;
    }
    o->x = screen_width + 10;
    o->active = true;
    o->destroyed = false;

    const char *chosen;
    do {
        chosen = english_dictionary[rand() % DICTIONARY_SIZE];
    } while ((int)strlen(chosen) > 4);
    strncpy(o->word, chosen, MAX_WORD_LEN);
    o->word[MAX_WORD_LEN] = '\0';
    o->word_len = (int)strlen(o->word);
    o->typed_len = 0;
    o->word_correct = true;

    base = OBSTACLE_SPAWN_MAX - (speed - INITIAL_SPEED) * 10;
    if (base < OBSTACLE_SPAWN_MIN) base = OBSTACLE_SPAWN_MIN;
    spawn_timer = OBSTACLE_SPAWN_MIN + rand() % (base - OBSTACLE_SPAWN_MIN + 1);
}

// Public API

int dino_game_init() {
    spr_run[0] = sprite_load((xpm_map_t)dino_run1_xpm);
    spr_run[1] = sprite_load((xpm_map_t)dino_run2_xpm);
    spr_duck[0] = sprite_load((xpm_map_t)dino_duck_xpm);
    spr_duck[1] = sprite_load((xpm_map_t)dino_duck1_xpm);
    spr_duck[2] = sprite_load((xpm_map_t)dino_duck2_xpm);
    spr_cactus_small[0] = sprite_load((xpm_map_t)cactus_small_xpm);
    spr_cactus_small[1] = sprite_load((xpm_map_t)cactus_small1_xpm);
    spr_cactus_small[2] = sprite_load((xpm_map_t)cactus_small2_xpm);
    spr_cactus_large[0] = sprite_load((xpm_map_t)cactus_large_xpm);
    spr_cactus_large[1] = sprite_load((xpm_map_t)cactus_large1_xpm);
    spr_cactus_large[2] = sprite_load((xpm_map_t)cactus_large2_xpm);
    spr_ptero_frame[0] = sprite_load((xpm_map_t)ptero_frame1_xpm);
    spr_ptero_frame[1] = sprite_load((xpm_map_t)ptero_frame2_xpm);
    spr_dead = sprite_load((xpm_map_t)dino_dead_xpm);
    spr_life = sprite_load((xpm_map_t)dino_life_xpm);

    if (!spr_run[0] || !spr_run[1] || !spr_duck[0] || !spr_duck[1] || !spr_duck[2] || !spr_cactus_small[0] || !spr_cactus_small[1] || !spr_cactus_small[2] || !spr_cactus_large[0] || !spr_cactus_large[1] || !spr_cactus_large[2] || !spr_ptero_frame[0] || !spr_ptero_frame[1] || !spr_dead || !spr_life) {
        dino_game_destroy();
        return 1;
    }

    dino_game_reset();
    return 0;
}

void dino_game_reset(void) {
    dino_state = DINO_RUN;
    dino_y = dino_ground_y();
    dino_vy = 0;
    gravity_timer = 0;
    duck_timer = 0;
    duck_for_ptero = -1;
    invincible_timer = 0;
    lives = STARTING_LIVES;
    game_over = false;
    score = 0;
    speed = INITIAL_SPEED;
    speed_timer = 0;
    anim_frame = 0;
    anim_timer = 0;
    target_obs = -1;
    jump_buf_len = 0;
    spawn_timer = 80;

    for (int i = 0; i < MAX_OBSTACLES; i++) {
        obstacles[i].active = false;
    }
}

void dino_game_tick(void) {
    if (game_over) return;
    score++;

    if (++speed_timer >= SPEED_TICKS) {
        speed_timer = 0;
        speed++;
    }

    if (invincible_timer > 0) invincible_timer--;

    if (dino_state == DINO_JUMP) {
        if (++gravity_timer >= GRAVITY_PERIOD) {
            gravity_timer = 0;
            dino_vy += GRAVITY;
        }
        dino_y += dino_vy;
        if (dino_y >= dino_ground_y()) {
            dino_y = dino_ground_y();
            dino_vy = 0;
            dino_state = DINO_RUN;
        }
    }
    else if (dino_state == DINO_DUCK) {
        if (duck_timer > 0) duck_timer--;
        if (duck_timer == 0) dino_state = DINO_RUN;
    }

    if (dino_state != DINO_DEAD) {
        if (++anim_timer >= 8) {
            anim_timer = 0;
            anim_frame = 1 - anim_frame;
        }
    }

    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (!obstacles[i].active) continue;
        obstacles[i].x -= speed;
        if (obstacles[i].x + obstacles[i].w < 0) {
            obstacles[i].active = false;
            obstacles[i].destroyed = false;
            continue;
        }
        if (obstacles[i].destroyed) continue;
        if (invincible_timer == 0 && dino_collides(&obstacles[i])) {
            lives--;
            invincible_timer = INVINCIBLE_TICKS;
            obstacles[i].active = false;
            if (lives <= 0) {
                game_over = true;
                dino_state = DINO_DEAD;
            }
        }
    }

    if (dino_state == DINO_DUCK && duck_for_ptero >= 0) {
        Obstacle *dp = &obstacles[duck_for_ptero];
        if (!dp->active || dp->x + dp->w <= DINO_X) duck_timer = 0;
    }

    if (--spawn_timer <= 0) spawn_obstacle();
    pick_next_target();
}

void dino_game_handle_key(uint8_t sc) {
    const char *tail;

    if (game_over) return;

    if (sc & 0x80) return;

    char c = scancode_to_char(sc);
    if (c == 0) return;

    if (c == '\b') {
        if (target_obs >= 0) {
            Obstacle *o = &obstacles[target_obs];
            if (o->typed_len > 0) {
                o->typed_len--;
                o->word_correct = true;
                for (int i = 0; i < o->typed_len; i++) {
                    if (o->typed_word[i] != o->word[i]) {
                        o->word_correct = false;
                        break;
                    }
                }
            }
        } 
        else {
            if (jump_buf_len > 0) jump_buf_len--;
        }
        return;
    }

    if (target_obs >= 0) {
        Obstacle *o = &obstacles[target_obs];
        if (o->typed_len < o->word_len) {
            bool ok = (c == o->word[o->typed_len]);
            if (!ok) o->word_correct = false;
            o->typed_word[o->typed_len] = c;
            o->typed_len++;

            if (o->typed_len == o->word_len) {
                if (o->word_correct) {
                    o->word_done = true;
                    if (dino_state == DINO_RUN) {
                        if (o->type == OBS_PTERODACTYL) {
                            dino_state = DINO_DUCK;
                            duck_timer = DUCK_HOLD_TICKS * 3;
                            duck_for_ptero = target_obs;
                        } 
                        else {
                            dino_vy = JUMP_VELOCITY;
                            dino_state = DINO_JUMP;
                        }
                    }
                } 
                else {
                    o->typed_len = 0;
                    o->word_correct = true;
                }
                pick_next_target();
            }
        }
        return;
    }

    jump_buf[jump_buf_len++] = c;
    jump_buf[jump_buf_len] = '\0';
    if (jump_buf_len > 4) {
        memmove(jump_buf, jump_buf + 1, 4);
        jump_buf_len = 4;
        jump_buf[4] = '\0';
    }
    if (strcmp(jump_buf, JUMP_WORD) == 0) {
        jump_buf_len = 0;
        if (dino_state == DINO_RUN) {
            dino_vy = JUMP_VELOCITY;
            dino_state = DINO_JUMP;
        }
    }
    if (jump_buf_len >= 4) {
        tail = jump_buf + (jump_buf_len - 4);
        if (strncmp(tail, "duck", 4) == 0) {
            jump_buf_len = 0;
            if (dino_state == DINO_RUN) {
                dino_state = DINO_DUCK;
                duck_timer = DUCK_HOLD_TICKS * 3;
                duck_for_ptero = -1;
            }
        }
    }
}

bool dino_game_is_over(void) {
    return game_over;
}

// Drawing

static void draw_lives(void) {
    uint16_t x = 40, y = 100;
    uint16_t text_y = (uint16_t)(y + (spr_life->img.height - char_height) / 2);
    font_draw_string_colored("LIVES:", x, text_y, COL_SCORE, 0, false);
    uint16_t icon_x = (uint16_t)(40 + 140);
    for (int i = 0; i < STARTING_LIVES; i++) {
        if (i < lives) sprite_draw(spr_life, icon_x + (uint16_t)(i * 40), y);
    }
}

static void draw_score(void) {
    char buf[16];
    snprintf(buf, sizeof(buf), "%lu", score / 6);
    uint16_t tw = font_string_width(buf);
    uint16_t sy = (uint16_t)(100 + (spr_life->img.height - char_height) / 2);
    font_draw_string_colored(buf, screen_width - tw - 40, sy, COL_SCORE, 0, false);
}

static void draw_word_on_obstacle(Obstacle *o) {
    int ci, wx, wy;
    int cw, ch;
    uint32_t col;

    if (!o->active || o->destroyed || o->word_done) return;
    if (target_obs < 0 || o != &obstacles[target_obs]) return;

    cw = char_width;
    ch = char_height;
    wx = DINO_X + DINO_SPRITE_W / 2 - (o->word_len * cw) / 2;
    wy = dino_ground_y() - ch - 8;

    for (ci = 0; ci < o->word_len; ci++, wx += cw) {
        if (ci < o->typed_len) col = o->word_correct ? COL_WORD_OK : COL_WORD_ERR;
        else col = COL_WORD_NORMAL;
        font_draw_char_colored(o->word[ci], (uint16_t)wx, (uint16_t)wy, col, 0, false);
    }
}

void dino_game_draw() {
    video_clear(COL_BG);

    video_draw_hline(0, GROUND_Y, screen_width, COL_GROUND);
    video_draw_hline(0, GROUND_Y + 1, screen_width, COL_GROUND);
    video_draw_hline(0, GROUND_Y + 2, screen_width, COL_GROUND - 0x111111);

    if (dino_state == DINO_DEAD) sprite_draw(spr_dead, DINO_X, (uint16_t)dino_ground_y());
    else {
        bool blink = (invincible_timer > 0 && (invincible_timer % 10) < 5);
        if (!blink) {
            if (dino_state == DINO_DUCK) sprite_draw(spr_duck[anim_frame], DINO_X, (uint16_t)duck_ground_y());
            else sprite_draw(spr_run[anim_frame], DINO_X, (uint16_t)dino_y);
        }
    }


    for (int i = 0; i < MAX_OBSTACLES; i++) {
        Obstacle *o = &obstacles[i];
        if (!o->active) continue;

        switch (o->type) {
            case OBS_CACTUS_SMALL:
                sprite_draw(spr_cactus_small[anim_frame % 3], (uint16_t)o->x, (uint16_t)o->y);
                break;
            case OBS_CACTUS_LARGE:
                sprite_draw(spr_cactus_large[anim_frame % 3], (uint16_t)o->x, (uint16_t)o->y);
                break;
            case OBS_PTERODACTYL:
                sprite_draw(spr_ptero_frame[anim_frame % 2], (uint16_t)o->x, (uint16_t)o->y);
                break;
            default: break;
        }
        if (o->destroyed) continue;

        draw_word_on_obstacle(o);
    }

    draw_lives();
    draw_score();
    ui_draw_esc_hint();

#define RESTART_BTN_W 240
#define RESTART_BTN_H 50

    if (game_over) {
        const char *go_str = "GAME OVER";
        uint16_t tw1 = font_string_width(go_str);
        uint16_t go_y = screen_height / 2 - (uint16_t)char_height;
        font_draw_string_colored(go_str, (screen_width - tw1) / 2, go_y, 0xE53935, 0, false);

        int btn_x = ((int)screen_width - RESTART_BTN_W) / 2;
        int btn_y = (int)go_y + char_height + 20;
        bool hovered = ui_button_hit(btn_x, btn_y, RESTART_BTN_W, RESTART_BTN_H, cursor_x, cursor_y);
        ui_draw_button(btn_x, btn_y, RESTART_BTN_W, RESTART_BTN_H, "RESTART", hovered);
    }
}

bool dino_game_handle_click() {
    if (!game_over) return false;
    int go_y = (int)screen_height / 2 - char_height;
    int btn_x = ((int)screen_width - RESTART_BTN_W) / 2;
    int btn_y = go_y + char_height + 20;
    if (!ui_button_hit(btn_x, btn_y, RESTART_BTN_W, RESTART_BTN_H, cursor_x, cursor_y)) return false;
    dino_game_reset();
    return true;
}

void dino_game_destroy(void) {
    sprite_destroy(spr_run[0]);
    sprite_destroy(spr_run[1]);
    sprite_destroy(spr_duck[0]);
    sprite_destroy(spr_duck[1]);
    sprite_destroy(spr_duck[2]);
    sprite_destroy(spr_cactus_small[0]);
    sprite_destroy(spr_cactus_small[1]);
    sprite_destroy(spr_cactus_small[2]);
    sprite_destroy(spr_cactus_large[0]);
    sprite_destroy(spr_cactus_large[1]);
    sprite_destroy(spr_cactus_large[2]);
    sprite_destroy(spr_ptero_frame[0]);
    sprite_destroy(spr_ptero_frame[1]);
    sprite_destroy(spr_dead);
    sprite_destroy(spr_life);
    spr_run[0] = spr_run[1] = spr_duck[0] = spr_duck[1] = spr_duck[2] = NULL;
    spr_cactus_small[0] = spr_cactus_small[1] = spr_cactus_small[2] = spr_cactus_large[0] = spr_cactus_large[1] = spr_cactus_large[2] = NULL;
    spr_ptero_frame[0] = spr_ptero_frame[1] = NULL;
    spr_dead = spr_life = NULL;
}