#include "hitbox.h"

invis_hitbox init_hitbox(int x, int y, uint16_t width, uint16_t length, char type[]){
    invis_hitbox hitbox;
    hitbox.x_pos=x;
    hitbox.y_pos=y;
    hitbox.width=width;
    hitbox.length=length;
    strcpy(hitbox.type, type);
    return hitbox;
}

int draw_hitbox(struct invis_hitbox hitbox){
    int x=hitbox.x_pos;
    int y=hitbox.y_pos;
    int width=hitbox.width;
    int height=hitbox.length;
    int color = 0x0000FF;
    if (!strcmp(hitbox.type,"door")) color=0x00FF00;
    if (!strcmp(hitbox.type,"damage")) color=0xFF0000;
    for (int h = 0 ; h < height ; h++) {
      for (int w = 0 ; w < width ; w++) {
        if (paint_pixel(x + w, y + h, color)) return 1;
      }
    }
    return 0; 
}

char verifyCollision(invis_hitbox hitbox_1, invis_hitbox hitbox_2){
    char collision = NO;
    
    int xi1 = hitbox_1.x_pos;
    int yi1 = hitbox_1.y_pos;
    int xf1 = xi1 + hitbox_1.width;
    int yf1 = yi1 + hitbox_1.length;

    int xi2 = hitbox_2.x_pos;
    int yi2 = hitbox_2.y_pos;
    int xf2 = xi2 + hitbox_2.width;
    int yf2 = yi2 + hitbox_2.length;

    if(xi1 <= xf2 && xf1 >= xi2 && yi1 <= yf2 && yf1 >= yi2){
        if(xf1 >= xf2){
            collision = collision | LEFT;
        }

        if(xi1 <= xi2){
            collision = collision | RIGHT;
        }

        if(yf1 >= yf2){
            collision = collision | TOP;
        }

        if(yi1 <= yi2){
            collision = collision | BOTTOM; 
        }
    }

    return collision;
}

gate init_gate(char orig, char dest, int x, int y){
    gate gate;
    gate.origin_level=orig;
    gate.destination_level=dest;
    gate.destination_x=x;
    gate.destination_y=y;
    return gate;
}
