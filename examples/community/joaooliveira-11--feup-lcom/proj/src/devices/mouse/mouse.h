#ifndef _MOUSE_PROJECT
#define _MOUSE_PROJECT

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "../util/KBC.h"

typedef struct{
  uint8_t bytes[3]; 
  bool rb, mb, lb;  
  int16_t delta_x;  
  int16_t delta_y;
  bool x_ov, y_ov;
  uint16_t xpos;
  uint16_t ypos;
} mouse_t;



///@brief Subscribes to the mouse interrupts.
///@note This function subscribes to the mouse interrupts by setting the appropriate bit in the bit_no and calling sys_irqsetpolicy to register the interrupt handler.
///@param bit_no Pointer to the bit number that will hold the mouse interrupt bit.
///@return 0 if the subscription is successful, otherwise returns 1.
int (mouse_subscribe_int)(uint8_t *bit_no);


///@brief Unsubscribes from the mouse interrupts.
///@note This function unsubscribes from the mouse interrupts by removing the previously set interrupt policy using sys_irqrmpolicy.
///@return 0 if the unsubscription is successful, otherwise returns 1.
int (mouse_unsubscribe_int)();

///@brief Mouse interrupt handler.
///@note This function is the interrupt handler for the mouse. It reads a byte from the mouse output port using read_KBC_mouseOUTPUT function.
void (mouse_ih)();

///@brief Process mouse packet bytes.
///@note This function processes the bytes received from the mouse and stores them in the mouse_bytes array to build a complete mouse packet.
void (packet_bytes)();

///@brief Build the mouse packet.
///@note This function constructs the mouse packet using the bytes received from the mouse and stores the relevant information in the mouse_structure.
void (build_mouse_packet)();

///@brief Write a command to the mouse input port.
///@note This function writes a command to the mouse input port and waits for the mouse to acknowledge the command.
///@param mouse_command The command to be sent to the mouse.
///@return 0 if the write is successful and the acknowledgment is received, otherwise returns 1.
int (mouse_write_input)(uint8_t mouse_command);

#endif
