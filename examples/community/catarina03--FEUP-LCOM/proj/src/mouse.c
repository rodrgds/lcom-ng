#include <lcom/lcf.h>
#include "mouse.h"

int hook_mouse=KBD_AUX_IRQ; 
uint8_t byte_mouse=0;
uint8_t ack_byte=0;
//struct mouse_state machine_state;
bool status_error_mouse;


int (mouse_subscribe_int)(uint8_t *bit_no){
  *bit_no = hook_mouse;
  if(sys_irqsetpolicy(KBD_AUX_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE,&hook_mouse))return 1;
  return 0;
}


int (mouse_unsubscribe_int)(){

  if(returnvalue_mouse(&ack_byte)) //extracts the acknowledgment byte from the OUT_BUF
    return 1;

  if(returnvalue_mouse(&ack_byte)) //extracts the acknowledgment byte from the OUT_BUF
    return 1;
   return sys_irqrmpolicy(&hook_mouse);
}


void(mouse_ih)(){
  uint8_t status;
  //reads the status
  util_sys_inb(STATUS_REGISTER,&status);
  
  //checks if status error ocurred
  if(status & (PARITY_ERROR |TIMEOUT_ERROR))status_error_mouse=true;
  if(status & OBF)util_sys_inb(OUT_BUF,& byte_mouse);
  
}


int kbc_issue_arg(uint8_t cmd){
  uint8_t status;
  if(util_sys_inb(STATUS_REGISTER,&status))return 1;
  if((status & IBF)== 0){
    if ( (status & (PARITY_ERROR | TIMEOUT_ERROR)) == 0 ){
      if(sys_outb(KBC_CMD_ARGS, cmd))return 1;
      return 0;
    }
  }   
  return 1;

}


int kbc_issue_command(uint8_t cmd){
    uint8_t stat;
    uint8_t tries= 0; //variable to count the amount of times the operation was not sucessfull

    while( tries <= 10) { // gives up after trying 10 times

      if(util_sys_inb(STATUS_REGISTER, &stat))return 1;
 
        if( (stat & (IBF |PARITY_ERROR| TIMEOUT_ERROR) )== 0) { //if the input buffer is not full and there's no parity or timeout errors

          if(sys_outb(IN_BUF, cmd))return 1;
          
          return 0;
        }
         
    
    tickdelay(micros_to_ticks(DELAY));
    tries++; //increments the counter
    }

    return 1;
}


int returnvalue_mouse(uint8_t *ret){
  uint8_t temp;
  uint8_t status;
  uint8_t tries  = 0; //counts the amount of times that we had to wait due to the input buffer being full

    while( tries <= 10 ){    //it gives up after 10 tries

    if(util_sys_inb(STATUS_REGISTER, &status))return 1;

    if( (status & OBF) && (status & MOUSE_ACTIVE) ) {  //if the output buffer is full and mouse is active

        if(util_sys_inb(OUT_BUF, &temp))return 1;
          
        if ( (status &(PARITY_ERROR | TIMEOUT_ERROR)) == 0 ){
            *ret = (uint8_t) temp;
            return 0;
        }
        else return 1;
    }
    tickdelay(micros_to_ticks(DELAY));
    tries++; //increments the counter
    }
  return 1; 
}


int issue_cmd_mouse(uint8_t mouse_cmd, uint8_t *ack_byte){
  if(kbc_issue_command(WRITE_BYTE_MOUSE))return 1;

  if(kbc_issue_arg(mouse_cmd)) return 1;//issues the mouse command as the arg of command 0xD4
    

  do{

  if(returnvalue_mouse(ack_byte)) //extracts the acknowledgment byte from the OUT_BUF
    return 1;

  }while(! ((*ack_byte == ACK) || (*ack_byte == NACK) || (*ack_byte == ERROR))); //makes sure that value read from OUT_BUF was indeed the acknowledgment byte of our command

  return 0;
}


int16_t convert(uint8_t delta, bool sign){
  if(sign){
    if(delta==0)return -256;
    else return -((delta^0xFF)+1);

  }
  else return delta;
}


struct mouse_ev* (mouse_detect_ev)(struct packet *pp){
  static bool first_ev = true;//check if its the first function call (if its the first pack read)
  static struct mouse_ev me; //event to return 
  struct mouse_ev* me_ptr;
  static struct packet ultimo_pac; // to store the previous packet
  
  if (first_ev) {
    me.type = BUTTON_EV;
    first_ev= false;
  }
  else {
    if (pp->lb && !ultimo_pac.lb) {
      me.type = LB_PRESSED;
    }
    else if (!pp->lb && ultimo_pac.lb) {
      me.type = LB_RELEASED;
    }
    else if (pp->rb && !ultimo_pac.rb) {
      me.type = RB_PRESSED;
    }
    else if (!pp->rb && ultimo_pac.rb) {
      me.type = RB_RELEASED;
    }
    else if ((!pp->mb && ultimo_pac.mb)&&(pp->mb && !ultimo_pac.mb)) {
      me.type = BUTTON_EV;
    }
    else if (pp->bytes[2] || pp->bytes[1] ) {
      me.type = MOUSE_MOV;
      me.delta_x = pp->delta_x;
      me.delta_y = pp->delta_y;
    }
  }

  ultimo_pac = *pp;
  me_ptr=&me;
  return me_ptr;
} 



