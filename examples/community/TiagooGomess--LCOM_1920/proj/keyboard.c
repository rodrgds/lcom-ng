#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>
#include <minix/sysutil.h>
#include "keyboard.h"

int hook_id = KBD_IRQ;  // hook_id = 1
uint8_t read_out_buf;
uint8_t status;
uint8_t scancode;
bool kbc_ih_valid = false;
bool make;
int size = 0;
uint8_t bytes[2];


int (keyboard_subscribe_int)(uint8_t *bit_no) {

    if (bit_no == NULL)
        return 1;

    *bit_no = hook_id;

    if(sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != 0)  // Esta função muda o valor de hook_id
        return 1;

    return 0;
}

int (keyboard_unsubscribe_int)() {

    if (sys_irqrmpolicy(&hook_id) != OK)  // o valor de hook_id foi determinado pela função sys_irqsetpolicy
        return 1;

    return 0;
}

int write_KBC_command_byte() {

    uint8_t command;

    if (util_sys_inb(OUTPUT_BUFFER, &command) != 0) {  // ler o output buffer
        return 1;
    }

    command = command | 0x01; // "Ou" bit a bit para meter o bit menos significativo a 1, mantendo os restantes intactos
                              // Ativa interrupcões no OBF, do teclado

    if (sys_outb(STATUS_REGISTER,READ_COMMAND_BYTE) != 0) { // escrever o read command byte (0x20) no status register
        return 1;
    }

    if (sys_outb(STATUS_REGISTER, OUTPUT_BUFFER) != 0)  // escrever no status register
        return 1;

    if (sys_outb(OUTPUT_BUFFER, command) != 0)  // escrever no output buffer
        return 1;

    return 0;
}

void (kbc_ih)() {

    kbc_ih_valid = false;

    uint8_t temp_scancode;  // Criamos uma variável temp_scancode para poder discartar scancode
                            // quando este não for válido

    if(util_sys_inb(STATUS_REGISTER, &status) != 0) {  // Ler o registo de estados
        kbc_ih_valid = false;
        return;
    }


    if ((status & OBF) == 0) {  // Se o bit OBF (bit 0 do status) for 0, o output buffer não tem dados para serem lidos
        kbc_ih_valid = false;
        return;
    }

    if (util_sys_inb(OUTPUT_BUFFER, &temp_scancode) != 0) {  // lê output buffer
        kbc_ih_valid = false;
        return;
    }

    if (((status & PARITY) == 0) && ((status & TIMEOUT) == 0)) {  // Verifica se existem erros

        scancode = temp_scancode;
        bytes[size] = scancode;  // Assim, é possível a existência de scancodes com 2 bytes (o primeiro byte é 0xe0)
        size++;

        if (scancode != 0xe0) {
            kbc_ih_valid = true;
            make = scancode >> 7;   // makecode se o primeiro bit do scancode for 1, senão é breakcode
        }
    }
}

