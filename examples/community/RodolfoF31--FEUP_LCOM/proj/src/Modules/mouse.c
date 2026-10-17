#include "mouse.h"
#include <stdbool.h>

int mouse_id;
uint8_t counter;
bool error_reading = false;
uint8_t mouse_byte;
static state_t pattern_state = INIT; 
static int lineXLen = 0; 
static int lineYlen = 0;
bool draw_line_up = false;
bool draw_line_down = false;
bool draw_line_up_done = false;



void first_byte(struct packet *pp, bool *x_negative, bool *y_negative){
  pp->bytes[0] = mouse_byte;
  pp->rb = false;
  pp->mb = false;
  pp->lb = false;
  pp->x_ov = false;
  pp->y_ov = false;
  *x_negative = false;
  *y_negative = false;
  pp->delta_x = 0;
  pp->delta_y = 0;

  if(mouse_byte & Y_OVFL){
      pp->y_ov = true;
  }
  if(mouse_byte & X_OVFL){
      pp->x_ov = true;
  }
  if(mouse_byte & Y_SIGN){
      *y_negative = true;
  }
  if(mouse_byte & X_SIGN){
      *x_negative = true;
  }
  if(mouse_byte & MID_BUTTON){
      pp->mb = true;
  }
  if(mouse_byte & RIGHT_BUTTON){
      pp->rb = true;
  }
  if(mouse_byte & LEFT_BUTTON){
      pp->lb = true;
  }
}

void second_byte(struct packet *pp, bool *x_negative){
  pp->bytes[1] = mouse_byte;
  if(*x_negative){
    pp->delta_x = mouse_byte -256;
  }
  else{
    pp->delta_x = mouse_byte; 
  }
}

void third_byte(struct packet *pp, bool *y_negative){
  pp->bytes[2] = mouse_byte;
  if(*y_negative){
    pp->delta_y = mouse_byte -256;
  }
  else{
    pp->delta_y = mouse_byte; 
  }
}

int write_to_mouse(uint8_t argument){
  uint8_t acknowledge;
  uint8_t st_reg;
  uint8_t fails = 0;

  while(fails < FAILS_MAX){
    if(util_sys_inb(STATUS_REG ,&st_reg) != 0){
      printf("ERROR READING STATUS_REG");
      return 1;
    }

    if((st_reg & IBF_ERROR) == 0){
      if(sys_outb(IN_BUF_COMMAND, WRITE_MOUSE)){
        return 1;
      }
    }
    else{continue;}

    if(util_sys_inb(STATUS_REG ,&st_reg) != 0){
      printf("ERROR READING STATUS_REG");
      return 1;
    }

    if((st_reg & IBF_ERROR) == 0){
      if(sys_outb(IN_BUF_ARG, argument)){
        return 1;
      }
    }
    else{continue;}

    tickdelay(micros_to_ticks(20000));

    if(util_sys_inb(OUT_BUF, &acknowledge)){
      return 1;
    }

    if(acknowledge == ACK){
      break;
    }

    fails++;
  }
  return 0;
}

int mouse_enable_info(){
  if(write_to_mouse(STREAM_MODE)){
    return 1;
  }
  if(write_to_mouse(ENABLE_DATA_REPORT)){
    return 1;
  }
  return 0;
}

int mouse_disable_info(){
  if(write_to_mouse(DISABLE_DATA_REPORT)){
    return 1;
  }
  if(write_to_mouse(STREAM_MODE)){
    return 1;
  }

  return 0;
}

int (mouse_subscribe_int)(uint8_t *bit_no){
  *bit_no = BIT(4);
  mouse_id = 4;
  if(sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_id) != 0){
    printf("CANNOT SUBSCRIBE INT\n");
    return 1;
  }
  return 0;
}

int (mouse_unsubscribe_int)(){
  if(sys_irqrmpolicy(&mouse_id) == 1){
    printf("CANNOT UNSUBSCRIBE INT\n");
    return 1;
  }

  return 0;
}

void (mouse_ih)(void){
  uint8_t st_reg;

  if(util_sys_inb(STATUS_REG ,&st_reg) != 0){
    printf("ERROR READING STATUS_REG");
    error_reading = true;
    counter++;
    return;
  }

  if((st_reg & PARITY_ERROR) != 0){
    printf("PARITY ERROR\n");
    error_reading = true;
    counter++;
    return;
  }

  if((st_reg & TIMEOUT_ERROR) != 0){
    printf("TIMEOUT ERROR\n");
    error_reading = true;
    counter++;
    return;
  }

  if((st_reg & OBF_ERROR) == 0){
    printf("ERROR OUTPUT BUFFER EMPTY \n");
    error_reading = true;
    counter++;
    return;
  }

  if(util_sys_inb(OUT_BUF ,&mouse_byte) != 0){
    printf("ERROR READING STATUS_REG");
    error_reading = true;
    counter++;
    return;
  }
  counter++;
  tickdelay(micros_to_ticks(20 * 10000));
}

void mouse_check_event(struct packet *pp, event_type_t *event){
  bool lb_click = (pp->lb == 1 && pp ->rb == 0 && pp->mb == 0);
  bool rb_click = (pp->rb == 1 && pp ->lb == 0 && pp->mb == 0);
  bool mb_click = (pp->rb == 0 && pp ->lb == 0 && pp->mb == 1);
  bool no_click = (pp->lb == 0  && pp ->rb == 0 && pp->mb == 0);

  if(lb_click){
    *event = LBDOWN;
    if(!draw_line_up){
      draw_line_up = true;
    }  
  }else if(rb_click){
    *event = RBDOWN;
    if(draw_line_up_done && !draw_line_down){
      draw_line_down = true;
    }
  }else if(no_click && draw_line_up){
    *event = LBUP;
    draw_line_up = false;
  }else if(no_click && draw_line_down){
    *event = RBUP;
    draw_line_down = false;
  }else if(mb_click){
    *event = INVALID;
  }
}

int mouse_check_pattern(mouse_event_t evt, uint8_t x_len, uint8_t tolerance) {
  switch (pattern_state) {
    case INIT: 
      lineXLen = 0;
      lineYlen = 0;
      if (evt.type == LBDOWN) {  
        pattern_state = DRAW_UP;
      } 
      break;
    case DRAW_UP:
      if (evt.type == LBDOWN) { 
        lineXLen += evt.moveX;
        lineYlen += evt.moveY;
      }else if (evt.type == LBUP){ 
         if(lineXLen >= x_len && (lineYlen/lineXLen) > 1.0 ){
            pattern_state = VERTEX;
            draw_line_up_done = true;
         }
      } 
      else { 
        pattern_state = INIT; 
      }
      break; 
    case VERTEX:
      lineXLen = 0;
      lineYlen = 0;
      if(evt.type == RBDOWN)
        pattern_state = DRAW_DOWN;
      else if( evt.type == LBDOWN){
        lineXLen = 0;
        lineYlen = 0;
        pattern_state = DRAW_UP;
      }else if(evt.type == INVALID){
        pattern_state = INIT;
      }
     break;
    case DRAW_DOWN:
      if(evt.type == RBDOWN){
        lineXLen += evt.moveX;
        lineYlen += evt.moveY;
      }else if(evt.type == RBUP){
        if(lineXLen >= x_len && (lineYlen/lineXLen) > 1.0 )
          return 0;
      }else if(evt.type == LBDOWN){
        lineXLen = 0;
        lineYlen = 0;
        pattern_state = DRAW_UP;
      }else{
        pattern_state = INIT;
      }
    break;
    default:  
      break;
  }
  return 1; 
}
