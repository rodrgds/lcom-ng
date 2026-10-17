#include "keyboard.h"
#include "MACROS.h"


static int hook_id_keyboard = 2;

uint8_t scancode[2] = {0, 0};

uint8_t kbd_data;

bool error_flag_keyboard = false;

/**
 * @brief Struct used to store the information of a key
 * 
 */
typedef struct{
  unsigned char lower; 
  unsigned char upper;
} key ;

/* Array of keys that makes the correspondece between a scancode and a key */
key keymap[] = {
  /*  0  */ { 0, 0},
  /*  1  */ { 0, 0},
  /*  2  */ { '1', '!' },     // 1
  /*  3  */ { '2', '"' },     // 2
  /*  4  */ { '3', 0 },       // 3
  /*  5  */ { '4', 0 },       // 4
  /*  6  */ { '5', 0 },       // 5
  /*  7  */ { '6', 0 },       // 6
  /*  8  */ { '7', '/' },     // 7
  /*  9  */ { '8', 0 },       // 8
  /* 10  */ { '9', 0 },       // 9
  /* 11  */ { '0', '=' },     // 0
  /* 12  */ { '\'', '?' },    // '
  /* 13  */ { 0, 0 },         // «
  /* 14  */ { '\b', '\b' },   /* Backspace */
  /* 15  */ { 0, 0 },         /* Tab */
  /* 16  */ { 'q', 'Q' },     // Q
  /* 17  */ { 'w', 'W' },     // W
  /* 18  */ { 'e', 'E' },     // E
  /* 19  */ { 'r', 'R' },     // R 
  /* 20  */ { 't', 'T' },     // T
  /* 21  */ { 'y', 'Y' },     // Y
  /* 22  */ { 'u', 'U' },     // U
  /* 23  */ { 'i', 'I' },     // I
  /* 24  */ { 'o', 'O' },     // O
  /* 25  */ { 'p', 'P' },     // P
  /* 26  */ { '+', 0 },       // +
  /* 27  */ { 0, 0 },         /*     */
  /* 28  */ { '\n', '\n' },   /* Enter key */
  /* 29  */ { 0, 0 },         /* Control */
  /* 30  */ { 'a', 'A' },     // A
  /* 31  */ { 's', 'S' },     // S
  /* 32  */ { 'd', 'D' },     // D
  /* 33  */ { 'f', 'F' },     // F
  /* 34  */ { 'g', 'G' },     // G
  /* 35  */ { 'h', 'H' },     // H
  /* 36  */ { 'j', 'J' },     // J
  /* 37  */ { 'k', 'K' },     // K
  /* 38  */ { 'l', 'L' },     // L
  /* 39  */ { 0, 0 },         /*  Ç  */
  /* 40  */ { 0, 0 },         /*  º  */
  /* 41  */ { 0, 0 },         /*  ~  */
  /* 42  */ { 0, 0 },         /* Left Shift */
  /* 43  */ { 0, 0 },         // <
  /* 44  */ { 'z', 'Z' },     // Z
  /* 45  */ { 'x', 'X' },     // X
  /* 46  */ { 'c', 'C' },     // C
  /* 47  */ { 'v', 'V' },     // V
  /* 48  */ { 'b', 'B' },     // B
  /* 49  */ { 'n', 'N' },     // N
  /* 50  */ { 'm', 'M' },     // M
  /* 51  */ { ',', ';' },     // ,
  /* 52  */ { '.', ':' },     // .
  /* 53  */ { '-', '_' },     // -
  /* 54  */ { 0, 0 },         /* Right Shift */
  /* 55  */ { 0, 0 },         /* Prt Sc */
  /* 56  */ { 0, 0 },         /* Alt */
  /* 57  */ { ' ', ' ' },     /* Space 0 */ 
  /* 58  */ { 0, 0 },         /* Caps lock */
  /* 59  */ { 0, 0 },         /* 59 - F0 */
  /* 60  */ { 0, 0 },
  /* 61  */ { 0, 0 },
  /* 62  */ { 0, 0 },
  /* 63  */ { 0, 0 },
  /* 64  */ { 0, 0 },
  /* 65  */ { 0, 0 },
  /* 66  */ { 0, 0 },
  /* 67  */ { 0, 0 },
  /* 68  */ { 0, 0 },         /* < ... F10 */
  /* 69  */ { 0, 0 },         /* 69 - Num lock  PAUSE*/
  /* 70  */ { 0, 0 },         /* Scroll Lock */
  /* 71  */ { 0, 0 },         /* Home key */
  /* 72  */ { 0, 0 },         /* Up Arrow */
  /* 73  */ { 0, 0 },         /* Page Up */
  /* 74  */ { 0, 0 },
  /* 75  */ { 0, 0 },         /* Left Arrow */
  /* 76  */ { 0, 0 },         
  /* 77  */ { 0, 0 },         /* Right Arrow */
  /* 78  */ { 0, 0 },
  /* 79  */ { 0, 0 },         /* 79 - End key*/
  /* 80  */ { 0, 0 },         /* Down Arrow */
  /* 81  */ { 0, 0 },         /* Page Down */
  /* 82  */ { 0, 0 },         /* Insert Key */
  /* 83  */ { 0, 0 },         /* Delete Key */
  /* 84  */ { 0, 0 },
  /* 85  */ { 0, 0 },
  /* 86  */ { 0, 0 },
  /* 87  */ { 0, 0 },         /* F11 Key */
  /* 88  */ { 0, 0 },         /* F12 Key */
  /* 89  */ { 0, 0 },         /* All other keys are undefined */ 
};

int(kbd_subscribe_int)(uint8_t *bit_no) {
  *bit_no = hook_id_keyboard;
  sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_keyboard);
  return 0;
}

int(kbd_unsubscribe_int)() {
  sys_irqrmpolicy(&hook_id_keyboard);
  return 0;
}

void(kbc_ih)(void) {

  uint32_t status_register;

  sys_inb(STAT_REG, &status_register);

  if ((status_register & ST_PARITY) || (status_register & ST_TIMEOUT))
    error_flag_keyboard = true;
  else
    error_flag_keyboard = false;

  if (status_register & ST_OBF) {
    uint32_t output;

    sys_inb(OUT_BUF, &output);

    kbd_data = (uint8_t) output;
  }

  uint8_t size_scancode = 1;

  if (!error_flag_keyboard) {

    if (kbd_data == 0xE0) {
      scancode[0] = kbd_data;
      size_scancode = 2;
    }
    else {
      if (size_scancode == 2) {
        scancode[1] = kbd_data;    
      }
      else {    
        scancode[0] = kbd_data;
      }
      size_scancode = 1;
    }
  }

}


int(kbc_cmd)(uint8_t command, uint8_t *arg_or_res, bool expect_res) {

  uint32_t status;
  uint32_t kbc_command = command;

  // issues a command to the KBC
  while (1) {
    sys_inb(STAT_REG, &status);

    if ((status & ST_IBF) == 0) {
      sys_outb(KBC_CMD_REG, kbc_command);
      break;
    }
    tickdelay(micros_to_ticks(DELAY));
  }

  if (expect_res) {
    // if a result is expected
    // reads the output buffer to get the response to the command
    while (1) {
      sys_inb(STAT_REG, &status);

      if ((status & ST_OBF) && !(status & (ST_PARITY | ST_TIMEOUT))) {
        sys_inb(OUT_BUF, &kbc_command);
        *arg_or_res = (uint8_t) kbc_command;
        break;
      }
      tickdelay(micros_to_ticks(DELAY));
    }
  }
  else {
    // if a result is not expected
    // it means the command has an argument
    kbc_command = *arg_or_res;

    sys_inb(STAT_REG, &status);

    sys_outb(OUT_BUF, kbc_command);
  }

  return 0;
}


char key_handler(uint8_t code) {
  
  bool make = !(code & BIT(7));     // checks if the scancode is a makecode or a breakcode
  static bool caps = false;         // True - caps is on             // False - caps is off
  static bool r_shift = false;      // True - right shift is pressed // False - it isn't pressed
  static bool l_shift = false;      // True - left shift is pressed  // False - it isn't pressed
  bool shift = r_shift || l_shift;  // True - any shift is pressed   // False - no shift is pressed

  if ( make ) { // KEY PRESS
    if ( code == 0x3a ) {         // pressed caps lock key
      if ( !caps )
        caps = true;
      else
        caps = false;
    }
    else if ( code == 0x2a ) {    // pressed left shift
      l_shift = true;
    }
    else if ( code == 0x36 ) {    // pressed right shift
      r_shift = true;
    }
    else {
      if ( (code >= 16 && code <= 25) || (code >= 30 && code <= 38) || (code >= 44 && code <= 50) ){ // ONLY LETTERS
        if ( caps != shift ) {        // UPPERCASE
        /* this condition is justified by the fact that, if caps is on, the letters should be 
           displayed as uppercase, howerver, if shift is pressed at the same time, it should 
           reverse the effect of caps. this is only true for letters and no other key is affected 
           by this condition */
          return keymap[code].upper;
        }
        else {                        // LOWERCASE          
          return keymap[code].lower;
        }
      }
      else { // for any key that does is not a letter key
        if ( shift ) {
          return keymap[code].upper;
        }
        else {
          return keymap[code].lower;
        }
      }
    }
  }
  else {  // key release
    if ( code == 0xaa ) {       // released left shift
      l_shift = false;
    }
    else if ( code == 0xb6 ) {  // released right shift
      r_shift = false;
    }
  }

  return 0;
}
