#include <lcom/lcf.h>
#include <lcom/utils.h>

#include "mouse.h"

int mouse_hook_id = 2;
uint8_t received_byte = 0;
uint8_t mouse_packet[3];
struct packet built_mouse_packet;

/* current mouse state for UI querying */
static int mouse_x = 0;
static int mouse_y = 0;
static int mouse_buttons = 0; /* bitmask: 1=left,2=right,4=middle */

static uint8_t curr_mouse_packet_index = 0;

int (mouse_subscribe_int)(uint8_t *bit_no) {
    if (bit_no == NULL) return 1;

    *bit_no = BIT(mouse_hook_id);
    if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id) != 0) return 1;
    return 0;
}

int (mouse_unsubscribe_int)() {
    if (sys_irqrmpolicy(&mouse_hook_id) != 0) return 1;

    return 0;
}

void (define_mouse_packet)() {
    memcpy(built_mouse_packet.bytes, mouse_packet, sizeof(mouse_packet));
    built_mouse_packet.lb = mouse_packet[0] & MOUSE_BYTE1_BIT0_LFT_BTTN;
    built_mouse_packet.rb = mouse_packet[0] & MOUSE_BYTE1_BIT1_RGT_BTTN;
    built_mouse_packet.mb = mouse_packet[0] & MOUSE_BYTE1_BIT2_MID_BTTN;

    
    if (mouse_packet[0] & MOUSE_BYTE1_BIT4_X_SIGN) {
        built_mouse_packet.delta_x = DELTA_NEG_SIGN_EXT | mouse_packet[1];
    }
    else built_mouse_packet.delta_x = mouse_packet[1];

    if (mouse_packet[0] & MOUSE_BYTE1_BIT5_Y_SIGN) {
        built_mouse_packet.delta_y = DELTA_NEG_SIGN_EXT | mouse_packet[2];
    }
    else built_mouse_packet.delta_y = mouse_packet[2];
    
    built_mouse_packet.x_ov = mouse_packet[0] & MOUSE_BYTE1_BIT6_X_OF;
    built_mouse_packet.y_ov = mouse_packet[0] & MOUSE_BYTE1_BIT7_Y_OF;
}

int (write_command_to_mouse)(uint8_t buffer_content) {
    uint8_t kbc_response = 0, remaining_tries = MAX_RETRIES;

    while (remaining_tries > 0) {
        if (write_kbc_input(STATUS_COMMAND_KBC_PORT, MOUSE_WRITE_TO_AUX_DEVICE) != 0) return 1;
        if (write_kbc_input(IN_OUT_BUFFER_KBC_PORT, buffer_content) != 0) return 1;
        
        if (read_kbc_output(IN_OUT_BUFFER_KBC_PORT, true, &kbc_response) != 0) return 1;       

        if (kbc_response == MOUSE_ACKNOWLEDGMENT) return 0;
        if (kbc_response == MOUSE_ERROR_RESEND || kbc_response == MOUSE_ERROR_RESPONSE) {
            remaining_tries--;
            continue;
        }
        return 1;
    }

  return 1;
}

void (mouse_ih)() {
    uint8_t status;
    if (util_sys_inb(STATUS_COMMAND_KBC_PORT, &status) != 0) { received_byte = 0; return; }
    if ((status & KBC_STATUS_CHECK_ERRS) || !(status & KBC_STATUS_AUX_MOUSE) || !(status & KBC_STATUS_OUTPUT_BUFFER_FULL)) {
        received_byte = 0;
        return;
    }
    if (util_sys_inb(IN_OUT_BUFFER_KBC_PORT, &received_byte) != 0) received_byte = 0;
}

void (process_mouse_interrupt)() {
    mouse_ih();

    if (((received_byte & MOUSE_BYTE1_BIT3_1) && curr_mouse_packet_index == 0) || curr_mouse_packet_index == 1) {
        mouse_packet[curr_mouse_packet_index] = received_byte;
        curr_mouse_packet_index++;
    }
    else if (curr_mouse_packet_index == 2) {
        mouse_packet[curr_mouse_packet_index] = received_byte;
        define_mouse_packet();

        mouse_x += built_mouse_packet.delta_x;
        if (mouse_x < 0) mouse_x = 0;
        else if (mouse_x >= get_h_res()) mouse_x = get_h_res() - 1;

        mouse_y -= built_mouse_packet.delta_y;
        if (mouse_y < 0) mouse_y = 0;
        else if (mouse_y >= get_v_res()) mouse_y = get_v_res() - 1;

        mouse_buttons = (built_mouse_packet.lb ? 1 : 0) | (built_mouse_packet.rb ? 2 : 0) | (built_mouse_packet.mb ? 4 : 0);

        curr_mouse_packet_index = 0;
    }
    else curr_mouse_packet_index = 0;
}

void mouse_get_pos(int *x, int *y) {
    if (x) *x = mouse_x;
    if (y) *y = mouse_y;
}

int mouse_get_buttons() {
    return mouse_buttons;
}

void (set_mouse_positions)(int x, int y) {
    mouse_x = x;
    mouse_y = y;
}
