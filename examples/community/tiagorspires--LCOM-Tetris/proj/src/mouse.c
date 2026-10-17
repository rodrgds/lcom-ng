#include "i8042.h"
#include <lcom/lcf.h>
#include "mouse.h"


int mouse_hook_id=2;
uint32_t count=0;
struct packet pp;
int i=0;
bool valid_packet=true;
bool update_count=false;
int x = 10, y = 10;
int max_x = 1024, max_y = 768;


int (mouse_subscribe_int)(uint8_t* bit_no){
  if(bit_no == NULL) return 1;
  *bit_no = BIT(mouse_hook_id);
  if(sys_irqsetpolicy(IRQ_LINE_MOUSE, IRQ_REENABLE|IRQ_EXCLUSIVE, &mouse_hook_id)!=0) return 1;
  return 0;
}

int (mouse_unsubscribe_int)(){
  if(sys_irqrmpolicy(&mouse_hook_id)) return 1;
  return 0;
}

void (mouse_ih)(){
  uint8_t status;

  if(util_sys_inb(STATUS_PORT,&status)) return ;

  if((status & MOUSE) && (status & OUT_BUF_FULL)){

    uint8_t byte;

    if(util_sys_inb(DATA_PORT,&byte)) return ;

    if(!(status & ERROR)){

        if(i==0 && (!(byte & FIRST_BYTE))){
          i--;
        }

        else{
          pp.bytes[i]=byte;
        }
        if(i==2){ 

          if(!valid_packet){
            valid_packet=true;
          }else{

            pp.lb = pp.bytes[0] & LEFT_BUTTON;
            pp.mb = pp.bytes[0] & MIDDLE_BUTTON;
            pp.rb = pp.bytes[0] & RIGHT_BUTTON;
            pp.x_ov = pp.bytes[0] & X_OVFL;
            pp.y_ov = pp.bytes[0] & Y_OVFL;
            bool x_neg = pp.bytes[0] & X_SIGN;
            bool y_neg = pp.bytes[0] & Y_SIGN;

            int16_t delta_x=pp.bytes[1];
            int16_t delta_y=pp.bytes[2];
            
            if(x_neg){
              delta_x |= SIGN_EXTENSION;
            }
            if(y_neg){
              delta_y |= SIGN_EXTENSION;
            }

            pp.delta_x = delta_x;
            pp.delta_y = delta_y;

            update_count=true;
            i=-1;
          }
        }
    }
    else valid_packet=false;

    i++;
  }
}

int send_command(uint8_t command) {
    return sys_outb(STATUS_PORT, command);
}

int read_buffer(uint8_t *buffer) {
    return util_sys_inb(DATA_PORT, buffer);
}

bool is_input_buffer_full() {
    uint8_t status;
    if (util_sys_inb(STATUS_PORT, &status)) {
        return false;
    }
    return status & IN_BUF_FULL;
}

int change_data_report_mode(uint8_t set) {
    int errorCount = 0;
    bool error = false;
    uint8_t receiver;

    while (errorCount < 2) {
        if (send_command(BYTE_TO_MOUSE)) {
            error = true;
            break;
        }

        int commandCount = 0;
        while (commandCount < 3) {
            if (is_input_buffer_full()) {
                commandCount++;
            } else {
                if (sys_outb(DATA_PORT, set)) {
                    error = true;
                    break;
                }
                if (read_buffer(&receiver)) {
                    error = true;
                    break;
                }
                break;
            }
        }

        if (error) {
            break;
        }

        if (receiver == ACK) {
            return 0;
        } else if (receiver == NACK) {
            errorCount++;
        } else if (receiver == ERROR_REP) {
            error = true;
            break;
        }
    }

    return error ? 1 : 0;
}

void mouse_event_handler(struct packet *pp){
  x += pp->delta_x;
  y -= pp->delta_y;

  if (x < 0) x = 0;
  else if (x > max_x - 1) x = max_x - 1;

  if (y < 0) y = 0;
  else if (y > max_y - 1) y = max_y - 1;
}






