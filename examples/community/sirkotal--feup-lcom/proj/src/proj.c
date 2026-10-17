#include <lcom/lcf.h>
#include <math.h>

#include <lcom/timer.h>
#include "devices/kbd_mouse/keyboard.h"
#include "devices/kbd_mouse/mouse.h"
#include "devices/kbd_mouse/i8042.h"
#include "devices/graphics/graphics.h"
#include "devices/graphics/vbe.h"
#include "sprites/sprites.h"
#include "utils/font.h"
#include "utils/colors.h"

#include "xpm/bullet.h"
#include "xpm/crosshair.h"
#include "sprites/sprites.h"
#include "xpm/player.h"
#include "xpm/mag.h"
#include "xpm/map_xpm.h"

int main(int argc, char *argv[]) {
    lcf_set_language("EN-US");
    lcf_trace_calls("/home/lcom/labs/proj/src/trace.txt");
    lcf_log_output("/home/lcom/labs/proj/src/output.txt");
    
    if (lcf_start(argc, argv)) {
        return 1;
    }

    lcf_cleanup();
    return 0;
}

int mouse_x = 0;
int mouse_y = 0;
int prev_mouse_x = 0;
int prev_mouse_y = 0;

/* bool to check if any bullet was fired */
bool fire = false;

/* Array of bullets:
    0 -> x position
    1 -> y position
    2 -> slope of the trajectory
    3 -> incrementer
*/
double bullets[10][4];
int number_bullets = 0;

/**
 * @brief calculates the slope based on player and cursor position and the incrementor
 * 
 * @param x_initial initial x position of the bullet
 * @param x_final final x position of the bullet
 * @param y_initial initial y position of the bullet
 * @param y_final final y position of the bullet
 * @param bullet array of bullets
 */
void (trajectory) (int x_initial, int x_final, int y_initial, int y_final, double bullet[4]){
    if (x_initial == x_final){
        bullet[2] = 0;
        bullet[3] = 0;
        return;
    }
    bullet[2] = (double)(y_final-y_initial)/ (double)(x_final-x_initial);
    bullet[3] = (1 / bullet[2]) > 1 ? 1 : ((1 / bullet[2]) < -1 ? -1 : (1 / bullet[2]));
    if (y_initial > y_final)
        bullet[3] = -bullet[3];
    
}

/**
 * @brief calculate if the player colides
 * 
 * @param x x position of the player or bullet
 * @param y y position of the player or bullet
 * @param sp sprite of the map
 * @param enemies array of enemies
 * @param flag if it is a collision with a wall or with a enemy
 * @param destroyed array of bools to check if the enemy was destroyed
 * @return true if it colides, false otherwise
 */
bool (collision)(int x, int y, sprite_t* sp, double enemies[3][3], bool flag, bool destroyed[3]){
    if (x <= 0 || x+12 >= get_h_res() || y <= 0 || y+12 >= get_v_res()){
        return true;
    }
    for (int i = 0; i< 3; i++){  
        for (int j = 0; j < 20; j++){ 
            if ((enemies[i][0]+j<=x+12 && enemies[i][0]+j>=x) && (enemies[i][1]<=y+12 && enemies[i][1]+enemies[i][2]>=y)){
                if (!flag){
                    sprite_t* delete_enemy = create_sprite((xpm_map_t) delete_enemy_xpm);
                    draw_sprite(delete_enemy, enemies[i][0], enemies[i][1]);
                    destroyed[i] = true;
                }    
                    return true;
            }
        }
    }
    uint32_t color = sp->colors[y * sp->width + x];
    uint32_t color1 = sp->colors[(y + 12) * sp->width + x];
    uint32_t color2 = sp->colors[y * sp->width + x + 12];
    uint32_t color3 = sp->colors[(y + 12) * sp->width + x + 12];
    
    return (color == 63 || color1 == 63 || color2 == 63 || color3 == 63 || color == 36 || color1 == 36 || color2 == 36 || color3 == 36); 
}

/**
 * @brief Module responsible for the movement of the bullets
 * 
 * @param bullet_no number of the bullet
 * @param player_x x position of the player
 * @param player_y y position of the player
 * @param player_sp sprite of the player
 * @param bullet_sp sprite of the bullet
 * @param map_sp sprite of the map
 * @param enemies array of enemies
 * @param destroyed array of bools to check if the enemy was destroyed
 * @return 0 if success, 1 otherwise
 */
int (move_bullets)(uint8_t bullet_no, uint16_t player_x, uint16_t player_y, sprite_t* player_sp, sprite_t* bullet_sp, sprite_t* map_sp, double enemies[3][3], bool destroyed[3]) {
    bullets[bullet_no][0] += bullets[bullet_no][3];
    bullets[bullet_no][1] += bullets[bullet_no][2] * bullets[bullet_no][3];

    const uint16_t bullet_size = bullet_sp->height; // bullet is a square
    const uint8_t floor_color = 0;

    if (!collision(bullets[bullet_no][0], bullets[bullet_no][1], map_sp, enemies, false, destroyed)) { /* Check if the bullet colides */
        if (vg_draw_rectangle(bullets[bullet_no][0] - bullets[bullet_no][3], bullets[bullet_no][1] - bullets[bullet_no][2] * bullets[bullet_no][3], bullet_size, bullet_size, floor_color)) {
            return 1; 
        }

        if (draw_sprite(bullet_sp, bullets[bullet_no][0], bullets[bullet_no][1])) {
            return 1;
        }

        // player
        if (draw_sprite(player_sp, player_x, player_y)) {
            return 1;
        }

        // move double buffer to video memory
        if (move_buffer_to_video()) {
            printf("Error moving double buffer to video memory\n");
            return 1;
        }
    } else { /* If it colides delete it from the array */
        if (vg_draw_rectangle(bullets[bullet_no][0] - bullets[bullet_no][3], bullets[bullet_no][1] - bullets[bullet_no][2] * bullets[bullet_no][3], bullet_size, bullet_size, floor_color)) {
            return 1; 
        }
        if (move_buffer_to_video()) {
            return 1;
        }
        double copied[10][4];
        for(int j = 0; j < number_bullets; j++) {
            copied[j][0] = bullets[j][0];
            copied[j][1] = bullets[j][1];
            copied[j][2] = bullets[j][2];
            copied[j][3] = bullets[j][3];
        }
        int counter = 0;
        for (int j = 0; j < number_bullets; j++) {
            if (bullet_no != j) {
                bullets[counter][0] = copied[j][0];
                bullets[counter][1] = copied[j][1];
                bullets[counter][2] = copied[j][2];
                bullets[counter][3] = copied[j][3];
                counter++;
            }
        }
        number_bullets--;

        /*If there are no bullets remaining, set fire to false*/
        fire = number_bullets == 0 ? false : fire;
    }

    return 0;
}

/**
 * @brief Module responsible for the kbd interrupts in the playing state
 * 
 * @param scancode scancode read from the kbc
 * @param player_x x position of the player
 * @param player_y y position of the player
 * @param key array of scancodes
 * @return 0 if success, 1 otherwise
 */
int (play_handler_kbd)(uint8_t scancode, uint16_t *player_x, uint16_t *player_y, uint8_t *key, sprite_t* player, sprite_t* bullet, sprite_t* map, double enemies[3][3], bool destroyed[3]) {
    uint16_t x_before = *player_x, y_before = *player_y;
    /*reads the scancode and updates key*/
    if (scancode == RIGHT_ARROW_MAKE_CODE && key[1] != RIGHT_ARROW_MAKE_CODE) {
        key[0] = key[1];
        key[1] = RIGHT_ARROW_MAKE_CODE;
    } else if (scancode == LEFT_ARROW_MAKE_CODE && key[1] != LEFT_ARROW_MAKE_CODE) {
        key[0] = key[1];
        key[1] = LEFT_ARROW_MAKE_CODE;
    } else if (scancode == UP_ARROW_MAKE_CODE && key[1] != UP_ARROW_MAKE_CODE) {
        key[0] = key[1];
        key[1] = UP_ARROW_MAKE_CODE;
    } else if (scancode == DOWN_ARROW_MAKE_CODE && key[1] != DOWN_ARROW_MAKE_CODE) {
        key[0] = key[1];
        key[1] = DOWN_ARROW_MAKE_CODE;
    }  

    /*Checks if a breakcode was read*/
    if (scancode == RIGHT_ARROW_BREAK_CODE) {
        if (key[0] == RIGHT_ARROW_MAKE_CODE) {
            key[0] = 0x00;
        } else if (key[1] == RIGHT_ARROW_MAKE_CODE) {
            key[1] = 0x00;
        }
    }
    else if (scancode == LEFT_ARROW_BREAK_CODE) {
        if (key[0] == LEFT_ARROW_MAKE_CODE) {
            key[0] = 0x00;
        } else if (key[1] == LEFT_ARROW_MAKE_CODE) {
            key[1] = 0x00;
        }
    }
    else if (scancode == UP_ARROW_BREAK_CODE) {
        if (key[0] == UP_ARROW_MAKE_CODE) {
            key[0] = 0x00;
        } else if (key[1] == UP_ARROW_MAKE_CODE) {
            key[1] = 0x00;
        }
    }
    else if (scancode == DOWN_ARROW_BREAK_CODE) {
        if (key[0] == DOWN_ARROW_MAKE_CODE) {
            key[0] = 0x00;
        } else if (key[1] == DOWN_ARROW_MAKE_CODE) {
            key[1] = 0x00;
        }
    }
    
    /* if the previous input still on and the current not, update current*/
    if (key[1] == 0x00 && key[0] != 0x00){
        key[1] = key[0];
    }

    /*update position*/
    if (key[0] == RIGHT_ARROW_MAKE_CODE || key[1] == RIGHT_ARROW_MAKE_CODE) {
        *player_x += 5;
    }

    if (key[0] == LEFT_ARROW_MAKE_CODE|| key[1] == LEFT_ARROW_MAKE_CODE) {
        *player_x -= 5;
    }

    if (key[0] == UP_ARROW_MAKE_CODE || key[1] == UP_ARROW_MAKE_CODE) {
        *player_y -= 5;
    }

    if (key[0] == DOWN_ARROW_MAKE_CODE || key[1] == DOWN_ARROW_MAKE_CODE) {
        *player_y += 5;
    }

    uint16_t player_size_x = 12;
    uint16_t player_size_y = 12;
    uint8_t floor_color = 0;

    /* checks if the user colides */
    if(!collision(*player_x, *player_y, map, enemies, true, destroyed)){
        if (vg_draw_rectangle(x_before, y_before, player_size_x, player_size_y, floor_color)) {
            return 1; 
        }

        // draw player
        if (draw_sprite(player, *player_x, *player_y)) {
            return 1;
        }

        // move double buffer to video memory
        if (move_buffer_to_video()) {
            printf("Error moving double buffer to video memory\n");
            return 1;
        }
    }
    /*reset the position*/
    else {
        *player_x = x_before;
        *player_y = y_before;
    }

    return 0;
}

uint8_t timer_bit_no, kbd_bit_no, mouse_bit_no;

/**
 * @brief Subscribes the interrupts
 * 
 * @return 0 if success, 1 otherwise
 */
int (subscribe_ints)() {
    if (timer_subscribe_int(&timer_bit_no)) {
        printf("Error subscribing timer interrupts\n");
        return 1;
    }

    if (kbd_subscribe_int(&kbd_bit_no)) {
        printf("Error subscribing keyboard interrupts\n");
        return 1;
    }

    if (mouse_write_command(KBC_CMD_ENABLE_DATA)) {
        printf("Error enabling mouse data report\n");
        return 1;
    }

    if (mouse_subscribe_int(&mouse_bit_no)) {
        printf("Error subscribing mouse interrupts\n");
        return 1;
    }

    return 0;
}

/**
 * @brief Unsubscribes the interrupts
 * 
 * @return 0 if success, 1 otherwise
 */
int (unsubscribe_ints)() {
    if (timer_unsubscribe_int()) {
        printf("Error unsubscribing timer interrupts\n");
        return 1;
    }

    if (kbd_unsubscribe_int()) {
        printf("Error unsubscribing keyboard interrupts\n");
        return 1;
    }

    if (mouse_write_command(KBC_CMD_DISABLE_DATA)) {
        printf("Error disabling mouse data report\n");
        return 1;
    }

    if (mouse_unsubscribe_int()) {
        printf("Error unsubscribing mouse interrupts\n");
        return 1;
    }

    return 0;
}

/**
 * @brief Enum to handle the menu state
 * 
 * PLAY -> start the game
 * EXIT -> exit the game
 */
enum menu_state {
    PLAY,
    EXIT
};

/**
 * @brief Enum to handle the game state
 * 
 * MAIN_MENU -> main menu
 * PLAYING -> playing
 * GAME_OVER -> game over
 */
enum game_state {
    MAIN_MENU,
    PLAYING,
    GAME_OVER
};

/**
 * @brief Draws the game over screen
 */
void (draw_over)() {
    if(draw_text("GAME OVER", 400, 350, WHITE)){
        font_destroy(font);
    }

    move_buffer_to_video();
}

/**
 * @brief Draws the main menu
 * 
 * @param state state of the menu
 */
void (draw_menu)(enum menu_state state) {
    if (font == NULL) {
        printf("Error initializing font\n");
        return;
    }

    if (draw_text("Start", 450, 350, state == PLAY ? YELLOW : WHITE)) {
        printf("Error drawing text\n");
        font_destroy(font);
        return;
    }

    if (draw_text("Exit", 460, 400, state == EXIT ? YELLOW : WHITE)) {
        printf("Error drawing text\n");
        font_destroy(font);
        return;
    }

    move_buffer_to_video();
}

/**
 * @brief Handles the keyboard interrupts in the menu state
 * 
 * @param state state of the menu
 * @param scancode scancode read from the kbc
 * @param game_state state of the game
 * @param exit bool to check if the game should exit
 * @param map sprite of the map
 */
void (handle_menu_kbd)(enum menu_state *state, uint8_t scancode, enum game_state *game_state, bool *exit, sprite_t *map) {
    switch (scancode) {
        case UP_ARROW_BREAK_CODE:
            *state = *state == PLAY ? EXIT : PLAY;
            break;
        case DOWN_ARROW_BREAK_CODE:
            *state = *state == PLAY ? EXIT : PLAY;
            break;
        case ENTER_BREAK_CODE:
            if (*state == PLAY) {
                *game_state = PLAYING;
            } else if (*state == EXIT) {
                *exit = true;
            }
            break;
        default:
            break;
    }
    draw_menu(*state);
}

static int restore_map_area(sprite_t *map, int x, int y, unsigned width, unsigned height) {
    for (unsigned row = 0; row < height; row++) {
        int py = y + row;
        if (py < 0 || py >= (int)map->height) continue;
        for (unsigned col = 0; col < width; col++) {
            int px = x + col;
            if (px < 0 || px >= (int)map->width) continue;
            if (set_pixel_color(px, py, map->colors[py * map->width + px])) return 1;
        }
    }
    return 0;
}

int (proj_main_loop)(int argc, char *argv[]) {
    if (subscribe_ints()) {
        printf("Error subscribing interrupts\n");
        return 1;
    }

    // Map video memory
    if (map_linear_frame_buffer(VBE_768p_INDEXED)) {
        printf("Error mapping video memory\n");
        unsubscribe_ints();
        return 1;
    }

    // Initialize graphics mode
    if (set_graphics_mode(VBE_768p_INDEXED)) {
        printf("Error initializing graphics mode\n");
        unsubscribe_ints();
        return 1;
    }

    font_init();

    int ipc_status, r;
    message msg;

    bool exit = false;

    /* key is an array of the positions:
        0 -> previous one
        1 -> current one
    */
    uint8_t scancode = 0x00, key[2]= {0x00,0x00};

    // start in main menu (draw before loop because it doesn't require timer interrupts)
    enum menu_state menu_state = PLAY; //! refactor this too
    enum game_state game_state = MAIN_MENU;
    draw_menu(menu_state);

    // initial position of the bullet
    uint16_t x = 100, y = 110;

    // Create bullet
    sprite_t *player = create_sprite((xpm_map_t) simple_player_xpm);
    sprite_t* bullet = create_sprite((xpm_map_t) bullet_xpm);
    sprite_t* crosshair = create_sprite((xpm_map_t) crosshair_xpm);
    sprite_t* erase_crosshair = create_sprite((xpm_map_t) vanish_crosshair_xpm);
    sprite_t* enemy = create_sprite((xpm_map_t) enemy_xpm);
    sprite_t *map = create_sprite((xpm_map_t) map_xpm);

    bool destroyed[3] = {false, false, false};

    double enemies[3][3];
    enemies[0][0] = 700;
    enemies[0][1] = 600;
    enemies[0][2] = 20;
    enemies[1][0] = 125;
    enemies[1][1] = 620;
    enemies[1][2] = 620;
    enemies[2][0] = 915;
    enemies[2][1] = 80;
    enemies[2][2] = 20;


    uint32_t int_frames = sys_hz() / 60;
    int timer_counter = 0;

    // (destroyed[0] == false || destroyed[1] == false || destroyed[2] == false)
    while(!exit) {
        /* Get a request message. */
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
            printf("driver_receive failed with: %d", r);
            continue;
        }

        if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */				
                if (msg.m_notify.interrupts & BIT(timer_bit_no)) {                    
                    /*Bullets' trajectory controller*/
                    timer_counter++;
                    if (fire){    
                        if (timer_counter % int_frames != 0) {
                            continue;
                        }
                        for (int i = 0; i < number_bullets; i++) {
                            if (move_bullets(i, x, y, player, bullet, map, enemies, destroyed)) {
                                printf("Error moving bullets\n");
                                return 1;
                            }
                        }
                    }

                    if (game_state == PLAYING) {
                        if (destroyed[0] == true && destroyed[1] == true && destroyed[2] == true) {
                            game_state = GAME_OVER;
                            fire = false;
                            vg_draw_rectangle(0, 0, get_h_res(), get_v_res(), 0);
                            draw_over();
                            move_buffer_to_video();
                            exit = true;
                            sleep(5);
                        }
                    }
                }
                
                if (msg.m_notify.interrupts & BIT(kbd_bit_no)) { /* subscribed interrupt */
                    kbc_ih();
                    scancode = get_scancode();

                    if (game_state == MAIN_MENU) {
                        handle_menu_kbd(&menu_state, scancode, &game_state, &exit, map);

                        // prepare for next interrupt
                        if (game_state == PLAYING) {
                            draw_sprite(map, 0, 0);
                            draw_sprite(enemy, 700, 600);
                            draw_sprite(enemy, 125, 620);
                            draw_sprite(enemy, 915, 80);
                        }
                    } else if (game_state == PLAYING) {
                        draw_sprite(map, 0, 0);

                        if (!destroyed[0]) {
                            draw_sprite(enemy, 700, 600);
                        }
                        if (!destroyed[1]) {
                            draw_sprite(enemy, 125, 620);
                        }
                        if (!destroyed[2]) {
                            draw_sprite(enemy, 915, 80);
                        }

                        if (scancode == ESC_BREAK_CODE) {
                            fire = false;
                            game_state = MAIN_MENU;
                            vg_draw_rectangle(0, 0, get_h_res(), get_v_res(), 0);
                            menu_state = PLAY;
                            draw_menu(menu_state);
                            move_buffer_to_video();
                            x = 100; y = 110;
                        }

                        if (play_handler_kbd(scancode, &x, &y, key, player, bullet, map, enemies, destroyed)) {
                            printf("Error handling play keyboard interrupts\n");
                            return 1;
                        }
                    }
                }

                if (msg.m_notify.interrupts & BIT(mouse_bit_no)) { /* subscribed interrupt */
                    if (game_state != PLAYING) {
                        continue;
                    }
                    
                    mouse_ih();
                    if (get_current_packet_index() == 3) {
                        if (generate_mouse_packet()) {
                            return 1;
                        }
                    
                        struct packet mouse_packet = get_mouse_packet();

                        prev_mouse_x = mouse_x;
                        prev_mouse_y = mouse_y;

                        mouse_x = mouse_x + mouse_packet.delta_x;
                        mouse_y = mouse_y - mouse_packet.delta_y;

                        if (mouse_x < 0) {
                            mouse_x = 0;
                        } 
                        else if (mouse_x >= 1024) {
                            mouse_x = 1023;
                        }
                        if (mouse_y < 0) {
                            mouse_y = 0;
                        } 
                        else if (mouse_y >= 768) {
                            mouse_y = 767;
                        }

                        if (restore_map_area(map, prev_mouse_x, prev_mouse_y,
                                             erase_crosshair->width, erase_crosshair->height)) return 1;
                        for (int i = 0; i < 3; i++) {
                            if (!destroyed[i] && draw_sprite(enemy, enemies[i][0], enemies[i][1])) return 1;
                        }
                        if (draw_sprite(player, x, y)) return 1;
                        for (int i = 0; i < number_bullets; i++) {
                            if (draw_sprite(bullet, bullets[i][0], bullets[i][1])) return 1;
                        }
                        draw_sprite(crosshair, mouse_x, mouse_y);
                        if (move_buffer_to_video()) {
                            printf("Error moving double buffer to video memory\n");
                            return 1;
                        }

                        if (mouse_packet.lb && number_bullets < 10 && !collision(x, y, map, enemies, false, destroyed)) {
                            fire = true;
                            bullets[number_bullets][0]=x;
                            bullets[number_bullets][1]=y;

                            trajectory(bullets[number_bullets][0], mouse_x, bullets[number_bullets][1], mouse_y, bullets[number_bullets]);
                            number_bullets++;

                            if (move_buffer_to_video()) {
                                printf("Error moving double buffer to video memory\n");
                                return 1;
                            }
                        }
                    }
                }

                break;
            default:
                break; /* no other notifications expected: do nothing */	
        }
        } else { /* received a standard message, not a notification */
            /* no standard messages expected: do nothing */
        }
    }

    // destroy sprites
    destroy_sprite(player);
    destroy_sprite(bullet);
    destroy_sprite(crosshair);
    destroy_sprite(erase_crosshair);
    destroy_sprite(enemy);
    destroy_sprite(map);
    
    font_destroy();

    if (unsubscribe_ints()) {
        printf("Error unsubscribing interrupts\n");
        exit_graphics_mode();
        return 1;
    }

    // exit graphics mode
    if (exit_graphics_mode()) {
        printf("Error exiting graphics mode\n");
        return 1;
    }
    
    printf("Catch you later, Diver!");
    return 0;
}
