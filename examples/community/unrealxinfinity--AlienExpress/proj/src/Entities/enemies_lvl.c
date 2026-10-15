#include "enemies_lvl.h"


void move_enemies_lvl(){
    for (int i = 0; i < (int) (sizeof(enemies_lv1) / sizeof(enemies_lv1[0])); i++){
        if(enemies_lv1[i].is_dead)continue;
        int width = player.hitbox_x - enemies_lv1[i].hitbox_x;
        int height = player.hitbox_y - enemies_lv1[i].hitbox_y;

        int distance;
        int final_x;
        int final_y;
        if(abs(width) <= animated_img_enemy1.speed && abs(height) <= animated_img_enemy1.speed) {
            distance = 0;
            final_x = 0;
            final_y = 0;
            player_lives -=2;
        }
        else {
            distance = sqrt(width*width + height*height);
            final_x = (width*animated_img_enemy1.speed)/distance;
            final_y = (height*animated_img_enemy1.speed)/distance;
        }

        if(final_x > 0) enemies_lv1[i].direction = RIGHT;
        else if(final_x < 0) {
            enemies_lv1[i].direction = LEFT;
            final_x = abs(final_x);
        }
        if(counter_timer % 5 == 0)enemies_lv1[i].no_img += 1;
        if(!is_occupied(enemies_lv1[i], final_x)){
            manage_collision(&enemies_lv1[i], final_x);
        }


        if(final_y>0)enemies_lv1[i].direction = DOWN;
        else if(final_y <0) {
            enemies_lv1[i].direction = UP;
            final_y = abs(final_y);
        }
        if(!is_occupied(enemies_lv1[i], final_y)){
            manage_collision(&enemies_lv1[i], final_y);
        }
        draw_enemies_lvl(enemies_lv1[i]);

    }
}


void draw_enemies_lvl(img_t enemy){
        switch (enemy.direction) {
            case UP:
                draw(animated_img_enemy1.up[enemy.no_img % 8], enemy);
                break;
            case DOWN:
                draw(animated_img_enemy1.down[enemy.no_img % 8], enemy);
                break;
            case LEFT:
                draw(animated_img_enemy1.left[enemy.no_img % 8], enemy);
                break;
            case RIGHT:
                draw(animated_img_enemy1.right[enemy.no_img % 8], enemy);
                break;
            default:
                break;
        }
}

