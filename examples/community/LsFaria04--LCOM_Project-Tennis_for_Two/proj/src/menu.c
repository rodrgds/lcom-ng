#include "menu.h"
#include "color.h"
#include <stdlib.h>

Menu* (initialize_menu)(bool isStartMenu){
    Menu *menu = (Menu*)malloc(sizeof(Menu));
    if(menu == NULL){
        printf("Menu is null");
        return NULL;
    }
    
    menu->selected=0;

    xpm_image_t img;

    if (isStartMenu) {
        Sprite *sprite_title = (Sprite*) malloc(sizeof(Sprite));
        sprite_title->map = (uint32_t *) xpm_load((xpm_map_t) title_xpm, XPM_8_8_8_8, &img);
        sprite_title->height = img.height;
        sprite_title->width = img.width;
        menu->title = *sprite_title;

        free(sprite_title);

        if(menu->title.map==NULL){
            printf("title is null");
            return NULL;
        }

        Sprite *sprite_start_button = (Sprite*) malloc(sizeof(Sprite));
        sprite_start_button->map = (uint32_t *) xpm_load((xpm_map_t) start_xpm, XPM_8_8_8_8, &img);
        sprite_start_button->height = img.height;
        sprite_start_button->width = img.width;
        menu->play_button = *sprite_start_button;

        free(sprite_start_button);

        Sprite *sprite_quit_button = (Sprite*) malloc(sizeof(Sprite));
        sprite_quit_button->map = (uint32_t *) xpm_load((xpm_map_t) quit_xpm, XPM_8_8_8_8, &img);
        sprite_quit_button->height = img.height;
        sprite_quit_button->width = img.width;
        menu->quit = *sprite_quit_button;

        free(sprite_quit_button);

        Sprite *sprite_start_button_hover = (Sprite*) malloc(sizeof(Sprite));
        sprite_start_button_hover->map = (uint32_t *) xpm_load((xpm_map_t) start_hover_xpm, XPM_8_8_8_8, &img);
        sprite_start_button_hover->height = img.height;
        sprite_start_button_hover->width = img.width;
        menu->play_button_hover = *sprite_start_button_hover;
        
        free(sprite_start_button_hover);

        Sprite *sprite_quit_button_hover = (Sprite*) malloc(sizeof(Sprite));
        sprite_quit_button_hover->map = (uint32_t *) xpm_load((xpm_map_t) quit_hover_xpm, XPM_8_8_8_8, &img);
        sprite_quit_button_hover->height = img.height;
        sprite_quit_button_hover->width = img.width;
        menu->quit_hover = *sprite_quit_button_hover;

        free(sprite_quit_button_hover);
    }
    else {
        Sprite *pause_menu = (Sprite*) malloc(sizeof(Sprite));
        pause_menu->map = (uint32_t *) xpm_load((xpm_map_t) pause_menu_xpm, XPM_8_8_8_8, &img);
        pause_menu->height = img.height;
        pause_menu->width = img.width;
        menu->pause_menu = *pause_menu;

        free(pause_menu);

        if(menu->pause_menu.map==NULL){
            printf("pause menu is null\n");
            return NULL;
        }

        Sprite *sprite_resume = (Sprite*) malloc(sizeof(Sprite));
        sprite_resume->map = (uint32_t *) xpm_load((xpm_map_t) resume_xpm, XPM_8_8_8_8, &img);
        sprite_resume->height = img.height;
        sprite_resume->width = img.width;
        menu->resume = *sprite_resume;

        free(sprite_resume);

        Sprite *sprite_restart = (Sprite*) malloc(sizeof(Sprite));
        sprite_restart->map = (uint32_t *) xpm_load((xpm_map_t) restart_xpm, XPM_8_8_8_8, &img);
        sprite_restart->height = img.height;
        sprite_restart->width = img.width;
        menu->restart = *sprite_restart;

        free(sprite_restart);

        Sprite *sprite_quit_pause = (Sprite*) malloc(sizeof(Sprite));
        sprite_quit_pause->map = (uint32_t *) xpm_load((xpm_map_t) quit_pause_xpm, XPM_8_8_8_8, &img);
        sprite_quit_pause->height = img.height;
        sprite_quit_pause->width = img.width;
        menu->quit_pause = *sprite_quit_pause;

        free(sprite_quit_pause);

        Sprite *sprite_resume_hover = (Sprite*) malloc(sizeof(Sprite));
        sprite_resume_hover->map = (uint32_t *) xpm_load((xpm_map_t) resume_hover_xpm, XPM_8_8_8_8, &img);
        sprite_resume_hover->height = img.height;
        sprite_resume_hover->width = img.width;
        menu->resume_hover = *sprite_resume_hover;
        
        free(sprite_resume_hover);

        Sprite *sprite_restart_hover = (Sprite*) malloc(sizeof(Sprite));
        sprite_restart_hover->map = (uint32_t *) xpm_load((xpm_map_t) restart_hover_xpm, XPM_8_8_8_8, &img);
        sprite_restart_hover->height = img.height;
        sprite_restart_hover->width = img.width;
        menu->restart_hover = *sprite_restart_hover;

        free(sprite_restart_hover);

        Sprite *sprite_quit_pause_hover = (Sprite*) malloc(sizeof(Sprite));
        sprite_quit_pause_hover->map = (uint32_t *) xpm_load((xpm_map_t) quit_pause_hover_xpm, XPM_8_8_8_8, &img);
        sprite_quit_pause_hover->height = img.height;
        sprite_quit_pause_hover->width = img.width;
        menu->quit_pause_hover = *sprite_quit_pause_hover;

        free(sprite_quit_pause_hover);
    }
    return menu;
}

int (draw_field)(int x_offset, int y_offset, Sprite sprite ){
    uint32_t *map = sprite.map;
    if(map==NULL ){
        printf("map is null");
        return EXIT_FAILURE;
    }
    for(int i = 0; i < sprite.height; i++){
        for(int j = 0; j < sprite.width; j++){
            if(*map != TRANSPARENCY_COLOR && j+x_offset < MAX_X && i+y_offset < MAX_Y){
                if(vg_draw_color(j+x_offset,i+y_offset,*map)!=0){
                    return EXIT_FAILURE;
                }
            }
            map++;
        }    
    }
    return EXIT_SUCCESS;
}

int (update_selected)(unsigned char code,Game_state* state, Menu* menu){
    if(code == ARROW_DOWN || code == ARROW_UP){
        if(menu->selected==2){
            menu->selected--;
        }
        else{
          menu->selected++;  
        }
    }
    if(code == ENTER_KEY){
        switch (menu->selected)
        {
        case 1:
            *state=GAME;
            break;
        case 2:
            *state=QUIT;
            break;
        default:
            break;
        }
    }
    
    return EXIT_SUCCESS;
}

int (kbd_handler_menu)(Game_state* state, Menu* menu, bool isStartMenu){
    if (isStartMenu) {
        update_selected(get_scancode(),state,menu);
    }
    else {
        update_selected_pause(get_scancode(), state, menu);
    }
    return EXIT_SUCCESS;
}

int (draw_menu)(Menu* menu){
    
    switch (menu->selected)
    {
        case 0:
            if(draw_field(467,350,menu->play_button)!=0){
                printf("draw start failed");
                return EXIT_FAILURE;
            }
            if(draw_field(492,475,menu->quit)!=0){
                printf("draw quit failed");
                return EXIT_FAILURE;
            }
            break;
        case 1:
            if(draw_field(461,345,menu->play_button_hover)!=0){
                return EXIT_FAILURE;
            }
            if(draw_field(492,475,menu->quit)!=0){
                return EXIT_FAILURE;
            }
            break;
        case 2:
            if(draw_field(467,350,menu->play_button)!=0){
                return EXIT_FAILURE;
            }
            if(draw_field(487,471,menu->quit_hover)!=0){
                return EXIT_FAILURE;
            }
            break;
        default:
            break;
    }

    return EXIT_SUCCESS;
}

int (timer_handler_menu)(Menu* menu, Mouse* mouse, bool isStartMenu){
    if(isStartMenu){
        if(draw_menu(menu)!=0 || drawMouse(mouse) != 0){
            printf("menu or mouse failed to draw\n");
            return EXIT_FAILURE;
        }
    }
    else{
        if (drawPause(menu) != 0 || drawMouse(mouse) != 0) {
            return EXIT_FAILURE;
        }
    }
    
    return EXIT_SUCCESS;   
}


int (choose_number_sprite)(uint8_t num, Sprite* sprite){
    xpm_image_t img;
    

    switch (num)
    {
    case 0:
        sprite->map=(uint32_t *) xpm_load((xpm_map_t) r0_xpm, XPM_8_8_8_8, &img);
        sprite->height=img.height;
        sprite->width=img.width;
        break;
    case 1:
        sprite->map=(uint32_t *) xpm_load((xpm_map_t) r1_xpm, XPM_8_8_8_8, &img);
        sprite->height=img.height;
        sprite->width=img.width;
        break;
    case 2:
        sprite->map=(uint32_t *) xpm_load((xpm_map_t) r2_xpm, XPM_8_8_8_8, &img);
        sprite->height=img.height;
        sprite->width=img.width;
        break;
    case 3:
        sprite->map=(uint32_t *) xpm_load((xpm_map_t) r3_xpm, XPM_8_8_8_8, &img);
        sprite->height=img.height;
        sprite->width=img.width;
        break;  
    case 4:
        sprite->map=(uint32_t *) xpm_load((xpm_map_t) r4_xpm, XPM_8_8_8_8, &img);
        sprite->height=img.height;
        sprite->width=img.width;
        break;
    case 5:
        sprite->map=(uint32_t *) xpm_load((xpm_map_t) r5_xpm, XPM_8_8_8_8, &img);
        sprite->height=img.height;
        sprite->width=img.width;
        break;  
    case 6:
        sprite->map=(uint32_t *) xpm_load((xpm_map_t) r6_xpm, XPM_8_8_8_8, &img);
        sprite->height=img.height;
        sprite->width=img.width;
        break;
    case 7:
        sprite->map=(uint32_t *) xpm_load((xpm_map_t) r7_xpm, XPM_8_8_8_8, &img);
        sprite->height=img.height;
        sprite->width=img.width;
        break; 
    case 8:
        sprite->map=(uint32_t *) xpm_load((xpm_map_t) r8_xpm, XPM_8_8_8_8, &img);
        sprite->height=img.height;
        sprite->width=img.width;
        break; 
    case 9:
        sprite->map=(uint32_t *) xpm_load((xpm_map_t) r9_xpm, XPM_8_8_8_8, &img);
        sprite->height=img.height;
        sprite->width=img.width;
        break;  
    default:
        sprite->map=(uint32_t *) xpm_load((xpm_map_t) small_ball_xpm, XPM_8_8_8_8, &img);
        sprite->height=img.height;
        sprite->width=img.width;
        break;
    }
    return EXIT_SUCCESS;
}


int update_selected_pause(unsigned char code, Game_state* game_state, Menu* menu) {
    if (code == ARROW_DOWN) {
        menu->selected = (menu->selected + 1) % 3;
    } else if (code == ARROW_UP) {
        menu->selected = (menu->selected + 2) % 3;
    }

    if (code == ENTER_KEY) {
        switch (menu->selected) {
            case 0:  // RESUME
                *game_state = GAME;
                break;
            case 1:  // RESTART
                *game_state = RESTART;
                break;
            case 2:  // QUIT
                *game_state = QUIT;
                break;
            default:
                break;
        }
    }

    return EXIT_SUCCESS;
}

int drawPause(Menu *menu) {
    if (menu == NULL) {
        return EXIT_FAILURE;
    }
    
    if (draw_field(454, 300, (menu->selected == 0) ? menu->resume_hover : menu->resume) != 0) {
        printf("Draw resume failed\n");
        return EXIT_FAILURE;
    }

    if (draw_field(454, 400, (menu->selected == 1) ? menu->restart_hover : menu->restart) != 0) {
        printf("Draw restart failed\n");
        return EXIT_FAILURE;
    }

    if (draw_field(454, 500, (menu->selected == 2) ? menu->quit_pause_hover : menu->quit_pause) != 0) {
        printf("Draw quit failed\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

Mouse* createMouse() {
    Mouse* mouse = (Mouse*)malloc(sizeof(Mouse));
    if (mouse == NULL) {
        return NULL;
    }

    mouse->sprite = (Sprite*)malloc(sizeof(Sprite));
    if (mouse->sprite == NULL) {
        free(mouse);
        return NULL;
    }

    xpm_image_t img;
    mouse->sprite->map = (uint32_t *) xpm_load((xpm_map_t) cursor_xpm, XPM_8_8_8_8, &img);
    if (mouse->sprite->map == NULL) {
        free(mouse->sprite);
        free(mouse);
        return NULL;
    }
    mouse->sprite->width = img.width;
    mouse->sprite->height = img.height;

    mouse->x = MAX_X / 2;
    mouse->y = MAX_Y / 2;

    return mouse;
}

int drawMouse(Mouse* mouse) {
    if (mouse == NULL || mouse->sprite == NULL) {
        return EXIT_FAILURE;
    }

    int width = mouse->sprite->width;
    int height = mouse->sprite->height;
    uint32_t *map = mouse->sprite->map;

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            uint16_t x = mouse->x + i;
            uint16_t y = mouse->y + j;

            if (x < MAX_X && y < MAX_Y && *map != TRANSPARENCY_COLOR) {
                if (vg_draw_color(x, y, *map) != 0) {
                    return EXIT_FAILURE;
                }
            }
            map++;
        }
    }
    return EXIT_SUCCESS;
}

void destroyMouse(Mouse* mouse) {
    if (mouse == NULL) {
        return;
    }

    if (mouse->sprite != NULL) {
        free(mouse->sprite->map);
        free(mouse->sprite);
    }

    free(mouse);
}

int updateMousePosition(Mouse* mouse, int dx, int dy, Game_state *state) {
    if(abs(dx) > 1 ){
        mouse->x += dx * 1.5;
    }
    if(abs(dy) > 1){
        mouse->y -= dy * 1.5;
    }

    // Adjust for screen boundaries
    if (mouse->x < 0) {
        mouse->x = 0;
    } else if (mouse->x + mouse->sprite->width > MAX_X) {
        mouse->x = MAX_X - mouse->sprite->width;
    }

    if (mouse->y < 0) {
        mouse->y = 0;
    }
    else if(mouse->y + mouse->sprite->height > MAX_Y - 80 && *state == START_MENU){
        mouse->y = MAX_Y - 80 - mouse->sprite->height;
    }
    else if (mouse->y + mouse->sprite->height > MAX_Y) {
        mouse->y = MAX_Y - mouse->sprite->height;
    }
    

    return EXIT_SUCCESS;
}

void update_selected_mouse(Menu* menu, Mouse* mouse, Game_state* game_state, bool isStartMenu) {
    if (!isStartMenu) {
        // Check if mouse is hovering over menu buttons
        bool isHoveringResume = (mouse->x >= 454 && mouse->x <= 661) && (mouse->y >= 300 && mouse->y <= 360);
        bool isHoveringRestart = (mouse->x >= 454 && mouse->x <= 697) && (mouse->y >= 400 && mouse->y <= 460);
        bool isHoveringQuit = (mouse->x >= 454 && mouse->x <= 588) && (mouse->y >= 500 && mouse->y <= 560);

        if (isHoveringResume) {
            menu->selected = 0;
        }
        else if (isHoveringRestart) {
            menu->selected = 1;
        }
        else if (isHoveringQuit) {
            menu->selected = 2;
        }

        // Handle mouse click
        if (get_mouse_packet().lb) {
            if (isHoveringResume) {
                *game_state = GAME;
            } else if (isHoveringRestart) {
                *game_state = RESTART;
            } else if (isHoveringQuit) {
                *game_state = QUIT;
            }
        }
    }
    else {
        // Check if mouse is hovering over menu buttons
        bool isHoveringPlay = (mouse->x >= 461 && mouse->x <= 689) && (mouse->y >= 345 && mouse->y <= 397);
        bool isHoveringQuit = (mouse->x >= 487 && mouse->x <= 663) && (mouse->y >= 471 && mouse->y <= 521);

        if (isHoveringPlay) {
            menu->selected = 1;
        }
        else if (isHoveringQuit) {
            menu->selected = 2;
        }

        // Handle mouse click
        if (get_mouse_packet().lb) {
            if (isHoveringPlay) {
                *game_state = GAME;
            } else if (isHoveringQuit) {
                *game_state = QUIT;
            }
        }
    }
}

void menuMouseHandler(bool start, Menu* menu, Mouse* mouse, Game_state* game_state) {
    if (!start) {
        update_selected_mouse(menu, mouse, game_state, false);
        if (*game_state == RESTART) {
            clear_mouse_packet(); //so that player does not hit right as user returns to game
        }
    }
    else {
        update_selected_mouse(menu, mouse, game_state, true);
        if (*game_state == GAME) {
            clear_mouse_packet(); //so that player does not hit right at the start
        }
    }
}
