#include "game.h"
#include "Drivers/VBE/vbe.h"
#include <lcom/xpm.h>
#include <stdio.h>
#include "Sprites/start_sprite.h"
#include "pause_menu.h"
#include "score.h"
#include "menu.h"
#include "Drivers/Keyboard/i8042.h"
#include "game_state.h"
#include <math.h>
#include <unistd.h>


extern void game_draw(void);
extern void menu_draw(void);  
extern void game_set_state(GameState state);
extern void score_reset(void);

static bool gameover_displayed = false;

bool update_mouse = false;

typedef struct {
    int x, y, ox, oy;
    int width, height;
    int vx, vy; 
    bool moving;
} Ball;

static Ball ball;

typedef struct {
    int x, y;
    int width, height;
    bool destroyed;
    uint8_t *sprite_map;
    xpm_image_t *sprite_img;
} Block;

int clamp(int base, int min, int max){
    if(base < min)return min;
    if(base > max)return max;

    return base;
}

#define BLOCK_ROWS 6
#define BLOCK_COLS 4
#define BLOCK_SPACING 20

Block blocks[BLOCK_ROWS * BLOCK_COLS];

#define PLATFORM_STEP 1
#define BALL_STEP 20

int column_width = 250;
extern bool needs_redraw;

static xpm_image_t platform_img;
static uint8_t    *platform_map = NULL;

static xpm_image_t life_full_img;
static uint8_t *life_full_map = NULL;

static xpm_image_t life_mid_img;
static uint8_t *life_mid_map = NULL;

static xpm_image_t life_low_img;
static uint8_t *life_low_map = NULL;

static xpm_image_t gameover_img;
static uint8_t *gameover_map = NULL;

static xpm_image_t win_img;
static uint8_t *win_map = NULL;

static int platform_x = 0;
static int platform_y = 0;

static int o_platform_x = 0;
static int o_platform_y = 0;

static xpm_image_t ball_img;
static uint8_t *ball_map = NULL;

static xpm_image_t points_img;
static uint8_t* points_map = NULL;

extern uint16_t h_res;
extern uint16_t v_res;

static xpm_image_t orange_img, blue_img, green_img, purple_img, red_img, pink_img;
static uint8_t *orange_map = NULL, *blue_map = NULL, *green_map = NULL;
static uint8_t *purple_map = NULL, *red_map = NULL, *pink_map = NULL;

static xpm_image_t pause_button_img;
static uint8_t *pause_button_map = NULL;

static bool game_paused = false;

bool is_game_paused() { return game_paused; }
void set_game_paused(bool value) { game_paused = value; }

int life_count = 3;

int pause_x = 0;
int pause_y = 0;
int life_x = 0;
int life_y = 0;
int score_y = 0;
int points_x = 0;
int points_y = 0;
float mouse_counter = 0;
float destroyed_count = 0;

void draw_life(bool swap, bool skip){

    vg_draw_rectangle(life_x, life_y, life_full_img.width, life_full_img.height, 0x0077B6);

    if(life_count == 3){
        vg_draw_xpm(life_x, life_y, life_full_img.width, life_full_img.height, life_full_map, false);
    }
    else if (life_count == 2){
        vg_draw_xpm(life_x, life_y, life_full_img.width, life_full_img.height, life_mid_map, false);
    }
    else{
        vg_draw_xpm(life_x, life_y, life_full_img.width, life_full_img.height, life_low_map, false);
    }

    if(swap)vg_swap_buffers();
}

void draw_points(bool swap, bool skip){

    vg_draw_rectangle(points_x, points_y, points_img.width, points_img.height, 0x0077B6);
    vg_draw_xpm(points_x, points_y, points_img.width, points_img.height, points_map, false);

    if(swap)vg_swap_buffers();
}

void draw_platform(bool swap, bool skip){

    if(!skip && floor(o_platform_x/PLATFORM_STEP) == floor(platform_x/PLATFORM_STEP))return;

    vg_draw_rectangle(o_platform_x, o_platform_y, platform_img.width, platform_img.height, 0xDEEFF5);
    vg_draw_xpm(platform_x, platform_y, platform_img.width, platform_img.height, platform_map, false);

    o_platform_x = platform_x;
    o_platform_y = platform_y;

    if(swap)vg_swap_buffers();
}

void clear_block(Block b){
    vg_draw_rectangle(b.x, b.y, b.width, b.height, 0xDEEFF5);
}

void draw_ball(bool swap, bool skip){

    int sum = platform_img.width/2 - ball.width/2;

    if(!skip){
        if(floor(ball.ox/BALL_STEP) == floor(ball.x/BALL_STEP) && floor(ball.oy/BALL_STEP) == floor(ball.y / BALL_STEP))return;
        if(!ball.moving){
            if(floor((ball.ox - sum)/PLATFORM_STEP) == floor((ball.x - sum)/PLATFORM_STEP)){
                return;
            }
        }
    }

    vg_draw_rectangle(ball.ox, ball.oy, ball.width, ball.height, 0xDEEFF5);
    vg_draw_xpm(ball.x, ball.y, ball.width, ball.height, ball_map, false);
    
    ball.ox = ball.x;
    ball.oy = ball.y;

    if(swap)vg_swap_buffers();
}

#define MOUSE_SENSITIVITY 7 

void game_handle_mouse(const struct packet *p)
{
    int dx = p->delta_x * MOUSE_SENSITIVITY;

    platform_x += dx;

    if (platform_x < 250) platform_x = 250;
    if (platform_x + platform_img.width > h_res)
        platform_x = h_res - platform_img.width;

    if (!ball.moving)
        ball.x = platform_x + platform_img.width/2 - ball.width/2;

    update_mouse = true;
}

#define PADDLE_SPEED 100

void game_handle_keyboard(uint8_t *scancode, size_t size) {

    if (size != 1 && !(size == 2 && scancode[0] == 0xE0)) return;
    uint8_t code = scancode[size - 1];

    switch (code) {

        case 0x4B: {                    
            platform_x -= PADDLE_SPEED;
            if (platform_x < column_width) platform_x = column_width;

            if (!ball.moving)          
                ball.x = platform_x + platform_img.width/2 - ball.width/2;

            printf("Move to left\n");

            draw_platform(true, false);
            break;
        }

        case 0x4D: {                   
            platform_x += PADDLE_SPEED;
            if (platform_x + platform_img.width > h_res)
                platform_x = h_res - platform_img.width;

            if (!ball.moving)    
                ball.x = platform_x + platform_img.width/2 - ball.width/2;

            draw_platform(true, false);
            break;
        }

        case 0x1C:                     
            if (!ball.moving) {
                ball.vx = 2;
                ball.vy = -2;
                ball.moving = true;
                fflush(stdout);
            }
            break;

        case 0x19: { 
            
            bool paused = !pause_menu_is_active();
            pause_menu_set_active(paused);
            printf("[PAUSE] Toggled pause. New state: %s\n", paused ? "PAUSED" : "PLAYING");
            needs_redraw = true;
            break;
        }      
    }
}

int game_assets_init(const vbe_mode_info_t *unused) {

    platform_map = xpm_load(platform_xpm, XPM_8_8_8_8, &platform_img);
    if (!platform_map) {
        printf("❌ Failed to load platform image\n");
        return 1;
    }

    ball_map = xpm_load(ball_xpm, XPM_8_8_8_8, &ball_img);
    if (!ball_map) {
        printf("❌ Failed to load ball image\n");
        return 1;
    }

    pause_button_map = xpm_load(pause_button_xpm, XPM_8_8_8_8, &pause_button_img);
    if (!pause_button_map) {
        printf("❌ Failed to load pause button image\n");
        return 1;
    }

    life_full_map = xpm_load(life_full_xpm, XPM_8_8_8_8, &life_full_img);
    if (!life_full_map) {
        printf("❌ Failed to load life image\n");
        return 1;
    }
    life_mid_map = xpm_load(life_mid_xpm, XPM_8_8_8_8, &life_mid_img);
    life_low_map = xpm_load(life_low_xpm, XPM_8_8_8_8, &life_low_img);

    points_map = xpm_load(points_xpm, XPM_8_8_8_8, &points_img);
    if (!points_map) {
        printf("❌ Failed to load points label image\n");
    }

    orange_map = xpm_load(block_orange_xpm, XPM_8_8_8_8, &orange_img);
    blue_map   = xpm_load(block_blue_xpm,   XPM_8_8_8_8, &blue_img);
    green_map  = xpm_load(block_green_xpm,  XPM_8_8_8_8, &green_img);
    purple_map = xpm_load(block_purple_xpm, XPM_8_8_8_8, &purple_img);
    red_map    = xpm_load(block_red_xpm,    XPM_8_8_8_8, &red_img);
    pink_map   = xpm_load(block_pink_xpm,   XPM_8_8_8_8, &pink_img);

    if (!orange_map || !blue_map || !green_map || !purple_map || !red_map || !pink_map) {
        printf("❌ Failed to load one or more block images\n");
        return 1;
    }

    xpm_image_t *sprites[] = {
        &orange_img, &blue_img, &green_img,
        &purple_img, &red_img, &pink_img
    };
    uint8_t *maps[] = {
        orange_map, blue_map, green_map,
        purple_map, red_map, pink_map
    };
    int color_count = 6;
    
    int start_x = column_width + 50;
    int start_y = 50;

    for (int row = 0; row < BLOCK_ROWS; row++) {
        for (int col = 0; col < BLOCK_COLS; col++) {
            int i = row * BLOCK_COLS + col;
            int color_index = (row + col) % color_count;
    
            blocks[i].x = start_x + col * (sprites[color_index]->width + BLOCK_SPACING);
            blocks[i].y = start_y + row * (sprites[color_index]->height + BLOCK_SPACING);
            blocks[i].width = sprites[color_index]->width;
            blocks[i].height = sprites[color_index]->height;
            blocks[i].destroyed = false;
            blocks[i].sprite_img = sprites[color_index];
            blocks[i].sprite_map = maps[color_index];
        }
    }

    ball.width = ball_img.width;
    ball.height = ball_img.height;
    ball.moving = false;

    ball.ox = 0;
    ball.oy = 0;

    int game_area_width = h_res - column_width;
    platform_x = column_width + (game_area_width - platform_img.width) / 2;

    platform_y =  v_res - platform_img.height - 50;      

    ball.x = platform_x + platform_img.width / 2 - ball.width / 2;
    ball.y = platform_y - ball.height/2;

    gameover_map = xpm_load(gameover_xpm, XPM_8_8_8_8, &gameover_img);
    if (!gameover_map) {
        printf("❌ Failed to load gameover image\n");
        return 1;
    }

    win_map = xpm_load(win_xpm, XPM_8_8_8_8, &win_img);
    if (!win_map) {
        printf("❌ Failed to load win_xpm image\n");
        return 1;
    }

    score_init();
    score_reset();

    needs_redraw = true;
    return 0;
}

void game_assets_destroy(void) {
    if (platform_map) { free(platform_map); platform_map = NULL; }
    if (ball_map) { free(ball_map); ball_map = NULL; }
    if (orange_map) { free(orange_map); orange_map = NULL; }
    if (blue_map)   { free(blue_map);   blue_map = NULL; }
    if (green_map)  { free(green_map);  green_map = NULL; }
    if (purple_map) { free(purple_map); purple_map = NULL; }
    if (red_map)    { free(red_map);    red_map = NULL; }
    if (pink_map)   { free(pink_map);   pink_map = NULL; }
    if (pause_button_map) { free(pause_button_map); pause_button_map = NULL; }
    if (points_map) { free(points_map); points_map = NULL; }
    if (life_full_map) { free(life_full_map); life_full_map = NULL; }
    if (life_mid_map) { free(life_mid_map); life_mid_map = NULL; }
    if (life_low_map) { free(life_low_map); life_low_map = NULL; }
    if (gameover_map) { free(gameover_map); gameover_map = NULL; }
    if (win_map) { free(win_map); win_map = NULL; }
}

bool check_victory() {
    for (int i = 0; i < BLOCK_ROWS * BLOCK_COLS; i++) {
        if (!blocks[i].destroyed) {
            return false;
        }
    }
    return true; 
}

GameState show_victory_screen() {
    if (win_map) {
        int x = (h_res - win_img.width) / 2;
        int y = (v_res - win_img.height) / 2;
        vg_draw_xpm(x, y, win_img.width, win_img.height, win_map, false);
    }
    vg_swap_buffers();
    sleep(5);
    needs_redraw = true;
    return STATE_MENU;
}

void reset_game(){
    for (int i = 0; i < BLOCK_ROWS * BLOCK_COLS; i++) {
        blocks[i].destroyed = false;
    }
    score_reset();

    life_count = 3;
    ball.moving = false;
    destroyed_count = 0;
}

void game_draw(void) {
    if (!needs_redraw) return;

    pause_y = 50;
    life_x = (column_width - life_full_img.width) / 2;
    pause_x = (column_width - pause_button_img.width) / 2;
    pause_y = 50;
    life_y = pause_y + 160;
    score_y = life_y + 160;

    points_x = 50;
    points_y = score_y + 50;

    printf("redrawing game");

    vg_draw_rectangle(0, 0, h_res, v_res, 0xDEEFF5);
    
    draw_platform(false, true);

    draw_ball(false, true);

    vg_draw_rectangle(0, 0, column_width, v_res, 0x0077B6); 

    draw_points(false, true);

    vg_draw_xpm(pause_x, pause_y, pause_button_img.width, pause_button_img.height, pause_button_map, false);

    draw_life(false, true);

    for (int i = 0; i < BLOCK_ROWS * BLOCK_COLS; i++) {
        if (!blocks[i].destroyed) {
            vg_draw_xpm(
                blocks[i].x, blocks[i].y,
                blocks[i].sprite_img->width, blocks[i].sprite_img->height,
                blocks[i].sprite_map, false
            );
        }
    }
    
    score_draw_at(0, score_y);
    needs_redraw = false;
}

int ball_spd = 3;

GameState show_gameover_screen() {

    int x = (h_res - gameover_img.width) / 2;
    int y = (v_res - gameover_img.height) / 2;

    vg_draw_xpm(x, y, gameover_img.width, gameover_img.height, gameover_map, false);
    vg_swap_buffers();

    sleep(5);
    needs_redraw = true;
    gameover_displayed = false;
    return STATE_MENU;
}

void take_damage() {
    life_count--;

    if (life_count <= 0) {
        ball.moving = false;
        if (!gameover_displayed) {
            gameover_displayed = true;
            GameState new_state = show_gameover_screen();
            game_set_state(new_state);
            reset_game();
        }
        return;
    }
    
    ball.moving = false;
    draw_life(true, false);
}

void game_update(void){
    if (pause_menu_is_active()) return;
    
    if (ball.moving) {

        ball_spd = 3 + floor(15 * (destroyed_count / (BLOCK_ROWS * BLOCK_COLS)));
        printf("ball spd: %u %u\n", ball_spd, (int)destroyed_count);

        bool hit = false;

        int next_x = ball.x + ball.vx * ball_spd;
        int next_y = ball.y + ball.vy * ball_spd;

        int og_vx = ball.vx;
        int og_vy = ball.vy;

        ball.x = next_x;

        for (int i = 0; i < BLOCK_ROWS * BLOCK_COLS; i++) {
            if (blocks[i].destroyed) continue;
        
            if (ball.x + ball.width >= blocks[i].x &&
                ball.x <= blocks[i].x + blocks[i].width &&
                ball.y + ball.height >= blocks[i].y &&
                ball.y <= blocks[i].y + blocks[i].height) {
        
                blocks[i].destroyed = true;
                destroyed_count += 1;
                score_add(5);
                clear_block(blocks[i]);
                ball.vx = -og_vx;  
                hit = true;
                
                if (check_victory()) {
                    ball.moving = false;
                    GameState new_state = show_victory_screen();
                    game_set_state(new_state);
                    reset_game();
                    return; 
                }
            }
        }

        ball.y  = next_y;

        for (int i = 0; i < BLOCK_ROWS * BLOCK_COLS; i++) {
            if (blocks[i].destroyed) continue;
        
            if (ball.x + ball.width >= blocks[i].x &&
                ball.x <= blocks[i].x + blocks[i].width &&
                ball.y + ball.height >= blocks[i].y &&
                ball.y <= blocks[i].y + blocks[i].height) {
        
                blocks[i].destroyed = true;
                destroyed_count += 1;
                score_add(5);
                clear_block(blocks[i]);
                ball.vy = -og_vy; 
                hit = true;
                
                if (check_victory()) {
                    ball.moving = false;
                    GameState new_state = show_victory_screen();
                    game_set_state(new_state);
                    reset_game();
                    return;
                }
            }
        }

        if(hit)score_draw_at(0, score_y);

        if (ball.x <= 250 || ball.x + ball.width >= h_res){
            ball.x = clamp(ball.x, 250, h_res);
            ball.vx *= -1;
        }

        if (ball.y <= 0){
            ball.y = 0;
            ball.vy *= -1;
        }

        if(ball.vy > 0){

            if(ball.y + ball.height >= v_res){
                take_damage();
                ball.vy = -1;
            }
            
            if (ball.y < platform_y){

                if (ball.y + ball.height >= platform_y &&
                    ball.x + ball.width >= platform_x &&
                    ball.x <= platform_x + platform_img.width){
                        ball.y = platform_y - ball.height;
                        ball.vy = -1;
                    }
            }
        }
    }else
    {
        ball.x = platform_x + platform_img.width/2 - ball.width/2;
        ball.y = platform_y - ball.height;
    }

    draw_ball(true, false);

    if(update_mouse && mouse_counter >= 3){
        draw_platform(true, false);
        update_mouse = false;
        mouse_counter = 0;
    }
    else {
        mouse_counter += 1;
    }
}
