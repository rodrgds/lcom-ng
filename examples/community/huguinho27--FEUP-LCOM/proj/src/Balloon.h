#ifndef BALLOON_H
#define BALLOON_H

/** @defgroup Balloon Balloon
 * @{
 * Functions for manipulating balloons
 */


typedef enum {INTACT, BLOWN, DESTROYED} balloon_state;
struct Balloon;
typedef struct Balloon balloon_t;

/**
 * @brief Creates a new instance of baloon
 * 
 * @param x Integer with the 'x' position of the bitmap
 * @param y Integer with the 'y' position of the bitmap
 *
 * @return Returns the pointer to the new created balloon
 */
balloon_t* createBalloon(int xIN, int yIN);

/**
 * @brief Checks if a balloon is active or not
 * @param b The balloon we are referring to
 *
 * @return The pointer to the new create structure
 */
int isBalloonActive(balloon_t *b);

/**
 * @brief Toggles the variable active to 1 (true)
 * @param b The balloon we are referring to
 */
void setBalloonActive(balloon_t *b);

/**
 * @brief Frees the memory addressed to a balloon
 * @param b Refers to the ballon we wish to clear
 */
void destroyBalloon(balloon_t *b);

/**
 * @brief Checks if a balloon is destroyed or not
 * @param b Refers to the ballon we wish to know if it's destroyed or not
 *
 * @return 1 on true, 0 otherwise
 */
int isBalloonDestroyed(balloon_t *b);

/**
 * @brief Checks if a balloon is blown or not
 * @param b Refers to the ballon we wish to know if it's blown or not
 *
 * @return 1 on true and 0 on false
 */
int isBalloonBlown(balloon_t *b);

/**
 * @brief Checks if a balloon is intact or not
 * @param b The balloon we are referring to
 *
 * @return 1 on true and 0 on false
 */
int isBalloonIntact(balloon_t *b);

/**
 * @brief Changes the current position of the balloon
 * @param b The ballon we wish to change position
 */
void updateBalloonPosition(balloon_t *b);

/**
 * @brief Changes the state of the balloon to 'blown'
 * @param b The ballon we wish to change the state
 */
void setBalloonHit(balloon_t *b);

/**
 * @brief This function returns the 'x' pos of a balloon
 * @param b The ballon we wish to know 'x' position
 *
 * @return The 'x' position of the balloon b
 */
int getBalloonPos_X(balloon_t *b);

/**
 * @brief This function returns the 'y' pos of a balloon
 * @param b The ballon we wish to know 'y' position
 * 
 * @return The 'y' position of the balloon b
 */
int getBalloonPos_Y(balloon_t *b);

/**
 * @brief This function displays the bitmap image associated with te balloon
 * @param b The ballon we wish to display
 */
void drawBalloon(balloon_t *b);

/**
 * @brief This function handles the right print on screen of the balloon
 * @param b The ballon we wish to check position
 */
void resetBalloonPosition(balloon_t *b);

#endif /**@}*/
