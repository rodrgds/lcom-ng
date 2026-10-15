#include "MinionController.h"
#include "ChampionController.h"

/**
 * @brief Initialises a MinionWave with fresh minions placed at the team's lane start.
 * @param wave Wave to initialise.
 * @param team Team the minions belong to (determines starting X position).
 * @param map_w Map width in pixels (used to position red-team minions).
 */
static void initWave(MinionWave *wave, Team team, int map_w) {
    wave->active = true;
    for (int i = 0; i < MINION_COUNT; i++) {
        wave->minions[i].team = team;
        wave->minions[i].hp = MINION_HP;
        wave->minions[i].state = MINION_MOVING;
        wave->minions[i].attack_timer = 0;
        wave->minions[i].pos.y = (float)(MINION_LANE_Y + i * (MINION_H + MINION_SPACING));
        wave->minions[i].pos.x = (float)((team == TEAM_BLUE) ? 0 : map_w - MINION_W);
    }
}

/**
 * @brief Spawns one new wave for each team, using the first available inactive slot.
 * Does nothing if either team has no free wave slots.
 * @param minions Minions container to spawn waves into.
 * @param map_w Map width in pixels (passed through to initWave).
 */
static void spawnWavePair(Minions *minions, int map_w) {
    int ri = -1, bi = -1;
    for (int i = 0; i < MAX_WAVES; i++) {
        if (!minions->red_waves[i].active && ri < 0) ri = i;
        if (!minions->blue_waves[i].active && bi < 0) bi = i;
    }
    if (ri < 0 || bi < 0) return;

    initWave(&minions->red_waves[ri], TEAM_RED, map_w);
    minions->red_count++;
    initWave(&minions->blue_waves[bi], TEAM_BLUE, map_w);
    minions->blue_count++;
}

/**
 * @brief Returns true if every minion in the wave has state MINION_DEAD.
 * @param wave Wave to check.
 * @return true when the wave is exhausted.
 */
static bool waveAllDead(MinionWave *wave) {
    for (int i = 0; i < MINION_COUNT; i++)
        if (wave->minions[i].state != MINION_DEAD) return false;
    return true;
}

/**
 * @brief Returns the axis-aligned bounding box of a minion at its current position.
 * @param m Source minion.
 * @return Bounding box covering the minion sprite.
 */
static Box minionBox(Minion *m) {
    return (Box){position_px(m->pos), position_py(m->pos), MINION_W, MINION_H};
}

/**
 * @brief Tests whether a proposed bounding box would overlap any other living minion.
 * @param self The minion being moved (excluded from the test).
 * @param next_box Proposed bounding box after the move.
 * @param minions All active minion waves to test against.
 * @return true if next_box intersects another living minion's box.
 */
static bool minionOverlapsMinions(Minion *self, Box next_box, Minions *minions) {
    MinionWave *all_waves[2] = {minions->red_waves, minions->blue_waves};
    for (int t = 0; t < 2; t++) {
        for (int w = 0; w < MAX_WAVES; w++) {
            if (!all_waves[t][w].active) continue;
            for (int i = 0; i < MINION_COUNT; i++) {
                Minion *other = &all_waves[t][w].minions[i];
                if (other == self || other->state == MINION_DEAD) continue;
                if (boxesOverlap(next_box, minionBox(other))) return true;
            }
        }
    }
    return false;
}

/**
 * @brief Tests whether a proposed bounding box would overlap a living champion's collision box.
 * @param next_box Proposed bounding box after the move.
 * @param c Champion to test against (dead or NULL returns false).
 * @return true if next_box intersects the champion's collision box.
 */
static bool minionOverlapsChampion(Box next_box, Champion *c) {
    if (!c || championGetRespawnTimer(c) != ALIVE) return false;
    return boxesOverlap(next_box, championGetCollisionBox(c));
}

/**
 * @brief Tests whether two minions are within ATTACK_RANGE of each other (centre to centre).
 * @param a First minion.
 * @param b Second minion.
 * @return true if the centres are within ATTACK_RANGE pixels.
 */
static bool inAttackRange(Minion *a, Minion *b) {
    int dx = (position_px(a->pos) + MINION_W / 2) - (position_px(b->pos) + MINION_W / 2);
    int dy = (position_py(a->pos) + MINION_H / 2) - (position_py(b->pos) + MINION_H / 2);
    return (dx * dx + dy * dy) <= (ATTACK_RANGE * ATTACK_RANGE);
}

/**
 * @brief Tests whether a minion is within ATTACK_RANGE of a tower (centre to centre).
 * @param m Attacking minion.
 * @param t Target tower.
 * @return true if the minion centre is within ATTACK_RANGE pixels of the tower centre.
 */
static bool inTowerAttackRange(Minion *m, Tower *t) {
    int dx = (position_px(m->pos) + MINION_W / 2) - (position_px(t->pos) + TOWER_W / 2);
    int dy = (position_py(m->pos) + MINION_H / 2) - (position_py(t->pos) + TOWER_H / 2);
    return (dx * dx + dy * dy) <= (ATTACK_RANGE * ATTACK_RANGE);
}

/**
 * @brief Tests whether a minion is within ATTACK_RANGE of a champion (centre to centre).
 * @param m Attacking minion.
 * @param c Target champion.
 * @return true if the minion centre is within ATTACK_RANGE pixels of the champion centre.
 */
static bool inChampionRange(Minion *m, Champion *c) {
    Position cp = championGetPos(c);
    int cx = position_px(cp) + championGetWidth(c) / 2;
    int cy = position_py(cp) + championGetHeight(c) / 2;
    int dx = (position_px(m->pos) + MINION_W / 2) - cx;
    int dy = (position_py(m->pos) + MINION_H / 2) - cy;
    return (dx * dx + dy * dy) <= (ATTACK_RANGE * ATTACK_RANGE);
}


/**
 * @brief Finds the closest living enemy minion across all active waves.
 * @param m Source minion (team determines which waves are enemies).
 * @param minions All active minion waves.
 * @return Pointer to the nearest living enemy minion, or NULL if none exist.
 */
static Minion *findNearestEnemy(Minion *m, Minions *minions) {
    MinionWave *enemy_waves = (m->team == TEAM_RED) ? minions->blue_waves : minions->red_waves;
    Minion *nearest = NULL;
    int min_dist = INT32_MAX;

    for (int w = 0; w < MAX_WAVES; w++) {
        if (!enemy_waves[w].active) continue;
        for (int j = 0; j < MINION_COUNT; j++) {
            Minion *e = &enemy_waves[w].minions[j];
            if (e->state == MINION_DEAD) continue;
            int dx = (position_px(m->pos) + MINION_W / 2) - (position_px(e->pos) + MINION_W / 2);
            int dy = (position_py(m->pos) + MINION_H / 2) - (position_py(e->pos) + MINION_H / 2);
            int dist = dx * dx + dy * dy;
            if (dist < min_dist) {
                min_dist = dist;
                nearest = e; 
            }
        }
    }
    return nearest;
}

/**
 * @brief Runs one tick of AI for a single minion.
 * Priority: attack nearest enemy minion in range > attack enemy champion in range >
 * attack enemy tower in range > move toward the enemy base.
 * Collision against other minions and champions is checked before applying movement.
 * @param m Minion to update.
 * @param minions All active minion waves (used for targeting and collision).
 * @param enemy_tower Enemy team's tower (attack target of last resort).
 * @param ally_champ Friendly champion (used as the attacker when dealing champion damage).
 * @param enemy_champ Enemy champion (attack and collision target).
 * @param dir Horizontal direction of travel (-1 for red, 1 for blue).
 */
static void updateMinion(Minion *m, Minions *minions, Tower *enemy_tower, Champion *ally_champ, Champion *enemy_champ, int dir) {
    if (m->state == MINION_DEAD) return;
    if (m->hp <= 0) { m->state = MINION_DEAD; return; }

    Minion *target = findNearestEnemy(m, minions);

    if (target != NULL && inAttackRange(m, target)) {
        m->state = MINION_ATTACKING;
        m->attack_timer++;
        if (m->attack_timer >= MINION_ATTACK_SPEED) {
            target->hp -= MINION_ATTACK_DMG;
            m->attack_timer = 0;
        }
        return;
    }

    if (enemy_champ && championGetRespawnTimer(enemy_champ) == ALIVE && inChampionRange(m, enemy_champ)) {
        m->state = MINION_ATTACKING;
        m->attack_timer++;
        if (m->attack_timer >= MINION_ATTACK_SPEED) {
            championDecreaseHp(ally_champ, enemy_champ, MINION_ATTACK_DMG);
            m->attack_timer = 0;
        }
        return;
    }

    if (enemy_tower && enemy_tower->hp > 0 && inTowerAttackRange(m, enemy_tower)) {
        m->state = MINION_ATTACKING;
        m->attack_timer++;
        if (m->attack_timer >= MINION_ATTACK_SPEED) {
            enemy_tower->hp -= MINION_ATTACK_DMG;
            m->attack_timer = 0;
        }
        return;
    }

    m->state = MINION_MOVING;
    m->attack_timer = 0;

    Position next = m->pos;
    position_move(&next, dir, 0, MINION_SPEED);
    Box next_box = (Box){position_px(next), position_py(next), MINION_W, MINION_H};
    if (!minionOverlapsMinions(m, next_box, minions) &&
        !minionOverlapsChampion(next_box, ally_champ) &&
        !minionOverlapsChampion(next_box, enemy_champ)) {
        m->pos = next;
    }
}

void minionsControllerInit(Minions *minions, int map_w) {
    spawnWavePair(minions, map_w);
}

void minionsUpdate(Minions *minions, Towers *towers, Champion *p1, Champion *p2, int map_w, bool second_elapsed) {
    if (second_elapsed) {
        minions->spawn_timer--;
        if (minions->spawn_timer <= 0) {
            minions->spawn_timer = SPAWN_SECONDS;
            spawnWavePair(minions, map_w);
        }
    }

    for (int w = 0; w < MAX_WAVES; w++) {
        if (!minions->red_waves[w].active) continue;
        for (int i = 0; i < MINION_COUNT; i++)
            updateMinion(&minions->red_waves[w].minions[i], minions, towers ? &towers->blue : NULL, p2, p1, -1);
        if (waveAllDead(&minions->red_waves[w])) {
            minions->red_waves[w].active = false;
            minions->red_count--;
        }
    }

    for (int w = 0; w < MAX_WAVES; w++) {
        if (!minions->blue_waves[w].active) continue;
        for (int i = 0; i < MINION_COUNT; i++)
            updateMinion(&minions->blue_waves[w].minions[i], minions, towers ? &towers->red : NULL, p1, p2, +1);
        if (waveAllDead(&minions->blue_waves[w])) {
            minions->blue_waves[w].active = false;
            minions->blue_count--;
        }
    }
}

int minionsTakeHit(Minions *minions, Box attack_box, int damage, Team attacker_team) {
    MinionWave *enemy_waves = (attacker_team == TEAM_RED) ? minions->blue_waves : minions->red_waves;
    int kills = 0;
    for (int w = 0; w < MAX_WAVES; w++) {
        if (!enemy_waves[w].active) continue;
        for (int i = 0; i < MINION_COUNT; i++) {
            Minion *m = &enemy_waves[w].minions[i];
            if (m->state == MINION_DEAD) continue;
            Box mb = {position_px(m->pos), position_py(m->pos), MINION_W, MINION_H};
            if (boxesOverlap(attack_box, mb)) {
                bool was_alive = m->hp > 0;
                m->hp -= damage;
                if (was_alive && m->hp <= 0) kills++;
            }
        }
    }
    return kills;
}
