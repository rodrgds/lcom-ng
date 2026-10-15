#pragma once

#include <math.h>

#include "../model/tower/TowerConfig.h"
#include "../model/tower/Tower.h"
#include "../model/minion/MinionConfig.h"
#include "../model/minion/Minion.h"
#include "../model/champion/Champion.h"

#include "../model/Box.h"

#include "ChampionController.h"

/**
 * @brief Applies damage to a single tower if the attack box overlaps the tower's bounding box.
 * @param t Tower to hit.
 * @param attack_box Area of effect for the attack.
 * @param dmg Damage to apply if the attack connects.
 */
void towerTakeHit(Tower *t, Box attack_box, int dmg);

/**
 * @brief Advances tower targeting and projectile logic for one tick.
 * For each tower: moves any active projectile toward its target and resolves a hit
 * when within PROJECTILE_HIT_RADIUS; then selects the nearest enemy minion or champion
 * within TOWER_RANGE and fires a new projectile when the attack timer is ready.
 * @param towers Both towers to update.
 * @param minions All active minion waves (potential targets).
 * @param p1 Player-1 champion (potential target for the red tower).
 * @param p2 Player-2 champion (potential target for the blue tower).
 */
void towersUpdate(Towers *towers, Minions *minions, Champion *p1, Champion *p2);

/**
 * @brief Applies damage to the enemy tower if the attack box overlaps it.
 * Only the tower belonging to the team opposite attacker_team is a valid target.
 * @param towers Both towers.
 * @param attack_box Area of effect for the attack.
 * @param damage Damage to apply if the attack connects.
 * @param attacker_team Team of the attacker (used to identify the enemy tower).
 */
void towersTakeHit(Towers *towers, Box attack_box, int damage, Team attacker_team);
