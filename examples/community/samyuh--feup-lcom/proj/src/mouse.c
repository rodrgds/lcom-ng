#include <lcom/lcf.h>
#include <lcom/lab4.h>

#include <stdint.h>

#include "mouse.h"
#include "i8042.h"

int mouse_setHook = 8;
int mouseByteCounter = 0;
bool mouseSync = false;
bool triggered = true;

uint8_t mouseByte[3];

void (mouse_ih)(void){
  uint8_t outBuf = 0;
  util_sys_inb(KBC_OUT_BUF, &outBuf);
    if((mouseByteCounter == 0) && (outBuf & BIT(3)))
      mouseSync = true;

    else if(mouseByteCounter == 0)
      mouseSync = false;


    if(mouseSync){
      mouseByte[mouseByteCounter] = outBuf;
      mouseByteCounter++;
    }
}

int (read_packet)(struct packet *p){
    p->bytes[0] = mouseByte[0];
    p->bytes[1] = mouseByte[1];
    p->bytes[2] = mouseByte[2];
    p->lb = LEFT_MOUSE_BUTTON & mouseByte[0];
    p->rb = RIGHT_MOUSE_BUTTON & mouseByte[0];
    p->mb = MIDDLE_MOUSE_BUTTON & mouseByte[0];
    if(BIT(4) & mouseByte[0])
      p->delta_x = mouseByte[1] - 256;
    else
      p->delta_x = mouseByte[1];

    if(BIT(5) & mouseByte[0])
      p->delta_y = mouseByte[2] - 256;
    else
      p->delta_y = mouseByte[2];
    p->x_ov = X_OVERFLOW & mouseByte[0];
    p->y_ov = Y_OVERFLOW & mouseByte[0];
    return 0;
}

int (mouse_subscribe_int)(uint8_t *bit_no) {
  *bit_no = mouse_setHook;
  if(sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_setHook)!= 0) return 1;
  return 0;
}

int (mouse_unsubscribe_int)() {
  if(sys_irqdisable(&mouse_setHook) != 0) return 1;
  if(sys_irqrmpolicy(&mouse_setHook)!= 0) return 1;
  return 0;
}

int (write_cmd_command)(uint8_t command){
  sys_outb(KBC_CMD_REG, command);
  return 0;
}

int (write_cmd_argument)(uint8_t argument){
  uint8_t status = 0;
  for (int i = 0; i < 10; i++){
    util_sys_inb(KBC_STAT_REG, &status);

    if ((status & KBC_IBF) == 0){
      sys_outb(KBC_OUT_BUF, argument);
      return 0;
    }

    tickdelay(micros_to_ticks(KBC_DELAY_US));
    }

  return 1;
}

int (write_mouse)(uint8_t mouse_cmd){
  uint8_t status;
  for (unsigned int i = 0; i < 10; i++){
    write_cmd_command(MOUSE_BYTE);
    write_cmd_argument(mouse_cmd);

    util_sys_inb(KBC_OUT_BUF, &status);
    if (status == MOUSE_ACK){
      return 0;
    }
    tickdelay(micros_to_ticks(KBC_DELAY_US));
    }
  return 1;
}

int (mouse_enable_data_report)()
{
  uint8_t status;

  for (unsigned int i = 0; i < 10; i++){
    write_cmd_command(MOUSE_BYTE);
    write_cmd_argument(MOUSE_ENABLE_REPORTING);
    util_sys_inb(KBC_OUT_BUF, &status);

    if (status == MOUSE_ACK){
      return 0;
    }
    tickdelay(micros_to_ticks(KBC_DELAY_US));
    }
  return 1;
}

int (mouse_disable_data_report)(){
  uint8_t status;
  for (unsigned int i = 0; i < 10; i++){
    write_cmd_command(MOUSE_BYTE);
    write_cmd_argument(MOUSE_DISABLE_REPORTING);
    util_sys_inb(KBC_OUT_BUF, &status);

    if (status == MOUSE_ACK){
      return 0;
    }
    tickdelay(micros_to_ticks(KBC_DELAY_US));
    }
  return 1;
}


int (minix_default_cmd_byte)(){
  for (unsigned int i = 0; i < 10; i++){
    write_cmd_command(WRITE_COMMAND_BYTE);
    write_cmd_argument(minix_get_dflt_kbc_cmd_byte());
    tickdelay(micros_to_ticks(KBC_DELAY_US));
    }

  return 0;
}

struct mouse_ev * (mouse_event_handler)(struct packet *p){
  static bool lb_change = true, rb_change = true, mb_change = true;
  static struct mouse_ev ev;
  ev.delta_x = p->delta_x;
  ev.delta_y = p->delta_y;

  if (lb_change && rb_change && mb_change && (p->mb == 0) && (p->rb == 0) && (p->lb == 1)) {
    lb_change = false;
    ev.type = LB_PRESSED;
  }
  else if (rb_change && lb_change && mb_change && (p->mb == 0) && (p->rb == 1) && (p->lb == 0)) {
    rb_change = false;
    ev.type = RB_PRESSED;
  }
  else if (!lb_change && rb_change && mb_change && (p->mb == 0) && (p->rb == 0) && (p->lb == 0)) {
    lb_change = true;
    ev.type = LB_RELEASED;
  }
  else if (!rb_change && lb_change && mb_change && (p->mb == 0) && (p->rb == 0) && (p->lb == 0)) {
    rb_change = true;
    ev.type = RB_RELEASED;
  }
  else if (mb_change && p->mb) {
    mb_change = true;
    ev.type = BUTTON_EV;
  }
  else if(!mb_change && !p->mb) {
    mb_change = true;
    ev.type = BUTTON_EV;
  }
  else
    ev.type = MOUSE_MOV;
  return &ev;
}


void (state_machine_draw_mouse)(struct mouse_ev *ev, uint8_t x_len, uint8_t tolerance){
  static enum mouse_state q_state = INIT;
  static int x_mov =0, y_mov=0;
  switch (q_state)
  {
  case INIT:
    //If LB is pressed, starts drawing
    if(ev->type == LB_PRESSED) {
      q_state = DRAW1;
      x_mov = 0;
      y_mov = 0;
    }
    //Otherwise just stays in the initial state
    break;

  case DRAW1://Drawing line from start to vertex state
    //If there is mouse movement, stores that movement
    if(ev->type == MOUSE_MOV){
      x_mov += ev->delta_x;
      y_mov += ev->delta_y;

      //Resets if delta values surpass allowed tolerance
      if(((ev->delta_x < 0) && (abs(ev->delta_x) > tolerance)) ||
      ((ev->delta_y < 0) && (abs(ev->delta_y) > tolerance))) q_state = INIT;
    }
    //If LB is released and line was drawn accordingly, reaches vertex
    if(ev->type == LB_RELEASED){
      if((abs(y_mov) > abs(x_mov)) && (x_mov >= x_len)){
        q_state = VERTEX;
        x_mov = 0;
        y_mov = 0;
      }
      //Resets if mouse is released and the line was not drawn accordingly
      else q_state = INIT;
      }
      break;

  case VERTEX://Beeing in the vertex of the inverted "V" state
    //If mouse moves, stores that movement
    if(ev->type == MOUSE_MOV){
      x_mov += ev->delta_x;
      y_mov += ev->delta_y;
    }
    //If RB is pressed, starts drawing the line from vertex to finishing state
    if(ev->type == RB_PRESSED){
      q_state = DRAW2;
      x_mov = 0;
      y_mov = 0;
    }
    //resets if there are pressed mouse buttons other than LB and RB
    if(ev->type == BUTTON_EV) q_state = INIT;
    break;

  case DRAW2: //Drawing line from vertex to the finishing place state
    //If mouse moves
    if(ev->type == MOUSE_MOV){
      q_state = DRAW2;
      x_mov += ev->delta_x;
      y_mov += ev->delta_y;
      //Resets if delta values surpass allowed tolerance
      if(((ev->delta_x < 0) && (abs(ev->delta_x) > tolerance)) ||
            ((ev->delta_y > 0) && (abs(ev->delta_y) > tolerance))) q_state = INIT;
    }

    if(ev->type == RB_RELEASED){
      if((abs(y_mov) > abs(x_mov)) && (x_mov >= x_len)){
        triggered = false;
      }
      //If mouse is released and the line was not drawn accordingly
      else q_state = INIT;
    }
    break;

  default:
    break;
  }
}
