#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>
#include "macros.h"


uint8_t keyboard[2]={0,0};
int numBytes=0;
bool codeDone= false;
uint8_t stat;
uint8_t data;


int hook_id=1;

int (kdb_subscribe_int)(uint8_t *bit_no) {
  *bit_no= hook_id;
  if (sys_irqsetpolicy(1, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id)){
    return 1;
  }
  return 0;
}

int (kdb_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id)){
    return 1;
  }
  return 0;
}


void (kbc_ih)(){
  if (codeDone){
    numBytes=0;
    codeDone=false;
  }
  while( 1 ) {
    if (util_sys_inb(KBC_ST_REG, &stat)){
     data=0x00; 
     continue;
    }
    if( stat & KBC_OBF ) {
      if(util_sys_inb(KBC_OUT_BUF, &data)){
        data=0x00;
      } /* ass. it returns OK */
      if ( (stat &(KBC_PAR_ERR | KBC_TO_ERR)) == 0 ){
        break;
      }
      break;
    }
    
  }
  if (numBytes==0){
    if (data==0xE0){
      keyboard[0]=data;
    }
    else{
      keyboard[0]=data;
      codeDone=true;
    }
  }
  numBytes+=1;
  if (numBytes==2){
    keyboard[1]=data;
    codeDone=true;
  }

}





