#include <lcom/lcf.h>

#include "mouse.h"

#define max(a, b) ((a > b) ? a : b)
#define min(a, b) ((a < b) ? a : b)

extern int mouse_hook_id;

int (mouse_subscribe_int)(uint8_t* bit_no){
    if (bit_no == NULL) return 1;

    *bit_no = mouse_hook_id;
    return sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}

int (mouse_unsubscribe_int)(){
    return sys_irqrmpolicy(&mouse_hook_id);
}

int (mouse_enable_data_report)(uint32_t wait_ticks){
    uint8_t ack = 0;
    while (ack != MOUSE_SUCCESS_BYTE){
        int flag = kbc_write_command(KBC_FORWARD_TO_MOUSE, wait_ticks);
        if (flag) return flag;

        flag = kbc_write_in_buf(KBC_ENABLE_DATA_REPORT, wait_ticks);
        if (flag) return flag;

        // read the acknowledgement byte
        flag = kbc_read_out_buf(&ack, wait_ticks);
        if (flag) return flag;
    }

    return 0;
}

int (mouse_disable_data_report)(uint32_t wait_ticks){
    uint8_t ack = 0;
    while (ack != MOUSE_SUCCESS_BYTE){
        int flag = kbc_write_command(KBC_FORWARD_TO_MOUSE, wait_ticks);
        if (flag) return flag;

        flag = kbc_write_in_buf(KBC_DISABLE_DATA_REPORT, wait_ticks);
        if (flag) return flag;

        // read the acknowledgement byte
        flag = kbc_read_out_buf(&ack, wait_ticks);
        if (flag) return flag;
    }

    return 0;
}

void (mouse_get_data)(mouse_data_t* mouse_data, uint32_t wait_ticks){
    uint8_t data = 0;

    mouse_data->error = kbc_read_out_buf(&data, wait_ticks);
    if (mouse_data->error) return;

    // check if the data read is valid
    kbc_status status = kbc_parse_status();
    if (status.parity_error || status.timeout_error || !status.mouse_data){
        mouse_data->packet_no = 0;
        return;
    }

    if (mouse_data->packet_no || is_first_byte(data))
        mouse_data->pp.bytes[mouse_data->packet_no++] = data;
}

void mouse_parse_packet(mouse_data_t* mouse_data){
    struct packet *pp = &mouse_data->pp;

    uint8_t first_byte = pp->bytes[0];
    uint8_t delta_x = pp->bytes[1], delta_y = pp->bytes[2];

    // overflow
    pp->y_ov = (first_byte & MOUSE_Y_OVFL);
    pp->x_ov = (first_byte & MOUSE_X_OVFL);

    // offsets
    pp->delta_x = (first_byte & MOUSE_X_MSB) ? sign_extend(delta_x) : delta_x;
    mouse_data->x += pp->delta_x;
    mouse_data->x = min(max(mouse_data->x, 50), 1180);

    pp->delta_y = (first_byte & MOUSE_Y_MSB) ? sign_extend(delta_y) : delta_y;
    mouse_data->y -= pp->delta_y;
    mouse_data->y = min(max(mouse_data->y, 50), 974);

    // buttons
    pp->mb = (first_byte & MOUSE_MIDDLE_BUTTON);
    pp->rb = (first_byte & MOUSE_RIGHT_BUTTON);
    pp->lb = (first_byte & MOUSE_LEFT_BUTTON);
}
