#pragma once

#include <stdlib.h>
#include <stdbool.h>

#include "../model/champion/ChampionConfig.h"
#include "../model/champion/Champion.h"
#include "../model/minion/Minion.h"
#include "../model/tower/Tower.h"

#include "../model/Box.h"

/**
 * @brief Advances per-frame champion logic: respawn countdown and animation-state resets.
 * Decrements the respawn timer once per second when the champion is dead and
 * calls championRespawn() when the timer reaches zero.
 * Resets the champion to CHAMPION_IDLE when an attack animation finishes.
 * @param champion Champion to update.
 * @param animationLooped True if the champion's current animation completed a loop this tick.
 * @param second_elapsed True if a full second has passed since the last update.
 */
void championUpdate(Champion *champion, bool animationLooped, bool second_elapsed);

/**
 * @brief Attempts to move a champion by (dx, dy) at its current speed.
 * Resolves collisions against the map boundaries, the other champion,
 * all towers, and all active minions. Updates the champion's direction
 * and sets its state to CHAMPION_MOVING on a successful move.
 * @param champion1 Champion to move.
 * @param champion2 The opposing champion (collision target).
 * @param towers Both towers (collision targets).
 * @param minions All active minion waves (collision targets).
 * @param dx Horizontal direction (-1, 0, or 1).
 * @param dy Vertical direction (-1, 0, or 1).
 * @param map_w Map width in pixels.
 * @param map_h Map height in pixels.
 */
void championMove(Champion *champion1, Champion *champion2, Towers *towers, Minions *minions, int dx, int dy, int map_w, int map_h);

/**
 * @brief Initiates an attack if the champion is not already attacking.
 * Sets the champion state to CHAMPION_ATTACKING.
 * @param champion Champion to attack with.
 * @return true if the attack was started, false if already attacking.
 */
bool championAttack(Champion *champion);

/**
 * @brief Tests whether the attacker's attack box currently overlaps the target's collision box.
 * @param attacker Attacking champion.
 * @param target Defending champion.
 * @return true if the hit lands.
 */
bool championHits(Champion *attacker, Champion *target);

/**
 * @brief Applies damage from the attacker to the target.
 * On kill, starts the target's respawn timer and awards the attacker points.
 * @param attacker Champion dealing the damage.
 * @param target Champion receiving the damage.
 * @param dmg Amount of damage to deal.
 */
void championDecreaseHp(Champion *attacker, Champion *target, int dmg);

/**
 * @brief Increases the champion's maximum and current HP by the upgrade step.
 * @param champion Champion to upgrade.
 */
void championUpgradeHp(Champion *champion);

/**
 * @brief Increases the champion's attack damage by the upgrade step.
 * @param champion Champion to upgrade.
 */
void championUpgradeDamage(Champion *champion);

/**
 * @brief Increases the champion's movement speed by the upgrade step.
 * @param champion Champion to upgrade.
 */
void championUpgradeSpeed(Champion *champion);
