#include "PlayerController.h"

/**
 * @file PlayerController.c
 * @brief Implements player input handling, movement, physics, and collision detection.
 *
 * Controls how the player character reacts to input,
 * gravity, jumps, and interaction with the environment.
 */

bool left_pressed = false;
bool right_pressed = false;
bool jump = false;

static bool AABB(Position p, int16_t Pw, int16_t Ph, Position o, int16_t Ow, int16_t Oh){
    if((p.x + Pw < o.x) ||(p.x > o.x + Ow)) return false;
    if((p.y+ Ph < o.y) || (p.y > o.y + Oh)) return false;

    return true;
}

static Position compute_horizontal_target(const Player *pl) {
    Position pos = pl->position;
    if (left_pressed && !right_pressed) {
        pos.x -= MOVE_SPEED;
    } else if (!left_pressed && right_pressed) {
        pos.x += MOVE_SPEED;
    }
    return pos;
}
static bool clamp_horizontal_to_screen(Position *pos, const Player *pl) {
    bool hit = false;
    if (pos->x < 0) {
        pos->x = 0;
        hit = true;
    } else if (pos->x + pl->width > mode_info.XResolution) {
        pos->x = mode_info.XResolution - pl->width;
        hit = true;
    }
    return hit;
}

static bool check_horizontal_collision(const Position *pos, const Player *pl, const MapController *mc) {
    for (size_t i = 0; i < mc->map->count_elements; i++) {
        Element *e = mc->map->elements[i];
        if(e->type != ELEMENT_EARTH && e->type != ELEMENT_WOOD) continue;
        if (AABB(*pos, pl->width, pl->height, e->position, e->width, e->height)) {
            return true;
        }
    }
    return false;
}
void handle_horizontal(Player *pl, const MapController *mc) {
    Position target = compute_horizontal_target(pl);
    bool screenHit = clamp_horizontal_to_screen(&target, pl);

    if (!screenHit && !check_horizontal_collision(&target, pl, mc)) {
        pl->position.x = target.x;
    }
}

static bool clamp_vertical_to_screen(Position *pos, const Player *pl) {
    bool hit = false;
    if (pos->y < 0) {
        pos->y = 0;
        hit = true;
    } else if (pos->y + pl->height > mode_info.YResolution) {
        pos->y = mode_info.YResolution - pl->height;
        hit = true;
    }
    return hit;
}
void apply_gravity_and_jump(Player *pl) {
    if (!pl->on_ground) {
        pl->vy += GRAVITY;
    }
    if (pl->on_ground && jump) {
        pl->on_ground = false;
        jump = false;
        pl->vy = JUMP_VELOCITY;
    }
}
void handle_vertical(Player *pl, const MapController *mc) {
    Position target = pl->position;
    target.y += pl->vy;
    bool hitScreenEdge = clamp_vertical_to_screen(&target, pl);
     if (hitScreenEdge) {
         if (pl->vy > 0) {
            pl->on_ground = true;
        }
        pl->vy = 0;
        pl->position.y = target.y;
        return;
    }
    bool landed = false;
    for (size_t i = 0; i < mc->map->count_elements; i++) {
        Element *e = mc->map->elements[i];
        if(e->type != ELEMENT_EARTH && e->type != ELEMENT_WOOD && e->type != ELEMENT_BEDROCK) continue;
        if (AABB(target, pl->width, pl->height, e->position, e->width, e->height)) {
            landed = true;
            break;
        }
    }

    if (landed) {
        pl->on_ground = true;
        pl->vy = 0;
    } else {
        pl->position.y = target.y;
        pl->on_ground = false;
    }
}

void update_player() {
    MapController *mc = get_MapController();
    Player *pl = mc->map->player;
    Position last_pos = pl->position;

    handle_horizontal(pl, mc);
    apply_gravity_and_jump(pl);
    handle_vertical(pl, mc);

    if (pl->position.x != last_pos.x) {
        pl->state = PLAYER_WALKING;
    } else if(pl->position.y != last_pos.y){
        pl->state = PLAYER_JUMPING;
    }
    else {
        pl->state = PLAYER_IDLE;
    }
}

void handle_player_input(uint8_t scan_code[2]) {
    if (scan_code[0] == 0xE0) {
        switch (scan_code[1]) {
            // Presses
            case 0x4B: // Left arrow make
                left_pressed = true;
                break;
            case 0x4D: // Right arrow make
                right_pressed = true;
                break;
            case 0xCB: // Left arrow break
                left_pressed = false;
                break;
            case 0xCD: // Right arrow break
                right_pressed = false;
                break;
            case 0x48: // Up arrow (jump)
                jump = true;
                break;
        }
    } else {
        switch (scan_code[0])
        {
        case 0x1E: // A Make
            left_pressed = true;
            break;
        case 0x20: // D Make
            right_pressed = true;
            break;
        case 0x39: // Jump space
            jump = true;
            break;
        case 0x9E: // A Break
            left_pressed = false;
            break;
        case 0xA0: // D Break
            right_pressed = false;
            break;
        default:
            break;
        }
    }

}
