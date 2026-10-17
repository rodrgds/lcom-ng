#include <lcom/lcf.h>
#include "images/frog.xpm"
#include "images/frog/frog1.xpm"
#include "images/frog/frog2.xpm"
#include "images/frog/frog3.xpm"
#include "images/frog/frog4.xpm"
#include "images/frog/frog5.xpm"
#include "images/frog/frog6.xpm"
#include "images/frog/frog7.xpm"

extern int xRes;

/**
 * @brief Struct frog to store the frog's position, xpm images for the animation and the index of which is the animation frame to be displayed
 * 
 */
typedef struct {
  xpm_image_t img[7];
  int x, y;
  int animationIndex;


} Frog;

/**
 * @brief Initiates the frog
 * 
 * @param frog Receives a frog and initiates its values 
 */
void initFrog(Frog * frog);

/**
 * @brief Resets the frog when needed
 * 
 * @param frog Receives a frog and resets its values
 */
void resetFrog(Frog * frog);

/**
 * @brief Determines if the frog is able to move considering the size of the move it wants to make and the type of move
 * 
 * @param frog Struct frog
 * @param move type of move
 * @param size size of move
 * @return true 
 * @return false 
 */
bool canFrogMove(Frog frog, int move, int size);

/**
 * @brief Changes the value of the variable frogIdle to determine if the froghas ended its animation otherwise just updates the index so that it can go to the next frame
 * 
 * @param frog 
 * @param frogIdle 
 */
void frogAnimation(Frog * frog,bool * frogIdle);

