#include "mouse.h"
#include "kbc.h"

uint8_t data;
uint8_t packet[3]; 
int byte_tracking = 0;

static int hook_id_mouse = 2;


void (mouse_ih)() {
  // lemos o status register
  uint8_t status_reg;

  if (util_sys_inb(ST_REG, &status_reg) != 0) {
    printf("status register reading failed");
    return;
  };

  // verificamos se o buffer esta full e se pertence ao rato
  if ((status_reg & KBC_OBF) && (status_reg & MOUSE_BIT)) {
    // verificamos erros de paridade ou de timeout

    if (!(status_reg & (KBC_PARITY | KBC_TIMEOUT))) {
      if (util_sys_inb(OUT_BUFFER, &data) != 0) {
        printf("error reading the output buffer");
        return; 
      };
    }
    else {
      util_sys_inb(OUT_BUFFER, &data); // lemos para descartar o buffer
      printf("error timeout/parity coulnt read data");
      return; 
    }

    if (byte_tracking == 0 && (data & BIT(3))) {
      packet[0] = data;
      byte_tracking++;
    }

    else if (byte_tracking == 1) {
      packet[1] = data;
      byte_tracking++;
    }
    else if (byte_tracking == 2) {
      packet[2] = data;
      byte_tracking++;      
    }
  }
}
void (mouse_parse_packet)(struct packet *p) {

    for (int i = 0; i < 3; i++) {
        p->bytes[i] = packet[i];
    }


    p->lb = packet[0] & BIT(0);
    p->rb = packet[0] & BIT(1);
    p->mb = packet[0] & BIT(2);


    p->x_ov = packet[0] & BIT(6);
    p->y_ov = packet[0] & BIT(7);


    p->delta_x = (packet[0] & BIT(4)) ? (0xFF00 | packet[1]) : packet[1];
    p->delta_y = (packet[0] & BIT(5)) ? (0xFF00 | packet[2]) : packet[2];
}


int mouse_write_cmd(uint8_t cmd) {
    uint8_t status, ack;
    int attempts = 10;

    while (attempts > 0) {
        // ler o status
        if (util_sys_inb(ST_REG, &status) != 0) return 1;
        
        if (!(status & KBC_IBF)) {
            // próximo byte é para o rato
            if (sys_outb(CMD_REG, MOUSE_WRITE_BYTE) != 0) return 1;

            // ler o status 
            if (util_sys_inb(ST_REG, &status) != 0) return 1;
            
            if (!(status & KBC_IBF)) {
                if (sys_outb(OUT_BUFFER, cmd) != 0) return 1;

                // esperar pelo ack
                tickdelay(micros_to_ticks(DELAY_US));

                if (util_sys_inb(ST_REG, &status) != 0) return 1;
                
                if (status & KBC_OBF) {
                    if (util_sys_inb(OUT_BUFFER, &ack) != 0) return 1;

                    if (ack == MOUSE_ACK) {
                        return 0; // funcionou
                    }
                    // Se for nack ou error tentamos outra vez 
                }
            }
        }
        
        attempts--;
        tickdelay(micros_to_ticks(DELAY_US));
    }

    return 1; 
}

int (my_mouse_disable_data_reporting)() {
    return mouse_write_cmd(MOUSE_DISABLE_DATA_REP);
  }

int (my_mouse_enable_data_reporting)() {
    return mouse_write_cmd(MOUSE_ENABLE_DATA_REP);
}

int (mouse_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  *bit_no = (uint8_t) hook_id_mouse;

  if (sys_irqsetpolicy(12, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse) != 0) {
    printf("Error: sys_irqsetpolicy failed\n");
    return 1;
  }

  return 0;
}

int (mouse_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id_mouse) != 0) {
    printf("Error: sys_irqrmpolicy failed\n");
    return 1;
  }
  return 0;
}
int (mouse_disable_int)() {

  if (sys_irqdisable(&hook_id_mouse) != 0) {
    printf("Error: sys_irqdisable failed\n");
    return 1;
  }
  return 0;
}

int (mouse_enable_int)() {
  if (sys_irqenable(&hook_id_mouse) != 0) {
    printf("Error: sys_irqenable failed\n");
    return 1;
  }
  return 0;
}