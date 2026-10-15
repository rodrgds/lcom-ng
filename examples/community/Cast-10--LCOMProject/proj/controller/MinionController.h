#pragma once

#include <stdlib.h>

#include "../model/minion/Minion.h"
#include "../model/minion/MinionConfig.h"
#include "../model/tower/Tower.h"
#include "../model/champion/Champion.h"
#include "../model/Box.h"

/**
 * @brief Spawns the initial wave pair and positions minions at the lane starting points.
 * @param minions Minions container to initialise.
 * @param map_w Map width in pixels (used to position the red-team starting wave).
 */
void minionsControllerInit(Minions *minions, int map_w);

/**
 * @brief Advances all minion AI for one tick.
 * Each second, decrements the spawn timer and spawns a new wave pair when it reaches zero.
 * For each active minion: selects the nearest attackable target
 * (enemy minion > enemy champion > enemy tower) within ATTACK_RANGE, attacks it,
 * or moves toward the enemy base while resolving collisions.
 * Dead waves are cleaned up after processing.
 * @param minions All active minion waves.
 * @param towers Both towers (attack and collision targets).
 * @param p1 Player-1 champion (attack and collision target).
 * @param p2 Player-2 champion (attack and collision target).
 * @param map_w Map width in pixels.
 * @param second_elapsed True if a full second has passed since the last update.
 */
void minionsUpdate(Minions *minions, Towers *towers, Champion *p1, Champion *p2, int map_w, bool second_elapsed);

/**
 * @brief Applies damage to all minions whose bounding box overlaps the given attack box.
 * Only damages minions belonging to the team opposite to attacker_team.
 * @param minions All active minion waves.
 * @param attack_box Area of effect for the attack.
 * @param damage Damage dealt to each hit minion.
 * @param attacker_team Team of the attacker (used to identify enemy minions).
 * @return Number of minions killed by this attack.
 */
int minionsTakeHit(Minions *minions, Box attack_box, int damage, Team attacker_team);
