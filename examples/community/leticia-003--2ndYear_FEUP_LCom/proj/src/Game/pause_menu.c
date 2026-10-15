/**
 * @file pause_menu.c
 * @brief Implements the pause menu logic, drawing, and input handling for the game.
 */

#include "pause_menu.h"
#include "Drivers/VBE/vbe.h"
#include <lcom/xpm.h>
#include <stdio.h>
#include "game_state.h"
#include "Sprites/start_sprite.h"
#include "game_logic.h"

/**
 * @enum PauseOption
 * @brief Represents the selectable options in the pause menu.
 */
typedef enum {
    PAUSE_OPTION_RESUME, 
    PAUSE_OPTION_QUIT, 
    PAUSE_OPTION_COUNT  
} PauseOption;

extern size_t scancode_size;
extern bool needs_redraw;
extern uint8_t scancode[];

static PauseOption selected_pause_option = PAUSE_OPTION_RESUME;
static bool active = false;

static xpm_image_t pause_menu_img;
static uint8_t *pause_menu_map = NULL;

static xpm_image_t resume_img, quit_img;
static uint8_t *resume_map = NULL, *quit_map = NULL;

static xpm_image_t resume_hovered_img, quit_hovered_img;
static uint8_t *resume_hovered_map = NULL;
static uint8_t *quit_hovered_map = NULL;

/**
 * @brief Loads all pause menu assets (sprites, images, etc).
 */
void pause_menu_assets_init(void) {
    pause_menu_map = xpm_load(pause_menu_xpm, XPM_8_8_8_8, &pause_menu_img);
    if (!pause_menu_map) {
        printf("❌ Failed to load pause_menu_xpm\n");
    } 

    resume_map = xpm_load(resume_button_xpm, XPM_8_8_8_8, &resume_img);
    quit_map   = xpm_load(quit_button_xpm,   XPM_8_8_8_8, &quit_img);

    if (!resume_map || !quit_map)
        printf("❌ Failed to load resume or quit button\n");
    
    resume_hovered_map = xpm_load(resume_button_hovered_xpm, XPM_8_8_8_8, &resume_hovered_img);
    quit_hovered_map   = xpm_load(quit_button_hovered_xpm,   XPM_8_8_8_8, &quit_hovered_img);
}

/**
 * @brief Frees all loaded pause menu assets.
 */
void pause_menu_assets_destroy(void) {
    if (pause_menu_map) { free(pause_menu_map); pause_menu_map = NULL; }
    if (resume_map) { free(resume_map); resume_map = NULL; }
    if (quit_map)   { free(quit_map);   quit_map = NULL; }
    if (resume_hovered_map) { free(resume_hovered_map); resume_hovered_map = NULL; }
    if (quit_hovered_map)   { free(quit_hovered_map);   quit_hovered_map = NULL; }
}

/**
 * @brief Sets the pause menu active state.
 * @param a true to activate, false to deactivate.
 */
void pause_menu_set_active(bool a) { active = a; }

/**
 * @brief Checks if the pause menu is currently active.
 * @return true if active, false otherwise.
 */
bool pause_menu_is_active(void)    { return active; }

/**
 * @brief Draws the pause menu and its options.
 */
void pause_menu_draw(void) {

    int pause_x = (h_res - pause_menu_img.width) / 2;
    int pause_y = (v_res - pause_menu_img.height) / 2;

    vg_draw_xpm(pause_x, pause_y,
        pause_menu_img.width, pause_menu_img.height,
        pause_menu_map, false);

    int resume_x = pause_x + (pause_menu_img.width - resume_img.width) / 2;
    int resume_y = pause_y + 300;

    int quit_x = pause_x + (pause_menu_img.width - quit_img.width) / 2;
    int quit_y = resume_y + resume_img.height + 70; 

    bool resume_hovered = (selected_pause_option == PAUSE_OPTION_RESUME);
    bool quit_hovered   = (selected_pause_option == PAUSE_OPTION_QUIT);

    vg_draw_xpm(resume_x, resume_y,
        resume_hovered ? resume_img.width  : resume_img.width,
        resume_hovered ? resume_img.height : resume_img.height,
        resume_hovered ? resume_hovered_map : resume_map,
        false);

    vg_draw_xpm(quit_x, quit_y,
        quit_hovered ? quit_img.width  : quit_img.width,
        quit_hovered ? quit_img.height : quit_img.height,
        quit_hovered ? quit_hovered_map : quit_map,
        false);
}

/**
 * @brief Handles keyboard input for the pause menu, updating selection and state.
 * @param scancode Pointer to the scancode array.
 * @param size Size of the scancode array.
 */
void pause_menu_handle_input(uint8_t *scancode, size_t size) {
    bool is_enter = (size == 1 && scancode[0] == 0x1C) ||
                    (size == 2 && scancode[0] == 0xE0 && scancode[1] == 0x1C);

    if (is_enter) {
        if (selected_pause_option == PAUSE_OPTION_RESUME) {
            printf("[PAUSE] Resume selected → resuming game\n");
            pause_menu_set_active(false);
            game_set_state(STATE_PLAYING);
            needs_redraw = true;
        }
        else if (selected_pause_option == PAUSE_OPTION_QUIT) {
            printf("[PAUSE] Quit selected → returning to menu\n");
            pause_menu_set_active(false);
            game_set_state(STATE_MENU);
            needs_redraw = true;
        }
        return;
    }

    if (size == 2 && scancode[0] == 0xE0) {
        switch (scancode[1]) {
            case 0x48:
                selected_pause_option = (selected_pause_option + PAUSE_OPTION_COUNT - 1) % PAUSE_OPTION_COUNT;
                needs_redraw = true;
                break;

            case 0x50:
                selected_pause_option = (selected_pause_option + 1) % PAUSE_OPTION_COUNT;
                needs_redraw = true;
                break;
        }
    }
}


