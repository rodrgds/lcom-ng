#include "keyboard.h"

int keyboard_hook_id = 5;
uint8_t scancode[2]; // Global variable that stores the code of the key that was pressed
bool is_make = true; // Pressed key is make code
int size = 1; // Number of bytes of a scan
bool two_bytes; // Flag for two byte scancode

int subscribe_keyboard(uint8_t *bit_no) {
    // Set and Receive notification
    if (bit_no == NULL) return 1;
  
    *bit_no = BIT(keyboard_hook_id);
  
    // Needs to be in exclusive mode so that other processes don't influence the result
    if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id) != 0) return 1;
  
    return 0;
}

int unsubscribe_keyboard() {
    if (sys_irqrmpolicy(&keyboard_hook_id) != 0) return 1;

    return 0;
}

void keyboard_ih() {
    uint32_t status;
    uint32_t data;

    sys_inb(KBC_STATUS, &status); // Get keyboard status

    if (status & KBC_OBF) {
        sys_inb(KBC_OUT_BUFFER, &data); // Read code

        if ((status & (KBC_PAR_ERR | KBC_TO_ERR)) == 0) {
            // Check if its two byte scan code
            if (data == 0xE0) {
                two_bytes = true;
                return;
            }

            if (two_bytes) {
                scancode[0] = 0xE0;  // Store first byte
                scancode[1] = data;   // Store second byte
                size = 2;
                two_bytes = false;
            } else {
                scancode[0] = data;  // Store first byte
                size = 1;
            }

            is_make = !(scancode[size - 1] & BIT(7)); // Check if it's a make or break
        }
    }
}
