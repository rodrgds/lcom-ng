#include "xpms/menu_bg.xpm"
#include "xpms/mouse_cursor.xpm"
#include "xpms/selected1.xpm"
#include "xpms/Instructions.xpm"

#include <lcom/lcf.h>
#include <lcom/xpm.h>
#include "menu.h"
#include "entity.h"
#include "gameState.h"
#include "controllers/video/graphics.h"


extern Cursor *cursor;
const uint8_t xpm_source_bpp = 2; //5:6:5
uint8_t *bg_menu, *bg_game, *bg_instructions;
xpm_image_t menu_bg, game_bg, instructions_bg;

/**
 * @brief Loads the visual assets required for the main menu screen.
 *
 * This function specifically loads the background XPM image for the menu
 * using `xpm_load`. The loaded pixmap data is stored in `bg_menu` and
 * its properties (like width and height) in `menu_bg`.
 * It assumes `menu_bg_xpm` (an array of strings defining the XPM) and
 * `XPM_5_6_5` (the desired XPM image type) are defined elsewhere.
 * The global variables `bg_menu` (pointer to pixmap data) and `menu_bg`
 * (xpm_image_t structure) are updated by this function.
 */
void load_menu_assets() {
    // Load the XPM images
    bg_menu = xpm_load(menu_bg_xpm, XPM_5_6_5, &menu_bg);
    // It would be good practice to check if bg_menu is NULL here and handle the error.
    // e.g., if (!bg_menu) printf("Failed to load menu background XPM!\n");
}

/**
 * @brief Draws the main menu screen.
 *
 * This function renders the pre-loaded main menu background image onto the screen.
 * It uses `vg_draw_scaled_pixmap` to draw the `bg_menu` pixmap, scaling it
 * from its original dimensions (`menu_bg.width`, `menu_bg.height`) to fit
 * the full screen resolution (`mode_info.XResolution`, `mode_info.YResolution`).
 * It assumes `xpm_source_bpp` (bits per pixel of the source XPM) and `mode_info`
 * (containing screen resolution) are correctly set.
 */
void draw_menu() {
    vg_draw_scaled_pixmap(bg_menu, menu_bg.width, menu_bg.height, xpm_source_bpp, 0, 0, mode_info.XResolution, mode_info.YResolution);
}


/**
 * @brief Loads the visual assets required for the instructions screen.
 *
 * This function loads the background XPM image for the instructions screen
 * using `xpm_load`. The loaded pixmap data is stored in `bg_instructions`
 * and its properties in `instructions_bg`.
 * It assumes `instructions_xpm` and `XPM_5_6_5` are defined.
 * The global variables `bg_instructions` and `instructions_bg` are updated.
 * Includes a basic check to see if the XPM loading failed.
 */
void load_instructions_assets() {
    bg_instructions = xpm_load(instructions_xpm, XPM_5_6_5, &instructions_bg);
    if (!bg_instructions) printf("Failed to load instructions XPM!\n");
}

/**
 * @brief Draws the instructions screen.
 *
 * This function renders the pre-loaded instructions background image onto the screen.
 * It uses `vg_draw_scaled_pixmap` to draw the `bg_instructions` pixmap,
 * scaling it from its original dimensions (`instructions_bg.width`,
 * `instructions_bg.height`) to fit the full screen resolution
 * (`mode_info.XResolution`, `mode_info.YResolution`).
 * It assumes `xpm_source_bpp` and `mode_info` are correctly set.
 */
void draw_instructions() {
    vg_draw_scaled_pixmap(bg_instructions, instructions_bg.width, instructions_bg.height, xpm_source_bpp, 0, 0, mode_info.XResolution, mode_info.YResolution);
}
