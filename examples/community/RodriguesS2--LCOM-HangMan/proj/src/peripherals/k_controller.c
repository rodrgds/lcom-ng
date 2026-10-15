#include <lcom/lcf.h>

#include "i8042.h"
#include "k_controller.h"
#include <stdbool.h>
#include <stdint.h>

//vai ler o buffer e verificar se existem/é necessário tratar de algum erro do status do keyboard
//vai ler da "port" e vai escrever em "data" o que se leu
int (read_controller) (uint8_t port, uint8_t *data, uint8_t mouse) {

    uint8_t attemps = 10;
    uint8_t keyboard_status; //vai armazenar os status do keyboard

    while (attemps > 0) {

        //Chama-se esta função para ela ir até Status Register e receber o status do keyboard que vai ser analisado (para deteção de erros)
        if (util_sys_inb(STAT_REG, &keyboard_status) != 0) return 1;


        if ((keyboard_status & OUT_BUF_FULL) != 0) { //O Output buffer está full, logo existem dados para ler/ser processados

            //se não existir erro no status (verificado pela leitura da porta 0x64 (status register)) então podemos passar a leitura da "data"
            //que irá estar na "port" e processa-la posteriormente
            if (util_sys_inb(port, data) != 0) return 1;
            
            if ((keyboard_status & PARITY_ERROR) != 0) return 1;//se for diferente de 0, o bit do erro está ativo (erro no parity)

            if ((keyboard_status & TIMEOUT_ERROR) != 0) return 1; //erro timeout

            if (mouse && !(keyboard_status & AUX)) return 1; //se o mouse não estiver ativo e o bit 5 do status register não estiver ativo, então existe erro
            
            if (!mouse && (keyboard_status & AUX)) return 1; //se o mouse estiver ativo e o bit 5 do status register estiver ativo, então existe erro

            return 0;
        }

        tickdelay(micros_to_ticks(30000)); //deelay devido a lentidão do KBC para processar os dados
        attemps--;
    }

    return 1;
}


//para escrever os dados vamos usar a mesma logica de loop que para ler
int (write_controller) (uint8_t port, uint8_t data) {

    uint8_t attemps = 10;
    uint8_t keyboard_status; //vai armazenar os status do keyboard

    while (attemps > 0) {

        //chama-se esta função para ela ir até Status Register e receber o status do keyboard que vai ser analisado (para deteção de erros)
        if (util_sys_inb(STAT_REG, &keyboard_status) != 0) return 1;

        //o input buffer está vazio (a 0), logo podemos escrever lá. Caso estivesse a 1 significa que estava cheio então tinhamos de esperar
        //que esses dados fossem processados para podermos escrever
        if ((keyboard_status & IN_BUFF_FULL) == 0) { 


            //se existir algo a escrever (verificado pela mascara de bits aplicada no "keyboard_status")
            //e iremos então escrever o "data" na determinada "port" com a ajuda de "sys_outb()" para que o controlador a receba
            if (sys_outb(port, data) != 0) return 1;
            
            return 0;
        }

        tickdelay(micros_to_ticks(30000)); //deelay devido a lentidão do KBC para processar os dados
        attemps--;
    }


    return 1;
}

