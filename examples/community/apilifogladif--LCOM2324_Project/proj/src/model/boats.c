#include <lcom/lcf.h>
#include "boats.h"

extern Sprite *s_boat_2p;
extern Sprite *s_boat_3p1;
extern Sprite *s_boat_3p2;
extern Sprite *s_boat_4p;
extern Sprite *s_boat_5p;
extern Sprite *s_boat_2p_selected;
extern Sprite *s_boat_3p1_selected;
extern Sprite *s_boat_3p2_selected;
extern Sprite *s_boat_4p_selected;
extern Sprite *s_boat_5p_selected;
extern Sprite *s_boat_2p_V;
extern Sprite *s_boat_3p1_V;
extern Sprite *s_boat_3p2_V;
extern Sprite *s_boat_4p_V;
extern Sprite *s_boat_5p_V;
extern Sprite *s_boat_2p_selected_V;
extern Sprite *s_boat_3p1_selected_V;
extern Sprite *s_boat_3p2_selected_V;
extern Sprite *s_boat_4p_selected_V;
extern Sprite *s_boat_5p_selected_V;

extern Sprite *small_s_boat_2p;
extern Sprite *small_s_boat_3p1;
extern Sprite *small_s_boat_3p2;
extern Sprite *small_s_boat_4p;
extern Sprite *small_s_boat_5p;
extern Sprite *small_s_boat_2p_V;
extern Sprite *small_s_boat_3p1_V;
extern Sprite *small_s_boat_3p2_V;
extern Sprite *small_s_boat_4p_V;
extern Sprite *small_s_boat_5p_V;

extern Spot matrix[MATRIX_SIZE][MATRIX_SIZE];

extern Boat *boat_2p; 
extern Boat *boat_3p1;
extern Boat *boat_3p2;
extern Boat *boat_4p;
extern Boat *boat_5p;

bool oneIsSelected = false;

Boat *create_boat(Sprite *sprite, Sprite *s_sprite, Sprite *v_sprite, Sprite *sv_sprite, Sprite *small_sprite, Sprite *small_v_sprite, int h, int w, int id){
  Boat *boat = (Boat*)malloc(sizeof(Boat));
  if(boat == NULL) return NULL;

  boat->id = id;
  boat->health = w;
  boat->damage = 0;
  boat->sprite = sprite;
  boat->s_sprite = s_sprite;
  boat->v_sprite = v_sprite;
  boat->sv_sprite = sv_sprite;
  boat->small_sprite = small_sprite;
  boat->small_v_sprite = small_v_sprite;
  boat->fixed = false;
  boat->rotated = false;
  boat->selected = false;
  boat->sunk = false;
  boat->first = matrix[0][0];
  boat->width = w;
  boat->height = h;

  if(boat->sprite == NULL ) {
    free(boat);
    return NULL;
  }

  return boat;
}

void destroy_boat(Boat *boat) {
    if(boat == NULL) return;

    if(boat->sprite)
        free(boat->sprite);
    if(boat->s_sprite)
        free(boat->s_sprite);
    if(boat->v_sprite)
        free(boat->v_sprite);
    if(boat->sv_sprite)
        free(boat->sv_sprite);
    if(boat->small_sprite)
        free(boat->small_sprite);
    if(boat->small_v_sprite)
        free(boat->small_v_sprite);

    free(boat);
    boat = NULL;
}

void updateBoatPos(Boat * boat, int x, int y) {
    if (x > MATRIX_SIZE - boat->width) {
        x = MATRIX_SIZE - boat->width;
    }
    else if (x < 0) {
        x = 0;
    }
    if (y > MATRIX_SIZE - boat->height) {
        y = MATRIX_SIZE - boat->height;
    }
    else if (y < 0) {
        y = 0;
    }

    boat->first = matrix[y][x];

    boat->sprite->y = boat->first.y_pixel;
    boat->sprite->x = boat->first.x_pixel;
    boat->s_sprite->y = boat->first.y_pixel;
    boat->s_sprite->x = boat->first.x_pixel;
    boat->v_sprite->y = boat->first.y_pixel;
    boat->v_sprite->x = boat->first.x_pixel;
    boat->sv_sprite->y = boat->first.y_pixel;
    boat->sv_sprite->x = boat->first.x_pixel;

    boat->small_sprite->y = boat->first.small_y_pixel;
    boat->small_sprite->x = boat->first.small_x_pixel;
    boat->small_v_sprite->y = boat->first.small_y_pixel;
    boat->small_v_sprite->x = boat->first.small_x_pixel;
}

void rotateBoat(Boat* boat) {
    int h = boat->height;
    boat->height = boat->width;
    boat->width = h;
    updateBoatPos(boat, boat->first.column, boat->first.line);
}

void load_boats(){
    load_boat_sprites(BIG_BOARD_X, BIG_BOARD_Y);
    boat_2p = create_boat(s_boat_2p, s_boat_2p_selected, s_boat_2p_V, s_boat_2p_selected_V, small_s_boat_2p, small_s_boat_2p_V, 1, 2, 2);
    boat_3p1 = create_boat(s_boat_3p1, s_boat_3p1_selected, s_boat_3p1_V, s_boat_3p1_selected_V, small_s_boat_3p1, small_s_boat_3p1_V,1, 3, 31);
    boat_3p2 = create_boat(s_boat_3p2, s_boat_3p2_selected, s_boat_3p2_V, s_boat_3p2_selected_V, small_s_boat_3p2, small_s_boat_3p2_V, 1, 3, 32);
    boat_4p = create_boat(s_boat_4p, s_boat_4p_selected, s_boat_4p_V, s_boat_4p_selected_V, small_s_boat_4p, small_s_boat_4p_V, 1, 4, 4);
    boat_5p = create_boat(s_boat_5p, s_boat_5p_selected, s_boat_5p_V, s_boat_5p_selected_V, small_s_boat_5p, small_s_boat_5p_V, 1, 5, 5);
}


void drawSelected(int b) {
    switch (b) {
        case 2:
            if(boat_2p->fixed) clearSpots(boat_2p);
            boat_2p->fixed = false;
            boat_2p->selected = true;
            drawBoat(2);
            oneIsSelected = true;
            break;
        case 31:
            if(boat_3p1->fixed) clearSpots(boat_3p1);
            boat_3p1->fixed = false;
            boat_3p1->selected = true;
            drawBoat(31);
            oneIsSelected = true;
            break;
        case 32:
            if(boat_3p2->fixed) clearSpots(boat_3p2);
            boat_3p2->fixed = false;
            boat_3p2->selected = true;
            drawBoat(32);
            oneIsSelected = true;
            break;
        case 4:
            if(boat_4p->fixed) clearSpots(boat_4p);
            boat_4p->fixed = false;
            boat_4p->selected = true;
            drawBoat(4);
            oneIsSelected = true;
            break;
        case 5:
            if(boat_5p->fixed) clearSpots(boat_5p);
            boat_5p->fixed = false;
            boat_5p->selected = true;
            drawBoat(5);
            oneIsSelected = true;
            break;
    }
}

void fixTheSelected() {
    if (boat_2p->selected) {
        if (overlapBoats(boat_2p)) {
            drawSelected(2);
            return;
        }
        setSpots(boat_2p);
        boat_2p->selected = false;
        boat_2p->fixed = true;
        drawBoat(2);
    }
    else if (boat_3p1->selected) {
        if (overlapBoats(boat_3p1)) {
            drawSelected(31);
            return;
        }
        setSpots(boat_3p1);
        boat_3p1->selected = false;
        boat_3p1->fixed = true;
        drawBoat(31);
    }
    else if (boat_3p2->selected) {
        if (overlapBoats(boat_3p2)) {
            drawSelected(32);
            return;
        }
        setSpots(boat_3p2);
        boat_3p2->selected = false;
        boat_3p2->fixed = true;
        drawBoat(32);
    }
    else if (boat_4p->selected) {
        if (overlapBoats(boat_4p)) {
            drawSelected(4);
            return;
        }
        setSpots(boat_4p);
        boat_4p->selected = false;
        boat_4p->fixed = true;
        drawBoat(4);
    }
    else if (boat_5p->selected) {
        if (overlapBoats(boat_5p)) {
            drawSelected(5);
            return;
        }
        setSpots(boat_5p);
        boat_5p->selected = false;
        boat_5p->fixed = true;
        drawBoat(5);
    }
    oneIsSelected = false;
}

void moveSelected(int x, int y) {
    if (boat_2p->selected) {
        updateBoatPos(boat_2p, boat_2p->first.column + x, boat_2p->first.line + y);
        drawBoat(2);
    }
    else if (boat_3p1->selected) {
        updateBoatPos(boat_3p1, boat_3p1->first.column + x, boat_3p1->first.line + y);
        drawBoat(31);
    }
    else if (boat_3p2->selected) {
        updateBoatPos(boat_3p2, boat_3p2->first.column + x, boat_3p2->first.line + y);
        drawBoat(32);
    }
    else if (boat_4p->selected) {
        updateBoatPos(boat_4p, boat_4p->first.column + x, boat_4p->first.line + y);
        drawBoat(4);
    }
    else if (boat_5p->selected) {
        updateBoatPos(boat_5p, boat_5p->first.column + x, boat_5p->first.line + y);
        drawBoat(5);
    }
}

void rotateSelected() {
    if (boat_2p->selected) {
        boat_2p->rotated = !boat_2p->rotated;
        rotateBoat(boat_2p);
        drawBoat(2);
    }
    else if (boat_3p1->selected) {
        boat_3p1->rotated = !boat_3p1->rotated;
        rotateBoat(boat_3p1);
        drawBoat(31);
    }
    else if (boat_3p2->selected) {
        boat_3p2->rotated = !boat_3p2->rotated;
        rotateBoat(boat_3p2);
        drawBoat(32);
    }
    else if (boat_4p->selected) {
        boat_4p->rotated = !boat_4p->rotated;
        rotateBoat(boat_4p);
        drawBoat(4);
    }
    else if (boat_5p->selected) {
        boat_5p->rotated = !boat_5p->rotated;
        rotateBoat(boat_5p);
        drawBoat(5);
    }
}

void drawBoats() {
    if (boat_2p->fixed) drawBoat(2);
    if (boat_3p1->fixed) drawBoat(31);
    if (boat_3p2->fixed) drawBoat(32);
    if (boat_4p->fixed) drawBoat(4);
    if (boat_5p->fixed) drawBoat(5);
}

void drawBoat(int b) {
    switch (b) {
        case 2:
            if (!boat_2p->selected && !boat_2p->rotated) draw_sprite(boat_2p->sprite);
            else if (boat_2p->selected && !boat_2p->rotated) draw_sprite(boat_2p->s_sprite);
            else if (!boat_2p->selected && boat_2p->rotated) draw_sprite(boat_2p->v_sprite);
            else if (boat_2p->selected && boat_2p->rotated) draw_sprite(boat_2p->sv_sprite);
            break;
        case 31:
            if (!boat_3p1->selected && !boat_3p1->rotated) draw_sprite(boat_3p1->sprite);
            else if (boat_3p1->selected && !boat_3p1->rotated) draw_sprite(boat_3p1->s_sprite);
            else if (!boat_3p1->selected && boat_3p1->rotated) draw_sprite(boat_3p1->v_sprite);
            else if (boat_3p1->selected && boat_3p1->rotated) draw_sprite(boat_3p1->sv_sprite);
            break;
        case 32:
            if (!boat_3p2->selected && !boat_3p2->rotated) draw_sprite(boat_3p2->sprite);
            else if (boat_3p2->selected && !boat_3p2->rotated) draw_sprite(boat_3p2->s_sprite);
            else if (!boat_3p2->selected && boat_3p2->rotated) draw_sprite(boat_3p2->v_sprite);
            else if (boat_3p2->selected && boat_3p2->rotated) draw_sprite(boat_3p2->sv_sprite);
            break;
        case 4:
            if (!boat_4p->selected && !boat_4p->rotated) draw_sprite(boat_4p->sprite);
            else if (boat_4p->selected && !boat_4p->rotated) draw_sprite(boat_4p->s_sprite);
            else if (!boat_4p->selected && boat_4p->rotated) draw_sprite(boat_4p->v_sprite);
            else if (boat_4p->selected && boat_4p->rotated) draw_sprite(boat_4p->sv_sprite);
            break;
        case 5:
            if (!boat_5p->selected && !boat_5p->rotated) draw_sprite(boat_5p->sprite);
            else if (boat_5p->selected && !boat_5p->rotated) draw_sprite(boat_5p->s_sprite);
            else if (!boat_5p->selected && boat_5p->rotated) draw_sprite(boat_5p->v_sprite);
            else if (boat_5p->selected && boat_5p->rotated) draw_sprite(boat_5p->sv_sprite);
            break;
        default:
            return;
    }
}

void setSpots (Boat* boat) {
    if (boat->width > 1) {
        for (int i = 0; i < boat->width; i++) {
            matrix[boat->first.line][boat->first.column + i].taken = boat->id;
        }
    }
    else {
        for (int i = 0; i < boat->height; i++) {
            matrix[boat->first.line + i][boat->first.column].taken = boat->id;
        }
    }
}

void clearSpots (Boat* boat) {
    if (boat->width > 1) {
        for (int i = 0; i < boat->width; i++) {
            matrix[boat->first.line][boat->first.column + i].taken = 0;
        }
    }
    else {
        for (int i = 0; i < boat->height; i++) {
            matrix[boat->first.line + i][boat->first.column].taken = 0;
        }
    }
}

bool overlapBoats(Boat* boat) {
    if (boat->width > 1) {
        for (int i = 0; i < boat->width; i++) {
            if (matrix[boat->first.line][boat->first.column + i].taken) return true;
        }
    }
    else {
        for (int i = 0; i < boat->height; i++) {
            if (matrix[boat->first.line + i][boat->first.column].taken) return true;
        }
    }
    return false;
}

bool allBoatsFixed() {
    if (boat_2p->fixed && boat_3p1->fixed && boat_3p2->fixed && boat_4p->fixed && boat_5p->fixed) return true;
    return false;
}
