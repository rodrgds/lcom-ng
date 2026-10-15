#include <lcom/lcf.h>

#include "../i8042.h"
#include "keyboard.h"
#include "../k_controller.h"
#include <stdbool.h>
#include <stdint.h>

int keyboard_hook_id = 1; //o hook id do keyboard é 1
uint8_t scancode = 0;


int (keyboard_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;

  //subscrever uma interrupção nada mais é do que pedir ao sistema para ser notificado sempre que um evento expecifico ocorrer

  //neste caso apenas fica ativo o bit da posição 1 (com a ajuda da "macro" BIT) de modo a que "bit_no" armazene a máscara de bits 
  //que representa a interrupção que irá ser subscrita
  *bit_no = BIT(keyboard_hook_id);

  //esta função vai dizer qual a "policy" usada para tratar a interupção expecifica e, enquanto ativa essa interrupção será sempre tratada
  if (sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE , &keyboard_hook_id) != 0) return 1;

  return 0;
}

int (keyboard_unsubscribe_int)() {
    
    return sys_irqrmpolicy(&keyboard_hook_id);
}


void (keyboard_ih) () {
    //basicamente a fução vai ler o "OUT_BUF" e escrever a info em "scancode" (no caso a info  sobre a tecla pressionada)
    if (read_controller(OUT_BUF, &scancode,0) != 0) {
        printf("Error reading the scancode");
    }
}


//função usada para restaurar as interrupções desativadas para se fazer o teste de polling
int restore_interruptions() {
    uint8_t commandByte; //byte de comando usado para ativar as interrupções

    //para dar restore as interrupções temos de cumprir alguns passos:
    
    //primeiro mandar para o "KBC_WRITING" (0x64) o comando "READ_CMD" (0x20) que vai dar return de um "comandByte", ou seja, vamos avisar
    //o controlador para preparar um "commandByte" que depois vamos ler em "OUT_BUF" (0x60)
    if (write_controller(KBC_WRITING, READ_CMD) != 0) return 1;

    //após avisar, vamos então ler de "OUT_BUF" o "commandByte"
    if (read_controller(OUT_BUF, &commandByte,0) != 0) return 1;

    //após guardarmos esse "commandByte" vamos então ativar o seu bit 0 que basicamente é o responsável por ativar as interrupções
    //bit 0 a 0 --> sem interrupções                 bit 0 a 1 --> com interrupçoes
    commandByte |= BIT(0);

    //depois da "commandByte" estar "construida" vamos avisar o controller que vamos escrever para ele se se preparar para receber a "commandByte"
    //enviando então o "WRITE_CMD" (0x60) para o "KBC_WRITING"
    if (write_controller(KBC_WRITING, WRITE_CMD) != 0) return 1;

    //depois de avisar vamos então mandar o "commandByte" para 0x60 (WRITE_CMD) e desta forma o KBC vai ser avisado que iremos ativar denovo
    //as interrupçoes uma vez que a info dessa porta vai diretamente para o controllador através da call a "sys_outb()"
    if (write_controller(WRITE_CMD, commandByte) != 0) return 1;

    return 0;
}

//retorna true se for makecode, falso se for breakcode
bool (is_makecode)(uint8_t scancode){
    if(scancode & BIT(7)){  //the scancode when a key is released (break code) is the make code of that key with the MSB set to 1
        return false;
    }
    return true;
}

int (code_size)(uint8_t scancode){
    if(scancode == MSB_SCANCODE){
        return 2;
    }

    return 1;
}

