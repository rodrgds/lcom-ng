#include "keyboard.h"
#include "i8042.h"
#define DELAY_US 20000
#define MAX_ATTEMPTS 5

int hook_id_keyboard = KBD_IRQ;
bool kbd_error = false;

struct Scancode {
    uint8_t bytes[2];
    uint8_t size;
    bool complete;
};

static struct Scancode current_scancode = {{0, 0}, 0, false};

int (kbd_subscribe_int)(uint8_t *bit_no) {
    *bit_no = hook_id_keyboard;
    if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_keyboard) != 0) return 1;
    return 0;
}

int (kbd_unsubscribe_int)(void) {
    if (sys_irqrmpolicy(&hook_id_keyboard) != 0) return 1;
    return 0;
}

void (kbd_ih)(void) {
    uint8_t status;
    uint8_t byte;
    if (util_sys_inb(STATUS_REGISTER, &status) != 0) { kbd_error = true; return; }
    if (status & (TIMEOUT_ERROR | PARITY_ERROR)) { kbd_error = true; return; }
    if (util_sys_inb(OUTPUT_BUFFER, &byte) != 0) { kbd_error = true; return; }
    process_scancode_byte(byte);
}

void process_scancode_byte(uint8_t byte) {
    if (byte == CODE_PREFIX) {
        current_scancode.bytes[0] = byte;
        current_scancode.size = 2;
        current_scancode.complete = false;
    } else {
        if (current_scancode.size == 2 && !current_scancode.complete) {
            current_scancode.bytes[1] = byte;
            current_scancode.complete = true;
        } else {
            current_scancode.bytes[0] = byte;
            current_scancode.size = 1;
            current_scancode.complete = true;
        }
    }
}

bool scancode_complete(void) { 
  return current_scancode.complete; 
}
uint8_t get_scancode_size(void) {
   return current_scancode.size; 
}
uint8_t *get_scancode_bytes(void) {
   return current_scancode.bytes; 
  }
void reset_scancode(void) { 
  current_scancode.complete = false; current_scancode.size = 0; 
}

int kbc_send_command(uint8_t cmd){
    int attempts = 0;
    int max_attempts = 5;
    uint8_t stat;
    while (attempts < max_attempts) {
    if (util_sys_inb(STATUS_REGISTER, &stat) != 0) return 1;
    /* loop while 8042 input buffer is not empty */
    if( (stat & INPUT_BUFFER_FULL) == 0 ) {
        sys_outb(COMMAND_REGISTER, cmd); /* no args command */
        return 0;
    }
    tickdelay(micros_to_ticks(DELAY_US));
    attempts++;
}

return 1;

}

int kbc_read_response(uint8_t* data){
   
    uint8_t stat;
    int attempts = 0;
    int max_attempts = 5; 

    while(attempts < max_attempts) {
        if (util_sys_inb(STATUS_REGISTER, &stat) != 0) return 1;
        if( stat & OUTPUT_BUFFER_FULL ) {
            util_sys_inb(OUTPUT_BUFFER, data); /* ass. it returns OK */
            if ( (stat &(PARITY_ERROR | TIMEOUT_ERROR)) == 0 ){
                 return 0;
            } else
                return 1;
        }
        tickdelay(micros_to_ticks(DELAY_US));
        attempts++;
    }

    return 1;


}
int kbc_write_data(uint8_t data) {
    uint8_t stat;
    int attempts = 0;
    while (attempts++ < MAX_ATTEMPTS) {
        if (util_sys_inb(STATUS_REGISTER, &stat) != 0) return 1;
        if (!(stat & INPUT_BUFFER_FULL))
            return sys_outb(INPUT_BUFFER, data);
        tickdelay(micros_to_ticks(DELAY_US));
    }
    return 1;
}

int wait_for_esc(void) {
    int r, ipc_status;
    message msg;
    bool running = true;
    uint8_t bit_no;

    if (kbd_subscribe_int(&bit_no) != 0) return 1;
    uint32_t irq_set = BIT(bit_no);

    while (running) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & irq_set) {
                        kbd_ih();
                        if (kbd_error) {
                            kbd_error = false;
                            reset_scancode();
                            continue;
                        }
                        if (scancode_complete()) {
                            uint8_t size = get_scancode_size();
                            uint8_t *bytes = get_scancode_bytes();
                            if (size == 1 && bytes[0] == (ESC | BREAKCODE)) running = false;
                            reset_scancode();
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }

    if (kbd_unsubscribe_int() != 0) return 1;
    return 0;
}

int enable_keyboard_interrupts(){
    uint8_t cmd_byte;
    if (kbc_send_command(READ_CMD_BYTE) != 0) return 1;
    if (kbc_read_response(&cmd_byte) != 0) return 1;
    cmd_byte |= ENABLE_KEYBOARD_INTERRUPTS;
    if (kbc_send_command(WRITE_CMD_BYTE) != 0) return 1;
    if (kbc_write_data(cmd_byte) != 0) return 1;
    return 0;
}

int disable_keyboard_interrupts(){ // initally i though i had to implement this function, it is redundant but it is kept anyway
    uint8_t cmd_byte;
    if (kbc_send_command(READ_CMD_BYTE) != 0) return 1;
    if (kbc_read_response(&cmd_byte) != 0) return 1;
    cmd_byte &= ~ENABLE_KEYBOARD_INTERRUPTS;
    if (kbc_send_command(WRITE_CMD_BYTE) != 0) return 1;
    if (kbc_write_data(cmd_byte) != 0) return 1;
    return 0;
}

