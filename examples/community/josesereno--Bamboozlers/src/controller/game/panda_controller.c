#include "panda_controller.h"

bool (above)(Sprite *item) {

    if (item->height[0] != 455 && panda->y + panda->height[panda->i] >= item->y + 1) return false;
    uint16_t feet = panda->height[panda->i] - 1;

    uint16_t yAbs = panda->y + feet;
    uint16_t yPos = yAbs - item->y;

    for (int i = 0; i < panda->width[panda->i]; i++)
    {
        uint16_t xAbs = panda->x + i;
        uint16_t xPos = xAbs - item->x;

        // Check if the positions are within valid bounds
        if (yPos < item->height[item->i] && xPos < item->width[item->i])
        {
            uint32_t panda_color = panda->pixmap_array[panda->i][i + (feet * panda->width[panda->i])];
            uint32_t item_color = item->pixmap_array[item->i][xPos + (yPos * item->width[item->i])];
            if (panda_color != TRANSPARENT && item_color != TRANSPARENT) return true;
        }
    }
    return false;
}

void (move_left)() {
    panda->x -= XSPEED;
}

void (move_right)() {
    panda->x += XSPEED;
}

void (jump)(uint32_t time, uint32_t y) {
    panda->y = y - YSPEED * time + time * time * (GRAVITY) / 2;
    panda->yspeed = YSPEED - GRAVITY * time;
}

void (fall)(uint32_t time, uint32_t y) {
    panda->y = y + time * time * (GRAVITY) / 2;
    panda->yspeed = GRAVITY * time;
}

bool (above_any_item)() {
    return above(map);
}

bool (collide_with_items)() {
    if (collide(panda, little_block)) {
        collide_item = little_block;
        return true;
    }
    else if (collide(panda, little_plank)) {
        collide_item = little_plank;
        return true;
    }
    else if (collide(panda, big_plank)) {
        collide_item = big_plank;
        return true;
    }
    else if (collide(panda, big_block)) {
        collide_item = big_block;
        return true;
    }
    if (collide(panda, map)) {
        collide_item = map;
        return true;
    }
    return false;
}

bool (hit_floor)() {
    return panda->y + panda->height[panda->i] > 20 + MAP_HEIGHT;
}

void (handle_boundary_conditions)() {
    if (panda->x < 25) panda->x = 25;
    if (panda->y < 25) panda->y = 25;
    if (panda->x + panda->width[panda->i] > 15 + MAP_WIDTH) panda->x = 15 + MAP_WIDTH - panda->width[panda->i];
    if (panda->y + panda->height[panda->i] >= 20 + MAP_HEIGHT) panda->y = 20 + MAP_HEIGHT - panda->height[panda->i];
}

void (fix_collision)() {
    if (!above(collide_item)) {
        if (isRightPressed) {
            if (collide_item->height[0] != 455 && panda->x < collide_item->x)
                while (collide(panda, collide_item)) panda->x--;

            else if (collide_item->height[0] == 455)
                while (collide(panda, collide_item)) panda->x--;
            
            else move_left();
        }
        else {
            if (collide_item->height[0] != 455 && panda->x + panda->width[panda->i] > collide_item->x + collide_item->width[collide_item->i])
                while (collide(panda, collide_item)) panda->x++;
            else if (collide_item->height[0] == 455)
                while (collide(panda, collide_item)) panda->x++;
            else move_right();
        }
    }
}

void (fix_jump_collision)() {
    while (collide(panda, collide_item)) panda->y++;
}

void (fix_fall_collision)() {
    while (collide(panda, collide_item)) panda->y--;
}

void (update_panda_animation)(){
 
    if (panda_state == JUMP) panda->i = (isRightPressed) ? 6 : 7;
    
    else {
        if (isRightPressed) {
            if (panda->i > 2) panda->i = 0;
            panda->i = (panda->i + 1) % 3;
        }
        else {
            if (panda->i < 3) panda->i = 3;
            panda->i = (panda->i + 1) % 3 + 3;
        }
    }
}
