#pragma once

#include <stdlib.h>

#include "ChampionConfig.h"

#include "../Position.h"
#include "../Box.h"

/**
 * @brief Eight-way movement/facing direction.
 */
typedef enum {
    NORTH, /**< Up. */
    SOUTH, /**< Down. */
    EAST, /**< Right. */
    WEST, /**< Left. */
    NORTHEAST, /**< Up-right. */
    NORTHWEST, /**< Up-left. */
    SOUTHEAST, /**< Down-right. */
    SOUTHWEST /**< Down-left. */
} Direction;

/**
 * @brief Team identifier.
 */
typedef enum {
    TEAM_RED, /**< Red team (player 2). */
    TEAM_BLUE /**< Blue team (player 1). */
} Team;

/**
 * @brief Animation / action state of a champion.
 */
typedef enum {
    CHAMPION_IDLE, /**< Standing still. */
    CHAMPION_MOVING, /**< Walking. */
    CHAMPION_ATTACKING /**< Performing an attack. */
} ChampionState;

/**
 * @brief Champion model.
 */
typedef struct Champion Champion;

/**
 * @brief Allocates and initialises a Champion.
 * @param respawnPos World position used as the spawn and respawn point.
 * @param dir Initial facing direction.
 * @param width Sprite width in pixels.
 * @param height Sprite height in pixels.
 * @return Pointer to the new Champion, or NULL on allocation failure.
 */
Champion *createChampion(Position respawnPos, Direction dir, int width, int height);

/**
 * @brief Frees a Champion previously created with createChampion().
 * @param champion Champion to destroy.
 */
void destroyChampion(Champion *champion);

/**
 * @brief Teleports the champion back to its respawn position and resets its state.
 * @param champion Champion to respawn.
 */
void championRespawn(Champion *champion);

/**
 * @brief Returns the current world position of the champion.
 * @param champion Target champion.
 * @return Current position.
 */
Position championGetPos(Champion *champion);

/**
 * @brief Returns the current facing direction of the champion.
 * @param champion Target champion.
 * @return Current direction.
 */
Direction championGetDir(Champion *champion);

/**
 * @brief Returns the current action state of the champion.
 * @param champion Target champion.
 * @return Current ChampionState.
 */
ChampionState championGetState(Champion *champion);

/**
 * @brief Returns the movement speed of the champion in pixels per tick.
 * @param champion Target champion.
 * @return Speed value.
 */
float championGetSpeed(Champion *champion);

/**
 * @brief Returns the current HP of the champion.
 * @param champion Target champion.
 * @return Current HP.
 */
int championGetHp(Champion *champion);

/**
 * @brief Returns the maximum HP of the champion.
 * @param champion Target champion.
 * @return Maximum HP.
 */
int championGetMaxHp(Champion *champion);

/**
 * @brief Returns the attack damage of the champion.
 * @param champion Target champion.
 * @return Attack damage value.
 */
int championGetAttackDmg(Champion *champion);

/**
 * @brief Returns the remaining respawn timer in seconds, or ALIVE if not dead.
 * @param champion Target champion.
 * @return Respawn timer value.
 */
int championGetRespawnTimer(Champion *champion);

/**
 * @brief Returns the team of the champion.
 * @param champion Target champion.
 * @return TEAM_RED or TEAM_BLUE.
 */
Team championGetTeam(Champion *champion);

/**
 * @brief Returns the sprite width of the champion in pixels.
 * @param champion Target champion.
 * @return Width in pixels.
 */
int championGetWidth(Champion *champion);

/**
 * @brief Returns the sprite height of the champion in pixels.
 * @param champion Target champion.
 * @return Height in pixels.
 */
int championGetHeight(Champion *champion);

/**
 * @brief Sets the action state of the champion.
 * @param champion Champion to update.
 * @param state New ChampionState.
 */
void championSetState(Champion *champion, ChampionState state);

/**
 * @brief Sets the facing direction of the champion.
 * @param champion Champion to update.
 * @param dir New direction.
 */
void championSetDir(Champion *champion, Direction dir);

/**
 * @brief Sets the world position of the champion.
 * @param champion Champion to update.
 * @param pos New position.
 */
void championSetPos(Champion *champion, Position pos);

/**
 * @brief Sets the current HP of the champion.
 * @param champion Champion to update.
 * @param hp New HP value.
 */
void championSetHp(Champion *champion, int hp);

/**
 * @brief Sets the respawn countdown timer.
 * @param champion Champion to update.
 * @param timer New timer value in seconds (use ALIVE when not dead).
 */
void championSetRespawnTimer(Champion *champion, int timer);

/**
 * @brief Resets the champion's position to its respawn point without changing other state.
 * @param champion Champion to update.
 */
void championClearPosition(Champion *champion);

/**
 * @brief Adds a delta to the champion's current HP.
 * @param champion Champion to update.
 * @param delta Amount to add (negative to subtract).
 */
void championAddHp(Champion *champion, int delta);

/**
 * @brief Adds a delta to the champion's maximum HP.
 * @param champion Champion to update.
 * @param delta Amount to add.
 */
void championAddMaxHp(Champion *champion, int delta);

/**
 * @brief Adds a delta to the champion's movement speed.
 * @param champion Champion to update.
 * @param delta Amount to add.
 */
void championAddSpeed(Champion *champion, int delta);

/**
 * @brief Adds a delta to the champion's attack damage.
 * @param champion Champion to update.
 * @param delta Amount to add.
 */
void championAddAttackDmg(Champion *champion, int delta);

/**
 * @brief Returns the collision box of the champion (50% of sprite size, centred).
 * @param champion Target champion.
 * @return Collision bounding box.
 */
Box championGetCollisionBox(Champion *champion);

/**
 * @brief Returns the attack hitbox of the champion in its current facing direction.
 * @param champion Target champion.
 * @return Attack bounding box.
 */
Box championGetAttackBox(Champion *champion);

/**
 * @brief Tests whether the attacker's attack box overlaps the target's collision box.
 * @param attacker Attacking champion.
 * @param target Defending champion.
 * @return true if the attack lands.
 */
bool championAttackHits(Champion *attacker, Champion *target);

/**
 * @brief Returns the accumulated kill/score points of the champion.
 * @param champion Target champion.
 * @return Current point total.
 */
int championCurrentPoints(Champion *champion);

/**
 * @brief Adjusts the champion's accumulated points by a delta.
 * @param champion Champion to update.
 * @param delta Amount to add (negative to subtract).
 */
void championUpdatePoints(Champion *champion, int delta);
