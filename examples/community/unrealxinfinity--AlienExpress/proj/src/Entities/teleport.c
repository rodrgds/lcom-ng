#include "teleport.h"

int reached = 0;


void draw_teleports(){
    for (int i = 0; i < (int) (sizeof(teleports) / sizeof(teleports[0])); i++){
        if(!teleports[i].is_dead){
            draw(teleports[i], teleports[i]);
            draw_img_number(teleports[i], teleports[i].x+15, teleports[i].y-10);
            if((counter_timer % 30) == 0)teleports[i].idle_time -= 1;
            if(teleports[i].idle_time <= 0){
                teleports[i].is_dead = true;
                teleports[i].is_in_inventory = true;
            }
        }
        if(teleports[i].is_in_inventory && teleports[i].idle_time > 0){
            score_animation(&teleports[i], 0);
        }
        if(teleports[i].is_in_inventory && teleports[i].idle_time <= 0){
            score_animation(&teleports[i], 1);
        }
    }
}

int score_animation(img_t *character, int type){
    int width = 890 - character->x;
    int height = 20 - character->y;

    int distance;
    int final_x;
    int final_y;
    if(abs(width) <= 20 && abs(height) <= 20) {
        if(type == 0){
            score += character->idle_time;
            reached = 20;
        }
        if(type == 1){
            score -= 30;
            reached = -20;
        }
        if(score < 0)score = 0;
        character->is_in_inventory = false;
        return 0;
    }
    else {
        distance = sqrt(width*width + height*height);
        final_x = (width*4)/distance;
        final_y = (height*4)/distance;
    }
    if(final_x > 0){
        character->x += 20;
    }
    else if(final_x < 0)character->x -= 20;

    if(final_y > 0){
        character->y += 20;
    }
    else if(final_y < 0)character->y -= 20;
    if(type == 1){
        character->idle_time = 30;
    }
    draw_img_number(*character, character->x, character->y);
    if(type == 1){
        character->idle_time = 0;
    }
    return 0; 
}

void pick_draw_type(){
    if(reached > 0){
        draw_score(1);
        reached--;
    }
    else if(reached <0){
        draw_score(4);
        reached++;
    }
    else draw_score(0);
}

void draw_score(int type){
    int score_store = score;
    int size = 0;
    if(score == 0)size = 1;
    while(score_store > 0){
        size++;
        score_store = score_store / 10;
    }
    score_store = score;
    int x = 875;
    while(size > 0){
        int number = score_store % 10;
        draw_number(number, x+(15*size), 20, type);
        score_store = score_store / 10;
        size--;
    }
}

