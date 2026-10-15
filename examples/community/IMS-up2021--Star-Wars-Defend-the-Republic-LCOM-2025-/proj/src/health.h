#ifndef HEALTH_H
#define HEALTH_H

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdio.h>

#include "controllers/video/graphics.h"

#include "xpms/health_enemy/1.xpm"
#include "xpms/health_enemy/2.xpm"
#include "xpms/health_enemy/3.xpm"
#include "xpms/health_enemy/4.xpm"
#include "xpms/health_enemy/5.xpm"
#include "xpms/health_enemy/6.xpm"
#include "xpms/health_enemy/7.xpm"
#include "xpms/health_enemy/8.xpm"
#include "xpms/health_enemy/9.xpm"
#include "xpms/health_enemy/10.xpm"
#include "xpms/health_enemy/11.xpm"

#include "xpms/health_player/1.xpm"
#include "xpms/health_player/2.xpm"
#include "xpms/health_player/3.xpm"
#include "xpms/health_player/4.xpm"
#include "xpms/health_player/5.xpm"
#include "xpms/health_player/6.xpm"
#include "xpms/health_player/7.xpm"
#include "xpms/health_player/8.xpm"
#include "xpms/health_player/9.xpm"
#include "xpms/health_player/10.xpm"
#include "xpms/health_player/11.xpm"

#define MAX_HEALTH 100

typedef struct {
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;
    unsigned int health;
    uint8_t *sprite;
} Player;

extern Player *player_health;
extern Player *enemy_health;


bool init_healthbar(void);
Player *create_healthbar(unsigned int x, unsigned int y, bool is_player);
void draw_healthbar(Player *healthbar, bool is_player);
void update_healthbar(Player *healthbar, bool is_player);

#endif // HEALTH_H
