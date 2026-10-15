#include <lcom/lcf.h>
#include <lcom/lab5.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include "video.h"
#include "projectiles.h"
#include "sprites.h"
#include "i8042.h"
#include "alien.h"
#include "score.h"
#include "player.h"

extern uint8_t* spaceship_projectile_sprite;
extern uint8_t* spaceship_projectile_black_sprite;
extern uint8_t* alien_projectile_sprite;
extern uint8_t* alien_projectile_black_sprite;
extern xpm_image_t img_spaceship_projectile_sprite;
extern xpm_image_t img_spaceship_projectile_black_sprite;
extern xpm_image_t img_alien_projectile_sprite;
extern xpm_image_t img_alien_projectile_black_sprite;
extern vbe_mode_info_t vmi_p;
bool cooldown = false;

#define MAX_PROJECTILES 10

int projectile_count = 0;
Projectile* projectiles[MAX_PROJECTILES];

int draw_spaceship_projectile(uint16_t x, uint16_t y) {
  if (draw_xpm(spaceship_projectile_sprite, img_spaceship_projectile_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int delete_spaceship_projectile(uint16_t x, uint16_t y) {
  if (draw_xpm(spaceship_projectile_black_sprite, img_spaceship_projectile_black_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_alien_projectile(uint16_t x, uint16_t y) {
  if (draw_xpm(alien_projectile_sprite, img_alien_projectile_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int delete_alien_projectile(uint16_t x, uint16_t y) {
  if (draw_xpm(alien_projectile_black_sprite, img_alien_projectile_black_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

//projectile collision
bool check_collision(Projectile* projectile, int* alien_index) {
  int proj_x = calc_x(projectile);
  int proj_y = calc_y(projectile);

  if (projectile->type == PLAYER_PROJECTILE) {
    //check collision with aliens
    for (int i = 0; i < ALIEN_COUNT; i++) {
      if (aliens[i].alive) {
        if (proj_x >= aliens[i].x && proj_x <= aliens[i].x + 60 &&
            proj_y >= aliens[i].y && proj_y <= aliens[i].y + 45) {
          *alien_index = i;
          return true;
        }
      }
    }
  } else if (projectile->type == ALIEN_PROJECTILE) {
    //check collision with player
    if (proj_x >= player_x && proj_x <= player_x + 60 && 
        proj_y >= player_y && proj_y <= player_y + 45) {
      return true;
    }
  }
  return false;
}

void move_projectiles() {
  for (int i = 0; i < projectile_count; ) {
    if (projectiles[i] != NULL) {
      move_projectile(projectiles[i]);
      int alien_index;
      if (check_collision(projectiles[i], &alien_index)) {
        int x = calc_x(projectiles[i]);
        int y = calc_y(projectiles[i]);
        
        // Clear the projectile from the screen
        if (projectiles[i]->type == PLAYER_PROJECTILE) {
          delete_spaceship_projectile(x, y);
        } else if (projectiles[i]->type == ALIEN_PROJECTILE) {
          delete_alien_projectile(x, y);
        }

        if (projectiles[i]->type == PLAYER_PROJECTILE) {
          //collision detected with alien, mark alien as dead, remove projectile from screen
          set_alien_dead(alien_index);
      
          update_score(&game_score, alien_score_value);
        } else if (projectiles[i]->type == ALIEN_PROJECTILE) {
          //collision detected with player, decrement lives
          decrement_lives(&game_score);
        }

        // Free the projectile memory
        free(projectiles[i]);

        //shift remaining projectiles
        for (int j = i; j < projectile_count - 1; j++) {
          projectiles[j] = projectiles[j + 1];
        }
        projectile_count--;

        continue;
      }

      if (projectiles[i]->t > 0 && 
          (calc_x(projectiles[i]) < 30 || calc_x(projectiles[i]) > (vmi_p.XResolution - 30) || 
           calc_y(projectiles[i]) < 30 || calc_y(projectiles[i]) > (vmi_p.YResolution - 30))) {

        int x = calc_x(projectiles[i]);
        int y = calc_y(projectiles[i]);
        
        // Clear the projectile from the screen
        if (projectiles[i]->type == PLAYER_PROJECTILE) {
          delete_spaceship_projectile(x, y);
        } else if (projectiles[i]->type == ALIEN_PROJECTILE) {
          delete_alien_projectile(x, y);
        }

        // Free the projectile memory
        free(projectiles[i]);

        //shift the remaining projectiles
        for (int j = i; j < projectile_count - 1; j++) {
          projectiles[j] = projectiles[j + 1];
        }
        projectile_count--;

        continue; 
      }
    }
    
    i++; //next projectile
  }
}

void move_projectile(Projectile* projectile) {
  int x = calc_x(projectile);
  int y = calc_y(projectile);
  
  if (projectile->type == PLAYER_PROJECTILE) {
    delete_spaceship_projectile(x, y);
  } else if (projectile->type == ALIEN_PROJECTILE) {
    delete_alien_projectile(x, y);
  }

  projectile->t += 1;
  
  x = calc_x(projectile);
  y = calc_y(projectile);

  if (x < vmi_p.XResolution - 30 && x > 30 && y < vmi_p.YResolution - 30 && y > 30) {
    if (projectile->type == PLAYER_PROJECTILE) {
      draw_spaceship_projectile(x, y);
    } else if (projectile->type == ALIEN_PROJECTILE) {
      draw_alien_projectile(x, y);
    }
  }
}

int calc_x(Projectile* projectile) {
  int x = projectile->xi + projectile->speed * cos(atan2((projectile->yf - projectile->yi), (projectile->xf - projectile->xi))) * projectile->t;
  return x;
}

int calc_y(Projectile* projectile) {
  int y = projectile->yi + projectile->speed * sin(atan2((projectile->yf - projectile->yi), (projectile->xf - projectile->xi))) * projectile->t;
  return y;
}

void create_projectile(int xi, int yi, int xf, int yf, int16_t speed, ProjectileType type) {
  if (projectile_count < MAX_PROJECTILES) {
    Projectile* new_projectile = (Projectile*)malloc(sizeof(Projectile));
    if (new_projectile == NULL) {
      printf("Memory allocation failed\n");
      return;
    }
    
    new_projectile->t = 0;
    new_projectile->xi = xi;
    new_projectile->xf = xf;
    new_projectile->yi = yi;
    new_projectile->yf = yf;
    new_projectile->speed = speed;
    new_projectile->type = type;
    
    projectiles[projectile_count] = new_projectile;

    int x = calc_x(new_projectile);
    int y = calc_y(new_projectile);

    if (type == PLAYER_PROJECTILE) {
      draw_spaceship_projectile(x, y);
    } else if (type == ALIEN_PROJECTILE) {
      draw_alien_projectile(x, y);
    }
    projectile_count++;
  } else {
    printf("Out of bullets\n");
  }
}
