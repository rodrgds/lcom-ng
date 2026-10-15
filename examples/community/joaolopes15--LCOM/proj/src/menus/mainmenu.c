#include "mainmenu.h"
#include "../assets/menus/welcome_xpm.h"
#include "../assets/menus/to_xpm.h"
#include "../assets/logo_xpm.h"
#include "../assets/menus/startS_xpm.h"
#include "../assets/menus/startgame_xpm.h"
#include "../assets/menus/howtoplayS_xpm.h"
#include "../assets/menus/howtoplay_xpm.h"
#include "../assets/menus/exitS_xpm.h"
#include "../assets/menus/exit_xpm.h"
#include "../assets/menus/background_xpm.h"
#include "../assets/menus/welcomeBIGGER_xpm.h"

main_menu_t *main_menu_init() {
    main_menu_t* main_menu = (main_menu_t *) malloc(sizeof(main_menu_t));

    if (main_menu == NULL) {
        printf("Error alocating memory for main menu structure\n");
        return NULL;
    }

    main_menu->welcome_sprite = NULL;
    main_menu->to_sprite = NULL;
    main_menu->logo_sprite = NULL;
    main_menu->startS_sprite = NULL;
    main_menu->start_sprite = NULL;
    main_menu->howtoplayS_sprite = NULL;
    main_menu->howtoplay_sprite = NULL;
    main_menu->exitS_sprite = NULL;
    main_menu->exit_sprite = NULL;

    main_menu->welcome_sprite = create_sprite((xpm_map_t) welcomeBIGGER_xpm);
    if (main_menu->welcome_sprite == NULL) {
        destroy_main_menu(main_menu);
        return NULL;
    }

    main_menu->to_sprite = create_sprite((xpm_map_t) to_xpm);
    if (main_menu->to_sprite == NULL) {
        destroy_main_menu(main_menu);
        return NULL;
    }

    main_menu->logo_sprite = create_sprite((xpm_map_t) logo_xpm);
    if (main_menu->logo_sprite == NULL) {
        destroy_main_menu(main_menu);
        return NULL;
    }

    main_menu->startS_sprite = create_sprite((xpm_map_t) startS_xpm);
    if (main_menu->startS_sprite == NULL) {
        destroy_main_menu(main_menu);
        return NULL;
    }

    main_menu->start_sprite = create_sprite((xpm_map_t) startgame_xpm);
    if (main_menu->start_sprite == NULL) {
        destroy_main_menu(main_menu);
        return NULL;
    }

    main_menu->howtoplayS_sprite = create_sprite((xpm_map_t) howtoplayS_xpm);
    if (main_menu->howtoplayS_sprite == NULL) {
        destroy_main_menu(main_menu);
        return NULL;
    }

    main_menu->howtoplay_sprite = create_sprite((xpm_map_t) howtoplay_xpm);
    if (main_menu->howtoplay_sprite == NULL) {
        destroy_main_menu(main_menu);
        return NULL;
    }

    main_menu->exitS_sprite = create_sprite((xpm_map_t) exitS_xpm);
    if (main_menu->exitS_sprite == NULL) {
        destroy_main_menu(main_menu);
        return NULL;
    }

    main_menu->exit_sprite = create_sprite((xpm_map_t) exit_xpm);
    if (main_menu->exit_sprite == NULL) {
        destroy_main_menu(main_menu);
        return NULL;
    }
    
    main_menu->selected_option = 0;
    
    return main_menu;
}

menu_action_t mainmenu_process_input(main_menu_t *main_menu, uint8_t scancode) {
    if (main_menu == NULL) {
        return MENU_ACTION_NONE;
    }
    
    bool is_release = (scancode & 0x80) != 0;
    uint8_t key_code = scancode & 0x7F;
    
    if (!is_release) {
        if (scancode == 0x1C) { // enter key
            if (main_menu->selected_option == 0) {
                return MENU_ACTION_START_GAME;
            } else if (main_menu->selected_option == 1) {
                return MENU_ACTION_HOW_TO_PLAY;
            } else if (main_menu->selected_option == 2) {
                return MENU_ACTION_EXIT;
            }
        } else if (scancode == 0x01) { // esc key to exit
            return MENU_ACTION_EXIT;
        } else if (key_code == 0x48) { // up arrow
            if (main_menu->selected_option > 0) {
                main_menu->selected_option--;
            }
        } else if (key_code == 0x50) { // down arrow
            if (main_menu->selected_option < 2) {
                main_menu->selected_option++;
            }
        }
    }
    return MENU_ACTION_NONE;
}

menu_action_t mainmenu_process_mouse_click(main_menu_t *main_menu, int mouse_x, int mouse_y) {
    if (main_menu == NULL) {
        return MENU_ACTION_NONE;
    }
    
    if (mouse_x >= 250 && mouse_x <= 450 && mouse_y >= 300 && mouse_y <= 380) {
        main_menu->selected_option = 0;
        return MENU_ACTION_START_GAME;
    }
    
    if (mouse_x >= 250 && mouse_x <= 450 && mouse_y >= 400 && mouse_y <= 480) {
        main_menu->selected_option = 1;
        return MENU_ACTION_HOW_TO_PLAY;
    }
    
    if (mouse_x >= 250 && mouse_x <= 450 && mouse_y >= 500 && mouse_y <= 580) {
        main_menu->selected_option = 2;
        return MENU_ACTION_EXIT;
    }
    
    return MENU_ACTION_NONE;
}

void mainmenu_update_hover(main_menu_t *main_menu, int mouse_x, int mouse_y) {
    if (main_menu == NULL) {
        return;
    }
    
    if (mouse_x >= 250 && mouse_x <= 450 && mouse_y >= 300 && mouse_y <= 380) {
        main_menu->selected_option = 0;
    } else if (mouse_x >= 250 && mouse_x <= 450 && mouse_y >= 400 && mouse_y <= 480) {
        main_menu->selected_option = 1;
    } else if (mouse_x >= 250 && mouse_x <= 450 && mouse_y >= 500 && mouse_y <= 580) {
        main_menu->selected_option = 2;
    }
}

void draw_main_menu(main_menu_t *main_menu) {
    if (main_menu == NULL) {
        return;
    }

    if (main_menu->welcome_sprite != NULL) {
        draw_sprite(main_menu->welcome_sprite, 200, 0);
    }

    if (main_menu->to_sprite != NULL) {
        draw_sprite(main_menu->to_sprite, 470, 20);
    }

    if (main_menu->logo_sprite != NULL) {
        draw_sprite(main_menu->logo_sprite, 270, 100);
    }
    
    if (main_menu->selected_option == 0) {
        if (main_menu->startS_sprite != NULL) {
            draw_sprite(main_menu->startS_sprite, 250, 300);
        }
    } else {
        if (main_menu->start_sprite != NULL) {
            draw_sprite(main_menu->start_sprite, 250, 300);
        }
    }
    
    if (main_menu->selected_option == 1) {
        if (main_menu->howtoplayS_sprite != NULL) {
            draw_sprite(main_menu->howtoplayS_sprite, 250, 400);
        }
    } else {
        if (main_menu->howtoplay_sprite != NULL) {
            draw_sprite(main_menu->howtoplay_sprite, 250, 400);
        }
    }
    
    if (main_menu->selected_option == 2) {
        if (main_menu->exitS_sprite != NULL) {
            draw_sprite(main_menu->exitS_sprite, 250, 500);
        }
    } else {
        if (main_menu->exit_sprite != NULL) {
            draw_sprite(main_menu->exit_sprite, 250, 500);
        }
    }
}

void destroy_main_menu(main_menu_t *main_menu) {
    if (main_menu == NULL) {
        return;
    }

    if (main_menu->welcome_sprite != NULL) {
        destroy_sprite(main_menu->welcome_sprite);
        main_menu->welcome_sprite = NULL;
    }

    if (main_menu->logo_sprite != NULL) {
        destroy_sprite(main_menu->logo_sprite);
        main_menu->logo_sprite = NULL;
    }

    if (main_menu->startS_sprite != NULL) {
        destroy_sprite(main_menu->startS_sprite);
        main_menu->startS_sprite = NULL;
    }

    if (main_menu->start_sprite != NULL) {
        destroy_sprite(main_menu->start_sprite);
        main_menu->start_sprite = NULL;
    }

    if (main_menu->howtoplayS_sprite != NULL) {
        destroy_sprite(main_menu->howtoplayS_sprite);
        main_menu->howtoplayS_sprite = NULL;
    }

    if (main_menu->howtoplay_sprite != NULL) {
        destroy_sprite(main_menu->howtoplay_sprite);
        main_menu->howtoplay_sprite = NULL;
    }

    if (main_menu->exitS_sprite != NULL) {
        destroy_sprite(main_menu->exitS_sprite);
        main_menu->exitS_sprite = NULL;
    }

    if (main_menu->exit_sprite != NULL) {
        destroy_sprite(main_menu->exit_sprite);
        main_menu->exit_sprite = NULL;
    }
    
    free(main_menu);
}
