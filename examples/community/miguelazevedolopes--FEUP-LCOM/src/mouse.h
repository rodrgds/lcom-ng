#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include <stdint.h>
/** @defgroup mouse mouse.h
 * @{
 *
 * This file contains the functions related to the mouse functioning.
 * 
 */

//bytes of the packet are passed through this var
uint8_t data;

typedef enum  {
    up_move,
    down_move,
    up_left_move,
    down_left_move,
    up_right_move,
    down_right_move,
}mouse_state;

//subscribes mouse interrupts
int mouse_subscribe_int(uint8_t *mouse_bitmask);
//unsubscribes mouse interrupts
int mouse_unsubscribe_int();
//handles interrupts
void(mouse_ih)();
//parses the data in bytes variable and puts it in pk
int mouse_handle_packet(uint8_t bytes[],struct packet *pk,int size);
//if 0 output buffer is not ready to be read, else output buffer is full and can be read
int check_output_buffer();

int(write_command)(uint32_t command,uint32_t argument);

int check_input_buffer();

int mouse_handle_commands(uint8_t response,uint32_t argument);

void mouse_update_state(mouse_state *state,struct packet *pk);
/**@}*/

