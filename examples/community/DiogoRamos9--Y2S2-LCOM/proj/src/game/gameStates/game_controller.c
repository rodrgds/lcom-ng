#include <lcom/lcf.h>

#include "game_controller.h"

extern GameState gameState;
extern int freq;
extern struct packet pp;

typedef struct {
    int x, y;
    int xspeed, yspeed;
    Sprite *sprite;
    bool is_destroyed;
} laser_t;

typedef struct {
    int x, y;
    int xspeed, yspeed;
    int direction;
    Sprite * sprite;
    bool is_destroyed;
} asteroid_t;

typedef struct {
    int x, y;
    Sprite * sprite;
    int type;
    bool is_consumed;
} powerup_t;

laser_t lasers[100];
int laser_index = 0;
int selected_cannon = 0;

asteroid_t asteroids[100];
int asteroid_index = 0;

powerup_t powerups[100];
int powerup_index = 0;

int score = 0;

int a_speed = 5;
int a_diagspeed = 3;

int l_speed = 10;
int l_diagspeed = 7;

int ammo = 5;
bool out_of_ammo = false;

void draw_lasers();

void draw_cannon();

void draw_asteroids();

void draw_game() {
    check_collisions();
    draw_sprite(background, 0, 0);
    draw_sprite(player, player->x, player->y);
    draw_cannon();
    draw_lasers();
    draw_asteroids();
    draw_powerups();
    draw_score();
    draw_ammo();
}

void create_laser() {
    switch (selected_cannon) {
        case 0:
            lasers[laser_index].x = 393;
            lasers[laser_index].y = 220;
            lasers[laser_index].xspeed = 0;
            lasers[laser_index].yspeed = -l_speed;
            break;
        case 1:
            lasers[laser_index].x = 442;
            lasers[laser_index].y = 238;
            lasers[laser_index].xspeed = l_diagspeed;
            lasers[laser_index].yspeed = -l_diagspeed;
            break;
        case 2:
            lasers[laser_index].x = 466;
            lasers[laser_index].y = 290;
            lasers[laser_index].xspeed = l_speed;
            lasers[laser_index].yspeed = 0;
            break;
        case 3:
            lasers[laser_index].x = 447;
            lasers[laser_index].y = 347;
            lasers[laser_index].xspeed = l_diagspeed;
            lasers[laser_index].yspeed = l_diagspeed;
            break;
        case 4:
            lasers[laser_index].x = 393;
            lasers[laser_index].y = 365;
            lasers[laser_index].xspeed = 0;
            lasers[laser_index].yspeed = l_speed;
            break;
        case 5:
            lasers[laser_index].x = 340;
            lasers[laser_index].y = 343;
            lasers[laser_index].xspeed = -l_diagspeed;
            lasers[laser_index].yspeed = l_diagspeed;
            break;
        case 6:
            lasers[laser_index].x = 318;
            lasers[laser_index].y = 290;
            lasers[laser_index].xspeed = -l_speed;
            lasers[laser_index].yspeed = 0;
            break;
        case 7:
            lasers[laser_index].x = 342;
            lasers[laser_index].y = 240;
            lasers[laser_index].xspeed = -l_diagspeed;
            lasers[laser_index].yspeed = -l_diagspeed;
            break;
        default:
            break;
    }
    lasers[laser_index].sprite = laser;
    lasers[laser_index].is_destroyed = false;
    laser_index++;
    ammo--;
    if (ammo == 0) {
        out_of_ammo = true;
    }
}

void update_lasers() {
    // move lasers
    for (int i = 0; i < laser_index; i++) {
        lasers[i].x += lasers[i].xspeed;
        lasers[i].y += lasers[i].yspeed;
    }

    // check if laser is out of bounds
    for (int i = 0; i < laser_index; i++) {
        if (lasers[i].y < 0 || lasers[i].y > mode_info.YResolution || lasers[i].x < 0 || lasers[i].x > mode_info.XResolution) {
            for (int j = i; j < laser_index - 1; j++) {
                lasers[j] = lasers[j + 1];
            }
            laser_index--;
        }
    }
}

void draw_lasers() {
    // draw lasers
    for (int i = 0; i < laser_index; i++) {
        draw_sprite(laser, lasers[i].x, lasers[i].y);
    }
}

void draw_cannon() {
    switch (selected_cannon) {
        case 0:
            draw_sprite(cannon_up, 380, 227);
            break;
        case 1:
            draw_sprite(cannon_diag1, 415, 240);
            break;
        case 2:
            draw_sprite(cannon_right, 431, 278);
            break;
        case 3:
            draw_sprite(cannon_diag2, 420, 320);
            break;
        case 4:
            draw_sprite(cannon_down, 382, 332);
            break;
        case 5:
            draw_sprite(cannon_diag3, 342, 315);
            break;
        case 6:
            draw_sprite(cannon_left, 328, 278);
            break;
        case 7:
            draw_sprite(cannon_diag4, 345, 242);
            break;
        default:
            break;
    }
}

void create_asteroid() {
    asteroid_t asteroid_;
    asteroid_.direction = rand() % 8;
    switch (asteroid_.direction) {
        case 0:
            asteroid_.x = 385;
            asteroid_.y = 0;
            asteroid_.xspeed = 0;
            asteroid_.yspeed = a_speed;
            break;
        case 1:
            asteroid_.x = 662;
            asteroid_.y = 0;
            asteroid_.xspeed = -a_diagspeed;
            asteroid_.yspeed = a_diagspeed;
            break;
        case 2:
            asteroid_.x = mode_info.XResolution;
            asteroid_.y = 280;
            asteroid_.xspeed = -a_speed;
            asteroid_.yspeed = 0;
            break;
        case 3:
            asteroid_.x = mode_info.XResolution - 100;
            asteroid_.y = mode_info.YResolution;
            asteroid_.xspeed = -a_diagspeed;
            asteroid_.yspeed = -a_diagspeed;
            break;
        case 4:
            asteroid_.x = 385;
            asteroid_.y = mode_info.YResolution;
            asteroid_.xspeed = 0;
            asteroid_.yspeed = -a_speed;
            break;
        case 5:
            asteroid_.x = 65;
            asteroid_.y = mode_info.YResolution;
            asteroid_.xspeed = a_diagspeed;
            asteroid_.yspeed = -a_diagspeed;
            break;
        case 6:
            asteroid_.x = 0;
            asteroid_.y = 280;
            asteroid_.xspeed = a_speed;
            asteroid_.yspeed = 0;
            break;
        case 7:
            asteroid_.x = 100;
            asteroid_.y = 0;
            asteroid_.xspeed = a_diagspeed;
            asteroid_.yspeed = a_diagspeed;
            break;
        default:
            break;
    }
    asteroids[asteroid_index] = asteroid_;
    asteroids[asteroid_index].sprite = asteroid;
    asteroids[asteroid_index].is_destroyed = false;
    asteroid_index++;
}

void update_asteroids() {
    // move asteroids
    for (int i = 0; i < asteroid_index; i++) {
        asteroids[i].x += asteroids[i].xspeed;
        asteroids[i].y += asteroids[i].yspeed;
    }

    // check if asteroid is out of bounds
    for (int i = 0; i < asteroid_index; i++) {
        if (asteroids[i].y < 0 || asteroids[i].y > mode_info.YResolution || asteroids[i].x < 0 || asteroids[i].x > mode_info.XResolution) {
            for (int j = i; j < asteroid_index - 1; j++) {
                asteroids[j] = asteroids[j + 1];
            }
            asteroid_index--;
        }
    }
}

void draw_asteroids() {
    // draw asteroids
    for (int i = 0; i < asteroid_index; i++) {
        draw_sprite(asteroid, asteroids[i].x, asteroids[i].y);
    }
}

void check_collisions() {
    if (gameState == GAME) {
        // check if laser hits asteroid
        for (int i = 0; i < laser_index; i++) {
            for (int j = 0; j < asteroid_index; j++) {
                if (lasers[i].x > asteroids[j].x && lasers[i].x < asteroids[j].x + asteroid->width && lasers[i].y > asteroids[j].y && lasers[i].y < asteroids[j].y + asteroid->height) {
                    score++;
                    lasers[i].is_destroyed = true;
                    asteroids[j].is_destroyed = true;
                }
            }
        }

        // remove destroyed lasers
        for (int i = 0; i < laser_index; i++) {
            if (lasers[i].is_destroyed) {
                for (int j = i; j < laser_index - 1; j++) {
                    lasers[j] = lasers[j + 1];
                }
                laser_index--;
            }
        }

        // remove destroyed asteroids
        for (int i = 0; i < asteroid_index; i++) {
            if (asteroids[i].is_destroyed) {
                for (int j = i; j < asteroid_index - 1; j++) {
                    asteroids[j] = asteroids[j + 1];
                }
                asteroid_index--;
            }
        }

        // check if asteroid hits player
        for (int i = 0; i < asteroid_index; i++) {
            switch (asteroids[i].direction) {
                case 0:
                    if (selected_cannon == 0) {
                        if (asteroids[i].y > 198) {
                            gameState = GAME_OVER;
                            return;
                        }
                    }
                    else {
                        if (asteroids[i].y > 227) {
                            gameState = GAME_OVER;
                            return;
                        }
                    }
                    break;
                case 1:
                    if (selected_cannon == 1) {
                        if (asteroids[i].x < 453 && asteroids[i].y > 225) {
                            gameState = GAME_OVER;
                            return;
                        }
                    }
                    else {
                        if (asteroids[i].x < 442 && asteroids[i].y > 238) {
                            gameState = GAME_OVER;
                            return;
                        }
                    }
                    break;
                case 2:
                    if (selected_cannon == 2) {
                        if (asteroids[i].x < 463) {
                            gameState = GAME_OVER;
                            return;
                        }
                    }
                    else if (asteroids[i].x < 448) {
                        gameState = GAME_OVER;
                        return;
                    }
                    break;
                case 3:
                    if (selected_cannon == 3) {
                        if (asteroids[i].x < 445 && asteroids[i].y < 345) {
                            gameState = GAME_OVER;
                            return;
                        }
                    }
                    else if (asteroids[i].x < 425 && asteroids[i].y < 325) {
                        gameState = GAME_OVER;
                        return;
                    }
                    break;
                case 4:
                    if (selected_cannon == 4) {
                        if (asteroids[i].y < 365) {
                            gameState = GAME_OVER;
                            return;
                        }
                    }
                    else if (asteroids[i].y < 342) {
                        gameState = GAME_OVER;
                        return;
                    }
                    break;
                case 5:
                    if (selected_cannon == 5) {
                        if (asteroids[i].x > 323 && asteroids[i].y < 340) {
                            gameState = GAME_OVER;
                            return;
                        }
                    }
                    else if (asteroids[i].x > 342 && asteroids[i].y < 367) {
                        gameState = GAME_OVER;
                        return;
                    }
                    break;
                case 6:
                    if (selected_cannon == 6) {
                        if (asteroids[i].x > 300) {
                            gameState = GAME_OVER;
                            return;
                        }
                    }
                    else if (asteroids[i].x > 324) {
                        gameState = GAME_OVER;
                        return;
                    }
                    break;
                case 7:
                    if (selected_cannon == 7) {
                        if (asteroids[i].x > 320 && asteroids[i].y > 210) {
                            gameState = GAME_OVER;
                            return;
                        }
                    }
                    else if (asteroids[i].x > 335 && asteroids[i].y > 230) {
                        gameState = GAME_OVER;
                        return;
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

void reset_game() {
    laser_index = 0;
    asteroid_index = 0;
    selected_cannon = 0;
    score = 0;
    freq = 40;
    a_speed = 5;
    a_diagspeed = 3;
    l_speed = 10;
    l_diagspeed = 7;
    ammo = 5;
    out_of_ammo = false;
    powerup_index = 0;
}

bool is_number = false;
bool is_alpha = false;
bool is_special = false;

int char_to_index(char c) {
    if (c >= 'A' && c <= 'Z') {
        is_alpha = true;
        return c - 'A';
    }
    else if (c >= 'a' && c <= 'z') {
        is_alpha = true;
        return c - 'a';
    }
    else if (c >= '0' && c <= '9') {
        is_number = true;
        return c - '0';
    }
    else if (c == '/') {
        is_special = true;
        return 0;
    }
    else if (c == ':') {
        is_special = true;
        return 1;
    }
    return -1;
}

void draw_text(char text[], int x, int y) {
    int x_offset = 0;
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] == ' ') {
            x_offset += 18;
            continue;
        }
        char c = text[i];
        if (char_to_index(c) == -1) {
            printf("Invalid character in text: %c\n", c);
            continue;
        }
        if (is_alpha) {
            draw_sprite(letters[char_to_index(c)], x + x_offset, y);
            x_offset += letters[char_to_index(c)]->width;
            is_alpha = false;
        }
        else if (is_number) {
            draw_sprite(numbers[char_to_index(c)], x + x_offset, y);
            x_offset += numbers[char_to_index(c)]->width;
            is_number = false;
        }
        else if (is_special) {
            draw_sprite(special_chars[char_to_index(c)], x + x_offset, y);
            x_offset += special_chars[char_to_index(c)]->width;
            is_special = false;
        }
        else if (!is_alpha && !is_number && !is_special) {
            printf("Invalid character in text: %c\n", c);
        }
        
    }
}

void draw_score() {
    char score_str[15];
    char points[3];
    sprintf(points, "%d", score);
    strcpy(score_str, "SCORE:");
    strcat(score_str, points);
    draw_text(score_str, mode_info.XResolution/2 - 300, 10);
}

void draw_ammo() {
    char ammo_str[15];
    char ammo_count[3];
    sprintf(ammo_count, "%d", ammo);
    strcpy(ammo_str, "AMMO:");
    strcat(ammo_str, ammo_count);
    strcat(ammo_str, "/5");
    draw_text(ammo_str, mode_info.XResolution/2 + 100, 10);
}

void create_powerup() {
    if (rand() % 500 == 0) {
        int x = 0;
        int y = 0;
        do {
            x = rand() % mode_info.XResolution;
            y = rand() % mode_info.YResolution;
        } while ((x > player->x - (player->width/2)) && (x < player->x + (player->width/2)) && (y > player->y - (player->height/2)) && (y < player->y + (player->height/2)));
        powerups[powerup_index].x = x;
        powerups[powerup_index].y = y;
        int powerup_type = rand() % 2;
        switch (powerup_type) {
            case 0:
                powerups[powerup_index].sprite = bomb;
                powerups[powerup_index].type = 0;
                break;
            case 1:
                powerups[powerup_index].sprite = coin;
                powerups[powerup_index].type = 1;
                break;
            default:
                break;
        }
        powerups[powerup_index].is_consumed = false;
        powerup_index++;
    }
}

void draw_powerups() {
    for (int i = 0; i < powerup_index; i++) {
        draw_sprite(powerups[i].sprite, powerups[i].x, powerups[i].y);
    }
}


void update_powerups() {
    for (int i = 0; i < powerup_index; i++) {
        if (powerups[i].is_consumed) {
            switch (powerups[i].type) {
                case 0:
                    asteroid_index = 0;
                    break;
                case 1:
                    score += 5;
                    break;
                default:
                    break;
            }
            for (int j = i; j < powerup_index - 1; j++) {
                powerups[j] = powerups[j + 1];
            }
            powerup_index--;
        }
    }
}
