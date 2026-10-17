#include <lcom/lcf.h>
#include "images/log.xpm"

/**
 * @brief Log struct that has the log xpm and its position
 * 
 */
typedef struct {
  xpm_image_t img;
  int x, y, speed;


} Log;

/**
 * @brief Function that initializes the log
 * 
 * @param log Log struct
 * @param count count to know which log it is
 */
void initLog(Log * log, int count);

