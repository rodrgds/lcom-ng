#include "menu.h"

#define MAKE_CODE_UP    0x48
#define MAKE_CODE_DOWN  0x50
#define MAKE_CODE_ENTER 0x1C
#define MAKE_CODE_W     0x11
#define MAKE_CODE_S     0x1F


int run_menu(Game *game, uint8_t kb_mask) {
    static bool initialized = false;
    static xpm_image_t imgs[4];
    static bool loaded[4] = {false, false, false, false};
    const char *const *xpm_list[4] = { play_xpm, leaderboard_xpm, instructions_xpm, quit_xpm };

    if (!initialized) {
        vg_initialize(VIDEO_MODE); // Só uma vez!

        load_xpm(space_xpm, &bg_img);
        for (int i = 0; i < 4; i++) {
            if (!loaded[i]) {
                load_xpm(xpm_list[i], &imgs[i]);
                loaded[i] = true;
            }
        }
        initialized = true;
    }

    int selected_option = 0;
    int last_selected = -1;

    message msg;
    int ipc_status;

    while (game->curState == GAME_STATE_MENU) {
        if (driver_receive(ANY, &msg, &ipc_status) != 0) continue;

        if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE) {
            if (msg.m_notify.interrupts & kb_mask) {
                kbc_ih();

                if ((scancode & 0x80) == 0) {
                    switch (scancode) {
                        case MAKE_CODE_UP:
                        case MAKE_CODE_W:
                            selected_option = (selected_option + 3) % 4;
                            break;
                        case MAKE_CODE_DOWN:
                        case MAKE_CODE_S:
                            selected_option = (selected_option + 1) % 4;
                            break;
                        case MAKE_CODE_ENTER:
                            switch (selected_option) {
                                case 0: game->curState = GAME_STATE_GAME; break;
                                case 1: game->curState = GAME_STATE_LEADERBOARD; break;
                                case 2: game->curState = GAME_STATE_INSTRUCTIONS; break;
                                case 3: game->curState = GAME_STATE_EXIT; break;
                            }
                            break;
                    }
                }

                scancode = 0;
            }
        }

        if (selected_option != last_selected) {
            draw_menu(selected_option, imgs, &bg_img);
            last_selected = selected_option;
        }
    }

    return 0;
}



void draw_menu(int selected, xpm_image_t imgs[4], xpm_image_t *bg_img) {
    vg_clear_screen();
    draw_background(bg_img);

    static xpm_image_t earthdefender_img;
    static bool earthdefender_loaded = false;

    if (!earthdefender_loaded) {
        load_xpm(earthdefender_xpm, &earthdefender_img);
        earthdefender_loaded = true;
    }

    draw_sprite(&earthdefender_img, (SCREEN_WIDTH-earthdefender_img.width)/2, 30);

    int y_start = 300;
    int spacing = 100;

    for (int i = 0; i < 4; i++) {
        int x = (SCREEN_WIDTH - imgs[i].width) / 2;
        int y = y_start + i * spacing;

        if (i == selected) {
            int padding = 10;
            vg_draw_rectangle(x - padding, y - padding,imgs[i].width + 2 * padding,imgs[i].height + 2 * padding, 63);
        }

        draw_sprite(&imgs[i], x, y);
    }

    swap_buffer();
}



void draw_instructions(Game *game, uint8_t kb_mask) {
    
    

    static xpm_image_t menu_instructions_img;
    static bool loaded = false;

    if (!loaded) {
        load_xpm(menu_instructions_xpm, &menu_instructions_img);
        loaded = true;
    }

    vg_clear_screen();
    draw_background(&bg_img);
    
    
    int box_width = 400;
    int box_height = 400;
    int box_x = (SCREEN_WIDTH - box_width) / 2;
    int box_y = (SCREEN_HEIGHT - box_height) / 2;
    vg_draw_rectangle(box_x, box_y, box_width, box_height, 0); 
    
    int img_x = box_x + (box_width - menu_instructions_img.width) / 2;
    int img_y = box_y + (box_height - menu_instructions_img.height) / 2;

    draw_sprite(&menu_instructions_img, img_x, img_y);

    swap_buffer();
    scancode = 0;

    message msg;
    int ipc_status;

    while (true) {
        if (driver_receive(ANY, &msg, &ipc_status) != 0)
            continue;

        if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE) {
            if (msg.m_notify.interrupts & kb_mask) {
                kbc_ih();
                if ((scancode & 0x80) == 0) {
                    game->curState = GAME_STATE_MENU;
                    break;
                }
            }
        }
    }
}

