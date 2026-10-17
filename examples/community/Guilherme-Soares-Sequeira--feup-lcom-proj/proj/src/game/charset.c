#include <lcom/lcf.h>

#include "../devices/kbc/i8042.h"
#include "charset.h"
#include "draw_text.h"

// NOTE: should be implemented as struct due to special flags/key codes

#define ENTER_MAKECODE 0x1c       /**< @brief the makecode of the enter key       */
#define TAB_MAKECODE 0x0f         /**< @brief the makecode of the tab key         */
#define CTRL_MAKECODE 0x1d        /**< @brief the makecode of the ctrl key        */
#define LEFT_SHIFT_MAKECODE 0x2a  /**< @brief the makecode of the left-shift key  */
#define RIGHT_SHIFT_MAKECODE 0x36 /**< @brief the makecode of the right-shift key */
#define BACKSPACE_MAKECODE 0x0e   /**< @brief the makecode of the backspace key   */

/**< @brief the charset supported by the program */
char default_charset[] = {NO_ADD_CHAR, NO_ADD_CHAR, '1', '2', '3',
                          '4', '5', '6', '7', '8',
                          '9', '0', '?', NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, 'q', 'w', 'e', 'r',
                          't', 'y', 'u', 'i', 'o',
                          'p', '+', NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          'a', 's', 'd', 'f', 'g',
                          'h', 'j', 'k', 'l', 'c',
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, '~', 'z',
                          'x', 'c', 'v', 'b', 'n',
                          'm', ',', '.', '-', NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, ' ', NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR,
                          NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR, NO_ADD_CHAR};

void(process_scancode)(uint8_t *scancode, int scancode_size, int scancode_type) {

  if (scancode_size > 1)
    return;

  switch (scancode_type) {
    case KBC_SCANCODE_MAKE: {

      uint8_t code = scancode[0];

      if (code == BACKSPACE_MAKECODE) {
        remove_char_from_current_text();
      }
      else if (code == ENTER_MAKECODE) {
        buf_draw_text(get_canvas_buffer(), get_current_text(), get_text_initial_position(), LEFT);
        set_is_typing(false);
        clear_current_text();
      }
      else {
        add_char_to_current_text(default_charset[code]);
      }
      break;
    }
    case KBC_SCANCODE_BREAK: { // NOTE: unused
    }
  };
}
