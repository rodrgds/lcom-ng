#include <lcom/lcf.h>

#include "images/turtle.xpm"
#include "images/turtle2.xpm"
#include "images/turtle3.xpm"



/**
 * @brief Turtle struct that has the image of three different turtles, the regular one, the one underwater during the day and another during the night
 * 
 */
typedef struct {
  xpm_image_t img1;
  xpm_image_t img2;
  xpm_image_t img3;
  int x, y, speed;
  bool underwater;
  bool dayTurtle;

} Turtle;


/**
 * @brief Initializes the turtle
 * 
 * @param turtle Struct turtle
 * @param count count to know which turtle it is
 */
void initTurtle(Turtle * turtle, int count);

