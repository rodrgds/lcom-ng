#include <lcom/lcf.h>

#include "target_controller.h"
#include "view/game_view.h"

#include "model/target.h"
#include "view/target_view.h"
#include "devices/video.h"
#include "xpms/target.xpm"


extern unsigned h_res;
extern unsigned v_res;

Target targets[MAX_ACTIVE_TARGETS]; /**< @brief Array of targets */
uint8_t target_hits = 0; /**< @brief Number of targets hit */
uint8_t target_fails = 0;
uint8_t target_losses = 0;

bool show_warning = false;
int warning_timer = 0;

void target_controller_clear_targets() {
    for (int i = 0; i < MAX_ACTIVE_TARGETS; i++) {
        memset(&targets[i], 0, sizeof(Target));
    }
}

void generate_random_position_in_game_area(int16_t *x, int16_t *y) {
    *x = rand() % 726;
    *y = 40 + rand() % (541 - 40);
}

void generate_random_position_in_game_area_mode2(int16_t *x, int16_t *y) {
    *x = rand() % 726;
    *y = 45;
}

void generate_random_position_in_game_area_mode3(int16_t *x, int16_t *y, int *direction) { 
    *y = 40 + rand() % (400 - 40);
    if (rand() % 2 == 0) {
        *x = 0;
        *direction = 1;
    } else {
        *x = 725;
        *direction = -1;
    }
}

bool is_overlapping(Target *a, int16_t x, int16_t y, uint16_t width, uint16_t height) {
    return !(x + width  <= a->x || a->x + a->width  <= x || y + height <= a->y || a->y + a->height <= y);
}


void (target_controller_init_mode1)() {
    for (int i = 0; i < MAX_ACTIVE_TARGETS; i++) {
        int16_t x, y;
        uint16_t width = TARGET_WIDTH;
        uint16_t height = TARGET_HEIGHT;

        bool valid_position = false;
        int max_attempts = 100;

        while (!valid_position && max_attempts-- > 0) {
            generate_random_position_in_game_area(&x, &y);
            valid_position = true;

            for (int j = 0; j < i; j++) {
                if (!targets[j].isVisible) continue;
                if (is_overlapping(&targets[j], x, y, width, height)) {
                    valid_position = false;
                    break;
                }
            }
        }

        if (valid_position) {
            create_target(&targets[i], x, y, (xpm_map_t) target);
        } else {
            targets[i].isVisible = false;
        }
    }
}

void (target_controller_init_mode2)() {
    for (int i = 0; i < MAX_ACTIVE_TARGETS; i++) {
        int16_t x, y;
        uint16_t width = TARGET_WIDTH;
        uint16_t height = TARGET_HEIGHT;

        bool valid_position = false;
        int max_attempts = 100;

        while (!valid_position && max_attempts-- > 0) {
            generate_random_position_in_game_area_mode2(&x, &y);
            valid_position = true;

            for (int j = 0; j < i; j++) {
                if (!targets[j].isVisible) continue;
                if (is_overlapping(&targets[j], x, y, width, height)) {
                    valid_position = false;
                    break;
                }
            }
        }

        if (valid_position) {
            create_target(&targets[i], x, y, (xpm_map_t) target);
            targets[i].fall_speed = 2;
        } else {
            targets[i].isVisible = false;
        }
    }
}

void target_controller_init_mode3() {
    for (int i = 0; i < MAX_ACTIVE_TARGETS; i++) {
        int16_t x, y;
        int direction;
        uint16_t width = TARGET_WIDTH;
        uint16_t height = TARGET_HEIGHT;

        bool valid_position = false;
        int max_attempts = 100;

        while (!valid_position && max_attempts-- > 0) {
            generate_random_position_in_game_area_mode3(&x, &y, &direction);
            valid_position = true;

            for (int j = 0; j < i; j++) {
                if (!targets[j].isVisible) continue;
                if (is_overlapping(&targets[j], x, y, width, height)) {
                    valid_position = false;
                    break;
                }
            }
        }

        if (valid_position) {
            create_target(&targets[i], x, y, (xpm_map_t) target);
            targets[i].move_speed = 4;
            targets[i].direction = direction;
        } else {
            targets[i].isVisible = false;
        }
    }
}

void (target_controller_update_mode1)() {
    for (int i = 0; i < MAX_ACTIVE_TARGETS; i++) {
        if (!targets[i].isVisible) {
            int16_t x, y;
            uint16_t width = TARGET_WIDTH;
            uint16_t height = TARGET_HEIGHT;

            bool valid_position = false;
            int max_attempts = 100;

            while (!valid_position && max_attempts-- > 0) {
                generate_random_position_in_game_area(&x, &y);
                valid_position = true;

                for (int j = 0; j < MAX_ACTIVE_TARGETS; j++) {
                    if (j == i || !targets[j].isVisible) continue;

                    if (is_overlapping(&targets[j], x, y, width, height)) {
                        valid_position = false;
                        break;
                    }
                }
            }
            
            if(valid_position){
                create_target(&targets[i], x, y, (xpm_map_t) target);
            }
        }
    }
}


void target_controller_update_mode2() { 
    for (int i = 0; i < MAX_ACTIVE_TARGETS; i++) {
        if (!targets[i].isVisible) {
            int16_t x, y;
            uint16_t width = TARGET_WIDTH;
            uint16_t height = TARGET_HEIGHT;

            bool valid_position = false;
            int max_attempts = 100;

            while (!valid_position && max_attempts-- > 0) {
                generate_random_position_in_game_area_mode2(&x, &y);
                valid_position = true;

                for (int j = 0; j < MAX_ACTIVE_TARGETS; j++) {
                    if (j == i || !targets[j].isVisible) continue;

                    if (is_overlapping(&targets[j], x, y, width, height)) {
                        valid_position = false;
                        break;
                    }
                }
            }
            
            if (valid_position) {
                create_target(&targets[i], x, y, (xpm_map_t) target);
                targets[i].fall_speed = 4;
            }
        }
    }
}

void target_controller_update_mode3() {
    for (int i = 0; i < 1; i++) {
        if (!targets[i].isVisible) {
            int16_t x, y;
            int direction;
            uint16_t width = TARGET_WIDTH;
            uint16_t height = TARGET_HEIGHT;

            bool valid_position = false;
            int max_attempts = 100;

            while (!valid_position && max_attempts-- > 0) {
                generate_random_position_in_game_area_mode3(&x, &y, &direction);
                valid_position = true;

                for (int j = 0; j < 1; j++) {
                    if (j == i || !targets[j].isVisible) continue;

                    if (is_overlapping(&targets[j], x, y, width, height)) {
                        valid_position = false;
                        break;
                    }
                }
            }

            if (valid_position) {
                create_target(&targets[i], x, y, (xpm_map_t) target);
                targets[i].move_speed = rand() % 8 + 3;
                targets[i].fall_speed = rand() % 3 + 1;
                targets[i].direction = direction;
            }
        }
    }
}

void (target_controller_draw)() {
    for (int i = 0; i < MAX_ACTIVE_TARGETS; i++) {
        if (targets[i].isVisible) {
            draw_target(&targets[i]);
        }
    }
}

bool (target_controller_check_hit)(uint16_t x, uint16_t y) {
    for (int i = 0; i < MAX_ACTIVE_TARGETS; i++) {
        if (!targets[i].isVisible) continue;

        int16_t tx = targets[i].x;
        int16_t ty = targets[i].y;
        int16_t tw = targets[i].width;
        int16_t th = targets[i].height;

        if (x >= tx && x < tx + tw && y >= ty && y < ty + th) {
            targets[i].isVisible = false;
            return true;
        }
    }
    target_fails++;
    show_warning = true;
    warning_timer = 30; 
    return false;
}

bool target_controller_check_hit_mode3(uint16_t x, uint16_t y) {
    for (int i = 0; i < 1; i++) {
        if (!targets[i].isVisible) continue;

        int16_t tx = targets[i].x;
        int16_t ty = targets[i].y;
        int16_t tw = targets[i].width;
        int16_t th = targets[i].height;

        if (x >= tx && x < tx + tw && y >= ty && y < ty + th) {
            targets[i].isVisible = false;
            return true;
        }
    }
    target_fails++;
    targets[0].isVisible = false;
    show_warning = true;
    warning_timer = 30; 
    return false;
}


void target_controller_fall_update() {
    for (int i = 0; i < MAX_ACTIVE_TARGETS; i++) {
        if (targets[i].isVisible) {
            targets[i].y += targets[i].fall_speed;

            if ((int)targets[i].y > (int)v_res) {
                targets[i].isVisible = false;
                target_losses++;
            }
        }
    }
}

void target_controller_horizontal_update() {
    for (int i = 0; i < MAX_ACTIVE_TARGETS; i++) {
        if (targets[i].isVisible) {
            targets[i].x += targets[i].move_speed * targets[i].direction;
            targets[i].y += targets[i].fall_speed;
            if (targets[i].x < -100 || targets[i].x > 800 || targets[i].y > 650 || targets[i].y < 40) {
                targets[i].isVisible = false;
                target_losses++;
            }
        }
    }
}




