#include "game.h"

Player player;
Entity enemies[MAX_ENEMIES];
Bullet bullets[MAX_BULLETS];
unsigned long game_time = 0;
Vec_pos cursor_pos = {.x = SCREEN_WIDTH / 2, .y = SCREEN_HEIGHT / 2};
// Sprite storage
xpm_image_t player_sprite, enemy_sprite, bullet_sprite, space_bg;



int run(Game *game, uint8_t kb_mask, uint8_t timer_mask, uint8_t mouse_mask) {
    printf("Entered run()\n");

    int end = 0;
    int ipc_status;
    message msg;

    int frame_counter = 0;


    printf("Initializing game...\n");
    init_game();
    printf("Waiting movement \n");
    while (!end) {
        if (driver_receive(ANY, &msg, &ipc_status) != 0) {
            printf("driver_receive failed\n");
            continue;
        }

        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & timer_mask) {
                        frame_counter++;
                        game_time++;
                        spawn_enemy();
                        update_entities();
                        check_collisions();
                        if (frame_counter >= FRAME_INTERVAL) {
                            draw_game();
                            frame_counter = 0;
                        }
                    }


                    if (msg.m_notify.interrupts & mouse_mask) {
                        proj_mouse_ih();
                    }



                    if (msg.m_notify.interrupts & kb_mask) {
                        
                        kbc_ih();
                        if ((scancode & 0x80) == 0) { // Make code (key press)
                            if (scancode == 0x01) { // ESC make code (not break)
                                end = 1;
                            } else if (scancode == 0x19) { // 'P' make code
                                run_pause_menu(game, kb_mask, mouse_mask);  
                            } else {
                                handle_input(scancode);
                            }
                        }
                    }
                    break;
                default:
                    break;
            }
        }

        if (scancode == 0x81) end = 1;
        scancode = 0;
    }

    game->curState = GAME_STATE_GAMEOVER;
    game->score = player.score;

    return 0;
}


void init_game() {

    vg_initialize(VIDEO_MODE);  
    
    if (load_xpm(battleground_xpm, &space_bg) != 0){
        printf("Failed loading batteground \n");
    }
    if (space_bg.bytes == NULL){
        printf("Failed loading battleground \n");
    }

    if (load_xpm(player_xpm, &player_sprite) != 0) {
        printf("Failed loading sprite\n");
    }
    if (player_sprite.bytes == NULL){
        printf("Failed loading sprite \n");
    }

    if (load_xpm(enemy_xpm, &enemy_sprite) != 0){
        printf("Failed loading enemy \n");
    }

    if (enemy_sprite.bytes == NULL){
        printf("Failed loading enemy \n");
    }
    
    if (load_xpm(bullet_xpm, &bullet_sprite) != 0){
        printf("Failed loading bullet \n");
    }

    if (bullet_sprite.bytes == NULL){
        printf("Failed loading bullet \n");
    }

    player.pos.x = SCREEN_WIDTH / 2 - player_sprite.width / 2;
    player.pos.y = SCREEN_HEIGHT - player_sprite.height - 70;
    player.health = 100;
    player.score = 0;
    
}

void fire_bullet(Vec_pos cursor_pos) {\
    for(int i=0; i<MAX_BULLETS; i++) {
        if(!bullets[i].active) {

            bullets[i].pos.x = player.pos.x + player_sprite.width /2 - bullet_sprite.width / 2;
            bullets[i].pos.y = player.pos.y;
            
            float dx = cursor_pos.x - bullets[i].pos.x;
            float dy = cursor_pos.y - bullets[i].pos.y;
            float length = quake_sqrt(dx*dx + dy*dy);
            
            if (length < 0.001f){
                continue;
            }
            float inv_len = 1.0f / length;

            bullets[i].speed.x = dx * inv_len * BULLET_SPEED;
            bullets[i].speed.y = dy * inv_len * BULLET_SPEED;

            bullets[i].active = true;
            bullets[i].damage = 10;

            break;
        }
    }
}

void spawn_enemy() {
    if(game_time % ENEMY_SPAWN_RATE != 0) return;
    for(int i=0; i<MAX_ENEMIES; i++) {
        if(!enemies[i].active) {
            enemies[i].pos.x = rand() % (SCREEN_WIDTH - enemy_sprite.width);
            enemies[i].pos.y = -enemy_sprite.height;
            enemies[i].speed.y = 1 + rand() % 3;
            enemies[i].active = true;
            enemies[i].hp = 20;
            break;
        }
    }
}

void update_entities() {
    // Update bullets
    for(int i=0; i<MAX_BULLETS; i++) {
        if(bullets[i].active) {
            bullets[i].pos.x += bullets[i].speed.x;
            bullets[i].pos.y += bullets[i].speed.y;
            
            // Deactivate if out of bounds
            if((bullets[i].pos.x < 0) || (bullets[i].pos.x > SCREEN_WIDTH) ||
               (bullets[i].pos.y < 0) || (bullets[i].pos.y > SCREEN_HEIGHT)) {
                bullets[i].active = false;
            }
        }
    }
    
    // Update enemies
    for(int i=0; i<MAX_ENEMIES; i++) {
        if(enemies[i].active) {
            enemies[i].pos.y += enemies[i].speed.y;
            
            // Deactivate if out of bounds
            if(enemies[i].pos.y > SCREEN_HEIGHT) {
                enemies[i].active = false;
                player.health -= 10;  // Penalty for missed enemies
            }
        }
    }
}

void draw_game() {
    if(space_bg.bytes){
        draw_background(&space_bg);
    }
    //Draw player
    if (player_sprite.bytes != NULL){
        draw_sprite(&player_sprite, player.pos.x, player.pos.y);
    }
    
    // Draw bullets
    if (bullet_sprite.bytes != NULL){
        for(int i=0; i<MAX_BULLETS; i++) {
            if(bullets[i].active) {
                draw_sprite(&bullet_sprite, bullets[i].pos.x, bullets[i].pos.y);
            }
        }
    }
    
    // Draw enemies
    if (enemy_sprite.bytes != NULL){
        for(int i=0; i<MAX_ENEMIES; i++) {
            if(enemies[i].active) {
                draw_sprite(&enemy_sprite, enemies[i].pos.x, enemies[i].pos.y);
            }
        }
    }
    

    draw_crosshair(cursor_pos);
    draw_hud();
    swap_buffer();
}

void run_pause_menu(Game *game, uint8_t kb_mask, uint8_t mouse_mask) {
    static xpm_image_t pause_img;
    static bool loaded = false;

    if (!loaded) {
        load_xpm(pause_xpm, &pause_img);
        loaded = true;
    }

    vg_clear_screen();

    int box_width = 500;
    int box_height = 500;
    int box_x = (SCREEN_WIDTH - box_width) / 2;
    int box_y = (SCREEN_HEIGHT - box_height) / 2;
    vg_draw_rectangle(box_x, box_y, box_width, box_height, 0);

    int img_x = box_x + (box_width - pause_img.width) / 2;
    int img_y = box_y + (box_height - pause_img.height) / 2;

    draw_sprite(&pause_img, img_x, img_y);

    swap_buffer();

    scancode = 0;

    message msg;
    int ipc_status;

    while (true) {
        if (driver_receive(ANY, &msg, &ipc_status) != 0)
            continue;

        if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE) {
            // Processa teclado
            if (msg.m_notify.interrupts & kb_mask) {
                kbc_ih();
                if ((scancode & 0x80) == 0) {
                    if (scancode == 0x19) { // P para continuar
                        break;
                    } else if (scancode == 0x01) { // ESC para sair
                        game->curState = GAME_STATE_GAMEOVER;
                        break;
                    }
                }
            }

            // Processa mouse para limpar buffer e evitar travamento
            if (msg.m_notify.interrupts & mouse_mask) {
                proj_mouse_ih();
                // Consome e descarta todos os pacotes disponíveis
                while (mouse_packet_ready()) {
                    (void)get_mouse_packet();
                }
            }
        }
    }
}



void check_collisions() {

    for(int i = 0; i < MAX_BULLETS; i++){
        if (!bullets[i].active){
            continue;
        }

        for(int j = 0; j < MAX_ENEMIES; j++){
            if (!enemies[j].active){
                continue;
            }

            bool hit_x = bullets[i].pos.x + bullet_sprite.width > enemies[j].pos.x && bullets[i].pos.x < enemies[j].pos.x + enemy_sprite.width;

            bool hit_y = bullets[i].pos.y + bullet_sprite.height > enemies[j].pos.y && bullets[i].pos.y < enemies[j].pos.y + enemy_sprite.height;

            if (hit_x && hit_y){
                enemies[j].hp -= bullets[i].damage;
                bullets[i].active = false;

                if (enemies[j].hp <= 0){
                    enemies[j].active = false;
                    player.score += 10;
                }
            }
        }
    }
}




void handle_input(uint8_t scanMove){
    switch(scanMove){
        case 0x4B:
        case 0x1E:
            if (player.pos.x > 0){
                player.pos.x -= PLAYER_SPEED;   //Left arrow or 'A'
            }
            break;

        case 0x4D:
        case 0x20: 
            if(player.pos.x < SCREEN_WIDTH ){
                player.pos.x += PLAYER_SPEED;           //Right arrow or 'D'
            }
            break;

        default:
            break;
    }
}

void draw_crosshair(Vec_pos mouse_cursor){
    
    const uint8_t crosshair_color = 54;
    const uint8_t crosshair_size = 20;
    const uint8_t crosshair_thickness = 2;
    const uint8_t crosshair_dot_size = 4;

    int left = mouse_cursor.x - crosshair_size / 2;
    int right = mouse_cursor.x + crosshair_size / 2 - crosshair_thickness;
    int top = mouse_cursor.y - crosshair_size / 2;
    int bottom = mouse_cursor.y + crosshair_size / 2 - crosshair_thickness;

    int dot_x = mouse_cursor.x - crosshair_dot_size / 2;
    int dot_y = mouse_cursor.y - crosshair_dot_size / 2;    



    vg_draw_rectangle(left, top, crosshair_size, crosshair_thickness, crosshair_color);                               
    vg_draw_rectangle(left,bottom, crosshair_size, crosshair_thickness, crosshair_color);

    vg_draw_rectangle(right,top, crosshair_thickness, crosshair_size, crosshair_color);                                                  
    vg_draw_rectangle(left,top, crosshair_thickness, crosshair_size, crosshair_color);

    vg_draw_rectangle(dot_x, dot_y, crosshair_dot_size, crosshair_dot_size, crosshair_color);           //center dot
}



float quake_sqrt(float n){

    float x = n * 0.5f;
    int i = *(int *)&n;
    i  = 0x5f3759df - (i >> 1);         // Quake 3 inverse sqrt formula
    float y = *(float *)&i;

    return n * y * (1.5f - x * y * y);
}
