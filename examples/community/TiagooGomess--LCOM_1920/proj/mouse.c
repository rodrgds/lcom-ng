#include <lcom/lcf.h>

#include "mouse.h"
#include "cursor.h"

int hookId = MOUSE_CTR_IRQ; // hookId = 12
uint8_t status_mouse;
uint32_t numberOfPackets = 0;
bool mouse_ih_valid = false;
uint8_t mouse_scancode = 0;
static uint8_t pacote_array[3]; // array com os 3 bytes constituintes de um pacote
static int number_of_byte = 0;
Sprite *sprite_cursor; //Sprite de mouse global
int mouse_x = 50,mouse_y = 50;
extern int32_t horizontalResolution;
extern int32_t verticalResolution;


int our_mouse_enable_data_reporting() {

    sys_irqdisable(&hookId);

    if (send_command_to_mouse(ENABLE_DATA_REPORT) != 0)  // ativa o data reporting
        return 1;

    sys_irqenable(&hookId);

    return 0;
}


int mouse_reset_state() {

    sys_irqdisable(&hookId);

    if (send_command_to_mouse(DISABLE_DATA_REPORT) != 0)  // desativa o data reporting
        return 1;

    return 0;
}


int (mouse_subscribe_int)(uint8_t *bit_no) {

    if (bit_no == NULL)
        return 1;

    *bit_no = hookId;

    if(sys_irqsetpolicy(MOUSE_CTR_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hookId) != 0)  // Esta função muda o valor de hookId
        return 1;

    return 0;
}


int (mouse_unsubscribe_int)() {

    if (sys_irqrmpolicy(&hookId) != 0)  // o valor de hookId foi determinado pela função sys_irqsetpolicy
        return 1;

    return 0;
}


void (mouse_ih)(void) {  // função para tratar das interrupções do rato

    mouse_ih_valid = false;

    if(util_sys_inb(STATUS_REGISTER, &status_mouse) != 0) {  // Ler o registo de estados
              return;
    }

    if ((status_mouse & OBF) == 0) {  // Se o bit OBF (bit 0 do status) for 0, o output buffer não tem dados para serem lidos
        return;
    }

    if (util_sys_inb(OUTPUT_BUFFER, &mouse_scancode) != 0) {  // lê output buffer
        return;
    }

    if (((status_mouse & PARITY) != 0) || ((status_mouse & TIMEOUT) != 0)) {  // Verifica se existem erros (de paridade ou de timeout)
        return;
    }

    mouse_ih_valid = true;

}


int send_command_to_mouse(uint32_t comando) {

    if (util_sys_inb(STATUS_REGISTER, &status_mouse) != 0)  // Ler status register
        return 1;

    if (((status_mouse & IBF) != 0) || ((status_mouse & AUX) != 0))  // Se o input buffer não estiver vazio
                                                                     // ou se o byte que está no output buffer
                                                                     // vem do teclado e não do rato
        return 1;

    if (sys_outb(STATUS_REGISTER, WRITE_BYTE_TO_MOUSE) != 0)  // escreve no status register o comando 0xD4 (Write Byte to Mouse)
        return 1;

    if (util_sys_inb(STATUS_REGISTER, &status_mouse) != 0) // ler status register
        return 1;

    if (((status_mouse & IBF) != 0) || ((status_mouse & AUX) != 0))  // Se o input buffer não estiver vazio
                                                                     // ou se o byte que está no output buffer
                                                                     // vem do teclado e não do rato
        return 1;

    if (sys_outb(INPUT_BUFFER_DATA, comando) != 0)  // escreve o comando no input buffer (0x60)
        return 1;

    if (util_sys_inb(OUTPUT_BUFFER, &status_mouse)) // lê o output buffer (resposta ao comando)
        return 1;

    if (status_mouse == ERROR) // se o valor lido do output buffer for ERROR (0xFC), então há um erro
        return 1;

    if (status_mouse == ACK) // se o valor lido do output buffer for ACK (0xFA), então está tudo OK
        return 0;

    return 1;  // Se o valor lido do output buffer nunca for ACK, então não foi possível enviar
               // o comando para o rato
}


int packet_parsing(struct packet* pacote) {

    if (number_of_byte == 0) {
        if ((mouse_scancode & BIT_3) == 0) // O bit 3 do primeiro byte é sempre 1 (caso não seja, há um erro)
            return 1;
    }

    pacote_array[number_of_byte++] = mouse_scancode;

    if (number_of_byte % 3 == 0) {  // se number_of_byte for múltiplo de 3 (0, 3, 6, 9, ...)

        pacote->bytes[0] = pacote_array[0]; // primeiro byte do pacote
        pacote->bytes[1] = pacote_array[1]; // segundo byte do pacote
        pacote->bytes[2] = pacote_array[2]; // terceiro byte do pacote

        pacote->rb = (pacote_array[0] & RIGHT_MOUSE_BUTTON_PRESSED) >> 1; // se o botão direito do rato estiver
                                                                          // pressionado, então packet::rb fica a 1
                                                                          // (true)

        pacote->mb = (pacote_array[0] & MIDDLE_MOUSE_BUTTON_PRESSED) >> 2; // se o botão do meio do rato estiver
                                                                           // pressionado, então packet::mb fica a 1
                                                                           // (true)

        pacote->lb = (pacote_array[0] & LEFT_MOUSE_BUTTON_PRESSED);      // se o botão esquerdo do rato estiver
                                                                         // pressionado, então packet::lb fica a 1
                                                                         // (true)


        pacote->x_ov = (pacote_array[0] & X_OVERFLOW) >> 6;      // se ocorrer overflow da coordenada x, então
                                                                 // packet::x_ov fica a 1 (true)

        pacote->y_ov = (pacote_array[0] & Y_OVERFLOW) >> 7;      // se ocorrer overflow da coordenada y, então
                                                                 // packet::y_ov fica a 1 (true)

        int16_t deltaX = (uint16_t) pacote_array[1];  // deltaX é o segundo byte de pacote_array
        int16_t deltaY = (uint16_t) pacote_array[2];  // deltaY é o terceiro byte de pacote_array

        // DELTA_X (4º bit do primeiro byte do pacote) é o 9º bit de deltaX (2º byte do pacote) ou seja, se for 0, deltaX é positivo, se for 1,
        // deltaX será negativo; deltaX está representado em complemento para 2, ou seja, caso o 9º bit seja 0,
        // o número em decimal é igual, logo não teremos que fazer a conversão; caso o 9º bit seja 1, teremos
        // que fazer a conversão de complemento para 2 para decimal (meter o byte mais significativo a FF e o
        // menos significativo fica igual); a mesma explicação se aplica para o deltaY

        if ((pacote_array[0] & DELTA_X) >> 4) // se o bit DELTA_X (4º bit do primeiro byte do pacote) estiver a 1
            deltaX = deltaX | TO_INTEGER;  // TO_INTEGER -> 0xFF00

        if ((pacote_array[0] & DELTA_Y) >> 5)  // se o bit DELTA_Y (5º bit do primeiro byte do pacote) estiver a 1
            deltaY = deltaY | TO_INTEGER;  // TO_INTEGER -> 0xFF00

        pacote->delta_x = deltaX;
        pacote->delta_y = deltaY;

        mouse_x += deltaX;
        mouse_y -= deltaY;

        if (mouse_x > (int)horizontalResolution - 40) {
            mouse_x = horizontalResolution - 40;
        }
        else if (mouse_x < 3) mouse_x = 3;
        if (mouse_y > (int)verticalResolution - 60) {
            mouse_y = verticalResolution - 60;
        }
        else if (mouse_y < 3) mouse_y = 3;

        number_of_byte = 0;
        numberOfPackets++;

        return 0;
    }
    return 1;
}
