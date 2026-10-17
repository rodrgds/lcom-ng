#include <lcom/lcf.h>
#include "macros.h"
#include <stdbool.h>
#include <stdint.h>
#include "images/mouse1.xpm"
#include "images/mouse2.xpm"

/**
 * @brief Mouse struct with mouse images for regular mouse and hovering mouse as well as position and bool to know if its hovering something or not
 * 
 */
typedef struct {
  xpm_image_t img1;
  xpm_image_t img2;
  int x, y;
  bool hover;


} Mouse;

/**
 * @brief Subscribes mouse interrupts
 * 
 */
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes mouse interrupts
 * 
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Builds a packet based on the 3 bytes received during the interrupts
 * 
 */
void (buildPacket)(struct packet * pp);

/**
 * @brief Resets the position of the mouse
 * 
 * @param mouse Struct mouse
 */
void resetMouse(Mouse * mouse);

/**
 * @brief Mouse interrupt handler that knows exactly what byte it is going to receive from the output buffer (out of the 3 bytes) and stores it in an array
 * 
 */
void (mouse_ih)();

/**
 * @brief Initializes the mouse by giving the position and loading the xpm
 * 
 * @param mouse Mouse struct
 */
void initMouse(Mouse * mouse);

/**
 * @brief Returns the packet
 * 
 * @return struct packet 
 */
struct packet getPacket();

/**
 * @brief Enables or disables the mouse data report functionality based on the argument it is given 
 * 
 * @param toggle 
 * @return int 
 */
int toggle_data_report(uint8_t toggle);


