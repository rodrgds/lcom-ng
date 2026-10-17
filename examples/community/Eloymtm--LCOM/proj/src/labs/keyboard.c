#include <lcom/lcf.h>
#include "keyboard.h"


int kb_hook_id=1;
uint8_t scancode=0;
int (keyboard_subscribe_int)(uint8_t *bit_no) {

  *bit_no = BIT(kb_hook_id);
  if(sys_irqsetpolicy(1,IRQ_REENABLE|IRQ_EXCLUSIVE,&kb_hook_id)!=0)return 1;

  return 0;
}


int (keyboard_unsubscribe_int)() {
  if(sys_irqrmpolicy(&kb_hook_id)!=0)return 1;

  return 0;
}

void (kbc_ih)(){
    
   if(read_output_buffer(&scancode, false)!=0) printf("Unable to read scancode!");
    
}

int(keyboard_restore_interrupts)(){

    //1. temos de avisar que vamos ler o command Byte , enviando 0x20 para o input buffer(0x64)
    //2. lemos o commandByte do output buffer (0x60)

    //3. ligar as interrupcoes no commandByte

    //4. temos de avisar que vamos escrever o command Byte , enviando 0x60 para o input buffer(0x64)
    //5. escrevemos o commandByte para o 0x60

    uint8_t commandByte;

    if(write_command(0x64,0x20)!=0)return 1;
    if(read_output_buffer(&commandByte, false)!=0)return 1;

    commandByte = commandByte | BIT(0);

     if(write_command(0x64,0x60)!=0)return 1;
    if(write_command(0x60,commandByte)!=0)return 1;

    return 0;
}
