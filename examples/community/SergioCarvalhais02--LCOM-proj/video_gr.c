#include "video_gr.h"


void * (vg_init)(uint16_t mode){

    if ((vbe_get_mode_info(mode, &vmi_p)) != 0) return NULL;

    reg86_t r86;

    memset(&r86, 0, sizeof(r86));

    r86.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
    r86.bx = 1<<14|mode; // set bit 14: linear framebuffer
    r86.intno = 0x10;

    struct minix_mem_range mr;
    unsigned int vram_base;  /* VRAM's physical addresss */
    unsigned int vram_size;  /* VRAM's size, but you can use the frame-buffer size, instead */
    int r;

    /* Use VBE function 0x01 to initialize vram_base and vram_size */

    vram_base = vmi_p.PhysBasePtr;
    vram_size = (vmi_p.XResolution * vmi_p.YResolution * (vmi_p.BitsPerPixel + 7)) / 8;   

    bytes_per_pixel = (vmi_p.BitsPerPixel + 7) / 8;


    vram_sz = vram_size;
    h_res = vmi_p.XResolution;
    v_res = vmi_p.YResolution;
    bits_p_pixel = vmi_p.BitsPerPixel;

    /* Allow memory mapping */

    mr.mr_base = (phys_bytes) vram_base;	
    mr.mr_limit = mr.mr_base + vram_size;  

    if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);

    /* Map memory */

    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

    if (video_mem == MAP_FAILED)
        panic("couldn't map video memory");

       
    if(sys_int86(&r86) !=  OK) {
        printf("set_vbe_mode: sys_int86() failed \n");
        return NULL;
    }

    vg_ptr = (uint8_t*)video_mem;

    vg_bg_buffer = malloc(vram_sz);

    vg_mouse_buffer = malloc(vram_sz);


    vg_elem_buffer = malloc(vram_sz);

    memset(vg_ptr, 0, vram_sz); //pode dar erro

    return video_mem;
}

int (my_vg_exit)(){
    
    free(vg_bg_buffer);

    free(vg_elem_buffer);

    free(vg_mouse_buffer);
    
    if(vg_exit()) return 1;
    
    return 0;
}

uint32_t (my_set_color)(uint32_t color){
    
    if (bits_p_pixel < 32) {
        uint32_t tmp = 0;
        for (unsigned int i = 0; i < bits_p_pixel; i++){
            tmp |= BIT(i);
        } 
        color |= tmp;
    }
    return color;
}

void (vg_bg_clear)() {
    memset(vg_bg_buffer, 0, vram_sz);
}

int (draw_game)(Game *game){

    if (vg_set_buffer(game) != 0) return 1;

    if (vg_draw_mouse_buffer() != 0) return 1;

    return 0;
}

int (vg_draw_buffer)(){
    
    if (memcpy(vg_ptr, vg_bg_buffer, vram_sz) == NULL) return 1;
    
    return 0; 
}

int (vg_draw_mouse_buffer)(){

    if (memcpy(vg_ptr, vg_mouse_buffer, vram_sz) == NULL) return 1;
    
    return 0;
}

int (vg_set_buffer)(Game* game){
    
    if(game->img_state == CHANGE_BG){
        vg_bg_clear();
        if ((draw_bg(game))!= 0) return 1; 
        game->img_state = CHANGE_ELEM;
    }
    
    if (game->img_state == CHANGE_ELEM){
        if ((draw_elem(game)) != 0) return 1;
        game->img_state = KEEP;
    }

    if ((my_draw_mouse(game->mouse.hitbox.pos, game->mouse.hitbox.width, game->mouse.hitbox.height, game->mouse.hitbox.color)) != 0){
        return 1;
    }
    
    return 0;
}

int (print_xpm_custom)(xpm_map_t xpm, uint16_t x, uint16_t y, bool in_bg, Video_Mode vm) {
    
    if (in_bg){    
        xpm_image_t img;

        uint8_t *colors;

        switch (vm){
            case INDEXED: {
                colors = xpm_load(xpm, XPM_INDEXED, &img);
                break;
            }

            case DIRECT_8_8_8: {
                colors = xpm_load(xpm, XPM_8_8_8, &img);
                break;
            }

            case DIRECT_8_8_8_8: {
                colors = xpm_load(xpm, XPM_8_8_8_8, &img);
                break;
            }

            default: break;
        }
        

        uint16_t min_h = (img.height < v_res) ? img.height : v_res;
        uint16_t min_w = (img.width  < h_res) ? img.width  : h_res;

        for (int h = 0; h < min_h; h++) {
            for (int w = 0; w < min_w; w++) {
                uint32_t color = 0;
                
                for (int i = 0; i < bytes_per_pixel; i++)
                    color |= (*(colors + i) << (8 * i));
                
                draw_pixel(x + w, y + h, color);
                
                colors += bytes_per_pixel;
            }
        }
    }

    else {
        xpm_image_t img;

        uint8_t *colors;

        uint32_t ignore_color = 0;

        switch (vm){
            case INDEXED: {
                colors = xpm_load(xpm, XPM_INDEXED, &img);
                ignore_color = IGNORE_COLOR_INDEXED;
                break;
            }

            case DIRECT_8_8_8: {
                colors = xpm_load(xpm, XPM_8_8_8, &img);
                ignore_color = IGNORE_COLOR_DIRECT;
                break;
            }    

            case DIRECT_8_8_8_8: {
                colors = xpm_load(xpm, XPM_8_8_8_8, &img);
                ignore_color = IGNORE_COLOR_DIRECT;
                break;
            }

            default: break;
        }


        uint16_t min_h = (img.height < v_res) ? img.height : v_res;
        uint16_t min_w = (img.width  < h_res) ? img.width  : h_res;

        for (int h = 0; h < min_h; h++) {
            for (int w = 0; w < min_w; w++) {
                uint32_t color = 0;
                
                for (int i = 0; i < bytes_per_pixel; i++) {
                    color |= (*(colors + i) << (8 * i));
                }
                
                if(color != ignore_color)
                    draw_pixel_2(x + w, y + h, color);
                
                colors += bytes_per_pixel;
            }
        }
    }

    return 0;
}


int (draw_rectangle_2)(Rectangle * r){

    if(!r->active) return 0;

    return my_draw_rectangle_2(r->pos, r->width, r->height, r->color);
}

int (draw_rectangle)(Rectangle * r){

    if(!r->active) return 0;

    return my_draw_rectangle(r->pos, r->width, r->height, r->color);
}

int (my_draw_mouse)(Position pos, uint16_t width, uint16_t height, uint32_t color){
    
    memcpy(vg_mouse_buffer, vg_elem_buffer, vram_sz);

    if((pos.x + width) < 0 || (pos.y + height) < 0 || pos.x >= h_res || pos.y >= v_res) return 0;
    
    for(int i = 0; i < height; i++)
        if (vg_draw_hline_3(pos.x, pos.y + i, width, color) != 0) return 1;

    return 0;
}

int (my_draw_rectangle)(Position pos, uint16_t width, uint16_t height, uint32_t color){ // BACKGROUND
    
    if((pos.x + width) < 0 || (pos.y + height) < 0 || pos.x >= h_res || pos.y >= v_res) return 0;
    
    for(int i = 0; i < height; i++)
        if (vg_draw_hline(pos.x, pos.y + i, width, color) != 0) return 1;

    return 0;
}

int (my_draw_rectangle_2)(Position pos, uint16_t width, uint16_t height, uint32_t color){ // ELEMENTS
    
    if((pos.x + width) < 0 || (pos.y + height) < 0 || pos.x >= h_res || pos.y >= v_res) return 0;
    
    for(int i = 0; i < height; i++)
        if (vg_draw_hline_2(pos.x, pos.y + i, width, color) != 0) return 1;

    return 0;
}

int (vg_draw_hline_3)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){ // MOUSE
    
    for(int i = 0; i < len; i++)
        if (draw_pixel_3(x + i, y, color) != 0) return 1;
    
    return 0;
}

int (vg_draw_hline_2)(uint16_t x,uint16_t y, uint16_t len,uint32_t color){ // ELEMENTS
    
    for(int i = 0; i < len; i++)
        if (draw_pixel_2(x + i, y, color) != 0) return 1;
    
    return 0;
}

int (vg_draw_hline)(uint16_t x,uint16_t y, uint16_t len,uint32_t color){ // BACKGROUND
    
    for(int i = 0; i < len; i++)
        if (draw_pixel(x + i, y, color) != 0) return 1;
    
    return 0;
}

int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color){ // BACKGROUND

    if (x < 0 || y < 0 || x >= h_res || y >= v_res) return 0;

    uint32_t pos = ((y * h_res) + x) * bytes_per_pixel;

    if ((memcpy((vg_bg_buffer + pos), &color, bytes_per_pixel)) == NULL) return 1;

    return 0;
}

int (draw_pixel_2)(uint16_t x, uint16_t y, uint32_t color){ // ELEMENTS

    if (x < 0 || y < 0 || x >= h_res || y >= v_res) return 0;

    uint32_t pos = ((y * h_res) + x) * bytes_per_pixel;

    if ((memcpy((vg_elem_buffer + pos), &color, bytes_per_pixel)) == NULL) return 1;

    return 0;
}

int (draw_pixel_3)(uint16_t x, uint16_t y, uint32_t color){ // MOUSE
    if (x < 0 || y < 0 || x >= h_res || y >= v_res) return 0;

    uint32_t pos = ((y * h_res) + x) * bytes_per_pixel;

    if ((memcpy((vg_mouse_buffer + pos), &color, bytes_per_pixel)) == NULL) return 1;

    return 0;
}

int (draw_elem)(Game * game){

    memcpy(vg_elem_buffer, vg_bg_buffer, vram_sz);

    switch (game->g_state){
        case MENU: {
            if ((draw_menu_elem(game)) != 0) return 1;
            break;
        }

        case RUNNING: {
            if ((draw_run_elem(game)) != 0) return 1;
            break;
        }

        case PONTUATION: {
            if ((draw_pontuation(game)) != 0) return 1;
            break;
        }
        
        case LEADERBOARD: {
            if ((draw_leaderboard(game)) != 0) return 1;
            break;
        }
        
        default: return 1;
    }
    return 0;
}

int (draw_run_elem)(Game * game){

    for (int i = 0 ; i < 9 ; i++)
        if (draw_button(&game->buttons[i], game->video_mode) != 0) return 1;

    if ((draw_timer_number(game)) != 0) return 1;
    
    return 0;
}

int (draw_button)(Button * button, Video_Mode video_mode) {
    if (video_mode == INDEXED){
        switch (button->button_type) {
            case RED_MOLE: {
                xpm_map_t b = (button->button_state == ALIVE) ? (xpm_map_t) red_button_indexed : (xpm_map_t) red_button_pressed_indexed;
                print_xpm_custom(b, button->hitbox.pos.x, button->hitbox.pos.y, false, video_mode);
                break;
            }
            case GREEN_MOLE: {
                xpm_map_t b = (button->button_state == ALIVE) ? (xpm_map_t) green_button_indexed : (xpm_map_t) green_button_pressed_indexed;
                print_xpm_custom(b, button->hitbox.pos.x, button->hitbox.pos.y, false, video_mode);
                break;
            }
            case BLUE_MOLE: {
                xpm_map_t b = (button->button_state == ALIVE) ? (xpm_map_t) blue_button_indexed : (xpm_map_t) blue_button_pressed_indexed;
                print_xpm_custom(b, button->hitbox.pos.x, button->hitbox.pos.y, false, video_mode);
                break;
            }
            case YELLOW_MOLE: {
                xpm_map_t b = (button->button_state == ALIVE) ? (xpm_map_t) yellow_button_indexed : (xpm_map_t) yellow_button_pressed_indexed;
                print_xpm_custom(b, button->hitbox.pos.x, button->hitbox.pos.y, false, video_mode);
                break;
            }
            case BLACK_MOLE: {
                xpm_map_t b = (button->button_state == ALIVE) ? (xpm_map_t) black_button_indexed : (xpm_map_t) black_button_pressed_indexed;
                print_xpm_custom(b, button->hitbox.pos.x, button->hitbox.pos.y, false, video_mode);
                break;
            }
            case WHITE_MOLE: {
                xpm_map_t b = (button->button_state == ALIVE) ? (xpm_map_t) white_button_indexed : (xpm_map_t) white_button_pressed_indexed;
                print_xpm_custom(b, button->hitbox.pos.x, button->hitbox.pos.y, false, video_mode);
                break;
            }
            default: break;
        }
    } else {
        switch (button->button_type) {
            case RED_MOLE: {
                xpm_map_t b = (button->button_state == ALIVE) ? (xpm_map_t) red_button_direct : (xpm_map_t) red_button_pressed_direct;
                print_xpm_custom(b, button->hitbox.pos.x, button->hitbox.pos.y, false, video_mode);
                break;
            }
            case GREEN_MOLE: {
                xpm_map_t b = (button->button_state == ALIVE) ? (xpm_map_t) green_button_direct : (xpm_map_t) green_button_pressed_direct;
                print_xpm_custom(b, button->hitbox.pos.x, button->hitbox.pos.y, false, video_mode);
                break;
            }
            case BLUE_MOLE: {
                xpm_map_t b = (button->button_state == ALIVE) ? (xpm_map_t) blue_button_direct : (xpm_map_t) blue_button_pressed_direct;
                print_xpm_custom(b, button->hitbox.pos.x, button->hitbox.pos.y, false, video_mode);
                break;
            }
            case YELLOW_MOLE: {
                xpm_map_t b = (button->button_state == ALIVE) ? (xpm_map_t) yellow_button_direct : (xpm_map_t) yellow_button_pressed_direct;
                print_xpm_custom(b, button->hitbox.pos.x, button->hitbox.pos.y, false, video_mode);
                break;
            }
            case BLACK_MOLE: {
                xpm_map_t b = (button->button_state == ALIVE) ? (xpm_map_t) black_button_direct : (xpm_map_t) black_button_pressed_direct;
                print_xpm_custom(b, button->hitbox.pos.x, button->hitbox.pos.y, false, video_mode);
                break;
            }
            case WHITE_MOLE: {
                xpm_map_t b = (button->button_state == ALIVE) ? (xpm_map_t) white_button_direct : (xpm_map_t) white_button_pressed_direct;
                print_xpm_custom(b, button->hitbox.pos.x, button->hitbox.pos.y, false, video_mode);
                break;
            }
            default: break;
        }
    }
    return 0;
}

int (draw_menu_elem)(Game * game){
    
    for (int i = 0 ; i < 3 ; i++)
        if (draw_rectangle_2(&game->menu_elements[i]) != 0) return 1;

    uint32_t menu_x = (game->lim_x)/2 - 514/2,
             play_x = (game->lim_x)/2 - (game->menu_elements[0].width)/2,
             leaderboard_x = (game->lim_x)/2 - (game->menu_elements[1].width)/2,
             exit_x = (game->lim_x)/2 - (game->menu_elements[2].width)/2,

             y = (game->lim_y)/2 - (game->menu_elements[0].height)/2 - 75;

    if (game->video_mode == INDEXED){
        xpm_map_t play = (xpm_map_t) play_img_indexed;
        xpm_map_t leaderboard = (xpm_map_t) leaderboard_img_indexed;
        xpm_map_t exit = (xpm_map_t) exit_img_indexed;
        xpm_map_t menu = (xpm_map_t) menu_img_indexed;

        print_xpm_custom(menu, menu_x, 50, false, game->video_mode);
        print_xpm_custom(play, play_x, y, false, game->video_mode);
        print_xpm_custom(leaderboard, leaderboard_x, y + 125, false, game->video_mode);
        print_xpm_custom(exit, exit_x, y + 250, false, game->video_mode);
    }
    else {
        xpm_map_t play = (xpm_map_t) play_img_direct;
        xpm_map_t leaderboard = (xpm_map_t) leaderboard_img_direct;
        xpm_map_t exit = (xpm_map_t) exit_img_direct;
        xpm_map_t menu = (xpm_map_t) menu_img_direct;

        print_xpm_custom(menu, menu_x, 50, false, game->video_mode);
        print_xpm_custom(play, play_x, y, false, game->video_mode);
        print_xpm_custom(leaderboard, leaderboard_x, y + 125, false, game->video_mode);
        print_xpm_custom(exit, exit_x, y + 250, false, game->video_mode);
    }
    
    return 0;
}

int (draw_pontuation)(Game * game){

    for (int i = 0 ; i < 3 ; i++)
        if (draw_rectangle_2(&game->pontuation_elements[i]) != 0) return 1;

    xpm_map_t pontuation = (game->video_mode == INDEXED) ? (xpm_map_t) pontuation_indexed : (xpm_map_t) pontuation_direct;
    print_xpm_custom(pontuation, game->pontuation_menu.pos.x, game->pontuation_menu.pos.y, false, game->video_mode);


    uint16_t lx = (game->lim_x)/2 - (game->pontuation_menu.width)/2 + 110 ,
             ly = (game->lim_y)/2 + 5;

    for (int i = 0 ; i < game->text.cur_key ; i++)
        draw_letter(game, game->text.key_list[i], &lx, ly);


    uint16_t nx = (game->lim_x)/2 + 40,
             ny = (game->lim_y)/2 + 57,
             score_number = game->pontuation;

    draw_score(game, nx, ny, score_number);

    return 0;
}

int (draw_score)(Game * game, uint16_t x, uint16_t y, uint16_t score_number){ // score in points

    do {
        switch (score_number % 10) {
            case 0: {
                x -= 32;
                xpm_map_t n = (game->video_mode == INDEXED) ? (xpm_map_t) xpm_0_indexed : (xpm_map_t) xpm_0_direct;
                if ((print_xpm_custom(n, x, y, false, game->video_mode)) != 0) return 1;
                break;
            }
            case 1: {
                x -= 24;
                xpm_map_t n = (game->video_mode == INDEXED) ? (xpm_map_t) xpm_1_indexed : (xpm_map_t) xpm_1_direct;
                if ((print_xpm_custom(n, x, y, false, game->video_mode)) != 0) return 1;
                break;
            }
            case 2: {
                x -= 30;
                xpm_map_t n = (game->video_mode == INDEXED) ? (xpm_map_t) xpm_2_indexed : (xpm_map_t) xpm_2_direct;
                if ((print_xpm_custom(n, x, y, false, game->video_mode)) != 0) return 1;
                break;
            }
            case 3: {
                x -= 30;
                xpm_map_t n = (game->video_mode == INDEXED) ? (xpm_map_t) xpm_3_indexed : (xpm_map_t) xpm_3_direct;
                if ((print_xpm_custom(n, x, y, false, game->video_mode)) != 0) return 1;
                break;
            }
            case 4: {
                x -= 32;
                xpm_map_t n = (game->video_mode == INDEXED) ? (xpm_map_t) xpm_4_indexed : (xpm_map_t) xpm_4_direct;
                if ((print_xpm_custom(n, x, y, false, game->video_mode)) != 0) return 1;
                break;
            }
            case 5: {
                x -= 30;
                xpm_map_t n = (game->video_mode == INDEXED) ? (xpm_map_t) xpm_5_indexed : (xpm_map_t) xpm_5_direct;
                if ((print_xpm_custom(n, x, y, false, game->video_mode)) != 0) return 1;
                break;
            }
            case 6: {
                x -= 30;
                xpm_map_t n = (game->video_mode == INDEXED) ? (xpm_map_t) xpm_6_indexed : (xpm_map_t) xpm_6_direct;
                if ((print_xpm_custom(n, x, y, false, game->video_mode)) != 0) return 1;
                break;
            }
            case 7: {
                x -= 30;
                xpm_map_t n = (game->video_mode == INDEXED) ? (xpm_map_t) xpm_7_indexed : (xpm_map_t) xpm_7_direct;
                if ((print_xpm_custom(n, x, y, false, game->video_mode)) != 0) return 1;
                break;
            }
            case 8: {
                x -= 32;
                xpm_map_t n = (game->video_mode == INDEXED) ? (xpm_map_t) xpm_8_indexed : (xpm_map_t) xpm_8_direct;
                if ((print_xpm_custom(n, x, y, false, game->video_mode)) != 0) return 1;
                break;
            }
            case 9: {
                x -= 30;
                xpm_map_t n = (game->video_mode == INDEXED) ? (xpm_map_t) xpm_9_indexed : (xpm_map_t) xpm_9_direct;
                if ((print_xpm_custom(n, x, y, false, game->video_mode)) != 0) return 1;
                break;
            }
            default: break;
        }
        
        x += 5;
        score_number /= 10;

    } while (score_number != 0);

    return 0;
}

int (draw_leaderboard)(Game * game){

    if (draw_rectangle_2(&game->leaderboard_elements[0]) != 0) return 1;

    xpm_map_t leaderboard = (game->video_mode == INDEXED) ? (xpm_map_t) leaderboard_indexed : (xpm_map_t) leaderboard_direct;
    print_xpm_custom(leaderboard, game->leaderboard_menu.pos.x, game->leaderboard_menu.pos.y, false, game->video_mode);

    draw_top_3(game);

    return 0;
}

int (draw_top_3)(Game * game){

    uint16_t lx = (game->lim_x / 2) - (game->leaderboard_menu.width/2) + 100,
             ly = (game->lim_y / 2) - (game->leaderboard_menu.height/2) + 145,
             nx = (game->lim_x / 2) + (game->leaderboard_menu.width/2) - 50,
             ny = (game->lim_y / 2) - (game->leaderboard_menu.height/2) + 145;

    for (int i = 0 ; i < 3 ; i++){
        for (int l = 0; l < 10; l++)
            draw_letter(game, game->top_score_list[i].name[l], &lx, ly);
        
        lx = (game->lim_x / 2) - (game->leaderboard_menu.width/2) + 100;
        ly += 70;
        

        draw_score(game, nx, ny, game->top_score_list[i].score);
        ny += 70;

    }

    return 0;
}

int (draw_bg)(Game * game){
    switch (game->g_state){
        case MENU: {
            if ((draw_menu_bg(game))!= 0) return 1;
            break;
        }

        case RUNNING: {
            if ((draw_run_bg(game))!= 0) return 1;        
            break;
        }

        default: return 1;
    }
    return 0;
}

int (draw_menu_bg)(Game * game){

    if(game->video_mode == INDEXED){
        xpm_map_t a = (xpm_map_t) background_indexed_xpm;
        print_xpm_custom(a, 0, 0, true, game->video_mode);
    }

    else if(game->video_mode == DIRECT_8_8_8){
        xpm_map_t a = (xpm_map_t) background_115_xpm;
        print_xpm_custom(a, 0, 0, true, game->video_mode);
    }

    else if(game->video_mode == DIRECT_8_8_8_8){
        xpm_map_t a = (xpm_map_t) background_14C_xpm;
        print_xpm_custom(a, 0, 0, true, game->video_mode);
    }
    
    return 0;
}

int (draw_run_bg)(Game * game){
    
    if(game->video_mode == INDEXED){
        xpm_map_t a = (xpm_map_t) background_indexed;
        print_xpm_custom(a, 0, 0, true, game->video_mode);
    }

    else if(game->video_mode == DIRECT_8_8_8){
        xpm_map_t a = (xpm_map_t) background_115_direct;
        print_xpm_custom(a, 0, 0, true, game->video_mode);
    }

    else if(game->video_mode == DIRECT_8_8_8_8){
        xpm_map_t a = (xpm_map_t) background_14C_direct;
        print_xpm_custom(a, 0, 0, true, game->video_mode);
    }

    return 0;
}

int (draw_timer_number)(Game * game){ //numero em segundos

    uint16_t x = game->lim_x - 91,
             y = 7,
             timer_number = game->game_timer / 60;
    
    do {
        switch (timer_number % 10) {
            case 0: {
                xpm_map_t a = (game->video_mode == INDEXED) ? (xpm_map_t) n_timer_0_indexed : (xpm_map_t) n_timer_0_direct;
                if ((print_xpm_custom(a, x, y, false, game->video_mode)) != 0) return 1;
                break;
            }
            case 1: {
                xpm_map_t a = (game->video_mode == INDEXED) ? (xpm_map_t) n_timer_1_indexed : (xpm_map_t) n_timer_1_direct;
                if ((print_xpm_custom(a, x, y, false, game->video_mode)) != 0) return 1;
                break;
            }
            case 2: {
                xpm_map_t a = (game->video_mode == INDEXED) ? (xpm_map_t) n_timer_2_indexed : (xpm_map_t) n_timer_2_direct;
                if ((print_xpm_custom(a, x, y, false, game->video_mode)) != 0) return 1;
                break;
            }
            case 3: {
                xpm_map_t a = (game->video_mode == INDEXED) ? (xpm_map_t) n_timer_3_indexed : (xpm_map_t) n_timer_3_direct;
                if ((print_xpm_custom(a, x, y, false, game->video_mode)) != 0) return 1;
                break;
            }
            case 4: {
                xpm_map_t a = (game->video_mode == INDEXED) ? (xpm_map_t) n_timer_4_indexed : (xpm_map_t) n_timer_4_direct;
                if ((print_xpm_custom(a, x, y, false, game->video_mode)) != 0) return 1;
                break;
            }
            case 5: {
                xpm_map_t a = (game->video_mode == INDEXED) ? (xpm_map_t) n_timer_5_indexed : (xpm_map_t) n_timer_5_direct;
                if ((print_xpm_custom(a, x, y, false, game->video_mode)) != 0) return 1;
                break;
            }
            case 6: {
                xpm_map_t a = (game->video_mode == INDEXED) ? (xpm_map_t) n_timer_6_indexed : (xpm_map_t) n_timer_6_direct;
                if ((print_xpm_custom(a, x, y, false, game->video_mode)) != 0) return 1;
                break;
            }
            case 7: {
                xpm_map_t a = (game->video_mode == INDEXED) ? (xpm_map_t) n_timer_7_indexed : (xpm_map_t) n_timer_7_direct;
                if ((print_xpm_custom(a, x, y, false, game->video_mode)) != 0) return 1;
                break;
            }
            case 8: {
                xpm_map_t a = (game->video_mode == INDEXED) ? (xpm_map_t) n_timer_8_indexed : (xpm_map_t) n_timer_8_direct;
                if ((print_xpm_custom(a, x, y, false, game->video_mode)) != 0) return 1;
                break;
            }
            case 9: {
                xpm_map_t a = (game->video_mode == INDEXED) ? (xpm_map_t) n_timer_9_indexed : (xpm_map_t) n_timer_9_direct;
                if ((print_xpm_custom(a, x, y, false, game->video_mode)) != 0) return 1;
                break;
            }
            default: break;
        }

        x -= 71;
        timer_number /= 10;

    } while (timer_number != 0);
    
    return 0;
}

int (draw_letter)(Game * game, Keyboard_Key key, uint16_t * x, uint16_t y){

    switch (key){
        case A_KEY: {
            xpm_map_t a = (game->video_mode == INDEXED) ? (xpm_map_t) A_xpm_indexed : (xpm_map_t) A_xpm_direct;
            print_xpm_custom (a, *x, y, false, game->video_mode);
            *x += 32;
            break;
        }
        case B_KEY: {
            xpm_map_t b = (game->video_mode == INDEXED) ? (xpm_map_t) B_xpm_indexed : (xpm_map_t) B_xpm_direct;
            print_xpm_custom (b, *x, y, false, game->video_mode);
            *x += 32;
            break;
        }
        case C_KEY: {
            xpm_map_t c = (game->video_mode == INDEXED) ? (xpm_map_t) C_xpm_indexed : (xpm_map_t) C_xpm_direct;
            print_xpm_custom (c, *x, y, false, game->video_mode);
            *x += 32;
            break;
        }
        case D_KEY: {
            xpm_map_t d = (game->video_mode == INDEXED) ? (xpm_map_t) D_xpm_indexed : (xpm_map_t) D_xpm_direct;
            print_xpm_custom (d, *x, y, false, game->video_mode);
            *x += 32;
            break;
        }
        case E_KEY: {
            xpm_map_t e = (game->video_mode == INDEXED) ? (xpm_map_t) E_xpm_indexed : (xpm_map_t) E_xpm_direct;
            print_xpm_custom (e, *x, y, false, game->video_mode);
            *x += 30;
            break;
        }
        case F_KEY: {
            xpm_map_t f = (game->video_mode == INDEXED) ? (xpm_map_t) F_xpm_indexed : (xpm_map_t) F_xpm_direct;
            print_xpm_custom (f, *x, y, false, game->video_mode);
            *x += 28;
            break;
        }
        case G_KEY: {
            xpm_map_t g = (game->video_mode == INDEXED) ? (xpm_map_t) G_xpm_indexed : (xpm_map_t) G_xpm_direct;
            print_xpm_custom (g, *x, y, false, game->video_mode);
            *x += 32;
            break;
        }
        case H_KEY: {
            xpm_map_t h = (game->video_mode == INDEXED) ? (xpm_map_t) H_xpm_indexed : (xpm_map_t) H_xpm_direct;
            print_xpm_custom (h, *x, y, false, game->video_mode);
            *x += 32;
            break;
        }
        case I_KEY: {
            xpm_map_t i = (game->video_mode == INDEXED) ? (xpm_map_t) I_xpm_indexed : (xpm_map_t) I_xpm_direct;
            print_xpm_custom (i, *x, y, false, game->video_mode);
            *x += 21;
            break;
        }
        case J_KEY: {
            xpm_map_t j = (game->video_mode == INDEXED) ? (xpm_map_t) J_xpm_indexed : (xpm_map_t) J_xpm_direct;
            print_xpm_custom (j, *x, y, false, game->video_mode);
            *x += 29;
            break;
        }
        case K_KEY: {
            xpm_map_t k = (game->video_mode == INDEXED) ? (xpm_map_t) K_xpm_indexed : (xpm_map_t) K_xpm_direct;
            print_xpm_custom (k, *x, y, false, game->video_mode);
            *x += 32;
            break;
        }
        case L_KEY: {
            xpm_map_t l = (game->video_mode == INDEXED) ? (xpm_map_t) L_xpm_indexed : (xpm_map_t) L_xpm_direct;
            print_xpm_custom (l, *x, y, false, game->video_mode);
            *x += 28;
            break;
        }
        case M_KEY: {
            xpm_map_t m = (game->video_mode == INDEXED) ? (xpm_map_t) M_xpm_indexed : (xpm_map_t) M_xpm_direct;
            print_xpm_custom (m, *x, y, false, game->video_mode);
            *x += 42;
            break;
        }
        case N_KEY: {
            xpm_map_t n = (game->video_mode == INDEXED) ? (xpm_map_t) N_xpm_indexed : (xpm_map_t) N_xpm_direct;
            print_xpm_custom (n, *x, y, false, game->video_mode);
            *x += 31;
            break;
        }
        case O_KEY: {
            xpm_map_t o = (game->video_mode == INDEXED) ? (xpm_map_t) O_xpm_indexed : (xpm_map_t) O_xpm_direct;
            print_xpm_custom (o, *x, y, false, game->video_mode);
            *x += 32;
            break;
        }
        case P_KEY: {
            xpm_map_t p = (game->video_mode == INDEXED) ? (xpm_map_t) P_xpm_indexed : (xpm_map_t) P_xpm_direct;
            print_xpm_custom (p, *x, y, false, game->video_mode);
            *x += 32;
            break;
        }
        case Q_KEY: {
            xpm_map_t q = (game->video_mode == INDEXED) ? (xpm_map_t) Q_xpm_indexed : (xpm_map_t) Q_xpm_direct;
            print_xpm_custom (q, *x, y, false, game->video_mode);
            *x += 32;
            break;
        }
        case R_KEY: {
            xpm_map_t r = (game->video_mode == INDEXED) ? (xpm_map_t) R_xpm_indexed : (xpm_map_t) R_xpm_direct;
            print_xpm_custom (r, *x, y, false, game->video_mode);
            *x += 32;
            break;
        }
        case S_KEY: {
            xpm_map_t s = (game->video_mode == INDEXED) ? (xpm_map_t) S_xpm_indexed : (xpm_map_t) S_xpm_direct;
            print_xpm_custom (s, *x, y, false, game->video_mode);
            *x += 30;
            break;
        }
        case T_KEY: {
            xpm_map_t t = (game->video_mode == INDEXED) ? (xpm_map_t) T_xpm_indexed : (xpm_map_t) T_xpm_direct;
            print_xpm_custom (t, *x, y, false, game->video_mode);
            *x += 29;
            break;
        }
        case U_KEY: {
            xpm_map_t u = (game->video_mode == INDEXED) ? (xpm_map_t) U_xpm_indexed : (xpm_map_t) U_xpm_direct;
            print_xpm_custom (u, *x, y, false, game->video_mode);
            *x += 32;
            break;
        }
        case V_KEY: {
            xpm_map_t v = (game->video_mode == INDEXED) ? (xpm_map_t) V_xpm_indexed : (xpm_map_t) V_xpm_direct;
            print_xpm_custom (v, *x, y, false, game->video_mode);
            *x += 32;
            break;
        }
        case W_KEY: {
            xpm_map_t w = (game->video_mode == INDEXED) ? (xpm_map_t) W_xpm_indexed : (xpm_map_t) W_xpm_direct;
            print_xpm_custom (w, *x, y, false, game->video_mode);
            *x += 42;
            break;
        }
        case X_KEY: {
            xpm_map_t x_ = (game->video_mode == INDEXED) ? (xpm_map_t) X_xpm_indexed : (xpm_map_t) X_xpm_direct;
            print_xpm_custom (x_, *x, y, false, game->video_mode);
            *x += 32;
            break;
        }
        case Y_KEY: {
            xpm_map_t y_ = (game->video_mode == INDEXED) ? (xpm_map_t) Y_xpm_indexed : (xpm_map_t) Y_xpm_direct;
            print_xpm_custom (y_, *x, y, false, game->video_mode);
            *x += 32;
            break;
        }
        case Z_KEY: {
            xpm_map_t z = (game->video_mode == INDEXED) ? (xpm_map_t) Z_xpm_indexed : (xpm_map_t) Z_xpm_direct;
            print_xpm_custom (z, *x, y, false, game->video_mode);
            *x += 30;
            break;
        }
        default: break;
    }

    return 0;
}



int (draw_xpm_)(xpm_map_t xpm, int x, int y){
    
    xpm_image_t img;

    uint8_t *colors = xpm_load(xpm, XPM_8_8_8_8, &img);;
    
    uint16_t min_h = (img.height < v_res) ? img.height : v_res;
    uint16_t min_w = (img.width  < h_res) ? img.width : h_res;

    for (int h = 0; h < min_h; h++) {
        for (int w = 0; w < min_w; w++) {
            uint32_t color = 0;
            
            for (int i = 0; i < bytes_per_pixel; i++)
                color |= (*(colors + i) << (8 * i));
            
            draw_pixel_(x + w, y + h, color);
            
            colors += bytes_per_pixel;
        }
    }

    return 0;
}

int (draw_pixel_)(int x, int y, uint32_t color){

    if (x < 0 || y < 0 || x >= h_res || y >= v_res) return 0;

    uint32_t pos = ((y * h_res) + x) * bytes_per_pixel;

    if ((memcpy((vg_ptr + pos), &color, bytes_per_pixel)) == NULL) return 1;

    return 0;
}

