#include "game_over.h"

#include "menu.h"

#include "../engine/graphics/renderer.h"
#include "../engine/graphics/font.h"
#include "../assets/assets.h"
#include "../config.h"

static const uint32_t team_colors[MAX_TEAMS] = { COLOR_TEAM_RED, COLOR_TEAM_BLUE };
static const char *const team_names[MAX_TEAMS] = { "RED", "BLU" };

static void draw_panel(const Font *f, int px, int py, int team, int winner_team) {
    renderer_draw_rect(px - 2, py - 2,
                       GAME_OVER_PANEL_W + 4, GAME_OVER_PANEL_H + 4, team_colors[team]);
    renderer_draw_rect(px, py, GAME_OVER_PANEL_W, GAME_OVER_PANEL_H, COLOR_HUD_BG);
    renderer_draw_rect(px, py, GAME_OVER_PANEL_W, GAME_OVER_HEADER_H, team_colors[team]);
    renderer_draw_rect(px, py + GAME_OVER_HEADER_H, GAME_OVER_PANEL_W, 2, COLOR_HUD_BORDER);

    const char *name = team_names[team];
    int nw = font_measure_string(f, name);
    font_draw_string(f, name,
                     px + (GAME_OVER_PANEL_W - nw) / 2,
                     py + (GAME_OVER_HEADER_H - FONT_GLYPH_HEIGHT) / 2);

    if (winner_team < 0) {
        const char *msg = "DRAW";
        int mw = font_measure_string(f, msg);
        font_draw_string(f, msg, px + (GAME_OVER_PANEL_W - mw) / 2,
                         py + GAME_OVER_LOSE_MSG_OFFSET_Y);
    } else if (team == winner_team) {
        const char *line1 = "WINNER WINNER";
        const char *line2 = "CHICKEN DINNER";
        int w1 = font_measure_string(f, line1);
        int w2 = font_measure_string(f, line2);
        font_draw_string(f, line1, px + (GAME_OVER_PANEL_W - w1) / 2,
                         py + GAME_OVER_WIN_LINE1_OFFSET_Y);
        font_draw_string(f, line2, px + (GAME_OVER_PANEL_W - w2) / 2,
                         py + GAME_OVER_WIN_LINE2_OFFSET_Y);
    } else {
        const char *msg = "GAME OVER ;-;";
        int mw = font_measure_string(f, msg);
        font_draw_string(f, msg, px + (GAME_OVER_PANEL_W - mw) / 2,
                         py + GAME_OVER_LOSE_MSG_OFFSET_Y);
    }
}

void game_over_draw(int winner_team) {
    const Font *f = assets_get_font();
    if (f == NULL) return;


    menu_draw_bg();

    int hint_h = FONT_GLYPH_HEIGHT + 2 * GAME_OVER_PROMPT_PAD_Y;
    int total_h = GAME_OVER_PANEL_H + GAME_OVER_PROMPT_OFFSET_Y + hint_h;
    int py = (SCREEN_H - total_h) / 2;

    int total_w = MAX_TEAMS * GAME_OVER_PANEL_W + (MAX_TEAMS - 1) * GAME_OVER_PANEL_GAP;
    int start_x = (SCREEN_W - total_w) / 2;

    for (int team = 0; team < MAX_TEAMS; team++) {
        int px = start_x + team * (GAME_OVER_PANEL_W + GAME_OVER_PANEL_GAP);
        draw_panel(f, px, py, team, winner_team);
    }

    const char *hint = "PRESS ENTER";
    int hw = font_measure_string(f, hint);
    int hint_box_w = hw + 2 * GAME_OVER_PROMPT_PAD_X;
    int hint_x = (SCREEN_W - hint_box_w) / 2;
    int hint_y = py + GAME_OVER_PANEL_H + GAME_OVER_PROMPT_OFFSET_Y;

    renderer_draw_rect(hint_x - 2, hint_y - 2, hint_box_w + 4, hint_h + 4, COLOR_HUD_BORDER);
    renderer_draw_rect(hint_x, hint_y, hint_box_w, hint_h, COLOR_HUD_BG);
    font_draw_string(f, hint, hint_x + GAME_OVER_PROMPT_PAD_X, hint_y + GAME_OVER_PROMPT_PAD_Y);
}

GameOverAction game_over_update(bool confirm) {
    if (confirm) return GAME_OVER_ACTION_QUIT;
    return GAME_OVER_ACTION_NONE;
}
