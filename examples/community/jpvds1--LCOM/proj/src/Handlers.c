#include "Handlers.h"

extern int current_enemy_sprite;
extern struct EnemyNode *EnemyFirst; //first node of the list
extern struct LaserNode *LaserFirst; //first node of the list
extern struct InputNode *InputFirst;
extern bool right;
extern bool left;
extern bool up;
extern bool down;
extern int shootfrequency;
extern int movementmode;

extern clock_fields_struct info_rtc;

int (draw_laser)(uint16_t x, uint16_t y, int user)
{
    if(!user)
    {
        switch (current_ebullet_sprite)
        {
        case 0:
            if(draw_xpm(eb1_sprite, eb1_img, x, y)) return 1;
            break;
        case 1:
            if(draw_xpm(eb2_sprite, eb2_img, x, y)) return 1;
            break;
        case 2:
            if(draw_xpm(eb3_sprite, eb3_img, x, y)) return 1;
            break;
        default:
            if(draw_xpm(eb1_sprite, eb1_img, x, y)) return 1;
            break;
        }
    }
    else
    {
        if (vg_draw_rectangle(x, y, 4, 20, color_laser)) return 1;
    }

    return 0;
}

int (draw_number)(uint8_t number, uint16_t x, uint16_t y) {
            switch (number)
        {
        case 0:
            if(draw_xpm(zero_sprite, zero_img, x, 15)) return 1;
            break;
        case 1:
            if(draw_xpm(one_sprite, one_img, x, 15)) return 1;
            break;
        case 2:
            if(draw_xpm(two_sprite, two_img, x, 15)) return 1;
            break;
        case 3:
            if(draw_xpm(three_sprite, three_img, x, 15)) return 1;
            break;
        case 4:
            if(draw_xpm(four_sprite, four_img, x, 15)) return 1;
            break;
        case 5:
            if(draw_xpm(five_sprite, five_img, x, 15)) return 1;
            break;
        case 6:
            if(draw_xpm(six_sprite, six_img, x, 15)) return 1;
            break;
        case 7:
            if(draw_xpm(seven_sprite, seven_img, x, 15)) return 1;
            break;
        case 8:
            if(draw_xpm(eight_sprite, eight_img, x, 15)) return 1;
            break;
        case 9:
            if(draw_xpm(nine_sprite, nine_img, x, 15)) return 1;
            break;
        default:
            return 1;
            break;
        }
        return 0;
}

int (draw_score)()
{
    uint16_t rest;
    uint16_t score2 = score;
    uint16_t drawx = mode_inf.XResolution - 200;
    while(score2 != 0)
    {
        rest = score2 % 10;
        score2 = score2 / 10;
        if(draw_number(rest, drawx, 15)) return 1;
        drawx -= 25;
    }
    return 0;
}


int draw_time() {
    uint16_t draw_x = mode_inf.XResolution - 600;
    uint16_t current;

    uint8_t hours = info_rtc.h;
    uint8_t minutes = info_rtc.min;
    uint8_t seconds = info_rtc.sec;
    uint8_t day = info_rtc.d;
    uint8_t month = info_rtc.m;
    uint8_t year = info_rtc.y;

    while (year != 0) {
        current = year % 10;
        year = year / 10;
        if(draw_number(current, draw_x, 15)) return 1;
        draw_x -= 25;
    }
    if(draw_xpm(back_slash_sprite, back_slash_img, draw_x, 15)) return 1;
    draw_x -= 25;
    if(month < 10)
    {
        if(draw_number(month, draw_x, 15)) return 1;
        draw_x -= 25;
        if(draw_number(0, draw_x, 15)) return 1;
        draw_x -= 25;
    }
    else
    {
        if(draw_number(month % 10, draw_x, 15)) return 1;
        draw_x -= 25;
        if(draw_number(month / 10, draw_x, 15)) return 1;
        draw_x -= 25;
    }
    if(draw_xpm(back_slash_sprite, back_slash_img, draw_x, 15)) return 1;
    draw_x -= 25;
    if(day < 10)
    {
        if(draw_number(day, draw_x, 15)) return 1;
        draw_x -= 25;
        if(draw_number(0, draw_x, 15)) return 1;
        draw_x -= 25;
    }
    else
    {
        if(draw_number(day % 10, draw_x, 15)) return 1;
        draw_x -= 25;
        if(draw_number(day / 10, draw_x, 15)) return 1;
        draw_x -= 25;
    }
    draw_x -= 50;
    if(seconds < 10)
    {
        if(draw_number(seconds, draw_x, 15)) return 1;
        draw_x -= 25;
        if(draw_number(0, draw_x, 15)) return 1;
        draw_x -= 25;
    }
    else
    {
        if(draw_number(seconds % 10, draw_x, 15)) return 1;
        draw_x -= 25;
        if(draw_number(seconds / 10, draw_x, 15)) return 1;
        draw_x -= 25;
    }
    if(draw_xpm(hifen_sprite, hifen_img, draw_x, 15)) return 1;
    draw_x -= 25;
    if(minutes < 10)
    {
        if(draw_number(minutes, draw_x, 15)) return 1;
        draw_x -= 25;
        if(draw_number(0, draw_x, 15)) return 1;
        draw_x -= 25;
    }
    else
    {
        if(draw_number(minutes % 10, draw_x, 15)) return 1;
        draw_x -= 25;
        if(draw_number(minutes / 10, draw_x, 15)) return 1;
        draw_x -= 25;
    }
    if(draw_xpm(hifen_sprite, hifen_img, draw_x, 15)) return 1;
    draw_x -= 25;
    if(hours < 10)
    {
        if(draw_number(hours, draw_x, 15)) return 1;
        draw_x -= 25;
        if(draw_number(0, draw_x, 15)) return 1;
        draw_x -= 25;
    }
    else
    {
        if(draw_number(hours % 10, draw_x, 15)) return 1;
        draw_x -= 25;
        if(draw_number(hours / 10, draw_x, 15)) return 1;
        draw_x -= 25;
    }

    return 0;
}

int (handle_top_bar)()
{
    uint16_t drawx = 10;
    for(int i = 0; i < 3; i++)
    {
        if(i < lives)
        {
            if(draw_xpm(vida_sprite, vida_img, drawx, 10)) return 1;
        }
        else
        {
            if(draw_xpm(sem_vida_sprite, sem_vida_img, drawx, 10)) return 1;
        }
        drawx += 50;
    }
    if(missile_ready())
    {
        if(draw_xpm(nuke_sprite, nuke_img, drawx, 10)) return 1;
    }
    draw_score();
    draw_time();
    return 0;
}

//Handles what a laser does on each timer interrupt
int handle_laser()
{
    struct LaserNode *next_laser = ll_t->next;
    if (ll_t->laser->user) {
        ll_t->laser->y -= 20;
    }
    else {
        ll_t->laser->y += 20;
    }
    if(ll_t->laser->y < mode_inf.YResolution - 20)
    {
        draw_laser(ll_t->laser->x, ll_t->laser->y, ll_t->laser->user);
        el_t = EnemyFirst;
        while(el_t != NULL) //check for collisions
        {
            if((ll_t->laser->x > (el_t->enemy->x - 5) && ll_t->laser->x < (el_t->enemy->x + 55) && ll_t->laser->y < (el_t->enemy->y + 30) && ll_t->laser->y > (el_t->enemy->y - 20)) && ll_t->laser->user)
            {
                al_addNode(al_getNewNode(el_t->enemy->x, el_t->enemy->y));
                el_removeNode(el_t);
                ll_removeNode(ll_t);
                score += 20;
                break;
            }
            else if (ll_t->laser->x > (x - 4) && ll_t->laser->x < (x + 164) && ll_t->laser->y > (y - 20) && ll_t->laser->y < (y + 120))
            {
                lives--;
                if(lives == 0)
                {
                    return 1;
                }
                else
                {
                    ll_removeNode(ll_t);
                    break;
                }
            }
            el_t = el_t->next;
        }
        ll_t = next_laser;
    }
    else //if laser is out of bounds remove it and clean its position
    {
        ll_removeNode(ll_t);
        ll_t = next_laser;
    }
    //printList(LaserFirst);
    //printf("\n");
    return 0;
}

void enemy_mov_1()
{
    if(el_t->enemy->x <= el_t->enemy->min_x) //changes the enemy direction if a wall is hit
    {
        el_t->enemy->direction = 0;
    }
    else if(el_t->enemy->x + 6 + 50 > el_t->enemy->max_x) //changes the enemy direction if a wall is hit
    {
        el_t->enemy->direction = 2;
    }
    if(el_t->enemy->direction == 0) //if the enemy goes to the right
    {
        if (el_t->enemy->x + 6 + 50 > mode_inf.XResolution) {el_t->enemy->x = mode_inf.XResolution - 50;}
        else {el_t->enemy->x += 6;}
    }
    else if(el_t->enemy->direction == 2) //if the enemy goes to the left
    {
        if (el_t->enemy->x - 6 < 0) {el_t->enemy->x = 0;}
        else {el_t->enemy->x -= 6;}
    }
    else
    {
        el_t->enemy->direction = 0;
    }
}

void enemy_mov_2()
{
    if(el_t->enemy->direction == 0)
    {
        el_t->enemy->x += 6;
        if(el_t->enemy->x >= el_t->enemy->max_x)
        {
            el_t->enemy->x = el_t->enemy->max_x;
            el_t->enemy->direction = 1;
        }
    }
    else if(el_t->enemy->direction == 1)
    {
        el_t->enemy->y += 6;
        if(el_t->enemy->y >= el_t->enemy->max_y)
        {
            el_t->enemy->y = el_t->enemy->max_y;
            el_t->enemy->direction = 2;
        }
    }
    else if(el_t->enemy->direction == 2)
    {
        el_t->enemy->x -= 6;
        if(el_t->enemy->x <= el_t->enemy->min_x)
        {
            el_t->enemy->x = el_t->enemy->min_x;
            el_t->enemy->direction = 3;
        }
    }
    else if(el_t->enemy->direction == 3)
    {
        el_t->enemy->y -= 6;
        if(el_t->enemy->y <= el_t->enemy->min_y)
        {
            el_t->enemy->y = el_t->enemy->min_y;
            el_t->enemy->direction = 0;
        }
    }
}

//handles what an enemy does on each timer interrupt
int handle_enemy()
{
    if(movementmode == 1) enemy_mov_1();
    if(movementmode == 2) enemy_mov_2();
    if(current_enemy_sprite == 0) if(draw_xpm(enemy1_sprite, enemy1_img, el_t->enemy->x, el_t->enemy->y)) return 1;
    if(current_enemy_sprite == 1) if(draw_xpm(enemy2_sprite, enemy2_img, el_t->enemy->x, el_t->enemy->y)) return 1;
    if(current_enemy_sprite == 2) if(draw_xpm(enemy3_sprite, enemy3_img, el_t->enemy->x, el_t->enemy->y)) return 1;

    if ((rand() % 1000) < shootfrequency) {ll_addNode(ll_getNewNode(el_t->enemy->x - 50, el_t->enemy->y + 22, 0));}
    //printList(LaserFirst);
    //printf("\n");
    el_t = el_t->next;
    return 0;
}

int handle_user()
{
    if(up)
    {
        if (y - 15 < 750) {y = 750;}
        else {y -= 15;}
    }
    else if(down)
    {
        if (y + 15 + 115 > mode_inf.YResolution) {y = mode_inf.YResolution - 115;}
        else {y += 15;}
    }
    else if(left)
    {
        if (x - 15 < 0) {x = 0;}
        else {x -= 15;}
    }
    else if(right)
    {
        if (x + 15 + 160 > mode_inf.XResolution) {x = mode_inf.XResolution - 160;}
        else {x += 15;}
    }
    if(draw_xpm(user_sprite, user_img, x, y)) return 1;
    return 0;
}

int handle_animation()
{
    struct AnimationNode *next_animation = al_t->next;
    al_t->animation->time_to_change--;
    if(al_t->animation->time_to_change == 0)
    {
        al_t->animation->curr_xpm++;
        al_t->animation->time_to_change = 5;
    }
    switch (al_t->animation->curr_xpm)
    {
    case 0:
        draw_xpm(explosion1_sprite, explosion1_img, al_t->animation->x, al_t->animation->y);
        break;
    case 1:
        draw_xpm(explosion2_sprite, explosion2_img, al_t->animation->x, al_t->animation->y);
        break;
    case 2:
        draw_xpm(explosion3_sprite, explosion3_img, al_t->animation->x, al_t->animation->y);
        break;
    case 3:
        draw_xpm(explosion4_sprite, explosion4_img, al_t->animation->x, al_t->animation->y);
        break;
    case 4:
        draw_xpm(explosion5_sprite, explosion5_img, al_t->animation->x, al_t->animation->y);
        break;
    case 5:
        draw_xpm(explosion6_sprite, explosion6_img, al_t->animation->x, al_t->animation->y);
        break;
    case 6:
        draw_xpm(explosion7_sprite, explosion7_img, al_t->animation->x, al_t->animation->y);
        break;
    default:
        al_removeFirst();
        break;
    }
    al_t = next_animation;
    return 0;
}
