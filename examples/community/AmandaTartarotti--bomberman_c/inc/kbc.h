#include <lcom/lcf.h>
#include <stdint.h>

#include "i8042.h"


/**
     * @brief Reads the characters pressed on the keyboard through the output buffer.
     * 
     * The information should only be read when the output buffer is full. The status indicates whether there is an error at the parity or timeout level.
     * 
     * @param error a bool that returns true is the status finds a error
     * @param scancode indicates each keyboard key is been pressed by the user
     * 
     * @return Return 0 upon success and non-zero otherwise
**/
int kbc_read(bool *error, uint8_t *scancode);

