#include "ChampionController.h"

void championUpdate(Champion *champion, bool animationLooped, bool second_elapsed) {
    if (champion == NULL) {
        return;
    }

    if (championGetHp(champion) <= 0) {
        if (second_elapsed) {
            int timer = championGetRespawnTimer(champion) - 1;
            championSetRespawnTimer(champion, timer);
            if (timer == 0) {
                championSetRespawnTimer(champion, ALIVE);
                championRespawn(champion);
            }
        }
        return;
    }

    ChampionState state = championGetState(champion);
    if ((state == CHAMPION_ATTACKING || state == CHAMPION_MOVING) && animationLooped) {
        championSetState(champion, CHAMPION_IDLE);
    }
}

void championMove(Champion *champion1, Champion *champion2, Towers *towers, Minions *minions, int dx, int dy, int map_w, int map_h) {
  if (champion1 == NULL || championGetHp(champion1) <= 0) {
    return;
  }

  if (dx != 0 || dy != 0) {
    Direction dir;
    if (dx == 0 && dy < 0)
      dir = NORTH;
    else if (dx == 0 && dy > 0)
      dir = SOUTH;
    else if (dx > 0 && dy == 0)
      dir = EAST;
    else if (dx < 0 && dy == 0)
      dir = WEST;
    else if (dx > 0 && dy < 0)
      dir = NORTHEAST;
    else if (dx < 0 && dy < 0)
      dir = NORTHWEST;
    else if (dx > 0 && dy > 0)
      dir = SOUTHEAST;
    else
      dir = SOUTHWEST;

    championSetDir(champion1, dir);
    if (championGetState(champion1) == CHAMPION_IDLE) {
      championSetState(champion1, CHAMPION_MOVING);
    }
  }

  Position pos = championGetPos(champion1);
  Position next = pos;
  position_move(&next, dx, dy, championGetSpeed(champion1));
  int nx = position_px(next);
  int ny = position_py(next);
  int w = championGetWidth(champion1);
  int h = championGetHeight(champion1);

  if (nx < 0 || nx + w > map_w || ny < 0 || ny + h > map_h) {
    return;
  }

  Box cb = championGetCollisionBox(champion1);
  cb.x += nx - position_px(pos);
  cb.y += ny - position_py(pos);

  if (champion2 != NULL && championGetHp(champion2) > 0) {
    if (boxesOverlap(cb, championGetCollisionBox(champion2)))
      return;
  }

  if ((towers->blue.hp > 0 && boxesOverlap(cb, towerGetBox(&towers->blue))) ||
      (towers->red.hp > 0 && boxesOverlap(cb, towerGetBox(&towers->red))))
    return;

  if (minions != NULL) {
    MinionWave *all_waves[2] = {minions->red_waves, minions->blue_waves};
    for (int t = 0; t < 2; t++) {
      for (int w = 0; w < MAX_WAVES; w++) {
        if (!all_waves[t][w].active)
          continue;
        for (int i = 0; i < MINION_COUNT; i++) {
          Minion *m = &all_waves[t][w].minions[i];
          if (m->state == MINION_DEAD)
            continue;
          Box mb = {position_px(m->pos), position_py(m->pos), MINION_W,
                    MINION_H};
          if (boxesOverlap(cb, mb))
            return;
        }
      }
    }
  }

  championSetPos(champion1, next);
}

bool championAttack(Champion *champion) {
    if (champion == NULL || championGetState(champion) == CHAMPION_ATTACKING) {
        return false;
    }
    championSetState(champion, CHAMPION_ATTACKING);
    return true;
}

bool championHits(Champion *attacker, Champion *target) {
    if (attacker == NULL || target == NULL) {
        return false;
    }
    if (championGetHp(attacker) <= 0 || championGetHp(target) <= 0) {
        return false;
    }
    return championAttackHits(attacker, target);
}

void championDecreaseHp(Champion *attacker, Champion *target, int dmg) {
    if (target == NULL) {
        return;
    }
    championAddHp(target, -dmg);
    if (championGetHp(target) <= 0) {
        championSetRespawnTimer(target, RESPAWN_SECONDS);
        championClearPosition(target);
        championUpdatePoints(attacker, 2);
    }
}

void championUpgradeHp(Champion *champion) {
    if (champion == NULL) {
        return;
    }
    championAddMaxHp(champion, 10);
}

void championUpgradeDamage(Champion *champion) {
    if (champion == NULL) {
        return;
    }
    championAddAttackDmg(champion, 5);
}

void championUpgradeSpeed(Champion *champion) {
    if (champion == NULL) {
        return;
    }
    championAddSpeed(champion, 1);
}
