/**
 * @file projectiles.h
 * @brief Header file for projectile management in the game.
 */

#ifndef PROJECTILES_H
#define PROJECTILES_H

/**
 * @brief Enumeration for projectile types.
 */
typedef enum {
  PLAYER_PROJECTILE,
  ALIEN_PROJECTILE
} ProjectileType;


/**
 * @brief Struct to represent a projectile.
 */
typedef struct {
  int xi, yi;
  int xf, yf;
  int t;
  int16_t speed;
  ProjectileType type; 
} Projectile;


/**
 * @brief Creates a new projectile.
 * @param xi Initial x position.
 * @param yi Initial y position.
 * @param xf Final x position.
 * @param yf Final y position.
 * @param speed Speed of the projectile.
 * @param type Type of the projectile.
 */
void create_projectile(int xi, int yi, int xf, int yf, int16_t speed, ProjectileType type);

/**
 * @brief Moves all projectiles in the game.
 */
void move_projectiles(void);

/**
 * @brief Moves a single projectile.
 * @param projectile Pointer to the Projectile struct.
 */
void move_projectile(Projectile* projectile);

/**
 * @brief Calculates the x position of a projectile based on its parameters.
 * @param projectile Pointer to the Projectile struct.
 * @return Calculated x position.
 */
int calc_x(Projectile* projectile);

/**
 * @brief Calculates the y position of a projectile based on its parameters.
 * @param projectile Pointer to the Projectile struct.
 * @return Calculated y position.
 */
int calc_y(Projectile* projectile);


/**
 * @brief Checks for collision of a projectile with an alien or the player.
 * @param projectile Pointer to the Projectile struct.
 * @param alien_index Pointer to store the index of the alien hit (if any).
 * @return True if a collision is detected, false otherwise.
 */

bool check_collision(Projectile* projectile, int* alien_index);

#endif // PROJECTILES_H
