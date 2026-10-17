#include <lcom/lcf.h>
#include <stdint.h>
#include "keyboard.h"
#include "i8042.h"

//global variables

//keyboard hook id
int kb_hookId;      
//counts the number of times sys_inb is called
uint32_t counter = 0;
//self explanatory
uint8_t scancode;         


//same as sys_inb but it count the number of times it is called
int(sys_inb_count)(port_t port, uint32_t *byte) {
  counter++;
  return sys_inb(port, byte);
}

int(keyboard_subscribe_int)(uint8_t *interrupt_request) {
  kb_hookId =1;
  //interrupt_request retqurns bit mask created from hook_id as argument
  *interrupt_request = BIT(kb_hookId);

  //subscribes notifications from keyboard using the hookID
  if (sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kb_hookId) != 0) {
    printf("sys_irqsetpolicy: Error setting policy\n");
    return 1;
  }

  return 0;
}

int(keyboard_unsubscribe_int)() {
  //Removes the subscription
  if (sys_irqrmpolicy(&kb_hookId) != 0) {
    printf("sys_irqrmpolicy: Error unsubscribing\n");
    return 1;
  }
  return 0;
}

int(keyboard_read_status)(uint8_t *status) {
  //temporary variable to save status reg
  uint32_t temp;

  if (sys_inb_count(STATUS_REG, &temp) != 0) {
    printf("keyboard_read_status: Error at sys_inb_count\n");
    return 1;
  }

  //status is returned as an argument which contains the status byte
  *status = (uint8_t) temp;
  return 0;
}

int(keyboard_read_buffer)(uint8_t *data) {
  //temporary variable to save contents of output buffer
  uint32_t temp;

  if (sys_inb_count(OUT_BUF, &temp) != 0) {
    printf("keyboard_read_buffer: Error at sys_inb_count\n");
    return 1;
  }
  //output buffer value is returned as an argument
  *data = (uint8_t) temp;
  
  return 0;
}

bool(keyboard_check_status)() {
  uint8_t status;

  if (keyboard_read_status(&status) != 0) {
    printf("keyboard_check_status: Error reading status register\n");
    return 1;
  }

  //checks the ouput buffer
  if (status & KBC_OUTB) {
    //checks for errors and if the data comes from the kbd
    if ((status & (KBC_PARE |KBC_TIMEO | KBC_AUXB)) == 0) {
      return true;
    }
    else {
      return false; 
    }
  }
  else
    return false; 
}


//handles keyboards interrupts, checks first if the datas is good and
//then it reads it to the global variable scancodes
void(kbc_ih)() {
  if (keyboard_check_status())
    keyboard_read_buffer(&scancode);
}


int(keyboard_write_command)(uint32_t cmd, uint32_t args) {
  uint8_t status;
  int calls = 0;

  //tries to write the command 3 times then stops
  while (calls < 3) {
    if (keyboard_read_status(&status) != OK) {
      printf("keyboard_write_command: Error at keyboard_read_status\n");
      return 1;
    }

    if ((status & KBC_INPB) == 0) {
      
      if (sys_outb(CMD_REG, cmd) != OK) {
        printf("keyboard_write_command: Error writing command to out_buf\n");
        return 1;
      }
      
      else if (cmd == WRITE_CMD_B) {
        if (sys_outb(CMD_ARGS_REG, args) != OK) {
          printf("keyboard_write_command: Error writing arguments to register\n");
          return 1;
        }
      }

      return 0;
    }

    
    calls++;
    //waits 20 ms between tries
    tickdelay(micros_to_ticks(DELAY_US));
  }
  return 0;
}

