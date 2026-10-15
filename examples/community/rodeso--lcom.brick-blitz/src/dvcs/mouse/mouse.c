#include <lcom/lcf.h>
#include "ps2.h" 


int hook_id3=0;
uint8_t response=0;
uint8_t mouse_bytes[3];
uint8_t byte_index=0;

struct packet mouse_packet;


int (mouse_write_data)(uint8_t command){
  int attempts=10; 
  uint8_t mouse_response;
  do {
    attempts--;
    if (write_command(INPUT_BUFFER, MOUSE_COMMAND_HEADS_UP)) return 1;              // Ativar do modo D4 do i8042
    if (write_command(OUTPUT_BUFFER, command)) return 1;           // O comando para o rato é escrito na porta 0x60
    tickdelay(micros_to_ticks(20000));                        // Esperar alguns milissegundos
    if (util_sys_inb(OUTPUT_BUFFER, &mouse_response)) return 1;        // Ler a resposta da porta do output buffer
    if (mouse_response == ACK) return 0;                      // Se a resposta for ACK, interromper o ciclo
  } while (mouse_response != ACK && attempts);       

 return 1;
}


void (mouse_ih)() {
    read_scancode(OUTPUT_BUFFER,&response);
    mouse_sync_bytes();
    if (byte_index==3) { 
      mouse_bytes_to_packet();
      byte_index=0;
    }
}

void (mouse_sync_bytes)(){
  if ((byte_index==0) && (response & BIT(3))){
    byte_index++;
    mouse_bytes[0]=response;
  }
  else if (byte_index>0){
     mouse_bytes[byte_index]=response;
    byte_index++;
  }
}

void (mouse_bytes_to_packet)(){
    for (int i = 0 ; i < 3 ; i++) {
    mouse_packet.bytes[i] = mouse_bytes[i];
  }
  mouse_packet.lb = mouse_bytes[0] & BIT(0);
  mouse_packet.mb = mouse_bytes[0] & BIT(2);
  mouse_packet.rb = mouse_bytes[0] & BIT(1);
  mouse_packet.x_ov = mouse_bytes[0] & BIT(6);
  mouse_packet.y_ov = mouse_bytes[0] & BIT(7);
  mouse_packet.delta_x = (mouse_bytes[0] & BIT(4)) ? (0xFF00 | mouse_bytes[1]) : mouse_bytes[1];
  mouse_packet.delta_y = (mouse_bytes[0] & BIT(5)) ? (0xFF00 | mouse_bytes[2]) : mouse_bytes[2];
}

int (mouse_subscribe_int)(uint8_t *bit_no) {

    if(bit_no==NULL) {return 1;}
    
    hook_id3=IRQ_LINE_MOUSE;
    *bit_no=hook_id3;

    int pol = sys_irqsetpolicy(IRQ_LINE_MOUSE,IRQ_REENABLE|IRQ_EXCLUSIVE,&hook_id3);
    if (pol==1) {return 1;}

    return 0;
}

int (mouse_unsubscribe_int)() {

    int pol = sys_irqrmpolicy(&hook_id3);
    if (pol==1) {return 1;}

    return 0;
}

