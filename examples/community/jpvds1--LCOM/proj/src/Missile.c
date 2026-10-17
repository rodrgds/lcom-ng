#include "Missile.h"

struct Missile missile;
struct EnemyNode *el_q;

void startup_missile()
{
    missile.shot = false;
    missile.x = 0;
    missile.y = 0;
    missile.time = 0;
    missile.frame_time = 0;
    missile.frame = 0;
}

void shoot_missile()
{
    if(missile.time != 0)
    {
        return;
    }
    missile.shot = true;
    missile.x = mouse_x;
    missile.y = mouse_y;
    missile.time = 135;
    missile.frame_time = 3;
    missile.frame = 0;
}

void increment_missile()
{
    if(missile.time > 0)
    {
        missile.time--;
    }
}

int draw_missile(uint16_t frame)
{
    switch (frame)
    {
    case 0:
        if(draw_xpm(missile1_sprite, missile1_img, missile.x - 40, missile.y - 40)) return 1;
        break;
    case 1:
        if(draw_xpm(missile2_sprite, missile2_img, missile.x - 40, missile.y - 40)) return 1;
        break;
    case 2:
        if(draw_xpm(missile3_sprite, missile3_img, missile.x - 40, missile.y - 40)) return 1;
        break;
    case 3:
        if(draw_xpm(missile4_sprite, missile4_img, missile.x - 40, missile.y - 40)) return 1;
        break;
    case 4:
        if(draw_xpm(missile5_sprite, missile5_img, missile.x - 40, missile.y - 40)) return 1;
        break;
    case 5:
        if(draw_xpm(missile6_sprite, missile6_img, missile.x - 40, missile.y - 40)) return 1;
        break;
    case 6:
        if(draw_xpm(missile7_sprite, missile7_img, missile.x - 40, missile.y - 40)) return 1;
        break;
    case 7:
        if(draw_xpm(missile8_sprite, missile8_img, missile.x - 40, missile.y - 40)) return 1;
        break;
    case 8:
        if(draw_xpm(missile9_sprite, missile9_img, missile.x - 40, missile.y - 40)) return 1;
        break;
    case 9:
        if(draw_xpm(missile10_sprite, missile10_img, missile.x - 40, missile.y - 40)) return 1;
        break;
    case 10:
        if(draw_xpm(missile11_sprite, missile11_img, missile.x - 40, missile.y - 40)) return 1;
        break;
    case 11:
        if(draw_xpm(missile12_sprite, missile12_img, missile.x - 40, missile.y - 40)) return 1;
        break;
    case 12:
        if(draw_xpm(missile13_sprite, missile13_img, missile.x - 40, missile.y - 40)) return 1;
        break;
    case 13:
        if(draw_xpm(missile14_sprite, missile14_img, missile.x - 40, missile.y - 40)) return 1;
        break;
    case 14:
        if(draw_xpm(missile15_sprite, missile15_img, missile.x - 40, missile.y - 40)) return 1;
        break;
    case 15:
        if(draw_xpm(missile16_sprite, missile16_img, missile.x - 40, missile.y - 40)) return 1;
        break;
    case 16:
        if(draw_xpm(missile17_sprite, missile17_img, missile.x - 40, missile.y - 40)) return 1;
        break;
    case 17:
        if(draw_xpm(missile18_sprite, missile18_img, missile.x - 40, missile.y - 40)) return 1;
        break;
    case 18:
        if(draw_xpm(missile19_sprite, missile19_img, missile.x - 40, missile.y - 40)) return 1;
        break;
    case 19:
        if(draw_xpm(missile20_sprite, missile20_img, missile.x - 40, missile.y - 40)) return 1;
        break;
    case 20:
        if(draw_xpm(missile21_sprite, missile21_img, missile.x - 40, missile.y - 40)) return 1;
        break;
    case 21:
        if(draw_xpm(missile22_sprite, missile22_img, missile.x - 40, missile.y - 40)) return 1;
        break;
    case 22:
        if(draw_xpm(missile23_sprite, missile23_img, missile.x - 40, missile.y - 40)) return 1;
        break;
    case 23:
        if(draw_xpm(missile24_sprite, missile24_img, missile.x - 40, missile.y - 40)) return 1;
        break;
    default:
        return 1;
        break;
    }
    return 0;
}

bool missile_ready()
{
    return missile.time == 0;
}

int handle_missile()
{
    if(!missile.shot)
    {
        return 0;
    }
    if(missile.frame_time == 0)
    {
        missile.frame_time = 3;
        missile.frame++;
    }
    else
    {
        missile.frame_time--;
    }
    if(missile.frame == 24)
    {
        missile.shot = false;
        return 0;
    }
    if(draw_missile(missile.frame)) {return 1;}
    if(missile.frame == 12 && missile.frame_time == 3)
    {
        printf("\n keyframe reached");
        el_q = EnemyFirst;
        while(el_q != NULL)
        {
            struct EnemyNode *next_enemy = el_q->next;
            printf("\npassing through enemy");
            if((el_q->enemy->x > missile.x-100) && (el_q->enemy->x < missile.x+40) && (el_q->enemy->y > missile.y-62) && (el_q->enemy->y < missile.y + 40))
            {
                printf("\nenemy hit");
                al_addNode(al_getNewNode(el_q->enemy->x, el_q->enemy->y));
                el_removeNode(el_q);
                score += 20;
            }
            el_q = next_enemy;
        }
    }
    return 0;
}
