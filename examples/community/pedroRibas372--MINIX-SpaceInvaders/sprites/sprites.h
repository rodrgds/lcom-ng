#ifndef _SPRITES_H_
#define _SPRITES_H_

#include <lcom/lcf.h>

/**
 * @brief Store every needed information of every Sprite created

 */
typedef struct {
  /**
 * @brief Holds the values of the xpm load of an image
 */
  uint8_t *map;
  /**
 * @brief Holds the image from xpm load
 */
  xpm_image_t img;

  /**
 * @brief Holds the position of the top left corner of the image
 */
  int x, y;

  /**
 * @brief dead state, false if alive
 */
  bool dead;

  /**
 * @brief hero / enemy
 */
  int team;

  int Xspeed;
  int Yspeed;
  
  /**
   * @brief Is the bullet guided or not
   */
  bool guidedBullet;


} Sprite;


/**
 * @brief Creates every enemy to be displayed in level 1
 */
void loadEnemiesLevel1();

/**
 * @brief Creates every enemy to be displayed in level 2
 */
void loadEnemiesLevel2();

/**
 * @brief Creates every enemy to be displayed in level 3
 */
void loadEnemiesLevel3();


/**
 * @brief Creates an enemy and inserts it into the enemy array
 *
 * @param xpm Holds the xpm name to be called
 * @param x the position related to the top left corner of the x axis where the enemy is to be created
 * @param y the position related to the top left corner of the y axis where the enemy is to be created
 * @return Return 0 upon success and non-zero otherwise
 */
int create_enemy(xpm_map_t xpm, int x, int y);

/**
 * @brief Creates the `hero`
 *
 * @param xpm Holds the xpm name to be called
 * @param x the position related to the top left corner of the x axis where the enemy is to be created
 * @param y the position related to the top left corner of the y axis where the enemy is to be created
 * @return Return a sprite of the image on the xpm
 */
Sprite* create_hero(xpm_map_t xpm, int x, int y);

/**
 * @brief Creates static images
 *
 * @param xpm Holds the xpm name to be called
 * @param x the position related to the top left corner of the x axis where the enemy is to be created
 * @param y the position related to the top left corner of the y axis where the enemy is to be created
 * @return Return a sprite of the image on the xpm
 */ 
Sprite* create_static(xpm_map_t xpm,int x,int y);

/**
 * @brief Creates a bullet and inserts it into the bullet array
 *
 * @param x the position related to the top left corner of the x axis where the enemy is to be created
 * @param y the position related to the top left corner of the y axis where the enemy is to be created
 * @param bulletSpeed the speed at witch the bullet will fall on the y axis
 * @param team whom does the bullet belong to
 * 
 * @return Return 0 upon success
 */
int createBullet(int x, int y, int bulletSpeed, int team);

/**
 * @brief Destroys Sprite
 *
 * @param sp the Sprite to be destroyed
 */
void destroy_sprite(Sprite *sp);

/**
 * @brief Destroys every enemy
 */
void destroyEnemies();

/**
 * @brief Destroys every bullet
 */
void destroyBullets();

/**
 * @brief Deletes the drawing of `sp` from the screen
 */
void eraseSprite(Sprite * sp);

/**
 * @brief Deletes every bullet from the screen
 */
void eraseBullets();

/**
 * @brief Creates a bullet from the top middle of the hero position
 * 
 * @return Return 0 upon success
 */
int heroShoot();

/**
 * @brief Creates 2 bullets from each side of the enemy position
 * 
 * @return Return 0 upon success
 */
int enemyShoot();

/**
 * @brief Checks if there is a collision between sprites of different teams
 * 
 * @param a first sprite
 * @param b second sprite
 * 
 * @return true if there is a collision, false otherwise
 */
bool check_collision(Sprite* a, Sprite* b);

/**
 * @brief moves a Sprite to the left
 * 
 * @param sp sprite to me moved to the left
 */
void moveLeft(Sprite * sp);

/**
 * @brief moves a Sprite to the right
 * 
 * @param sp sprite to me moved to the right
 */
void moveRight(Sprite * sp);

/**
 * @brief moves a Sprite upward
 * 
 * @param sp sprite to me moved upward
 */
void moveUp(Sprite* sp);

/**
 * @brief moves a Sprite downward
 * 
 * @param sp sprite to me moved downward
 */
void moveDown(Sprite* sp);

/**
 * @brief moves every enemy either to the left or right
 */
void moveEnemies();

/**
 * @brief moves every bullet either up or down
 */
int moveBullets();


#endif
