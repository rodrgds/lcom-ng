#include "hud.h"
#include "font.h"
#include "v_card.h"
#include "game_state.h"
#include "plants_cooldown.h"
#include "board.h"

#include "../sprites/plants/peashooter/peashooter_icon_loaded_selected.xpm"
#include "../sprites/plants/peashooter/peashooter_icon_loaded_unselected.xpm"
#include "../sprites/plants/peashooter/peashooter_icon_unloaded_selected.xpm"
#include "../sprites/plants/peashooter/peashooter_icon_unloaded_unselected.xpm"
#include "../sprites/plants/sunflower/sunflower_icon_loaded_selected.xpm"
#include "../sprites/plants/sunflower/sunflower_icon_loaded_unselected.xpm"
#include "../sprites/plants/sunflower/sunflower_icon_unloaded_selected.xpm"
#include "../sprites/plants/sunflower/sunflower_icon_unloaded_unselected.xpm"
#include "../sprites/plants/nut/nut_icon_loaded_selected.xpm"
#include "../sprites/plants/nut/nut_icon_loaded_unselected.xpm"
#include "../sprites/plants/nut/nut_icon_unloaded_selected.xpm"
#include "../sprites/plants/nut/nut_icon_unloaded_unselected.xpm"

static xpm_image_t pea_ls_img, pea_lu_img, pea_us_img, pea_uu_img;
static uint8_t *pea_ls_pix = NULL, *pea_lu_pix = NULL, *pea_us_pix = NULL, *pea_uu_pix = NULL;
static xpm_image_t sun_ls_img, sun_lu_img, sun_us_img, sun_uu_img;
static uint8_t *sun_ls_pix = NULL, *sun_lu_pix = NULL, *sun_us_pix = NULL, *sun_uu_pix = NULL;
static xpm_image_t nut_ls_img, nut_lu_img, nut_us_img, nut_uu_img;
static uint8_t *nut_ls_pix = NULL, *nut_lu_pix = NULL, *nut_us_pix = NULL, *nut_uu_pix = NULL;

// load sprites
void hud_init() {
    pea_ls_pix = xpm_load((xpm_map_t) peashooter_icon_loaded_selected_xpm, XPM_8_8_8_8, &pea_ls_img);
    pea_lu_pix = xpm_load((xpm_map_t) peashooter_icon_loaded_unselected_xpm, XPM_8_8_8_8, &pea_lu_img);
    pea_us_pix = xpm_load((xpm_map_t) peashooter_icon_unloaded_selected_xpm, XPM_8_8_8_8, &pea_us_img);
    pea_uu_pix = xpm_load((xpm_map_t) peashooter_icon_unloaded_unselected_xpm, XPM_8_8_8_8, &pea_uu_img);

    sun_ls_pix = xpm_load((xpm_map_t) sunflower_icon_loaded_selected_xpm, XPM_8_8_8_8, &sun_ls_img);
    sun_lu_pix = xpm_load((xpm_map_t) sunflower_icon_loaded_unselected_xpm, XPM_8_8_8_8, &sun_lu_img);
    sun_us_pix = xpm_load((xpm_map_t) sunflower_icon_unloaded_selected_xpm, XPM_8_8_8_8, &sun_us_img);
    sun_uu_pix = xpm_load((xpm_map_t) sunflower_icon_unloaded_unselected_xpm, XPM_8_8_8_8, &sun_uu_img);

    nut_ls_pix = xpm_load((xpm_map_t) nut_icon_loaded_selected_xpm, XPM_8_8_8_8, &nut_ls_img);
    nut_lu_pix = xpm_load((xpm_map_t) nut_icon_loaded_unselected_xpm, XPM_8_8_8_8, &nut_lu_img);
    nut_us_pix = xpm_load((xpm_map_t) nut_icon_unloaded_selected_xpm, XPM_8_8_8_8, &nut_us_img);
    nut_uu_pix = xpm_load((xpm_map_t) nut_icon_unloaded_unselected_xpm, XPM_8_8_8_8, &nut_uu_img);
}

void ui_render_hud(int suns, int scr, int lvl, int selected_plant, bool is_frozen, bool is_shovel_mode, rtc_date *time) {
    // top bar
    vg_draw_rectangle(0, 0, 800, 100, 0x3b4d24);

    // stats
    font_draw_string("SUNS:", 10, 15, 0xFFFF00);
    font_draw_number(suns, 80, 15, 0xFFFF00);

    font_draw_string("SCORE:", 200, 15, 0xFFFFFF);
    font_draw_number(scr, 280, 15, 0xFFFFFF);

    font_draw_string("LVL:", 400, 15, 0xFFFFFF);
    font_draw_number(lvl, 450, 15, 0xFFFFFF);

    uint32_t shovel_color = is_shovel_mode ? 0xFFAA00 : 0x888888;
    font_draw_string("R-SHOVEL", 520, 15, shovel_color);

    // plants
    uint32_t sun_color = (selected_plant == 1) ? 0x00FF00 : 0x888888;
    font_draw_string("1-", 10, 60, sun_color);

    uint32_t pea_color = (selected_plant == 2) ? 0x00FF00 : 0x888888;
    font_draw_string("2-", 110, 60, pea_color);

    uint32_t nut_color = (selected_plant == 3) ? 0x00FF00 : 0x888888;
    font_draw_string("3-", 210, 60, nut_color);

    uint8_t *sun_pix_to_draw = NULL;
    xpm_image_t sun_img_to_draw;
    bool is_sun_selected = (selected_plant == 1);
    bool is_sun_ready = cooldown_is_ready(PLANT_SUNFLOWER);

    bool is_pea_selected = (selected_plant == 2);
    bool is_pea_ready = cooldown_is_ready(PLANT_PEASHOOTER);
    uint8_t *pea_pix_to_draw = NULL;
    xpm_image_t pea_img_to_draw;

    bool is_nut_selected = (selected_plant == 3);
    bool is_nut_ready = cooldown_is_ready(PLANT_NUT);
    uint8_t *nut_pix_to_draw = NULL;
    xpm_image_t nut_img_to_draw;
    

    //choose icon according to plant state
    if (is_sun_ready && is_sun_selected) {
        sun_pix_to_draw = sun_ls_pix;
        sun_img_to_draw = sun_ls_img;
    } else if (is_sun_ready && !is_sun_selected) {
        sun_pix_to_draw = sun_lu_pix;
        sun_img_to_draw = sun_lu_img;
    } else if (!is_sun_ready && is_sun_selected) {
        sun_pix_to_draw = sun_us_pix;
        sun_img_to_draw = sun_us_img;
    } else {
        sun_pix_to_draw = sun_uu_pix;
        sun_img_to_draw = sun_uu_img;
    }

    if (is_pea_ready && is_pea_selected) {
        pea_pix_to_draw = pea_ls_pix;
        pea_img_to_draw = pea_ls_img;
    } else if (is_pea_ready && !is_pea_selected) {
        pea_pix_to_draw = pea_lu_pix;
        pea_img_to_draw = pea_lu_img;
    } else if (!is_pea_ready && is_pea_selected) {
        pea_pix_to_draw = pea_us_pix;
        pea_img_to_draw = pea_us_img;
    } else {
        pea_pix_to_draw = pea_uu_pix;
        pea_img_to_draw = pea_uu_img;
    }

    if (is_nut_ready && is_nut_selected) {
        nut_pix_to_draw = nut_ls_pix;
        nut_img_to_draw = nut_ls_img;
    } else if (is_nut_ready && !is_nut_selected) {
        nut_pix_to_draw = nut_lu_pix;
        nut_img_to_draw = nut_lu_img;
    } else if (!is_nut_ready && is_nut_selected) {
        nut_pix_to_draw = nut_us_pix;
        nut_img_to_draw = nut_us_img;
    } else {
        nut_pix_to_draw = nut_uu_pix;
        nut_img_to_draw = nut_uu_img;
    }

    if (sun_pix_to_draw != NULL) {
        vg_draw_xpm(sun_img_to_draw, sun_pix_to_draw, 40, 36); 
    }

    if (pea_pix_to_draw != NULL) {
        vg_draw_xpm(pea_img_to_draw, pea_pix_to_draw, 140, 36); 
    }

    if (nut_pix_to_draw != NULL) {
        vg_draw_xpm(nut_img_to_draw, nut_pix_to_draw, 240, 36); 
    }

    if (is_frozen) {
        font_draw_string("FROZEN", 520, 60, 0x0000FF);
    } else {
        font_draw_string("FRZ-SPC", 520, 60, 0x888888);
    }

    if (time != NULL) {
        char ts[9];
        ts[0] = '0' + time->hours / 10;
        ts[1] = '0' + time->hours % 10;
        ts[2] = ':';
        ts[3] = '0' + time->minutes / 10;
        ts[4] = '0' + time->minutes % 10;
        ts[5] = ':';
        ts[6] = '0' + time->seconds / 10;
        ts[7] = '0' + time->seconds % 10;
        ts[8] = '\0';

        font_draw_string(ts, 680, 15, 0xFFFFFF);
    }
}

void ui_render_level_complete(int suns_spent) {

    vg_draw_rectangle(220, 200, 360, 260, 0x111111);

    font_draw_string("LEVEL COMPLETE!", 310, 230, 0xFFFFFF);
    
    font_draw_string("SUNS SPENT:", 304, 280, 0xFFFFFF);
    font_draw_number(suns_spent, 448, 280, 0xFFFFFF);

    font_draw_string("PRESS ESC TO RETURN", 286, 340, 0xFFFFFF);
    if(current_level < 10){
        font_draw_string("PRESS ENTER FOR NEXT LEVEL", 256, 390, 0x00FF00);
    }
    else{
        font_draw_string("YOU COMPLETED THE GAME!! THX FOR PLAYING :)", 292, 420, 0x00FF00);
    }
}

void ui_render_game_over(int suns_spent) {
    vg_draw_rectangle(220, 200, 360, 260, 0x111111);

    font_draw_string("GAME OVER!", 340, 240, 0xFF0000); 
    
    font_draw_string("SUNS SPENT:", 304, 300, 0xFFFFFF);
    font_draw_number(suns_spent, 448, 300, 0xFFFFFF);

    font_draw_string("PRESS ESC TO RETURN", 286, 380, 0xFFFFFF);
}
