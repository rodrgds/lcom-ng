#include <lcom/lcf.h>
#include <keyboard.h>
#include <i8042.h>
#include <utils.h>

#include <stdint.h>

int KBC_HOOK_ID = KBC_IRQ;
uint8_t SCANCODE[2]={0,0};
bool SCANCODE_2B = false; // true if it is a 2 byte scancode 

int (kbc_subscribe_int)(uint8_t *bit_no) {
  *bit_no = KBC_HOOK_ID;

  //subscribe a notification on every interrupt in the IRQ1 (interrupt request line) of the KBC

  if(sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &KBC_HOOK_ID) != OK){
    return 1;
  }
  return 0;
}

int (kbc_unsubscribe_int()) {
  //unsubscribe a previous subscription of a specific interrupt notification (the one associated with the HOOK_ID)
  if(sys_irqrmpolicy(&KBC_HOOK_ID) != OK){
    return 1;
  }
  return 0;
}

void (kbc_ih)(){
  uint8_t state;
  uint8_t scancode_byte;

  while(1){
    if(util_sys_inb(STAT_REG, &state) != 0){
      break; 
    }
    else{
      if(kbc_read_state_field(state,ksf_obf) == 1 && kbc_read_state_field(state,ksf_aux) != 1) {

        if(util_sys_inb(OUT_BUF,&scancode_byte) != 0)
          break;
        else
        {
          if(kbc_read_state_field(state,ksf_parity) == 0 || kbc_read_state_field(state,ksf_timeout == 0)){
            if(SCANCODE_2B == false){
              if(scancode_byte == SCANCODE_2B_FB){
              SCANCODE_2B = true;
              SCANCODE[0] = scancode_byte;
              }
              else
                SCANCODE[0] = scancode_byte;
            }
            else{
              SCANCODE[1] = scancode_byte;
              SCANCODE_2B = false;
            }
            break;
          }
          else
             break;
        }  
      }

    }
  }
}

uint8_t kbc_read_state_field(const uint8_t state, enum kbc_state_field field) {
  uint8_t result;

  switch(field){
    case(ksf_all):
      result = state;
      break;
    case(ksf_parity):
      result = ((state & KB_SR_PARITY)>>7);
      break;
    case(ksf_timeout):
      result =((state & KB_SR_TIMEOUT)>>6);
      break;
    case(ksf_aux):
      result = ((state & KB_SR_AUX)>>5);
      break;
    case(ksf_inh):
      result = ((state & KB_SR_INH)>>4);
      break;
    case(ksf_a2):
      result = ((state & KB_SR_A2)>>3);
    case(ksf_sys):
      result = ((state & KB_SR_SYS)>>2);
      break;
    case(ksf_ibf):
      result = ((state & KB_SR_IBF)>>1);
      break;
    case(ksf_obf):
      result = (state & KB_SR_OBF);
      break;
    default:
      return 1;
  }

  return result;
}

int kbc_read_command_return(uint8_t *cmd){

  if(util_sys_inb(OUT_BUF,cmd) != 0)
    return 1;
  return 0;
}

int kbc_write_command_args(uint8_t cmdArg){
   uint8_t state;
 int fail = 0;
  int failLimit = 5;

  while (fail < failLimit) {
    
    if(util_sys_inb(STAT_REG, &state) !=0 ) return 1;
    
    if(kbc_read_state_field(state,ksf_ibf) == 0){
      if(sys_outb(KBC_CMD_ARGS,cmdArg) != 0)
        return 1;
      return 0;
    }
    tickdelay(micros_to_ticks(DELAY_US));
    fail++;
  }
  return 1;
}

int kbc_issue_command(uint8_t *state, uint8_t cmd){
  int fail = 0;
  int failLimit = 5;

  while (fail < failLimit) {
    if(util_sys_inb(STAT_REG, state) !=0 )return 1;
    
    if(kbc_read_state_field(*state,ksf_ibf) == 0){
      if(sys_outb(KBC_CMD_REG, cmd) != 0)
        return 1;
      return 0;
    }
    tickdelay(micros_to_ticks(DELAY_US));
    fail++;
  }
  return 1;
}
