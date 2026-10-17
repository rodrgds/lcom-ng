#include "keyboard.h"

int kbc_hook_id=1;
static uint8_t kbc_data[2];
static int err;
bool flag = false;

int(kbc_subscribe_int)(uint8_t *bit_no) {

  *bit_no = BIT(kbc_hook_id);

  if (sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbc_hook_id) != OK) {
    printf("Error in %s \n", __func__);
    return ERROR;
  }

  return OK;
}


int(kbc_unsubscribe_int)() {
  if (sys_irqrmpolicy(&kbc_hook_id) != OK) {
    printf("Error in %s \n", __func__);
    return ERROR;
  }

  return OK;
}

int(kbc_write_cmd)(uint8_t command){
  uint8_t status;
  int attemps = MAX_ATTEMPS;

  while (attemps)
  {
    if(kbc_read_status(&status) != OK){
      printf("Error reading status at: %s \n",__func__);
      return ERROR;
    }

    if((status & STATUS_IBF) == OK){
      if(sys_outb(KBC_IN_BUF,command)){
        printf("Error: Sys_out error at: %s \n", __func__);
        return ERROR;
      }

      return OK;

    }

    tickdelay(micros_to_ticks(DELAY_US));
    attemps--;
  }

  printf("Error: Timeout at: %s \n", __func__);
  return ERROR;
}

int(kbc_read_cmd)(uint8_t *data) {

  uint8_t status;
  int attemps = MAX_ATTEMPS;

  while (attemps) {
    
    if(kbc_read_status(&status)){
      printf("Error reading status\n");
      attemps--;
      continue;
    }
    
    /* loop while 8042 output buffer is empty */
    if (status & STATUS_OBF) {
      util_sys_inb(KBC_OUT_BUF, data);  // segmentation fault
      if(*data == FLAG_BREAK){
        flag = true;
        return OK;
      }

      data[0] = *data;
      data[1] = (flag) ? FLAG_BREAK : 0;
      flag = false;

      if ((status & (STATUS_PARITY | STATUS_TIMEOUT)) == 0){
        return OK;
      }
      else{
        return ERROR;
      }
    }
    printf("Waiting\n");
    tickdelay(micros_to_ticks(DELAY_US));
    attemps--;
  }

  printf("exit  %s\n", __func__);
  return OK;
}

int(kbc_read_status)(uint8_t *status) {
  return util_sys_inb(KBC_STAT_REG, status);
}

void(kbc_ih)() {
  err = kbc_read_cmd(kbc_data);
}

int get_kbc_error() {
  return err;
}

int(get_kbc_data)(uint8_t *data) {
  data[0] = kbc_data[0];
  data[1] = kbc_data[1];
  return OK;
}

int get_flag(bool *t) {
  *t = flag;
  return OK;
}




