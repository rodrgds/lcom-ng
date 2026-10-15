#include "font.h"
#include "video.h"
#include <lcom/lcf.h>

#include "../xpm/font_sourcecodepro_semibold/ascii_colon.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_dash.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_dot.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_excl.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_lower_a.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_lower_b.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_lower_c.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_lower_d.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_lower_e.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_lower_f.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_lower_g.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_lower_h.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_lower_i.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_lower_j.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_lower_k.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_lower_l.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_lower_m.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_lower_n.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_lower_o.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_lower_p.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_lower_q.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_lower_r.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_lower_s.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_lower_t.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_lower_u.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_lower_v.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_lower_w.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_lower_x.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_lower_y.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_lower_z.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_num_0.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_num_1.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_num_2.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_num_3.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_num_4.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_num_5.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_num_6.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_num_7.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_num_8.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_num_9.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_quest.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_upper_A.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_upper_B.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_upper_C.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_upper_D.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_upper_E.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_upper_F.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_upper_G.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_upper_H.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_upper_I.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_upper_J.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_upper_K.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_upper_L.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_upper_M.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_upper_N.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_upper_O.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_upper_P.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_upper_Q.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_upper_R.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_upper_S.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_upper_T.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_upper_U.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_upper_V.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_upper_W.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_upper_X.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_upper_Y.xpm"
#include "../xpm/font_sourcecodepro_semibold/ascii_upper_Z.xpm"

uint16_t char_width = 0;
uint16_t char_height = 0;

static Font *g_font = NULL;

static Font *font_load() {
    Font *f = malloc(sizeof(Font));
    if (f == NULL) return NULL;

    memset(f->chars, 0, sizeof(f->chars));
    f->char_width = 21;
    f->char_height = 42;
    char_width = f->char_width;
    char_height = f->char_height;

    void *map[128];
    memset(map, 0, sizeof(map));

    map['!'] = ascii_excl;
    map['-'] = ascii_dash;
    map['.'] = ascii_dot;
    map[':'] = ascii_colon;
    map['?'] = ascii_quest;

    void *nums[] = {
        ascii_num_0, ascii_num_1, ascii_num_2, ascii_num_3, ascii_num_4,
        ascii_num_5, ascii_num_6, ascii_num_7, ascii_num_8, ascii_num_9,
    };

    for (int i = 0; i <= 9; i++) map['0' + i] = nums[i];

    void *uppers[] = {
        ascii_upper_A, ascii_upper_B, ascii_upper_C, ascii_upper_D,
        ascii_upper_E, ascii_upper_F, ascii_upper_G, ascii_upper_H,
        ascii_upper_I, ascii_upper_J, ascii_upper_K, ascii_upper_L,
        ascii_upper_M, ascii_upper_N, ascii_upper_O, ascii_upper_P,
        ascii_upper_Q, ascii_upper_R, ascii_upper_S, ascii_upper_T,
        ascii_upper_U, ascii_upper_V, ascii_upper_W, ascii_upper_X,
        ascii_upper_Y, ascii_upper_Z,
    };

    for (int i = 0; i < 26; i++) map['A' + i] = uppers[i];

    void *lowers[] = {
        ascii_lower_a, ascii_lower_b, ascii_lower_c, ascii_lower_d,
        ascii_lower_e, ascii_lower_f, ascii_lower_g, ascii_lower_h,
        ascii_lower_i, ascii_lower_j, ascii_lower_k, ascii_lower_l,
        ascii_lower_m, ascii_lower_n, ascii_lower_o, ascii_lower_p,
        ascii_lower_q, ascii_lower_r, ascii_lower_s, ascii_lower_t,
        ascii_lower_u, ascii_lower_v, ascii_lower_w, ascii_lower_x,
        ascii_lower_y, ascii_lower_z,
    };

    for (int i = 0; i < 26; i++) map['a' + i] = lowers[i];

    for (int c = 0; c < 128; c++) {
        if (map[c] == NULL) continue;

        f->chars[c] = sprite_load_alpha((xpm_map_t)map[c]);
    }

    return f;
}

Font *(font_get)() {
    if (g_font == NULL) g_font = font_load();

    return g_font;
}

void(font_draw_char_colored)(char c, uint16_t x, uint16_t y, uint32_t fg, uint32_t bg, bool has_bg) {
    Font *f = g_font;
    if (f == NULL) return;

    unsigned char uc = (unsigned char)c;
    if (uc >= 128 || f->chars[uc] == NULL) return;

    sprite_draw_colored(f->chars[uc], x, y, fg, bg, has_bg);
}

void(font_draw_string_colored)(const char *str, uint16_t x, uint16_t y, uint32_t fg, uint32_t bg, bool has_bg) {

    Font *f = g_font;
    if (f == NULL || str == NULL) return;

    uint16_t cx = x;
    for (int i = 0; str[i] != '\0'; i++) {
        font_draw_char_colored(str[i], cx, y, fg, bg, has_bg);
        cx += f->char_width;
    }
}

void (font_draw_centered)(const char *str, uint16_t y, uint32_t fg) {
    uint16_t x = (screen_width - font_string_width(str)) / 2;
    font_draw_string_colored(str, x, y, fg, 0, false);
}

uint16_t(font_string_width)(const char *str) {
    Font *f = g_font;
    if (f == NULL || str == NULL) return 0;

    uint16_t len = 0;
    while (*str++) len++;

    return len * f->char_width;
}

void(font_destroy)() {
    if (g_font == NULL) return;

    for (int i = 0; i < 128; i++) sprite_destroy(g_font->chars[i]);

    free(g_font);
    g_font = NULL;
}
