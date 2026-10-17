#ifndef _GAME_VAR_H
#define _GAME_VAR_H

/* VARIAVELS OF THE GAME */
/** @defgroup gameVar
 * @{
 *
 * Constants for the unwind of the game movement and positions. 
 */

#define BACKGROUND_SPEED  1   /**< @brief speed of the background */

#define SPEED             5   /**< @brief speed of the car */

#define CLAWSPEED         7   /**< @brief speed of the claw */

#define ANIMATION_SPEED   4   /**< @brief animation change sprite frequency */

#define STONE_TRANS_RATE  4   /**< @brief obsidian appearance rate */

#define SCORE_INCREMENT   50  /**< @brief score increment value */
#define SCORE_DECREMENT   20  /**< @brief score decrement value */

#define N_GOLDS           25  /**< @brief total number of golds in game */
#define GOLD_MIN_Y        500 /**< @brief minimal y coodinates to generate golds */
#define GOLD_WIDTH        46  /**< @brief gold width */
#define GOLD_HEIGHT       35  /**< @brief gold height */

#define CLAW_SIZE         21  /**< @brief metal claw part size */

#define CLOCKSTART        100 /**< @brief begin val of the clock*/
#define CLOCKINCREMENT    10  /**< @brief clock increment value */
#define ClOCKDECREMENT    5   /**< @brief clock decremente value */

#define CLOCK_POS_X       1120 /**< @brief first x pos of the first clock number */
#define CLOCK_POS_Y       10   /**< @brief pos y of the clock number */

#define NEXT_POS          40   /**< @brief increment to the next number pos */

#define SCORE_POS_X       10   /**< @brief first x pos of the first score number */
#define SCORE_POS_Y       10   /**< @brief pos y of the score number */

#define M_SCORE_POS_X     370  /**< @brief first x pos of the first score number in score menu*/
#define M_SCORE_POS_Y     450  /**< @brief pos y of the score number in score menu*/

#define PLAYER1_SCORE_POS_X     250  /**< @brief first x pos of the first score number in score menu*/
#define PLAYER1_SCORE_POS_Y     400  /**< @brief pos y of the score number in score menu*/

#define PLAYER2_SCORE_POS_X     250  /**< @brief first x pos of the first score number in score menu*/
#define PLAYER2_SCORE_POS_Y     490  /**< @brief pos y of the score number in score menu*/
            

#include <lcom/lcf.h>

#endif
