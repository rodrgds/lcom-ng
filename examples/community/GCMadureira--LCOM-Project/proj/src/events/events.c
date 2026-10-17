/** 
 * @file events.c
 * @brief Source code file for all input event related logic.
 * 
 * This file implements all the functions for the input events' logic.
 */


#include "events.h"

static input_event_node* input_event_list = NULL;     ///< A pointer to the start of the linked list
static input_event_node* input_event_list_tail = NULL; ///< A pointer to the tail of the linked list


static uint8_t packet_bytes[3]; ///< The mouse packet vector
static int state = 0; //0 means first byte, 1 means second by, 2 means third byte
void (event_handle_mouse)() {
  if(!kbc_valid_output(true)){ // check if the output buffer has valid data
    kbc_discard_output(); // discard if not
    return;
  }
  else mouse_ih(); // handle interrupt if yes

  uint8_t packet_byte = mouse_get_packet_byte();

  // the first byte always has the bit 3 on, skip byte if it does not for synchronization
  if(state == 0 && !(packet_byte & BIT(3))) return;

  packet_bytes[state] = packet_byte;

  if(state == 2){
    struct packet packet = mouse_assemble_packet(packet_bytes);

    input_event new_event;
    new_event.event_type = MOUSE_EVENT;
    new_event.mouse_packet = packet;
    
    input_event_node* new_event_node = (input_event_node*)malloc(sizeof(input_event_node));
    new_event_node->event = new_event;
    new_event_node->next_event = NULL;

    if(input_event_list == NULL) {
      input_event_list = new_event_node;
      input_event_list_tail = new_event_node;
    }
    else {
      input_event_list_tail->next_event = new_event_node;
      input_event_list_tail = new_event_node;
    }
  }
  state = (state + 1) % 3;
}

static bool extended_flag = false; ///< The flag to identify multiple byte scancodes
void (event_handle_keyboard)() {
  if(!kbc_valid_output(false)){ //check if the output buffer has valid data
    kbc_discard_output(); //discard if not
    return;
  }
  else kbc_ih(); // handle interrupt if yes

  uint8_t scancode_nbytes;
  uint8_t scancode_byte1 = 0, scancode_byte2 = 0;

  if(keyboard_get_scancode() == EXTENDED_SCANCODE) { //wait for next interrupt
    extended_flag = true;
    return;
  }
  else if(extended_flag){ //received the second byte
    scancode_nbytes = 2;
    scancode_byte1 = EXTENDED_SCANCODE;
    scancode_byte2 = keyboard_get_scancode();
    extended_flag = false;
  }
  else { //one byte scancode
    scancode_nbytes = 1;
    scancode_byte1 = keyboard_get_scancode();
  }

  input_event new_event;
  new_event.event_type = KEYBOARD_EVENT;
  new_event.scancode_nbytes = scancode_nbytes;
  new_event.scancode_byte1 = scancode_byte1;
  new_event.scancode_byte2 = scancode_byte2;

  input_event_node* new_event_node = (input_event_node*)malloc(sizeof(input_event_node));
  new_event_node->event = new_event;
  new_event_node->next_event = NULL;

  if(input_event_list == NULL) {
    input_event_list = new_event_node;
    input_event_list_tail = new_event_node;
  }
  else {
    input_event_list_tail->next_event = new_event_node;
    input_event_list_tail = new_event_node;
  }
}

int (events_get_next)(input_event* next_event) {
  if(input_event_list == NULL) return 1;

  input_event_node* victim_node = input_event_list;
  memcpy(next_event, &(input_event_list->event), sizeof(input_event));
  input_event_list = input_event_list->next_event;
  free(victim_node);

  return 0;
}

int (events_clear)() {
  while(input_event_list != NULL) {
    input_event_node* victim_node = input_event_list;
    input_event_list = input_event_list->next_event;
    free(victim_node);
  }
  return 0;
}
