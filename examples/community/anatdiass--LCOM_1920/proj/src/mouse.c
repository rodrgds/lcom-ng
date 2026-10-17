#include <lcom/lcf.h>
#include <stdint.h>
#include "mouse.h"

int mouse_hookID;
uint8_t byte;

int mouse_subscribe_int(uint8_t *bit_no) {

  mouse_hookID = *bit_no;

  if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hookID) != OK) {
    printf("Failed to subscribe the KBC interrupts.\n");
    return 1;
  }

  return 0;
}

int mouse_unsubscribe_int() {
  if (sys_irqrmpolicy(&mouse_hookID) != OK)
    return 1;
  return 0;
}

/*int util_sys_inb(int port, uint8_t *value) {
  uint32_t var;
  sys_inb(port, &var);
  *value = (uint8_t)var;

  return 0;
}*/

void(mouse_ih)(){

  uint8_t st;
  util_sys_inb(STAT_REG, &st);

  if (st & OBF) {
    util_sys_inb(OUT_BUF, &byte);
    if ((st & (PAR_ERR | TO_ERR)) == 0)
      return;
  }
}

int enableDataReporting() {
  sys_irqdisable(&mouse_hookID); //disable notifications
  if (write_command(MOUSE_ENABLE) == 1){
    printf("Failed to enable mouse data reporting\n");
    return 1;
  }
  sys_irqenable(&mouse_hookID); //enable notifications
  return 0;
}

int disableDataReporting() {
  sys_irqdisable(&mouse_hookID); //disable notifications
  if (write_command(MOUSE_DISABLE) == 1) {
    printf("Failed to disable mouse data reporting\n");
    return 1;
  }
  return 0;
}

void mouse_process_packets(struct packet *pp) {
  //Right button
  if (pp->bytes[0] & MOUSE_RB)
    pp->rb = true;
  else
    pp->rb = false;

  //Middle button
  if (pp->bytes[0] & MOUSE_MB)
    pp->mb = true;
  else
    pp->mb = false;

  //Left button
  if (pp->bytes[0] & MOUSE_LB)
    pp->lb = true;
  else
    pp->lb = false;

  //Mouse x-displacement
  if (pp->bytes[0] & MOUSE_X)
    pp->delta_x = pp->bytes[1] - 256;
  else
    pp->delta_x = pp->bytes[1];

  //Mouse y-displacement
  if (pp->bytes[0] & MOUSE_Y)
    pp->delta_y = pp->bytes[2] - 256;
  else
    pp->delta_y = pp->bytes[2];

  //Mouse x-displacement overflow
  if (pp->bytes[0] & MOUSE_X_OVERFLOW)
    pp->x_ov = true;
  else
    pp->x_ov = false;

  //Mouse y-displacement overflow
  if (pp->bytes[0] & MOUSE_Y_OVERFLOW)
    pp->y_ov = true;
  else
    pp->y_ov = false;
}

int write_command(uint8_t command) {

  uint8_t st, res;

  if (util_sys_inb(STAT_REG, &st) != OK)
    return 1;

  if (!(st & IBF)){
    if (sys_outb(CTRL_REG, MOUSE_WRITE_BYTE)) {
      return 1;
    }
  }

  if (util_sys_inb(STAT_REG, &st) != OK)
    return 1;

  if (!(st & IBF)) {
    if (sys_outb(IN_BUF, command)) {
      return 1;
    }
  }

  tickdelay(micros_to_ticks(DELAY_US));

  if (util_sys_inb(OUT_BUF, &res) != OK)
    return 1;

  if (res != ACK)
    write_command(command);

  return 0;
}


void mouse_gesture_handler(struct mouse_ev *evt, uint8_t x_len, uint8_t tolerance) {
  static enum state_t st = INIT; // initial state; keep state
  static int16_t xLength=0;
  static int16_t yLength=0;

  extern bool finalState;
  //enum state_t st;

  printf("%d\n", evt->type);
  switch (st) {
  case INIT:
    if (evt->type == LB_PRESSED){
      st = DRAW;  //Começa 1a linha
    }
    break;

  case DRAW:
   //outro botão é pressionado antes de soltar botao esquerdo
    //if ((evt->type == RB_PRESSED) | BUTTON_EV) {
      if ((evt->type == RB_PRESSED) || (evt->type==BUTTON_EV)) {

      st = INIT;
    }
    /** soltar botao esquerdo --> verifica inclinacao e deslocamento em x
    ** inclinacao > 1
    ** delta x > x_len */
    else if (evt->type == LB_RELEASED) {
      if ((abs(yLength / xLength) > 1) && (xLength >= x_len)) {
        st = FIRSTLINE; //Termina primeira linha
        }
      else{
        st = INIT;
      }
    }
    /** movimentacao durante o desenho da linha
    ** verifica que deslocamentos(grandes) nao podem ser negativos
    ** verifica "pequenos" deslocamentos negativos */
    else if(evt->type==MOUSE_MOV){
      if(((abs(evt->delta_x))<tolerance && (abs(evt->delta_y)<tolerance)) || (evt->delta_x >0 && evt->delta_y>0)){
        xLength = xLength + evt->delta_x;
        yLength = yLength + evt->delta_y;
      }
      else{
        st=INIT;
      }
    }
    break;
  case FIRSTLINE:
    xLength=0; yLength=0;
    //outro botão é pressionado entre o desenho das 2 linhas
    if ((evt->type == LB_PRESSED) || (evt->type == BUTTON_EV)) {
      st = INIT;
    }
    else if(evt->type ==  RB_PRESSED){
      st = DRAWSECOND;
    }
    break;
  case DRAWSECOND:
    /** soltar botao direito --> verifica inclinacao e deslocamento em x
    ** inclinacao > 1
    ** delta x > x_len */
    if(evt->type==RB_RELEASED){
      if((abs(yLength/xLength)>1) && (xLength>=x_len)){
        //st = COMP;
        st=INIT;
        finalState=true;
      }
      else{
        st=INIT;
      }
    }
    /** movimentacao durante o desenho da linha
    ** verifica que deslocamentos(grandes) nao podem ser negativos
    ** verifica "pequenos" deslocamentos negativos */
    else if(evt->type==MOUSE_MOV){
      if(((abs(evt->delta_x))<tolerance && (abs(evt->delta_y))<tolerance) || (evt->delta_x >0 && evt->delta_y<0)){
        xLength = xLength + evt->delta_x;
        yLength = yLength + evt->delta_y;
      }
      else{
        st = INIT;
      }
    }
    break;
  }
  printf("%d\n", st);
}

struct mouse_ev event;

struct mouse_ev* detectMouseEvent(struct packet *pp){

  bool leftButton=false;
  bool rightButton=false;
  event.delta_x = pp->delta_x;
  event.delta_y = pp->delta_y;

  //Checks buttons' pressure
  if(pp->lb && !pp->rb && !pp->mb && !leftButton){
    event.type = LB_PRESSED;
    leftButton = true;
  }
  else if (!pp->lb && !pp->rb && !pp->mb && leftButton){
    event.type = LB_RELEASED;
    leftButton = true;
  }
  else if(pp->rb && !pp->lb && !pp->mb && !rightButton){
    event.type = RB_PRESSED;
    rightButton = true;
  }
  else if(!pp->rb && !pp->lb && !pp->mb && rightButton){
    event.type = RB_RELEASED;
    rightButton = false;
  }

  //Checks movements
  else if (event.delta_x>0 || event.delta_y>0){
    event.type = MOUSE_MOV;
  }

  else {
    event.type = BUTTON_EV;
  }

  return &event;
}
