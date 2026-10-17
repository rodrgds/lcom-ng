#include "mouse.h"

int mouse_hook_id = 12;
uint8_t scancode;
extern unsigned h_res;	        /* Horizontal resolution in pixels */
extern unsigned v_res;	        /* Vertical resolution in pixels */

int (mouse_subscribe_int)(uint8_t *bit_no){
    *bit_no = (uint8_t) mouse_hook_id;
    if (sys_irqsetpolicy(MOUSE_IRQ,(IRQ_REENABLE|IRQ_EXCLUSIVE),&mouse_hook_id)){
        return 1;
    }
    return 0;
}

int (mouse_unsubscribe_int)() {
    if (sys_irqrmpolicy(&mouse_hook_id)){
        return 1;
    }
    return 0;
}

void (mouse_ih)() {
    uint8_t stat;
    uint8_t output;

    if (util_sys_inb(STAT_REG,&stat)){
        return;
    }

    if (stat & ST_OUT_BUF){
        if (util_sys_inb(OUT_BUF,&output)){
            return;
        }

        if (stat & (ST_PAR_ERR | ST_TO_ERR)){
            scancode = 0;
        }

        else {
            scancode = output;
            return;}
    }

    scancode = 0;
}

void (buildPacket)(struct packet * pacote) {
    
  pacote->lb = pacote->bytes[0] & BIT(0);
  
  pacote->rb = pacote->bytes[0] & BIT(1);

  pacote->mb = pacote->bytes[0] & BIT(2);

  pacote->delta_x = convert_2_complement(pacote->bytes[1],BIT(4), pacote);

  pacote->delta_y = convert_2_complement(pacote->bytes[2],BIT(5), pacote);

  pacote->x_ov = pacote->bytes[0] & BIT(6);

  pacote->y_ov = pacote->bytes[0] & BIT(7);

}

int(mouse_write_cmd)(uint32_t cmd, uint8_t *resp) {
  int count = 0;
  uint8_t stat;
  while (count < 3) {
    
    if (util_sys_inb(KBC_CMD_REG,&stat) == 0){
        
        if (stat & ST_IN_BUF) return 1;

        if (sys_outb(WRITE_CMD_BYTE, cmd) != 0) {return 1;} // escreve no porto 0x60 o comando que dá disable ao mouse data reporting

        if (util_sys_inb(OUT_BUF, resp) != 0) {return 1;} // lê o OUT_BUF para ver qual a resposta obtida

        return 0;
    }
    
    count++;
  }

  return 1;
}

int (mouse_enable_data_report)() {
  uint8_t resp;
  uint8_t seconderror = 0;

  do {
    if (sys_outb(KBC_CMD_REG, WRITE_BYTE_MOUSE) != 0) {return 1;} // escreve no porto 0x64 o commando 0xD4 para avisar que quer enviar um comando do rato

    if (mouse_write_cmd(ENA_DATA_REP, &resp) != 0) {return 1;} // escreve o comando 0xF4 , obtendo resposta 

    if (resp == ACK) {return 0;}
    else if (resp == NACK) {seconderror++;}
    else if (resp == ERROR){return 1;}
    
  } while (seconderror < 2);

  return 1;
}

int (mouse_disable_data_report)() {
    uint8_t resp;
    uint8_t error = 0;
    while (error < 2) {
        if (sys_outb(KBC_CMD_REG,WRITE_BYTE_MOUSE)) {return 1;} // escreve no porto 0x64 o commando 0xD4 para avisar que quer enviar um comando do rato

        if (mouse_write_cmd(DIS_DATA_REP,&resp)) {return 1;} // escreve o comando 0xF5 , obtendo resposta 

        if (resp == ACK) return 0;
        else if (resp == NACK) {error++;}
        else if (resp == ERROR) return 1;
        tickdelay(micros_to_ticks(20000));
    }
    return 1;
}

Cursor *cursor;

Cursor * create_cursor() {
  cursor = (Cursor *) malloc(sizeof(Cursor));
  xpm_load(cursor_xpm,XPM_8_8_8,&cursor->img);

  cursor->x = 20;
  cursor->y = 20;

  return cursor;
}

void draw_cursor(){
    draw_xpm(cursor->img.bytes,&cursor->img,cursor->x,cursor->y);
}

void clean_cursor() {
    delete_xpm(cursor->img,cursor->x,cursor->y);
}

void mouse_update(struct packet * pacote) {
  if (pacote->delta_x > 0) {
    if (cursor->x + pacote->delta_x > (int)h_res - cursor->img.width)
      cursor->x = (int)h_res - cursor->img.width;
    else
      cursor->x += pacote->delta_x;
  }
  else if (pacote->delta_x < 0) {
    if (cursor->x + pacote->delta_x < 0)
      cursor->x = 0;
    else
      cursor->x += pacote->delta_x;
  }
  if (pacote->delta_y < 0) {
    if (cursor->y + cursor->img.height - pacote->delta_y > (int)v_res)
      cursor->y = (int)v_res - cursor->img.height;
    else
      cursor->y -= pacote->delta_y;
  }
  else if (pacote->delta_y > 0) {
    if (cursor->y - pacote->delta_y < 0)
      cursor->y = 0;
    else
      cursor->y -= pacote->delta_y;
  }
  

}


unsigned int check_collision_main_menu () {
  if (cursor->x > PLAY_BUTTON_X_POS_LEFT && cursor->x < PLAY_BUTTON_X_POS_RIGHT && cursor->y > PLAY_BUTTON_Y_POS_UP && cursor->y < PLAY_BUTTON_Y_POS_DOWN){ // Play
    return 1;}
  else if (cursor->x > SCOREBOARD_BUTTON_X_POS_LEFT && cursor->x < SCOREBOARD_BUTTON_X_POS_RIGHT && cursor->y > SCOREBOARD_BUTTON_Y_POS_UP  && cursor->y < SCOREBOARD_BUTTON_Y_POS_DOWN) // Scoreboard
    return 2;  
  else if (cursor->x > EXIT_BUTTON_X_POS_LEFT && cursor->x < EXIT_BUTTON_X_POS_RIGHT && cursor->y > EXIT_BUTTON_Y_POS_UP && cursor->y <EXIT_BUTTON_Y_POS_DOWN )  // Exit
    return 3;
  else return 0;
}
