#include "Tower.h"

Box towerGetBox(Tower *tower) {
    return (Box){position_px(tower->pos), position_py(tower->pos), TOWER_W, TOWER_H};
}

void towersInit(Towers *towers, int map_w) {
    towers->blue.pos = (Position){TOWER_X_MARGIN, TOWER_LANE_Y};
    towers->blue.team = TEAM_BLUE;
    towers->blue.hp = TOWER_HP;
    towers->blue.attack_timer = 0;
    towers->blue.minion_target = NULL;
    towers->blue.targeting_champion = false;
    towers->blue.projectile.active = false;

    towers->red.pos = (Position){map_w - TOWER_W - TOWER_X_MARGIN, TOWER_LANE_Y};
    towers->red.team = TEAM_RED;
    towers->red.hp = TOWER_HP;
    towers->red.attack_timer = 0;
    towers->red.minion_target = NULL;
    towers->red.targeting_champion = false;
    towers->red.projectile.active = false;
}
