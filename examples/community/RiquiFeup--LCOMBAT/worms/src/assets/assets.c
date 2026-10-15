#include "assets.h"

#include <stdlib.h>

#include "xpm_includes.h"
#include "../engine/graphics/font.h"

#define WALK_FRAME_COUNT    15
#define JUMP_FRAME_COUNT    15
#define DIE_FRAME_COUNT     60
#define IDLE1_FRAME_COUNT   20
#define IDLE2_FRAME_COUNT    6
#define IDLE3_FRAME_COUNT   12
#define IDLE4_FRAME_COUNT   36
#define EXPLOSION_FRAME_COUNT 8
#define BAZOOKA_PULL_FRAME_COUNT 7
#define BAZOOKA_RETRACT_FRAME_COUNT 7
#define GRENADE_PULL_FRAME_COUNT 10
#define GRENADE_RETRACT_FRAME_COUNT 10
#define FALL_FRAME_COUNT 2
#define MISSILE_ANGLE_COUNT 32
#define GRENADE_ANGLE_COUNT 32
#define FONT_LETTER_COUNT 26
#define FONT_DIGIT_COUNT 10


static Sprite *s_walk_l[WALK_FRAME_COUNT];
static Sprite *s_walk_r[WALK_FRAME_COUNT];
static Sprite *s_jump_l[JUMP_FRAME_COUNT];
static Sprite *s_jump_r[JUMP_FRAME_COUNT];
static Sprite *s_die[DIE_FRAME_COUNT];
static Sprite *s_idle1_r[IDLE1_FRAME_COUNT];
static Sprite *s_idle1_l[IDLE1_FRAME_COUNT];
static Sprite *s_idle2_r[IDLE2_FRAME_COUNT];
static Sprite *s_idle2_l[IDLE2_FRAME_COUNT];
static Sprite *s_idle3_r[IDLE3_FRAME_COUNT];
static Sprite *s_idle3_l[IDLE3_FRAME_COUNT];
static Sprite *s_idle4_r[IDLE4_FRAME_COUNT];
static Sprite *s_idle4_l[IDLE4_FRAME_COUNT];
static Sprite *s_bazooka;
static Sprite *s_explosion[EXPLOSION_FRAME_COUNT];
static Sprite *s_bazooka_pull_l[BAZOOKA_PULL_FRAME_COUNT];
static Sprite *s_bazooka_pull_r[BAZOOKA_PULL_FRAME_COUNT];
static Sprite *s_bazooka_retract_l[BAZOOKA_RETRACT_FRAME_COUNT];
static Sprite *s_bazooka_retract_r[BAZOOKA_RETRACT_FRAME_COUNT];
static Sprite *s_grenade_pull_l[GRENADE_PULL_FRAME_COUNT];
static Sprite *s_grenade_pull_r[GRENADE_PULL_FRAME_COUNT];
static Sprite *s_grenade_retract_l[GRENADE_RETRACT_FRAME_COUNT];
static Sprite *s_grenade_retract_r[GRENADE_RETRACT_FRAME_COUNT];
static Sprite *s_fall_l[FALL_FRAME_COUNT];
static Sprite *s_fall_r[FALL_FRAME_COUNT];
static Sprite *s_missile_angles[MISSILE_ANGLE_COUNT];
static Sprite *s_grenade_angles[GRENADE_ANGLE_COUNT];
static Sprite *s_grenade_icon;
static Sprite *s_background;
static Sprite *s_terrain;
static Sprite *s_terrain_dirt;
static Sprite *s_water;
static Sprite *s_wind_l;
static Sprite *s_wind_r;
static Sprite *s_crosshair = NULL;
static Sprite *s_arrow = NULL;
static Sprite *s_cursor = NULL;
static Sprite *s_font_glyphs[FONT_GLYPH_COUNT];
static Font   *s_font = NULL;
static Sprite *s_logo_menu = NULL;

static int load_one(Sprite **slot, xpm_map_t xpm) {
    *slot = create_sprite_xpm(xpm);
    return (*slot == NULL) ? 1 : 0;
}

static int load_logo_menu(void) {
#ifdef HAVE_LOGO_XPM
    return load_one(&s_logo_menu, (xpm_map_t)Logo_200x200);
#else
    return 0;
#endif
}

int assets_init(void) {


    static xpm_map_t walk_r_maps[WALK_FRAME_COUNT] = {
        (xpm_map_t)wright1,  (xpm_map_t)wright2,  (xpm_map_t)wright3,
        (xpm_map_t)wright4,  (xpm_map_t)wright5,  (xpm_map_t)wright6,
        (xpm_map_t)wright7,  (xpm_map_t)wright8,  (xpm_map_t)wright9,
        (xpm_map_t)wright10, (xpm_map_t)wright11, (xpm_map_t)wright12,
        (xpm_map_t)wright13, (xpm_map_t)wright14, (xpm_map_t)wright15
    };
    for (int i = 0; i < WALK_FRAME_COUNT; i++) {
        if (load_one(&s_walk_r[i], walk_r_maps[i])) return 1;
        s_walk_l[i] = create_sprite_flipped_horizontal(s_walk_r[i]);
        if (s_walk_l[i] == NULL) return 1;
    }


    static xpm_map_t jump_r_maps[JUMP_FRAME_COUNT] = {
        (xpm_map_t)jumpr1,  (xpm_map_t)jumpr2,  (xpm_map_t)jumpr3,
        (xpm_map_t)jumpr4,  (xpm_map_t)jumpr5,  (xpm_map_t)jumpr6,
        (xpm_map_t)jumpr7,  (xpm_map_t)jumpr8,  (xpm_map_t)jumpr9,
        (xpm_map_t)jumpr10, (xpm_map_t)jumpr11, (xpm_map_t)jumpr12,
        (xpm_map_t)jumpr13, (xpm_map_t)jumpr14, (xpm_map_t)jumpr15
    };
    for (int i = 0; i < JUMP_FRAME_COUNT; i++) {
        if (load_one(&s_jump_r[i], jump_r_maps[i])) return 1;
        s_jump_l[i] = create_sprite_flipped_horizontal(s_jump_r[i]);
        if (s_jump_l[i] == NULL) return 1;
    }

    static xpm_map_t die_maps[DIE_FRAME_COUNT] = {
        (xpm_map_t)die_die1,  (xpm_map_t)die_die2,  (xpm_map_t)die_die3,  (xpm_map_t)die_die4,
        (xpm_map_t)die_die5,  (xpm_map_t)die_die6,  (xpm_map_t)die_die7,  (xpm_map_t)die_die8,
        (xpm_map_t)die_die9,  (xpm_map_t)die_die10, (xpm_map_t)die_die11, (xpm_map_t)die_die12,
        (xpm_map_t)die_die13, (xpm_map_t)die_die14, (xpm_map_t)die_die15, (xpm_map_t)die_die16,
        (xpm_map_t)die_die17, (xpm_map_t)die_die18, (xpm_map_t)die_die19, (xpm_map_t)die_die20,
        (xpm_map_t)die_die21, (xpm_map_t)die_die22, (xpm_map_t)die_die23, (xpm_map_t)die_die24,
        (xpm_map_t)die_die25, (xpm_map_t)die_die26, (xpm_map_t)die_die27, (xpm_map_t)die_die28,
        (xpm_map_t)die_die29, (xpm_map_t)die_die30, (xpm_map_t)die_die31, (xpm_map_t)die_die32,
        (xpm_map_t)die_die33, (xpm_map_t)die_die34, (xpm_map_t)die_die35, (xpm_map_t)die_die36,
        (xpm_map_t)die_die37, (xpm_map_t)die_die38, (xpm_map_t)die_die39, (xpm_map_t)die_die40,
        (xpm_map_t)die_die41, (xpm_map_t)die_die42, (xpm_map_t)die_die43, (xpm_map_t)die_die44,
        (xpm_map_t)die_die45, (xpm_map_t)die_die46, (xpm_map_t)die_die47, (xpm_map_t)die_die48,
        (xpm_map_t)die_die49, (xpm_map_t)die_die50, (xpm_map_t)die_die51, (xpm_map_t)die_die52,
        (xpm_map_t)die_die53, (xpm_map_t)die_die54, (xpm_map_t)die_die55, (xpm_map_t)die_die56,
        (xpm_map_t)die_die57, (xpm_map_t)die_die58, (xpm_map_t)die_die59, (xpm_map_t)die_die60
    };
    for (int i = 0; i < DIE_FRAME_COUNT; i++)
        if (load_one(&s_die[i], die_maps[i])) return 1;

    static xpm_map_t idle1_maps[IDLE1_FRAME_COUNT] = {
        (xpm_map_t)worm_idle1_r0,  (xpm_map_t)worm_idle1_r1,
        (xpm_map_t)worm_idle1_r2,  (xpm_map_t)worm_idle1_r3,
        (xpm_map_t)worm_idle1_r4,  (xpm_map_t)worm_idle1_r5,
        (xpm_map_t)worm_idle1_r6,  (xpm_map_t)worm_idle1_r7,
        (xpm_map_t)worm_idle1_r8,  (xpm_map_t)worm_idle1_r9,
        (xpm_map_t)worm_idle1_r10, (xpm_map_t)worm_idle1_r11,
        (xpm_map_t)worm_idle1_r12, (xpm_map_t)worm_idle1_r13,
        (xpm_map_t)worm_idle1_r14, (xpm_map_t)worm_idle1_r15,
        (xpm_map_t)worm_idle1_r16, (xpm_map_t)worm_idle1_r17,
        (xpm_map_t)worm_idle1_r18, (xpm_map_t)worm_idle1_r19
    };
    for (int i = 0; i < IDLE1_FRAME_COUNT; i++) {
        if (load_one(&s_idle1_l[i], idle1_maps[i])) return 1;
        s_idle1_r[i] = create_sprite_flipped_horizontal(s_idle1_l[i]);
        if (s_idle1_r[i] == NULL) return 1;
    }

    static xpm_map_t idle2_maps[IDLE2_FRAME_COUNT] = {
        (xpm_map_t)worm_idle2_r0, (xpm_map_t)worm_idle2_r1,
        (xpm_map_t)worm_idle2_r2, (xpm_map_t)worm_idle2_r3,
        (xpm_map_t)worm_idle2_r4, (xpm_map_t)worm_idle2_r5
    };
    for (int i = 0; i < IDLE2_FRAME_COUNT; i++) {
        if (load_one(&s_idle2_l[i], idle2_maps[i])) return 1;
        s_idle2_r[i] = create_sprite_flipped_horizontal(s_idle2_l[i]);
        if (s_idle2_r[i] == NULL) return 1;
    }

    static xpm_map_t idle3_maps[IDLE3_FRAME_COUNT] = {
        (xpm_map_t)worm_idle3_r0,  (xpm_map_t)worm_idle3_r1,
        (xpm_map_t)worm_idle3_r2,  (xpm_map_t)worm_idle3_r3,
        (xpm_map_t)worm_idle3_r4,  (xpm_map_t)worm_idle3_r5,
        (xpm_map_t)worm_idle3_r6,  (xpm_map_t)worm_idle3_r7,
        (xpm_map_t)worm_idle3_r8,  (xpm_map_t)worm_idle3_r9,
        (xpm_map_t)worm_idle3_r10, (xpm_map_t)worm_idle3_r11
    };
    for (int i = 0; i < IDLE3_FRAME_COUNT; i++) {
        if (load_one(&s_idle3_l[i], idle3_maps[i])) return 1;
        s_idle3_r[i] = create_sprite_flipped_horizontal(s_idle3_l[i]);
        if (s_idle3_r[i] == NULL) return 1;
    }

    static xpm_map_t idle4_maps[IDLE4_FRAME_COUNT] = {
        (xpm_map_t)worm_idle4_r0,  (xpm_map_t)worm_idle4_r1,
        (xpm_map_t)worm_idle4_r2,  (xpm_map_t)worm_idle4_r3,
        (xpm_map_t)worm_idle4_r4,  (xpm_map_t)worm_idle4_r5,
        (xpm_map_t)worm_idle4_r6,  (xpm_map_t)worm_idle4_r7,
        (xpm_map_t)worm_idle4_r8,  (xpm_map_t)worm_idle4_r9,
        (xpm_map_t)worm_idle4_r10, (xpm_map_t)worm_idle4_r11,
        (xpm_map_t)worm_idle4_r12, (xpm_map_t)worm_idle4_r13,
        (xpm_map_t)worm_idle4_r14, (xpm_map_t)worm_idle4_r15,
        (xpm_map_t)worm_idle4_r16, (xpm_map_t)worm_idle4_r17,
        (xpm_map_t)worm_idle4_r18, (xpm_map_t)worm_idle4_r19,
        (xpm_map_t)worm_idle4_r20, (xpm_map_t)worm_idle4_r21,
        (xpm_map_t)worm_idle4_r22, (xpm_map_t)worm_idle4_r23,
        (xpm_map_t)worm_idle4_r24, (xpm_map_t)worm_idle4_r25,
        (xpm_map_t)worm_idle4_r26, (xpm_map_t)worm_idle4_r27,
        (xpm_map_t)worm_idle4_r28, (xpm_map_t)worm_idle4_r29,
        (xpm_map_t)worm_idle4_r30, (xpm_map_t)worm_idle4_r31,
        (xpm_map_t)worm_idle4_r32, (xpm_map_t)worm_idle4_r33,
        (xpm_map_t)worm_idle4_r34, (xpm_map_t)worm_idle4_r35
    };
    for (int i = 0; i < IDLE4_FRAME_COUNT; i++) {
        if (load_one(&s_idle4_l[i], idle4_maps[i])) return 1;
        s_idle4_r[i] = create_sprite_flipped_horizontal(s_idle4_l[i]);
        if (s_idle4_r[i] == NULL) return 1;
    }

    if (load_one(&s_bazooka, (xpm_map_t)bazooka)) return 1;

    static xpm_map_t exp_maps[EXPLOSION_FRAME_COUNT] = {
        (xpm_map_t)explosion_1, (xpm_map_t)explosion_2,
        (xpm_map_t)explosion_3, (xpm_map_t)explosion_4,
        (xpm_map_t)explosion_5, (xpm_map_t)explosion_6,
        (xpm_map_t)explosion_7, (xpm_map_t)explosion_8
    };
    for (int i = 0; i < EXPLOSION_FRAME_COUNT; i++)
        if (load_one(&s_explosion[i], exp_maps[i])) return 1;

    static xpm_map_t bazooka_pull_maps[BAZOOKA_PULL_FRAME_COUNT] = {
        (xpm_map_t)bazooka_pull_0, (xpm_map_t)bazooka_pull_1, (xpm_map_t)bazooka_pull_2,
        (xpm_map_t)bazooka_pull_3, (xpm_map_t)bazooka_pull_4, (xpm_map_t)bazooka_pull_5,
        (xpm_map_t)bazooka_pull_6
    };
    for (int i = 0; i < BAZOOKA_PULL_FRAME_COUNT; i++) {
        if (load_one(&s_bazooka_pull_l[i], bazooka_pull_maps[i])) return 1;
        s_bazooka_pull_r[i] = create_sprite_flipped_horizontal(s_bazooka_pull_l[i]);
        if (s_bazooka_pull_r[i] == NULL) return 1;
    }

    static xpm_map_t bazooka_retract_maps[BAZOOKA_RETRACT_FRAME_COUNT] = {
        (xpm_map_t)bazooka_retract_0, (xpm_map_t)bazooka_retract_1, (xpm_map_t)bazooka_retract_2,
        (xpm_map_t)bazooka_retract_3, (xpm_map_t)bazooka_retract_4, (xpm_map_t)bazooka_retract_5,
        (xpm_map_t)bazooka_retract_6
    };
    for (int i = 0; i < BAZOOKA_RETRACT_FRAME_COUNT; i++) {
        if (load_one(&s_bazooka_retract_l[i], bazooka_retract_maps[i])) return 1;
        s_bazooka_retract_r[i] = create_sprite_flipped_horizontal(s_bazooka_retract_l[i]);
        if (s_bazooka_retract_r[i] == NULL) return 1;
    }

    static xpm_map_t grenade_pull_maps[GRENADE_PULL_FRAME_COUNT] = {
        (xpm_map_t)grenade_pull_0, (xpm_map_t)grenade_pull_1, (xpm_map_t)grenade_pull_2,
        (xpm_map_t)grenade_pull_3, (xpm_map_t)grenade_pull_4, (xpm_map_t)grenade_pull_5,
        (xpm_map_t)grenade_pull_6, (xpm_map_t)grenade_pull_7, (xpm_map_t)grenade_pull_8, (xpm_map_t)grenade_pull_9
    };
    for (int i = 0; i < GRENADE_PULL_FRAME_COUNT; i++) {
        if (load_one(&s_grenade_pull_l[i], grenade_pull_maps[i])) return 1;
        s_grenade_pull_r[i] = create_sprite_flipped_horizontal(s_grenade_pull_l[i]);
        if (s_grenade_pull_r[i] == NULL) return 1;
    }

    static xpm_map_t grenade_retract_maps[GRENADE_RETRACT_FRAME_COUNT] = {
        (xpm_map_t)grenade_retract_0, (xpm_map_t)grenade_retract_1, (xpm_map_t)grenade_retract_2,
        (xpm_map_t)grenade_retract_3, (xpm_map_t)grenade_retract_4, (xpm_map_t)grenade_retract_5,
        (xpm_map_t)grenade_retract_6, (xpm_map_t)grenade_retract_7, (xpm_map_t)grenade_retract_8, (xpm_map_t)grenade_retract_9
    };
    for (int i = 0; i < GRENADE_RETRACT_FRAME_COUNT; i++) {
        if (load_one(&s_grenade_retract_l[i], grenade_retract_maps[i])) return 1;
        s_grenade_retract_r[i] = create_sprite_flipped_horizontal(s_grenade_retract_l[i]);
        if (s_grenade_retract_r[i] == NULL) return 1;
    }

    static xpm_map_t fall_maps[FALL_FRAME_COUNT] = {
        (xpm_map_t)falling_0, (xpm_map_t)falling_1
    };
    for (int i = 0; i < FALL_FRAME_COUNT; i++) {
        if (load_one(&s_fall_r[i], fall_maps[i])) return 1;
        s_fall_l[i] = create_sprite_flipped_horizontal(s_fall_r[i]);
        if (s_fall_l[i] == NULL) return 1;
    }

    static xpm_map_t missile_angle_maps[MISSILE_ANGLE_COUNT] = {
        (xpm_map_t)missile_0, (xpm_map_t)missile_1, (xpm_map_t)missile_2, (xpm_map_t)missile_3,
        (xpm_map_t)missile_4, (xpm_map_t)missile_5, (xpm_map_t)missile_6, (xpm_map_t)missile_7,
        (xpm_map_t)missile_8, (xpm_map_t)missile_9, (xpm_map_t)missile_10, (xpm_map_t)missile_11,
        (xpm_map_t)missile_12, (xpm_map_t)missile_13, (xpm_map_t)missile_14, (xpm_map_t)missile_15,
        (xpm_map_t)missile_16, (xpm_map_t)missile_17, (xpm_map_t)missile_18, (xpm_map_t)missile_19,
        (xpm_map_t)missile_20, (xpm_map_t)missile_21, (xpm_map_t)missile_22, (xpm_map_t)missile_23,
        (xpm_map_t)missile_24, (xpm_map_t)missile_25, (xpm_map_t)missile_26, (xpm_map_t)missile_27,
        (xpm_map_t)missile_28, (xpm_map_t)missile_29, (xpm_map_t)missile_30, (xpm_map_t)missile_31
    };
    for (int i = 0; i < MISSILE_ANGLE_COUNT; i++) {
        if (load_one(&s_missile_angles[i], missile_angle_maps[i])) return 1;
    }

    static xpm_map_t grenade_angle_maps[GRENADE_ANGLE_COUNT] = {
        (xpm_map_t)grenade_0, (xpm_map_t)grenade_1, (xpm_map_t)grenade_2, (xpm_map_t)grenade_3,
        (xpm_map_t)grenade_4, (xpm_map_t)grenade_5, (xpm_map_t)grenade_6, (xpm_map_t)grenade_7,
        (xpm_map_t)grenade_8, (xpm_map_t)grenade_9, (xpm_map_t)grenade_10, (xpm_map_t)grenade_11,
        (xpm_map_t)grenade_12, (xpm_map_t)grenade_13, (xpm_map_t)grenade_14, (xpm_map_t)grenade_15,
        (xpm_map_t)grenade_16, (xpm_map_t)grenade_17, (xpm_map_t)grenade_18, (xpm_map_t)grenade_19,
        (xpm_map_t)grenade_20, (xpm_map_t)grenade_21, (xpm_map_t)grenade_22, (xpm_map_t)grenade_23,
        (xpm_map_t)grenade_24, (xpm_map_t)grenade_25, (xpm_map_t)grenade_26, (xpm_map_t)grenade_27,
        (xpm_map_t)grenade_28, (xpm_map_t)grenade_29, (xpm_map_t)grenade_30, (xpm_map_t)grenade_31
    };
    for (int i = 0; i < GRENADE_ANGLE_COUNT; i++) {
        if (load_one(&s_grenade_angles[i], grenade_angle_maps[i])) return 1;
    }

    if (load_one(&s_grenade_icon, (xpm_map_t)grenade)) return 1;

    if (load_one(&s_background, (xpm_map_t)background_xpm)) return 1;
    if (load_one(&s_terrain,  (xpm_map_t)terrenocomrelva)) return 1;
    if (load_one(&s_terrain_dirt, (xpm_map_t)terreno)) return 1;
    if (load_one(&s_water,    (xpm_map_t)water))           return 1;
    if (load_one(&s_wind_r,   (xpm_map_t)windr))           return 1;
    s_wind_l = create_sprite_flipped_horizontal(s_wind_r);
    if (s_wind_l == NULL) return 1;

    if (load_one(&s_crosshair, (xpm_map_t)crosshair)) return 1;
    if (load_one(&s_arrow, (xpm_map_t)arrowd)) return 1;
    if (load_one(&s_cursor, (xpm_map_t)cursor)) return 1;

    s_font_glyphs[FONT_IDX_SPACE] = NULL;
    static xpm_map_t letter_maps[FONT_LETTER_COUNT] = {
        (xpm_map_t)font_a, (xpm_map_t)font_b, (xpm_map_t)font_c, (xpm_map_t)font_d,
        (xpm_map_t)font_e, (xpm_map_t)font_f, (xpm_map_t)font_g, (xpm_map_t)font_h,
        (xpm_map_t)font_i, (xpm_map_t)font_j, (xpm_map_t)font_k, (xpm_map_t)font_l,
        (xpm_map_t)font_m, (xpm_map_t)font_n, (xpm_map_t)font_o, (xpm_map_t)font_p,
        (xpm_map_t)font_q, (xpm_map_t)font_r, (xpm_map_t)font_s, (xpm_map_t)font_t,
        (xpm_map_t)font_u, (xpm_map_t)font_v, (xpm_map_t)font_w, (xpm_map_t)font_x,
        (xpm_map_t)font_y, (xpm_map_t)font_z,
    };
    for (int i = 0; i < FONT_LETTER_COUNT; i++)
        if (load_one(&s_font_glyphs[FONT_IDX_A + i], letter_maps[i])) return 1;

    static xpm_map_t digit_maps[FONT_DIGIT_COUNT] = {
        (xpm_map_t)font_0, (xpm_map_t)font_1, (xpm_map_t)font_2, (xpm_map_t)font_3,
        (xpm_map_t)font_4, (xpm_map_t)font_5, (xpm_map_t)font_6, (xpm_map_t)font_7,
        (xpm_map_t)font_8, (xpm_map_t)font_9,
    };
    for (int i = 0; i < FONT_DIGIT_COUNT; i++)
        if (load_one(&s_font_glyphs[FONT_IDX_0 + i], digit_maps[i])) return 1;

    s_font = font_create(s_font_glyphs, FONT_CHAR_SPACING);
    if (s_font == NULL) return 1;

    if (load_logo_menu() != 0) return 1;

    return 0;
}

void assets_cleanup(void) {
    int i;

    for (i = 0; i < WALK_FRAME_COUNT; i++) {
        destroy_sprite(s_walk_l[i]); s_walk_l[i] = NULL;
        destroy_sprite(s_walk_r[i]); s_walk_r[i] = NULL;
    }
    for (i = 0; i < JUMP_FRAME_COUNT; i++) {
        destroy_sprite(s_jump_l[i]); s_jump_l[i] = NULL;
        destroy_sprite(s_jump_r[i]); s_jump_r[i] = NULL;
    }
    for (i = 0; i < DIE_FRAME_COUNT; i++) {
        destroy_sprite(s_die[i]); s_die[i] = NULL;
    }
    for (i = 0; i < IDLE1_FRAME_COUNT; i++) {
        destroy_sprite(s_idle1_r[i]); s_idle1_r[i] = NULL;
        destroy_sprite(s_idle1_l[i]); s_idle1_l[i] = NULL;
    }
    for (i = 0; i < IDLE2_FRAME_COUNT; i++) {
        destroy_sprite(s_idle2_r[i]); s_idle2_r[i] = NULL;
        destroy_sprite(s_idle2_l[i]); s_idle2_l[i] = NULL;
    }
    for (i = 0; i < IDLE3_FRAME_COUNT; i++) {
        destroy_sprite(s_idle3_r[i]); s_idle3_r[i] = NULL;
        destroy_sprite(s_idle3_l[i]); s_idle3_l[i] = NULL;
    }
    for (i = 0; i < IDLE4_FRAME_COUNT; i++) {
        destroy_sprite(s_idle4_r[i]); s_idle4_r[i] = NULL;
        destroy_sprite(s_idle4_l[i]); s_idle4_l[i] = NULL;
    }
    destroy_sprite(s_bazooka);   s_bazooka  = NULL;
    for (i = 0; i < EXPLOSION_FRAME_COUNT; i++) {
        destroy_sprite(s_explosion[i]); s_explosion[i] = NULL;
    }
    for (i = 0; i < BAZOOKA_PULL_FRAME_COUNT; i++) {
        destroy_sprite(s_bazooka_pull_l[i]); s_bazooka_pull_l[i] = NULL;
        destroy_sprite(s_bazooka_pull_r[i]); s_bazooka_pull_r[i] = NULL;
    }
    for (i = 0; i < BAZOOKA_RETRACT_FRAME_COUNT; i++) {
        destroy_sprite(s_bazooka_retract_l[i]); s_bazooka_retract_l[i] = NULL;
        destroy_sprite(s_bazooka_retract_r[i]); s_bazooka_retract_r[i] = NULL;
    }
    for (i = 0; i < GRENADE_PULL_FRAME_COUNT; i++) {
        destroy_sprite(s_grenade_pull_l[i]); s_grenade_pull_l[i] = NULL;
        destroy_sprite(s_grenade_pull_r[i]); s_grenade_pull_r[i] = NULL;
    }
    for (i = 0; i < GRENADE_RETRACT_FRAME_COUNT; i++) {
        destroy_sprite(s_grenade_retract_l[i]); s_grenade_retract_l[i] = NULL;
        destroy_sprite(s_grenade_retract_r[i]); s_grenade_retract_r[i] = NULL;
    }
    for (i = 0; i < FALL_FRAME_COUNT; i++) {
        destroy_sprite(s_fall_l[i]); s_fall_l[i] = NULL;
        destroy_sprite(s_fall_r[i]); s_fall_r[i] = NULL;
    }
    for (i = 0; i < MISSILE_ANGLE_COUNT; i++) {
        destroy_sprite(s_missile_angles[i]); s_missile_angles[i] = NULL;
    }
    for (i = 0; i < GRENADE_ANGLE_COUNT; i++) {
        destroy_sprite(s_grenade_angles[i]); s_grenade_angles[i] = NULL;
    }
    destroy_sprite(s_grenade_icon); s_grenade_icon = NULL;
    destroy_sprite(s_background); s_background = NULL;
    destroy_sprite(s_terrain);   s_terrain  = NULL;
    destroy_sprite(s_terrain_dirt); s_terrain_dirt = NULL;
    destroy_sprite(s_water);     s_water    = NULL;
    destroy_sprite(s_wind_l);    s_wind_l   = NULL;
    destroy_sprite(s_wind_r);    s_wind_r   = NULL;
    destroy_sprite(s_crosshair); s_crosshair = NULL;
    destroy_sprite(s_arrow); s_arrow = NULL;
    destroy_sprite(s_cursor);    s_cursor   = NULL;

    font_destroy(s_font); s_font = NULL;
    for (i = 1; i < FONT_GLYPH_COUNT; i++) {
        destroy_sprite(s_font_glyphs[i]); s_font_glyphs[i] = NULL;
    }
    if (s_logo_menu != NULL) { destroy_sprite(s_logo_menu); s_logo_menu = NULL; }
}

Sprite *const *assets_get_walk_l(void)      { return s_walk_l; }
Sprite *const *assets_get_walk_r(void)      { return s_walk_r; }
uint8_t        assets_get_walk_count(void)  { return WALK_FRAME_COUNT; }

Sprite *const *assets_get_jump_l(void)      { return s_jump_l; }
Sprite *const *assets_get_jump_r(void)      { return s_jump_r; }
uint8_t        assets_get_jump_count(void)  { return JUMP_FRAME_COUNT; }

Sprite *const *assets_get_die(void)         { return s_die; }
uint8_t        assets_get_die_count(void)   { return DIE_FRAME_COUNT; }

Sprite *const *assets_get_idle1_l(void)       { return s_idle1_l; }
Sprite *const *assets_get_idle2_l(void)       { return s_idle2_l; }
Sprite *const *assets_get_idle3_l(void)       { return s_idle3_l; }
Sprite *const *assets_get_idle4_l(void)       { return s_idle4_l; }

Sprite *const *assets_get_idle1_r(void)       { return s_idle1_r; }
Sprite *const *assets_get_idle2_r(void)       { return s_idle2_r; }
Sprite *const *assets_get_idle3_r(void)       { return s_idle3_r; }
Sprite *const *assets_get_idle4_r(void)       { return s_idle4_r; }
uint8_t        assets_get_idle1_count(void) { return IDLE1_FRAME_COUNT; }
uint8_t        assets_get_idle2_count(void) { return IDLE2_FRAME_COUNT; }
uint8_t        assets_get_idle3_count(void) { return IDLE3_FRAME_COUNT; }
uint8_t        assets_get_idle4_count(void) { return IDLE4_FRAME_COUNT; }

const Sprite  *assets_get_bazooka(void)          { return s_bazooka; }
Sprite *const *assets_get_explosion(void)         { return s_explosion; }
uint8_t        assets_get_explosion_count(void)   { return EXPLOSION_FRAME_COUNT; }

Sprite *const *assets_get_missile_angles(void)         { return s_missile_angles; }
uint8_t        assets_get_missile_angles_count(void)   { return MISSILE_ANGLE_COUNT; }

Sprite *const *assets_get_grenade_angles(void)         { return s_grenade_angles; }
uint8_t        assets_get_grenade_angles_count(void)   { return GRENADE_ANGLE_COUNT; }

const Sprite  *assets_get_grenade_icon(void)          { return s_grenade_icon; }

Sprite *const *assets_get_bazooka_pull_l(void)      { return s_bazooka_pull_l; }
Sprite *const *assets_get_bazooka_pull_r(void)      { return s_bazooka_pull_r; }
uint8_t        assets_get_bazooka_pull_count(void)  { return BAZOOKA_PULL_FRAME_COUNT; }

Sprite *const *assets_get_bazooka_retract_l(void)      { return s_bazooka_retract_l; }
Sprite *const *assets_get_bazooka_retract_r(void)      { return s_bazooka_retract_r; }
uint8_t        assets_get_bazooka_retract_count(void)  { return BAZOOKA_RETRACT_FRAME_COUNT; }

Sprite *const *assets_get_grenade_pull_l(void)      { return s_grenade_pull_l; }
Sprite *const *assets_get_grenade_pull_r(void)      { return s_grenade_pull_r; }
uint8_t        assets_get_grenade_pull_count(void)  { return GRENADE_PULL_FRAME_COUNT; }

Sprite *const *assets_get_grenade_retract_l(void)      { return s_grenade_retract_l; }
Sprite *const *assets_get_grenade_retract_r(void)      { return s_grenade_retract_r; }
uint8_t        assets_get_grenade_retract_count(void)  { return GRENADE_RETRACT_FRAME_COUNT; }

Sprite *const *assets_get_fall_l(void)      { return s_fall_l; }
Sprite *const *assets_get_fall_r(void)      { return s_fall_r; }
uint8_t        assets_get_fall_count(void)  { return FALL_FRAME_COUNT; }

const Sprite  *assets_get_background(void) { return s_background; }
const Sprite  *assets_get_terrain(void)    { return s_terrain; }
const Sprite  *assets_get_terrain_dirt(void) { return s_terrain_dirt; }
const Sprite  *assets_get_water(void)      { return s_water; }
const Sprite  *assets_get_wind_l(void)     { return s_wind_l; }
const Sprite  *assets_get_wind_r(void)     { return s_wind_r; }

const Sprite  *assets_get_crosshair(void)  { return s_crosshair; }
const Sprite  *assets_get_arrow(void)      { return s_arrow; }
const Sprite  *assets_get_cursor(void)     { return s_cursor; }

const Font    *assets_get_font(void)       { return s_font; }
const Sprite  *assets_get_logo(void)       { return s_logo_menu; }
const Sprite  *assets_get_logo_menu(void)  { return s_logo_menu; }
