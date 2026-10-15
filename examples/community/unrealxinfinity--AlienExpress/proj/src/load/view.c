#include "view.h"

int is_occupied(img_t img, int distance){
    uint32_t colors_32 = 0;
    switch(img.direction){
        case UP:
            for(int j = 0; j < distance; j++){
                for(int i = img.hitbox_x-3; i <= img.hitbox_x+3; i++){
                    memcpy(&colors_32, &frame_buffer[(x_res*(img.y-(j+1))+ i) * bytes_per_pixel], bytes_per_pixel);
                    if(colors_32 == ENEMY && img.type != WORKER && img.type != WOLF){
                        if(img.type == BULLET){
                            hit_monster(i, img.y-j-1);
                        }
                        return 1;
                    }
                    else if(colors_32 == WORKER && img.type == WORKER){
                        return 1;
                    }
                }
            }
            break;
        case LEFT:
            for(int i = 0; i < distance; i++){
                for(int j = img.hitbox_y-3; j <= img.hitbox_y+3; j++){
                    memcpy(&colors_32, &frame_buffer[(x_res*(j)+img.x-(i+1)) * bytes_per_pixel], bytes_per_pixel);
                    if(colors_32 == ENEMY && img.type != WORKER && img.type != WOLF){
                        if(img.type == BULLET){
                            hit_monster(img.x-i-1, j);
                        }
                        return 1;
                    }
                    else if(colors_32 == WORKER && img.type == WORKER){
                        return 1;
                    }
                }
            }
            break;
        case RIGHT:
            for(int i = 0; i < distance; i++){
                for(int j = img.hitbox_y-3; j <= img.hitbox_y+3; j++){
                    memcpy(&colors_32, &frame_buffer[(x_res*(j)+(img.x+img.width+i+1)) * bytes_per_pixel], bytes_per_pixel);
                    if(colors_32 == ENEMY && img.type != WORKER && img.type != WOLF){
                        if(img.type == BULLET){
                            hit_monster(img.x+img.width+i+1, j);
                        }
                        return 1;
                    }
                    else if(colors_32 == WORKER && img.type == WORKER){
                        return 1;
                    }
                }
            }
            break;
        case DOWN:
            for(int j = 0; j < distance; j++){
                for(int i = img.hitbox_x-3; i <= img.hitbox_x+3; i++){
                    memcpy(&colors_32, &frame_buffer[(x_res*(img.y+img.height+j+1)+i) * bytes_per_pixel], bytes_per_pixel);
                    if(colors_32 == ENEMY && img.type != WORKER && img.type != WOLF){
                        if(img.type == BULLET){
                            hit_monster(i, img.y+img.height+j+1);
                        }
                        return 1;
                    }
                    else if(colors_32 == WORKER && img.type == WORKER){
                        return 1;
                    }
                }
            }
            break;
        default:
            break;
    }
    return 0;
}
int manage_collision(img_t *img, int distance){
    if(img->direction == STILL)return 0;
    uint32_t colors_32 = 0;
    switch(img->direction) {
        case UP:
            if((img->y-distance) < 0){
                if(img->type == BULLET){
                    img->is_dead = true;
                }
                img->y = 0;
                img->hitbox_y = img->y + (img->height/2);
                break;
            }
            for(int j = 0; j < distance; j++){
                for(int i = img->hitbox_x-3; i <= img->hitbox_x+3; i++){
                    memcpy(&colors_32, &prev_buffer[(x_res*(img->y-(j+1))+ i) * bytes_per_pixel], bytes_per_pixel);
                    if((colors_32 == UNBREAKABLE_WALL) ||  (colors_32 == ENEMY && img->type != WORKER && img->type != GRENADE && img->type != WOLF) || (colors_32 == WORKER && img->type == WORKER)){
                        if(img->type == BULLET){
                            if(colors_32 == ENEMY)hit_monster(i, img->y-j-1);
                            img->is_dead = true;
                        }
                        img->y -= j;
                        img->hitbox_y -= j;
                        return 0;
                    }
                    if(colors_32 == PISTOL_BULLET && img->type == ENEMY && img->type == WOLF){
                        img->is_dead = true;
                        break;
                    }
                    if(colors_32 == PLAYER && (img->type == ENEMY || img->type == WOLF)){
                        player_lives--;
                    }
                    if(img->type == BULLET){
                        if(hit_wolf(i, img->y-j-1))img->is_dead = true;
                    }
                }
            }
            img->y -= distance;
            img->hitbox_y -= distance;

            break;
        case LEFT:
            if((img->x-distance) < 0){
                if(img->type == BULLET){
                    img->is_dead = true;
                }
                img->x = 0;
                img->hitbox_x = (img->width/2);
                break;
            }
            for(int i = 0; i < distance; i++){
                for(int j = img->hitbox_y-3; j <= img->hitbox_y+3; j++){
                    memcpy(&colors_32, &prev_buffer[(x_res*(j)+img->x-(i+1)) * bytes_per_pixel], bytes_per_pixel);
                    if((colors_32 == UNBREAKABLE_WALL) || (colors_32 == ENEMY && img->type != WORKER && img->type != GRENADE && img->type != WOLF) || (colors_32 == WORKER && img->type == WORKER)){
                        if(img->type == BULLET){
                            if(colors_32 == ENEMY)hit_monster(img->x-i-1, j);
                            img->is_dead = true;
                        }
                        img->x -= i;
                        img->hitbox_x -= i;
                        return 0;
                    }
                    if(colors_32 == PISTOL_BULLET && img->type == ENEMY && img->type == WOLF){
                        img->is_dead = true;
                        break;
                    }
                    if(colors_32 == PLAYER && (img->type == ENEMY || img->type == WOLF)){
                        player_lives--;
                    }
                    if(img->type == BULLET){
                        if(hit_wolf(img->x-i-1, j))img->is_dead = true;
                    }
                    
                }
            }
            img->x -= distance;
            img->hitbox_x -= distance;
            break;
        case RIGHT:
            if((img->x+img->width+distance) > x_res){
                if(img->type == BULLET){
                    img->is_dead = true;
                }
                img->x = x_res-img->width;
                img->hitbox_x = img->x+(img->width/2);
                break;
            }
            for(int i = 0; i < distance; i++){
                for(int j = img->hitbox_y-3; j <= img->hitbox_y+3; j++){
                    memcpy(&colors_32, &prev_buffer[(x_res*(j)+(img->x+img->width+i+1)) * bytes_per_pixel], bytes_per_pixel);
                    if((colors_32 == UNBREAKABLE_WALL) || (colors_32 == ENEMY && img->type != WORKER && img->type != GRENADE && img->type != WOLF) || (colors_32 == WORKER && img->type == WORKER)){
                        if(img->type == BULLET){
                            if(colors_32 == ENEMY)hit_monster(img->x+img->width+i+1, j);
                            img->is_dead = true;
                        }
                        img->x += i;
                        img->hitbox_x += i;
                        return 0;
                    }
                    if(colors_32 == PISTOL_BULLET && img->type == ENEMY && img->type == WOLF){
                        img->is_dead = true;
                        break;
                    }
                    if(colors_32 == PLAYER && (img->type == ENEMY || img->type == WOLF)){
                        player_lives--;
                    }
                    if(img->type == BULLET){
                        if(hit_wolf(img->x+img->width+i+1, j))img->is_dead = true;
                    }
                }
            }
            img->x += distance;
            img->hitbox_x += distance;
            break;
        case DOWN:
            if((img->y + img->height+distance) > y_res){
                if(img->type == BULLET){
                    img->is_dead = true;
                }
                img->y = y_res-img->height;
                img->hitbox_y = img->y + (img->height/2);
                break;
            }
            for(int j = 0; j < distance; j++){
                for(int i = img->hitbox_x-3; i <= img->hitbox_x+3; i++){
                    memcpy(&colors_32, &prev_buffer[(x_res*(img->y+img->height+j+1)+i) * bytes_per_pixel], bytes_per_pixel);
                    if((colors_32 == UNBREAKABLE_WALL) || (colors_32 == ENEMY && img->type != WORKER && img->type != GRENADE && img->type != WOLF) || (colors_32 == WORKER && img->type == WORKER)){
                        if(img->type == BULLET){
                            if(colors_32 == ENEMY)hit_monster(i, img->y+img->height+j+1);
                            img->is_dead = true;
                        }
                        img->y += j;
                        img->hitbox_y += j;
                        return 0;
                    }
                    if(colors_32 == PISTOL_BULLET && img->type == ENEMY && img->type == WOLF){
                        img->is_dead = true;
                        break;
                    }
                    if(colors_32 == PLAYER && (img->type == ENEMY || img->type == WOLF)){
                        player_lives--;
                    }
                    if(img->type == BULLET){
                        if(hit_wolf(i, img->y+img->height+j+1)) img->is_dead = true;
                    }
                }
            }
            img->y += distance;
            img->hitbox_y += distance;
            break;
        default:
            break;
    }
    return 0;
}
void hit_monster(int x, int y){
        for(int i = 0; i < (int) (sizeof(enemies_lv1) / sizeof(enemies_lv1[0])); i++){
            if(!enemies_lv1[i].is_dead){
                if(((y<=(enemies_lv1[i].y+enemies_lv1[i].height)) && y >= enemies_lv1[i].y) && (x >= enemies_lv1[i].x && x <= (enemies_lv1[i].x+enemies_lv1[i].width))){
                    enemies_lv1[i].is_dead = true;
                    break;
                }
            }
        }
}
bool hit_wolf(int x, int y){
    if(((y<=(wolf.y+wolf.height)) && y >= wolf.y) && (x >= wolf.x && x <= (wolf.x+wolf.width)) && !wolf.is_dead){
        wolf.is_dead = true;
        return true;
    }
    else if(wolf.is_dead)return false;
    return false;
}
bool manage_hitbox(img_t target, img_t character){
    if(character.hitbox_x >= target.x && character.hitbox_x <= (target.x+target.width) && character.hitbox_y <= (target.y+target.height) && character.hitbox_y >= target.y){
        return true;
    }
    return false;
}
int draw(img_t draw_img, img_t img){
    for(int j = 0; j < draw_img.height; j++){
        for(int i = 0; i < draw_img.width; i++){
            int index = j*draw_img.width + i;
            if(draw_pixel(img.x+i, img.y+j, draw_img.colors_32[index])) return 1;
        }
    }
    return 0;
}

int draw_map(img_t draw_img, img_t img){
    for(int j = 0; j < draw_img.height; j++){
        for(int i = 0; i < draw_img.width; i++){
            int index = j*draw_img.width + i;
            if(draw_pixel_map(img.x+i, img.y+j, draw_img.colors_32[index])) return 1;
        }
    }
    return 0;
}
int draw_numbers(img_t number, int type){
    for(int j = 0; j < number.height; j++){
        for(int i = 0; i < number.width; i++){
            int index = j*number.width + i;
            if(draw_pixel_number(number.x+i, number.y+j, number.colors_32[index], type)) return 1;
        }
    }
    return 0;
}

int draw_small_xpm(xpm_map_t xpm, uint16_t x, uint16_t y){
    img_t img = make_img(xpm, x, y);
    if(draw(img, img)) return 1;
    return 0;
}

int drawTiles(){
    tilesPerAxis tilesXY;
    xpm_map_t image=(xpm_map_t) ground_xpm;
    img_t img = make_img(image,0,0);

    tilesXY.tilesPerXAxis=inner_border_right/img.width+1;
    tilesXY.tilesPerYAxis=y_res/img.height+1;
   for(int i=0 ; i<tilesXY.tilesPerYAxis;i++){
        for(int j=0; j<tilesXY.tilesPerXAxis;j++){
            img.x = j*img.width;
            img.y = i*img.height;
            if(draw_map(img,img))return 1;
        }
    }
    return 0;
}
int drawBorder(){
    tilesPerAxis tilesXY;
    xpm_map_t image=(xpm_map_t)unbreakableWalls_xpm;
    img_t img = make_img(image,0,0);
    tilesXY.tilesPerXAxis=x_res/img.width+1;
    tilesXY.tilesPerYAxis=y_res/img.height+1;
    for(int i=0 ; i<tilesXY.tilesPerYAxis;i++){
        img.y = i*img.height;
        if(draw_map(img,img)) return 1;
    }
    for(int i=0 ; i<tilesXY.tilesPerXAxis;i++){
        img.x = i*img.width;
        img.y = 0;
        if(draw_map(img,img)) return 1;
    }
    for(int i=0 ; i<tilesXY.tilesPerYAxis;i++){
        img.x = (x_res-img.width-200);
        img.y = i*img.height;
        if(draw_map(img,img)) return 1;
    }
    for(int i=0 ; i<tilesXY.tilesPerXAxis;i++){
        img.x = img.width*i;
        img.y = y_res - img.height;
        if(draw_map(img,img)) return 1;
    }

    inner_border_up=img.height;
    inner_border_down=y_res-img.height;
    inner_border_left=img.width;
    inner_border_right=tilesXY.tilesPerXAxis*img.width;
    outter_border_right=inner_border_right+img.width;
    return 0;

}
int draw_timer(int sec, int min){
    int x = 515;
    int number;
    int count = 1;
    while(count < 3){
        number = min % 10;
        min = min/10;
        draw_number(number, x, 20, 0);
        x -= 15;
        count++;
    }
    x = 560;
    count = 1;
    while(count < 3){
        number = sec % 10;
        sec = sec / 10;
        draw_number(number, x, 20, 0);
        x-= 15;
        count++;
    }
    return 0;
}
int draw_number(int number, int x, int y, int type){
    numbers[number].x = x;
    numbers[number].y = y;

    small_numbers[number].x = x;
    small_numbers[number].y = y;
    switch(number){
        case 0:
            if(type == 2)draw_numbers(small_numbers[number], type);
            else draw_numbers(numbers[number], type);
            break;
        case 1:
            if(type == 2)draw_numbers(small_numbers[number], type);
            else draw_numbers(numbers[number], type);
            break;
        case 2:
            if(type == 2)draw_numbers(small_numbers[number], type);
            else draw_numbers(numbers[number], type);
            break;
        case 3:
            if(type == 2)draw_numbers(small_numbers[number], type);
            else draw_numbers(numbers[number], type);
            break;
        case 4:
            if(type == 2)draw_numbers(small_numbers[number], type);
            else draw_numbers(numbers[number], type);
            break;
        case 5:
            if(type == 2)draw_numbers(small_numbers[number], type);
            else draw_numbers(numbers[number], type);
            break;
        case 6:
            if(type == 2)draw_numbers(small_numbers[number], type);
            else draw_numbers(numbers[number], type);
            break;
        case 7:
            if(type == 2)draw_numbers(small_numbers[number], type);
            else draw_numbers(numbers[number], type);
            break;
        case 8:
            if(type == 2)draw_numbers(small_numbers[number], type);
            else draw_numbers(numbers[number], type);
            break;
        case 9:
            if(type == 2)draw_numbers(small_numbers[number], type);
            else draw_numbers(numbers[number], type);
            break;
    }
    return 0;
}
int draw_health(){
    if(player_lives < -20)player_lives = -20;
    for(int i = 1; i <= max_health; i++){
        if(i > player_lives){
            draw_line(player.x + i-6, player.y -8, 4, EMPTY_HEALTH);
        }
        else{
            draw_line(player.x + i - 6, player.y -8, 4, HEALTH);
        }
    }
    return 0;
}
int draw_img_number(img_t img, int x, int y){
    int count = img.idle_time;
    if(count == 0){
        draw_number(0, x, y, 2);
    }
    else{
        while(count > 0){
            int number = count % 10;
            count = count/10;
            draw_number(number, x, y, 2);
            x -= 9;
        }
    }
    return 0;
}
int draw_line(int x, int y, int len, uint32_t colors){
    for(int i = 0; i < len; i++){
        if(draw_pixel(x, y+i, colors)) return 1;
    }
    return 0;
}
void set_char(img_t *img, int x, int y, int type){
    img->x = x;
    img->y = y;
    draw_numbers(*img, type);
}




void init_monsters(){
    img_t zombie = make_img((xpm_map_t) enemy3_xpm, 0, 0);
    for (int i =0; i<150; i++) {
        enemies_lv1[i] = zombie;
        enemies_lv1[i].direction = RIGHT;
        enemies_lv1[i].type = ENEMY;
    }
    xpm_map_t down1[8] = {(xpm_map_t)enemy1_xpm, (xpm_map_t)enemy2_xpm,(xpm_map_t)enemy3_xpm,(xpm_map_t)enemy4_xpm,(xpm_map_t)enemy5_xpm,(xpm_map_t)enemy6_xpm,(xpm_map_t)enemy7_xpm, (xpm_map_t)enemy8_xpm};
    xpm_map_t left1[8] = {(xpm_map_t)enemyLeft1_xpm, (xpm_map_t)enemyLeft2_xpm,(xpm_map_t)enemyLeft3_xpm,(xpm_map_t)enemyLeft4_xpm,(xpm_map_t)enemyLeft5_xpm,(xpm_map_t)enemyLeft6_xpm,(xpm_map_t)enemyLeft7_xpm, (xpm_map_t)enemyLeft8_xpm};
    xpm_map_t right1[8] = {(xpm_map_t)enemyRight1_xpm, (xpm_map_t)enemyRight2_xpm,(xpm_map_t)enemyRight3_xpm,(xpm_map_t)enemyRight4_xpm,(xpm_map_t)enemyRight5_xpm,(xpm_map_t)enemyRight6_xpm,(xpm_map_t)enemyRight7_xpm, (xpm_map_t)enemyRight8_xpm};
    xpm_map_t up1[8] = {(xpm_map_t)enemyUp1_xpm, (xpm_map_t)enemyUp2_xpm,(xpm_map_t)enemyUp3_xpm,(xpm_map_t)enemyUp4_xpm,(xpm_map_t)enemyUp5_xpm,(xpm_map_t)enemyUp6_xpm,(xpm_map_t)enemyUp7_xpm, (xpm_map_t)enemyUp8_xpm};
    animated_img_enemy1 = make_animated_img(8, 2, up1, left1, right1, down1);
    zombie = make_img((xpm_map_t) idle2_xpm, 0, 0);
    for(int i = 0; i<5; i++){
        workers[i] = zombie;
        workers[i].direction = RIGHT;
        workers[i].type = WORKER;
    }
    xpm_map_t down[8] = {(xpm_map_t)idle2_xpm, (xpm_map_t)right1_xpm,(xpm_map_t)right2_xpm,(xpm_map_t)right3_xpm,(xpm_map_t)right4_xpm,(xpm_map_t)right5_xpm,(xpm_map_t)idle2_xpm, (xpm_map_t)right1_xpm};
    xpm_map_t left[8] = {(xpm_map_t)idle2_xpm, (xpm_map_t)right1_xpm,(xpm_map_t)right2_xpm,(xpm_map_t)right3_xpm,(xpm_map_t)right4_xpm,(xpm_map_t)right5_xpm,(xpm_map_t)idle2_xpm, (xpm_map_t)right1_xpm};
    xpm_map_t right[8] = {(xpm_map_t)idle2_xpm, (xpm_map_t)right1_xpm,(xpm_map_t)right2_xpm,(xpm_map_t)right3_xpm,(xpm_map_t)right4_xpm,(xpm_map_t)right5_xpm,(xpm_map_t)idle2_xpm, (xpm_map_t)right1_xpm};
    xpm_map_t up[8] = {(xpm_map_t)idle2_xpm, (xpm_map_t)right1_xpm,(xpm_map_t)right2_xpm,(xpm_map_t)right3_xpm,(xpm_map_t)right4_xpm,(xpm_map_t)right5_xpm,(xpm_map_t)idle2_xpm, (xpm_map_t)right1_xpm};
    animated_img_workers = make_animated_img(8, 5, up, left, right, down);
    
    xpm_map_t wolf_friendly_up[8]={(xpm_map_t)wolfNormalBack1_xpm,(xpm_map_t)wolfNormalBack2_xpm,(xpm_map_t)wolfNormalBack3_xpm,(xpm_map_t)wolfNormalBack2_xpm,(xpm_map_t)wolfNormalBack1_xpm,(xpm_map_t)wolfNormalBack2_xpm,(xpm_map_t)wolfNormalBack3_xpm,(xpm_map_t)wolfNormalBack2_xpm};
    xpm_map_t wolf_friendly_down[8]={(xpm_map_t)wolfnormal1_xpm,(xpm_map_t)wolfnormal2_xpm,(xpm_map_t)wolfnormal3_xpm,(xpm_map_t)wolfnormal2_xpm,(xpm_map_t)wolfnormal1_xpm,(xpm_map_t)wolfnormal2_xpm,(xpm_map_t)wolfnormal3_xpm,(xpm_map_t)wolfnormal2_xpm};
    xpm_map_t wolf_friendly_left[8]={(xpm_map_t)wolfNormalLeft1_xpm,(xpm_map_t)wolfNormalLeft2_xpm,(xpm_map_t)wolfNormalLeft3_xpm,(xpm_map_t)wolfNormalLeft2_xpm,(xpm_map_t)wolfNormalLeft1_xpm,(xpm_map_t)wolfNormalLeft2_xpm,(xpm_map_t)wolfNormalLeft3_xpm,(xpm_map_t)wolfNormalLeft2_xpm};
    xpm_map_t wolf_friendly_right[8]={(xpm_map_t)wolfNormalRight1_xpm,(xpm_map_t)wolfNormalRight2_xpm,(xpm_map_t)wolfNormalRight3_xpm,(xpm_map_t)wolfNormalRight2_xpm,(xpm_map_t)wolfNormalRight1_xpm,(xpm_map_t)wolfNormalRight2_xpm,(xpm_map_t)wolfNormalRight3_xpm,(xpm_map_t)wolfNormalRight2_xpm};
    
    xpm_map_t wolf_up[8]={(xpm_map_t)wolfBack1_xpm,(xpm_map_t)wolfBack2_xpm,(xpm_map_t)wolfBack3_xpm,(xpm_map_t)wolfBack2_xpm,(xpm_map_t)wolfBack1_xpm,(xpm_map_t)wolfBack2_xpm,(xpm_map_t)wolfBack3_xpm,(xpm_map_t)wolfBack2_xpm};
    xpm_map_t wolf_down[8]={(xpm_map_t)wolfAngry1_xpm,(xpm_map_t)wolfAngry2_xpm,(xpm_map_t)wolfAngry3_xpm,(xpm_map_t)wolfAngry2_xpm,(xpm_map_t)wolfAngry1_xpm,(xpm_map_t)wolfAngry2_xpm,(xpm_map_t)wolfAngry3_xpm,(xpm_map_t)wolfAngry2_xpm};
    xpm_map_t wolf_left[8]={(xpm_map_t)wolfAngryLeft1_xpm,(xpm_map_t)wolfAngryLeft2_xpm,(xpm_map_t)wolfAngryLeft3_xpm,(xpm_map_t)wolfAngryLeft2_xpm,(xpm_map_t)wolfAngryLeft1_xpm,(xpm_map_t)wolfAngryLeft2_xpm,(xpm_map_t)wolfAngryLeft3_xpm,(xpm_map_t)wolfAngryLeft2_xpm};
    xpm_map_t wolf_right[8]={(xpm_map_t)wolfAngryRight1_xpm,(xpm_map_t)wolfAngryRight2_xpm,(xpm_map_t)wolfAngryRight3_xpm,(xpm_map_t)wolfAngryRight2_xpm,(xpm_map_t)wolfAngryRight1_xpm,(xpm_map_t)wolfAngryRight2_xpm,(xpm_map_t)wolfAngryRight3_xpm,(xpm_map_t)wolfAngryRight2_xpm};
    
    animated_img_friendly_wolf = make_animated_img(8,3,wolf_friendly_up,wolf_friendly_left,wolf_friendly_right,wolf_friendly_down);
    animated_img_wolf = make_animated_img(8,3, wolf_up,wolf_left,wolf_right,wolf_down);
    zombie = make_img((xpm_map_t)wolfnormal2_xpm,0,0);
    wolf= zombie;
    wolf.direction = RIGHT;
    wolf.type = WOLF;
}
void init_packages(){
    img_t zombie = make_img((xpm_map_t)small_box_xpm, 0, 0);
    for (int i =0; i<6; i++) {
        packages[i] = zombie;
        packages[i].type = PACKAGE;
    }
}
void init_teleports(){
    img_t zombie = make_img((xpm_map_t)teleport_xpm, 0, 0);
    for (int i =0; i<3; i++) {
        teleports[i] = zombie;
    }
}
void init_inventory(){
    inventory_box = make_img((xpm_map_t)inventory_xpm, 0, 0);
     for(int j = 0; j < 3; j++){
        for(int i = 0; i < 3; i++){
            inventory[j*3+i] = make_img((xpm_map_t)small_box_xpm, 10+(i*36), 10+(j*36));
            inventory[j*3+i].type = PACKAGE;
        }
    }
    for(int i = 0; i < 3; i++){
        equipment[i] = make_img((xpm_map_t)small_box_xpm, 166, 5+(i*36));
        equipment[i].type = PACKAGE;
    }


    selected = make_img((xpm_map_t)selected_xpm, 0, 0);
}
void init_items(){
    img_t zombie_pistol = make_img((xpm_map_t)pistol32_xpm, 0, 0);
    img_t zombie_ammo = make_img((xpm_map_t)bullet32_xpm, 0, 0);
    img_t zombie_pizza = make_img((xpm_map_t)pizza32_xpm, 0, 0);
    img_t zombie_grenade = make_img((xpm_map_t)granadeScifi_xpm, 0, 0);
    for(int i = 0; i < 3; i++){
        pistols[i] = zombie_pistol;
        pistols[i].type = PISTOL;
        ammo[i] = zombie_ammo;
        ammo[i].type = AMMO;
        ammo[i].idle_time = 8;
        pizza[i] = zombie_pizza;
        pizza[i].type = PIZZA;
        pizza[i].idle_time = 20;
        grenades[i] = zombie_grenade;
        grenades[i].type = GRENADE;
        grenades[i].idle_time = 0;
        grenades[i].direction = STILL;
    }
}
void init_bullets(){
    img_t zombie = make_img((xpm_map_t)shoot2_xpm, 0, 0);
    for(int i = 0; i < 50; i++){
        bullets[i] = zombie;
        bullets[i].type = BULLET;
    }
}
void init_chars(){
    a = make_img((xpm_map_t)a_xpm, 0, 0);
    b = make_img((xpm_map_t)b_xpm, 0, 0);
    c = make_img((xpm_map_t)c_xpm, 0, 0);
    d = make_img((xpm_map_t)d_xpm, 0, 0);
    e = make_img((xpm_map_t)e_xpm, 0, 0);
    f = make_img((xpm_map_t)f_xpm, 0, 0);
    g = make_img((xpm_map_t)g_xpm, 0, 0);
    h = make_img((xpm_map_t)h_xpm, 0, 0);
    i = make_img((xpm_map_t)i_xpm, 0, 0);
    j = make_img((xpm_map_t)j_xpm, 0, 0);
    k = make_img((xpm_map_t)k_xpm, 0, 0);
    l = make_img((xpm_map_t)l_xpm, 0, 0);
    m = make_img((xpm_map_t)m_xpm, 0, 0);
    n = make_img((xpm_map_t)n_xpm, 0, 0);
    o = make_img((xpm_map_t)o_xpm, 0, 0);
    p = make_img((xpm_map_t)p_xpm, 0, 0);
    q = make_img((xpm_map_t)q_xpm, 0, 0);
    r = make_img((xpm_map_t)r_xpm, 0, 0);
    s = make_img((xpm_map_t)s_xpm, 0, 0);
    t = make_img((xpm_map_t)t_xpm, 0, 0);
    u = make_img((xpm_map_t)u_xpm, 0, 0);
    v = make_img((xpm_map_t)v_xpm, 0, 0);
    w = make_img((xpm_map_t)w_xpm, 0, 0);
    x = make_img((xpm_map_t)x_xpm, 0, 0);
    y = make_img((xpm_map_t)y_xpm, 0, 0);
    z = make_img((xpm_map_t)z_xpm, 0, 0);

    two_dots = make_img((xpm_map_t)two_dots_xpm, 875, 20);
}
void init_numbers(){
    numbers[0] =make_img((xpm_map_t)zero_xpm, 0, 0);
    numbers[1] =make_img((xpm_map_t)one_xpm, 0, 0);
    numbers[2] =make_img((xpm_map_t)two_xpm, 0, 0);
    numbers[3] =make_img((xpm_map_t)three_xpm, 0, 0);
    numbers[4] =make_img((xpm_map_t)four_xpm, 0, 0);
    numbers[5] =make_img((xpm_map_t)five_xpm, 0, 0);
    numbers[6] =make_img((xpm_map_t)six_xpm, 0, 0);
    numbers[7] =make_img((xpm_map_t)seven_xpm, 0, 0);
    numbers[8] =make_img((xpm_map_t)eight_xpm, 0, 0);
    numbers[9] =make_img((xpm_map_t)nine_xpm, 0, 0);

    small_numbers[0] =make_img((xpm_map_t)small_zero_xpm, 0, 0);
    small_numbers[1] =make_img((xpm_map_t)small_one_xpm, 0, 0);
    small_numbers[2] =make_img((xpm_map_t)small_two_xpm, 0, 0);
    small_numbers[3] =make_img((xpm_map_t)small_three_xpm, 0, 0);
    small_numbers[4] =make_img((xpm_map_t)small_four_xpm, 0, 0);
    small_numbers[5] =make_img((xpm_map_t)small_five_xpm, 0, 0);
    small_numbers[6] =make_img((xpm_map_t)small_six_xpm, 0, 0);
    small_numbers[7] =make_img((xpm_map_t)small_seven_xpm, 0, 0);
    small_numbers[8] =make_img((xpm_map_t)small_eight_xpm, 0, 0);
    small_numbers[9] =make_img((xpm_map_t)small_nine_xpm, 0, 0);
}

void init_player(){
    player = make_img((xpm_map_t) mc1_xpm,214,100);
    player.prev_direction = RIGHT;
    player_lives = max_health;
    player.idle_time = 1;
    info_type = PLAYER;
}
void init_misc(){
    watch = make_img((xpm_map_t)clock32_xpm, 0, 0);
    boots = make_img((xpm_map_t)speedBoots_xpm, 0, 0);
    backpack = make_img((xpm_map_t)backpack_xpm, 0, 0);
    bone = make_img((xpm_map_t)uraniumStick_xpm, 0, 0);
    xpm_map_t explosion_animated[8] = {(xpm_map_t)explosion1_1_xpm,(xpm_map_t)explosion1_2_xpm,(xpm_map_t)explosion1_3_xpm,(xpm_map_t)explosion1_4_xpm,(xpm_map_t)explosion1_5_xpm,(xpm_map_t)explosion1_6_xpm,(xpm_map_t)explosion1_7_xpm,(xpm_map_t)explosion1_8_xpm};
    explosion=make_img((xpm_map_t)explosion1_1_xpm,0,0);
    for(int i=0;i<3;i++){
        explosions[i]=explosion;
    }
    explosion_effect= make_simple_animation(8,explosion_animated);
}



void initialize_game(){
    inventory_size = 1;
    init_player();
    init_numbers();
    init_monsters();
    init_packages();
    init_teleports();
    init_inventory();
    init_inventory();
    init_items();
    init_bullets();
    init_chars();
    init_misc();
}
void init_level(){
    seconds = 0;
    minutes = 0;
    score = 0;
    counter_timer = 0;
    inventory_size = 1;
    ammo_size = 3;
    pistol_size = 1;
    food_size = 3;
    max_health = 40;
    grenade_size = 3;
    count = 0;

    player.x = 214;
    player.hitbox_x = player.x + (player.width/2);
    player.y = 100;
    player.hitbox_y = player.y + (player.height/2);
    player_lives = max_health;
    player.idle_time = 1;

}
void eliminate_entities(){
    for (int i = 0; i < (int) (sizeof(enemies_lv1) / sizeof(enemies_lv1[0])); i++){
        if(!enemies_lv1[i].is_dead){
            enemies_lv1[i].is_dead = true;
        }
    }
    for(int i = 0; i < (int) (sizeof(workers) / sizeof(workers[0])); i++){
        if(!workers[i].is_dead){
            workers[i].is_dead = true;
        }
    }

    wolf.is_dead = true;
    wolf.is_in_inventory = false;

    for(int i = 0; i < (int) (sizeof(teleports) / sizeof(teleports[0])); i++){
        teleports[i].is_dead = true;
        teleports[i].is_in_inventory = false;
    }
    for(int i = 0; i < (int) (sizeof(packages) / sizeof(packages[0])); i++){
        packages[i].is_dead = true;
        packages[i].is_in_inventory = false;
    }
    for(int i = 0; i < 9; i++){
        inventory[i].is_in_inventory = false;
        inventory[i].is_dead = true;
    }
    for(int i = 0; i < (int) (sizeof(pistols) / sizeof(pistols[0])); i++){
        pistols[i].is_dead = true;
        pistols[i].is_in_inventory = false;
        ammo[i].is_dead = true;
        ammo[i].is_in_inventory = false;
        pizza[i].is_dead = true;
        pizza[i].is_in_inventory = false;
        grenades[i].is_dead = true;
        grenades[i].is_in_inventory = false;
    }
    for(int i = 0; i < (int) (sizeof(bullets) / sizeof(bullets[0])); i++){
        bullets[i].is_dead = true;
        bullets[i].is_in_inventory = false;
    }
    for(int i = 0; i < (int) (sizeof(equipment) / sizeof(equipment[0])); i++){
        equipment[i].is_dead = true;
        equipment[i].is_in_inventory = false;
    }
    for(int i=0 ; i< (int) (sizeof(explosions)/sizeof(explosion));i++){
        explosions[i].is_dead=true;
        explosions[i].is_in_inventory=false;
    }
    watch.is_dead = true;
    watch.is_in_inventory = false;
    boots.is_dead = true;
    boots.is_in_inventory = false;
    backpack.is_dead = true;
    backpack.is_in_inventory = false;
    bone.is_dead = true;
    bone.is_in_inventory = false;
    
}
void init_mouse(){
    mouse_hover = false;
    mouse_click = false;
    mouse_x = -20;
    mouse_y = -20;
    mouse = make_img((xpm_map_t)mouse_normal_xpm, mouse_x, mouse_y);  
    xpm_map_t mouse_movement[2] = {(xpm_map_t)mouse_normal_xpm, (xpm_map_t)mouse_selected_xpm};
    mouse_animation = make_simple_animation(2, mouse_movement);  
}
void init_anim_img(){
    //player
    xpm_map_t up_pistol[8] = {(xpm_map_t)mcUp1_xpm, (xpm_map_t)mcUp2_xpm,(xpm_map_t)mcUp3_xpm,(xpm_map_t)mcUp4_xpm,(xpm_map_t)mcUp5_xpm,(xpm_map_t)mcUp6_xpm,(xpm_map_t)mcUp7_xpm, (xpm_map_t)mcUp8_xpm};
    xpm_map_t left_pistol[8] = {(xpm_map_t)mcLeft1_xpm, (xpm_map_t)mcLeft2_xpm,(xpm_map_t)mcLeft3_xpm,(xpm_map_t)mcLeft4_xpm,(xpm_map_t)mcLeft5_xpm,(xpm_map_t)mcLeft6_xpm,(xpm_map_t)mcLeft7_xpm, (xpm_map_t)mcLeft8_xpm};
    xpm_map_t right_pistol[8] = {(xpm_map_t)mcRight1_xpm, (xpm_map_t)mcRight2_xpm,(xpm_map_t)mcRight3_xpm,(xpm_map_t)mcRight4_xpm,(xpm_map_t)mcRight5_xpm,(xpm_map_t)mcRight6_xpm,(xpm_map_t)mcRight7_xpm, (xpm_map_t)mcRight8_xpm};
    xpm_map_t down_pistol[8] = {(xpm_map_t)mcDown1_xpm, (xpm_map_t)mcDown2_xpm,(xpm_map_t)mcDown3_xpm,(xpm_map_t)mcDown4_xpm,(xpm_map_t)mcDown5_xpm,(xpm_map_t)mcDown6_xpm,(xpm_map_t)mcDown7_xpm, (xpm_map_t)mcDown8_xpm};

    xpm_map_t up[8] = {(xpm_map_t)mcUpPistol1_xpm, (xpm_map_t)mcUpPistol2_xpm,(xpm_map_t)mcUpPistol3_xpm,(xpm_map_t)mcUpPistol4_xpm,(xpm_map_t)mcUpPistol5_xpm,(xpm_map_t)mcUpPistol6_xpm,(xpm_map_t)mcUpPistol7_xpm, (xpm_map_t)mcUpPistol8_xpm};
    xpm_map_t right[8] = {(xpm_map_t)mcRightPistol1_xpm, (xpm_map_t)mcRightPistol2_xpm,(xpm_map_t)mcRightPistol3_xpm,(xpm_map_t)mcRightPistol4_xpm,(xpm_map_t)mcRightPistol5_xpm,(xpm_map_t)mcRightPistol6_xpm,(xpm_map_t)mcRightPistol7_xpm, (xpm_map_t)mcRightPistol8_xpm};
    xpm_map_t left[8] = {(xpm_map_t)mcLeftPistol1_xpm, (xpm_map_t)mcLeftPistol2_xpm,(xpm_map_t)mcLeftPistol3_xpm,(xpm_map_t)mcLeftPistol4_xpm,(xpm_map_t)mcLeftPistol5_xpm,(xpm_map_t)mcLeftPistol6_xpm,(xpm_map_t)mcLeftPistol7_xpm, (xpm_map_t)mcLeftPistol8_xpm};
    xpm_map_t down[8] = {(xpm_map_t)mcDownPistol1_xpm, (xpm_map_t)mcDownPistol2_xpm,(xpm_map_t)mcDownPistol3_xpm,(xpm_map_t)mcDownPistol4_xpm,(xpm_map_t)mcDownPistol5_xpm,(xpm_map_t)mcDownPistol6_xpm,(xpm_map_t)mcDownPistol7_xpm, (xpm_map_t)mcDownPistol8_xpm};
    animated_img_player_pistol= make_animated_img(8, 4, up_pistol, left_pistol, right_pistol, down_pistol);
    animated_img_player_bear_hands= make_animated_img(8, 4, up, left, right, down);
    animated_img_player = animated_img_player_bear_hands;
    
}


img_t make_img(xpm_map_t xpm, unsigned int x, unsigned int y){
    xpm_image_t xpm_img;
    img_t img;
    
        img.colors_32 = (uint32_t *) xpm_load(xpm, XPM_8_8_8_8, &xpm_img);
        img.width = xpm_img.width;
        img.height = xpm_img.height;
        img.x = x;
        img.y = y;
        img.no_img = 0;
        img.direction = STILL;
        img.hitbox_x = x + (img.width/2);
        img.hitbox_y = y + (img.height/2);
        img.is_dead = true;
        img.is_in_inventory = false;
        img.idle_time = 0;

    return img;
}

AnimatedImg_t make_animated_img(int no_images, int speed, xpm_map_t *up, xpm_map_t *left, xpm_map_t *right, xpm_map_t *down){
    AnimatedImg_t anime_img;
    for(int i=0;i<no_images;i++){
        anime_img.up[i] = make_img(up[i],0,0);
        anime_img.left[i] = make_img(left[i],0,0);
        anime_img.right[i] = make_img(right[i],0,0);
        anime_img.down[i] = make_img(down[i],0,0);

    }
    anime_img.speed = speed;
    return anime_img;
}
simple_animation_t make_simple_animation(int no_images, xpm_map_t *animation){
    simple_animation_t simple_animation;
    for(int i = 0; i < no_images; i++){
        simple_animation.animation[i] = make_img(animation[i], 0, 0);
    }
    return simple_animation;
}

