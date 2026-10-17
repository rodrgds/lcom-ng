#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int counter = 0;


int hook_id_ = TIMER0_IRQ;  // hook_id = 0 (hook_id tem que ter valor entre 0 e 31, já que é
                           // o número do bit que será ativado aquando da notificação / interrupção)


int (timer_set_frequency)(uint8_t timer, uint32_t freq) {

    if (timer < 0 || timer > 2) {  // Testa-se se o valor de timer é válido
        printf("Timer value is not valid!\n");
        return 1;
    }

    if (freq <= 18 || freq > TIMER_FREQ) // Testa-se se o valor da frequência é válido. Uma vez que o
        return 1;                        // divisor não pode ter mais de 16 bits, porque o i8254
                                         // usa uma estrutura de 16 bits, e 2 ^ 16 = 65536,
                                         // e TIMER_FREQ / 18 = 66287, conclui-se que a frequência tem
                                         // de ser 19 ou superior


    uint8_t conf; // Guarda configuração do timer

    uint8_t mask = 0x0F;

    uint8_t msb,lsb;

    unsigned int result;  // valor de contagem

    timer_get_conf(timer, &conf); // Passa a configuração timer para conf

    conf = conf & mask; // Põe os 4 mais significativos bits a 0,
                        // deixando os 4 menos significativos intactos


    // Para cada valor do timer, a configuração é igual a um "ou lógico"
    // entre a configuração do timer, o timer selecionado e o modo de
    // inicialização (LSB seguido de MSB)
    if (timer == 0)
        conf = (conf | TIMER_SEL0 | TIMER_LSB_MSB);

    if (timer == 1)
        conf = (conf | TIMER_SEL1 | TIMER_LSB_MSB);

    if (timer == 2)
        conf = (conf | TIMER_SEL2 | TIMER_LSB_MSB);


    sys_outb(TIMER_CTRL, conf);  // Escreve a configuração do timer selecionado
                                 // para o timer control

    result = TIMER_FREQ / freq; // Resultado para o timer

    msb = result >> 8;   // byte mais significativo
    lsb = (result << 8) >> 8;  // byte menos significativo

    //Escreve o valor certo no timer
    if (timer == 0) {
        sys_outb(TIMER_0, lsb);
        sys_outb(TIMER_0, msb);
    }

    if (timer == 1) {
        sys_outb(TIMER_1, lsb);
        sys_outb(TIMER_1, msb);
    }

    if (timer == 2) {
        sys_outb(TIMER_2, lsb);
        sys_outb(TIMER_2, msb);
    }


    return 0;
}



int (timer_subscribe_int)(uint8_t *bit_no) {

    if (bit_no == NULL)
        return 1;

    *bit_no = hook_id_;

    if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id_) != OK)  // Esta função muda o valor de hook_id
        return 1;



    /*
    if (sys_irqenable(&hook_id_) != OK)  // Ativa interrupções na IRQ line associadas ao hook_id;
        return 1;                       // As interrupções na IRQ line associadas ao hook_id
                                        // já são ativadas quando quando passamos o parâmetro
                                        // IRQ_REENABLE é passado à função sys_irqsetpolicy, pelo que
                                        // decidimos não incluir (comentar) a função sys_irqenable
    */

    return 0;
}

int (timer_unsubscribe_int)() {

    if (sys_irqrmpolicy(&hook_id_) != OK)  // o valor de hook_id foi determinado pela função sys_irqsetpolicy
        return 1;

    return 0;
}

void (timer_int_handler)() {

    counter++;      // Incrementa-se o valor da variável global counter em uma unidade

}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {

    if (st == NULL)
        return 1;

    if (timer < 0 || timer > 2) {  // Testa-se se o valor de timer é válido
        printf("Timer value is not valid!\n");
        return 1;
    }

    uint8_t comando = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer); // Criar comando read-back

    sys_outb(TIMER_CTRL, comando); // Escrever comando read-back para o timer control

    util_sys_inb(TIMER_0 + timer, st); // Ler estado do timer selecionado

    return 0;
}
int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {

    if (timer < 0 || timer > 2) {  // Testa-se se o valor de timer é válido
        printf("Timer value is not valid!\n");
        return 1;
    }

    union timer_status_field_val val;

    if (field == tsf_all)   // se field for a configuração em hexadecimal
        val.byte = st;      // estado, em hexadecimal

    else if (field == tsf_initial) // se field for o modo de inicialização do timer
        val.in_mode = ((st & 0x30) >> 4);   // os bits de st que definem o modo de iniciação
                                            // do timer são os bits 5 e 4; quando fazemos um and
                                            // de st com 0x30, todos os bits de st ficam a zero,
                                            // com exceção dos bits 5 e 4, que ficam intactos;
                                            // depois de fazermos um deslocamento de 4 bits para a direita,
                                            // ficamos com o modo de inicialização

    else if (field == tsf_mode)  // se field for o modo de contagem
        val.count_mode = ((st & 0x0E) >> 1);  // os bits de st que definem o modo de contagem são
                                              // os bits 3, 2 e 1; quando fazemos um and de st com
                                              // 0x0E, todos os bits de st ficam a zero, com excessão
                                              // dos bits 3, 2 e 1, que ficam intactos; depois de fazermos
                                              // um deslocamento de 1 bit para a direita, ficamos com o
                                              // modo de contagem

    else if (field == tsf_base)    // se field for a base da contagem
        val.bcd = (st & 0x01);  // o bit de st que define a base da contagem é o bit 0;
                                // quando fazemos um and de st com 0x01, todos os bits
                                // de st ficam a zero, com exceção do bit 0, que fica
                                // intacto; assim, ficamos com a base da contagem
    else
        return 1;

    if (timer_print_config(timer, field, val) != 0)
        return 1;

    return 0;
}
