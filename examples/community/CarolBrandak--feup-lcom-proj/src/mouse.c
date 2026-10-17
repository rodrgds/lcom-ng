#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>
#include "mouse.h"
#define MOUSE_OUT_BUF 0x60
#define MOUSE_PAR_ERR BIT(7)
#define MOUSE_TO_ERR BIT(6)
#define MOUSE_ST_REG 0x64
#define MOUSE_CMD_REG 0x64
#define MOUSE_OBF BIT(0)

int hook_id_mouse=2;
bool codeDoneMouse=false;
struct packet pp;
int step=0;
uint8_t data;
uint8_t stat;





void initMouse(Mouse * mouse){
  xpm_image_t img;
  xpm_load(mouse1_xpm,XPM_8_8_8_8,&img);
  xpm_image_t img2;
  xpm_load(mouse2_xpm,XPM_8_8_8_8,&img2);
  mouse->x=550;
  mouse->y=432;
  mouse->img1=img;  
  mouse->img2=img2;
  mouse->hover=false;
}

void resetMouse(Mouse * mouse){
  mouse->x=550;
  mouse->y=432;
}

int (mouse_subscribe_int)(uint8_t *bit_no) {
  *bit_no= hook_id_mouse;
  if (sys_irqsetpolicy(12, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse)){
    return 1;
  }
  return 0;
}

int (mouse_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id_mouse)){
    return 1;
  }
  return 0;
}

void (buildPacket)(struct packet * pp) {
    
  pp->lb = pp->bytes[0] & BIT(0);
  
  pp->rb = pp->bytes[0] & BIT(1);

  pp->mb = pp->bytes[0] & BIT(2);

  if (pp->bytes[0]&BIT(5)){
    pp->delta_y = pp->bytes[2]-256;
  }
  else{
    pp->delta_y = pp->bytes[2];
  }
  if (pp->bytes[0]&BIT(4)){
    pp->delta_x = pp->bytes[1]-256;
  }
  else{
    pp->delta_x = pp->bytes[1];
  }

  pp->x_ov = pp->bytes[0] & BIT(6);

  pp->y_ov = pp->bytes[0] & BIT(7);

}

struct packet getPacket(){
  return pp;
}





void (mouse_ih)(){
  if (codeDoneMouse){
    step=0;
    codeDoneMouse=false;
  }
  while( 1 ) {
    if (util_sys_inb(MOUSE_ST_REG, &stat)){
     data=0x00; 
     continue;
    }
    if( stat & MOUSE_OBF ) {
      if(util_sys_inb(MOUSE_OUT_BUF, &data)){
        data=0x00;
      } /* ass. it returns OK */
      if ( (stat &(MOUSE_PAR_ERR | MOUSE_TO_ERR)) == 0 ){
        break;
      }
      break;
    }
  }
    
  
  if (step==0 && (data & BIT(3))){
    pp.bytes[step]=data;
  }
  if (step==1){
    pp.bytes[step]=data;
  }
  if (step==2){
    pp.bytes[step]=data;
    codeDoneMouse=true;
    buildPacket(&pp);
  }
  else{
    step++;
  }
}


int toggle_data_report(uint8_t toggle){
  bool error=false;
  uint8_t inputBuffer;
  int errorCount=0;
  uint8_t receiver;
  while (errorCount<2){
    int commandCount=0;
    if (sys_outb(0x64,0xD4)){
      error=true;
    }
    while(commandCount<3){
      if (util_sys_inb(0x64,&inputBuffer)){
        error=true;
      }
      if (inputBuffer & BIT(1)){
        commandCount+=1;
      }
      else{
        if (sys_outb(0x60,toggle)){
          error=true;
        }
        if (util_sys_inb(0x60,&receiver)){
          error=true;
        }
        break;
      }
    }
    if (receiver==0xFA){
      break;
    }
    else if (receiver==0xFE){
      errorCount+=1;
    }
    else if (receiver==0xFC){
      error=true;
    }
  }
  if (!error){
    return 0;
  }
  else{
    return 1;
  }
}

