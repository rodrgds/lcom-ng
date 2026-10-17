#include "mouse.h"


int (mouse_subscribe_int)(uint8_t *bit_no){
    
  mouse_hook_id = *bit_no;
  
  return sys_irqsetpolicy(MOUSE_IRQ, (IRQ_REENABLE|IRQ_EXCLUSIVE), &mouse_hook_id); 
}

int (mouse_unsubscribe_int)(){
  return sys_irqrmpolicy(&mouse_hook_id);
}

void (mouse_ih)(){
  uint8_t st;

  for (int counter = 0; counter < 3; counter++){
        
    if ((util_sys_inb(KBC_ST_REG, &st)) != 0){  
      found_err = 1;
      return;
    }

    if (st & KBC_ST_OBF){
      
      if (st & KBC_ERR){
        found_err = 1;
        return;                
      }
      
      if ((util_sys_inb(KBC_OUT_BUF, &m_data)) != 0){  
        found_err = 1;
      }
      
      return;
    }   
     
    tickdelay(micros_to_ticks(DELAY_US));
  }

  data_read_to = 1;
}

int (mouse_issue_cmd)(uint8_t mouse_cmd){

  if ((kbc_issue_cmd(KBC_WR_BYTE_MOUSE)) != 0) return 1;

  uint8_t st, ack;

  for (int i = 0; i < 3; i++){

    if ((util_sys_inb(KBC_ST_REG, &st)) != 0) return 1;

    if ((st & KBC_ST_IBF) == 0){
      
      if ((sys_outb(KBC_WRITE_CMD_ARG, mouse_cmd)) != 0) return 1;

      if ((get_mouse_ack(&ack)) != 0) return 1;

      switch (ack){
        case MOUSE_ACK: return 0;

        case MOUSE_NACK: break;

        case MOUSE_ERROR: return 1;
      
        default: return 1;
      }
    }

    tickdelay(micros_to_ticks(DELAY_US));
  }

  return 1;
}

int (get_mouse_ack)(uint8_t * ack){

  uint8_t st, tmp_ack;

  for (int i = 0; i < 3; i++){
    
    if ((util_sys_inb(KBC_ST_REG, &st)) != 0){
      return 1;
    }

    else {

      if (st & KBC_OUT_BUF_BIT){
        if ((util_sys_inb(KBC_OUT_BUF, &tmp_ack)) != 0) return 1;
        
        *ack = tmp_ack;
        
        return 0;
      }
    }

    tickdelay(micros_to_ticks(DELAY_US));
  }

  return 1;
}

