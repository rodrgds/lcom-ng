#include "mouse.h"
#include "kbd.h"
#include "mouse_macros.h"

int hook_id_mouse = 14;
bool sychrnonize = true;
bool packet_ready = false;
struct packet pp;
uint8_t index_packet = 0;

int (mouse_subscribe_int) (uint8_t* bit_no){
    if (bit_no == NULL){
		printf("Invalid pointer in timer_subscribe_int().\n");
		return ERROR;
	}
	*bit_no = (uint8_t) hook_id_mouse;
	int ret = sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse);
	if (ret != 0){
		printf("Error subscribing the timer.\n");
		return ERROR;
	}
	return OK;
}

int (mouse_unsubscribe_int)() {
  	int ret = sys_irqrmpolicy(&hook_id_mouse);
	if (ret != 0){
		printf("Error unsubscribing.\n");
		return ERROR;
	}
	return OK;
}

int (read_packet) (){
    uint8_t byte;
    int ret = read_OBF(&byte, true);
    if (ret != OK){
        printf("Error reading the byte from a packet\n");
        return ERROR;
    }
    if (sychrnonize){
        pp.bytes[index_packet++] = byte;

        if (index_packet == 3) packet_ready = true;
    }
    else{
        if ((byte & BIT(3))) index_packet = 0;
        
        pp.bytes[index_packet++] = byte;

        if (index_packet == 3){
            sychrnonize = true;
            packet_ready = true;
        }
    }
    return OK;
}

void (mouse_ih) (){
    int ret = read_packet();
    if (ret != OK){
        printf("Error reading the packet byte.\n");
    }
}

void (organize_packet) (struct packet* pp){
    pp->y_ov = (pp->bytes[0] & MOUSE_Y_OV);
    pp->x_ov = (pp->bytes[0] & MOUSE_X_OV);
    pp->rb = (pp->bytes[0] & MOUSE_RB);
    pp->mb = (pp->bytes[0] & MOUSE_MB);
    pp->lb = (pp->bytes[0] & MOUSE_LB);
    pp->delta_x = (uint16_t) pp->bytes[1];
    pp->delta_y = (uint16_t) pp->bytes[2];

    // Check if the 9th bit is 1 meaning the displacement is negative
    if (pp->bytes[0] & BIT(5)) pp->delta_y |= 0xFF00;
    if (pp->bytes[0] & BIT(4)) pp->delta_x |= 0xFF00;
}

int (write_command_mouse) (uint8_t command){
    int ret = write_command(KBC_WRITE_COMMAND_CMD, MOUSE_WRITE_BYTE);
    if (ret != OK) return ERROR;

    ret = write_command(KBC_WRITE_COMMAND_ARG, command);
    if (ret != OK) return ERROR;

    uint8_t ack_byte;
    // Read the ack byte
    ret = read_OBF(&ack_byte, true);
    if (ret != OK) return ERROR;
    // While the ack byte is not ok, we try to write it again
    if (ack_byte != MOUSE_ACK) return write_command_mouse(command);
    return OK;
}

int (mouse_gesture) (uint8_t x_len, uint8_t tolerance){
    static States state = INITIAL; // Only initialized the first time
    static int16_t total_x_mov = 0;
    static int16_t total_y_mov = 0;

    int16_t delta_x = (int16_t) pp.delta_x;
    int16_t delta_y = (int16_t) pp.delta_y;
    switch(state){
        case INITIAL:
            /// Only left button pressed
            if (pp.lb == BUTTON_PRESSED && pp.rb != BUTTON_PRESSED && pp.mb != BUTTON_PRESSED){
                state = FIRST_LINE;
            }
            break;

        case FIRST_LINE:
            if (pp.lb != BUTTON_PRESSED){ /// Check if the draw line is a valid one
                if (total_x_mov != 0 && (total_y_mov / total_x_mov > 1) && total_x_mov >= x_len){
                    state = VERTEX;
                }
                else state = INITIAL;
                /// Reset values
                total_x_mov = 0;
                total_y_mov = 0;
            }
            else if (pp.rb == BUTTON_PRESSED || pp.mb == BUTTON_PRESSED) state = INITIAL; /// Check if no other button is pressed
            else if ((delta_x <= 0 || delta_y <= 0) && (abs(delta_x) > tolerance || abs(delta_y) > tolerance)) state = INITIAL; /// Check the tolerance
            else{ /// The line is being drawn
                total_x_mov += delta_x;
                total_y_mov += delta_y;
            }

            break;

        case VERTEX:
            if ((abs(delta_x) > tolerance || abs(delta_y) > tolerance)) state = INITIAL; /// Check the tolerance
            else if (pp.lb != BUTTON_PRESSED && pp.rb != BUTTON_PRESSED && pp.mb != BUTTON_PRESSED){ /// No pressed button
                total_x_mov += delta_x;
                total_y_mov += delta_y;
            }
            else if (pp.rb == BUTTON_PRESSED && pp.lb != BUTTON_PRESSED && pp.mb != BUTTON_PRESSED){ /// Check if can change to next state
                if (total_x_mov <= tolerance){
                    state = SECOND_LINE;
                }
                else state = INITIAL;
                /// Reset values
                total_x_mov = 0;
                total_y_mov = 0;
            }
            break;

        case SECOND_LINE:
            if (pp.rb != BUTTON_PRESSED){ /// Check if the draw line is a valid one
                if (total_x_mov != 0 && (abs(total_y_mov / total_x_mov) > 1) && total_x_mov >= x_len){ /// slope is negative, so use abs()
                    state = SUCCESS;
                    return GESTURE_COMPLETE;
                }
                else state = INITIAL;
                /// Reset values
                total_x_mov = 0;
                total_y_mov = 0;
            }
            else if (pp.lb == BUTTON_PRESSED || pp.mb == BUTTON_PRESSED) state = INITIAL; /// Check if no other button is pressed
            else if ((delta_x <= 0 || delta_y >= 0) && (abs(delta_x) > tolerance || abs(delta_y) > tolerance)) state = INITIAL; /// Check the tolerance
            else{ /// The line is being drawn
                total_x_mov += delta_x;
                total_y_mov += delta_y;
            }
            break;

        case SUCCESS:
            return GESTURE_COMPLETE;
    }
    return GESTURE_IN_PROGRESS;
}

void (reset_packets) (){
    if (packet_ready){
        packet_ready = false;
        index_packet = 0;
    }
}
