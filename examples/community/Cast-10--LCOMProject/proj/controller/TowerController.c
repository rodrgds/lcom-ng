#include "TowerController.h"

/**
 * @brief Tests whether a minion's centre is within the tower's attack range.
 * @param t Source tower.
 * @param m Minion to test.
 * @return true if the minion is within TOWER_RANGE pixels of the tower centre.
 */
static bool minionInRange(Tower *t, Minion *m) {
    int tcx = position_px(t->pos) + TOWER_W / 2;
    int tcy = position_py(t->pos) + TOWER_H / 2;
    int dx = tcx - (position_px(m->pos) + MINION_W / 2);
    int dy = tcy - (position_py(m->pos) + MINION_H / 2);
    return (dx * dx + dy * dy) <= TOWER_RANGE * TOWER_RANGE;
}

/**
 * @brief Tests whether a living champion's centre is within the tower's attack range.
 * @param t Source tower.
 * @param champ Champion to test (dead or NULL champions return false).
 * @return true if the champion is within TOWER_RANGE pixels of the tower centre.
 */
static bool championInRange(Tower *t, Champion *champ) {
    if (!champ || championGetHp(champ) <= 0) return false;
    int tcx = position_px(t->pos) + TOWER_W / 2;
    int tcy = position_py(t->pos) + TOWER_H / 2;
    Position p = championGetPos(champ);
    int dx = tcx - (position_px(p) + championGetWidth(champ) / 2);
    int dy = tcy - (position_py(p) + championGetHeight(champ) / 2);
    return (dx * dx + dy * dy) <= TOWER_RANGE * TOWER_RANGE;
}

/**
 * @brief Finds the closest living enemy minion within TOWER_RANGE of the tower.
 * @param t Source tower (team determines which minion waves are considered enemies).
 * @param minions All active minion waves.
 * @return Pointer to the nearest enemy minion, or NULL if none are in range.
 */
static Minion *findNearestMinion(Tower *t, Minions *minions) {
    int tcx = position_px(t->pos) + TOWER_W / 2;
    int tcy = position_py(t->pos) + TOWER_H / 2;
    MinionWave *enemy = (t->team == TEAM_BLUE) ? minions->red_waves : minions->blue_waves;
    Minion *best = NULL;
    int min_dist = TOWER_RANGE * TOWER_RANGE;

    for (int w = 0; w < MAX_WAVES; w++) {
        if (!enemy[w].active) continue;
        for (int i = 0; i < MINION_COUNT; i++) {
            Minion *m = &enemy[w].minions[i];
            if (m->state == MINION_DEAD) continue;
            int dx = tcx - (position_px(m->pos) + MINION_W / 2);
            int dy = tcy - (position_py(m->pos) + MINION_H / 2);
            int dist = dx * dx + dy * dy;
            if (dist < min_dist) { min_dist = dist; best = m; }
        }
    }
    return best;
}

/**
 * @brief Moves an active projectile toward its target and resolves a hit on contact.
 * Deactivates the projectile if its target dies before impact.
 * @param p Projectile to update.
 * @param team Team that fired the projectile (determines tip offset).
 * @param friend_champ Friendly champion used as the attacker when dealing champion damage.
 * @param enemy_champ Enemy champion target when targeting_champion is true.
 */
static void updateProjectile(TowerProjectile *p, Team team, Champion *friend_champ, Champion *enemy_champ) {
    if (!p->active) return;

    float tx, ty;
    if (p->minion_target != NULL) {
        if (p->minion_target->state == MINION_DEAD) { p->active = false; return; }
        tx = p->minion_target->pos.x + MINION_W / 2.0f;
        ty = p->minion_target->pos.y + MINION_H / 2.0f;
    } else {
        if (!enemy_champ || championGetHp(enemy_champ) <= 0) { p->active = false; return; }
        Position cp = championGetPos(enemy_champ);
        tx = cp.x + championGetWidth(enemy_champ) / 2.0f;
        ty = cp.y + championGetHeight(enemy_champ) / 2.0f;
    }

    float tip_x = p->pos.x + (team == TEAM_BLUE ? PROJECTILE_TIP_BLUE_X : PROJECTILE_TIP_RED_X);
    float tip_y = p->pos.y + PROJECTILE_TIP_Y;

    float dx = tx - tip_x;
    float dy = ty - tip_y;
    float dist = sqrtf(dx * dx + dy * dy);

    if (dist <= PROJECTILE_HIT_RADIUS) {
        if (p->minion_target != NULL) {
            p->minion_target->hp -= p->damage;
        } else {
            championDecreaseHp(friend_champ, enemy_champ, p->damage);
        }
        p->active = false;
    } else {
        p->pos.x += (dx / dist) * PROJECTILE_SPEED;
        p->pos.y += (dy / dist) * PROJECTILE_SPEED;
    }
}

/**
 * @brief Runs one tick of logic for a single tower.
 * Updates the in-flight projectile, increments the attack timer, revalidates the
 * current target, acquires a new one when needed, and fires a new projectile
 * when the attack timer reaches TOWER_ATTACK_SPEED.
 * @param t Tower to update.
 * @param minions All active minion waves (enemy minions are potential targets).
 * @param friend_champ Friendly champion passed through to the projectile updater.
 * @param enemy_champ Enemy champion (potential target when no minion is in range).
 */
static void updateTower(Tower *t, Minions *minions, Champion *friend_champ, Champion *enemy_champ) {
    if (t->hp <= 0) return;

    updateProjectile(&t->projectile, t->team, friend_champ, enemy_champ);

    t->attack_timer++;
    if (t->attack_timer < TOWER_ATTACK_SPEED) return;
    t->attack_timer = 0;

    if (t->minion_target != NULL) {
        if (t->minion_target->state == MINION_DEAD || !minionInRange(t, t->minion_target)) {
            t->minion_target = NULL;
            t->targeting_champion = false;
        }
    }

    if (t->targeting_champion && !championInRange(t, enemy_champ))
        t->targeting_champion = false;

    if (t->minion_target == NULL && !t->targeting_champion) {
        t->minion_target = findNearestMinion(t, minions);
        if (t->minion_target == NULL)
            t->targeting_champion = championInRange(t, enemy_champ);
    }

    if (t->minion_target == NULL && !t->targeting_champion) return;

    t->projectile.pos.x = (t->team == TEAM_BLUE)
        ? position_px(t->pos) + TOWER_W - PROJECTILE_TIP_BLUE_X
        : position_px(t->pos) - PROJECTILE_TIP_RED_X;
    t->projectile.pos.y = position_py(t->pos) - PROJECTILE_TIP_Y + 10;
    t->projectile.minion_target    = t->minion_target;
    t->projectile.targeting_champion = t->targeting_champion;
    t->projectile.damage = TOWER_DMG;
    t->projectile.active = true;
}

void towerTakeHit(Tower *t, Box attack_box, int dmg) {
    if (t->hp <= 0) return;
    Box tower_box = {position_px(t->pos), position_py(t->pos), TOWER_W, TOWER_H};
    if (boxesOverlap(attack_box, tower_box))
        t->hp -= dmg;
}

void towersUpdate(Towers *towers, Minions *minions, Champion *p1, Champion *p2) {
    updateTower(&towers->blue, minions, p1, p2);
    updateTower(&towers->red,  minions, p2, p1);
}

void towersTakeHit(Towers *towers, Box attack_box, int damage, Team attacker_team) {
    Tower *target = (attacker_team == TEAM_RED) ? &towers->blue : &towers->red;
    if (target->hp <= 0) return;
    if (boxesOverlap(attack_box, towerGetBox(target)))
        target->hp -= damage;
}
