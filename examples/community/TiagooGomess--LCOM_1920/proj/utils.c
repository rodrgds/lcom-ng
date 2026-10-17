#include <lcom/lcf.h>

#include <stdint.h>


int(util_get_LSB)(uint16_t val, uint8_t *lsb) {

    if (lsb == NULL)
        return 1;

    *lsb = (uint8_t)(val & 0x00ff); // fazendo um and de val com 0x00ff, o byte mais significativo fica a zero,
                                    // mantendo-se o byte menos significativo intacto; quando fazemos a conversão
                                    // de uint16_t para uint8_t, o byte mais significativo, que é igaul a zero, desaparece;
                                    // decidimos deixar ficar (val & 0x00ff) ainda assim para uma melhor compreensão do código
                                    // (note-se que se fizessemos "*lsb = (uint8_t) val", o resultado seria igual)

    return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {

    if (msb == NULL)
        return 1;

    *msb = (uint8_t)(val >> 8); // deslocando val 8 bits para a direita, e convertendo de uint16_t para uint8_t,
                                // ficamos com o byte mais significativo, tal como pretendido

    return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {

    if(value == NULL)
        return 1;

    uint32_t temp = 0;

    if (sys_inb(port, &temp) != 0)    // temp é do tipo uint32_t, pelo que é aceite no segundo parâmetro da função sys_inb,
        return 1;                     // ao contrário do que aconteciria com uma variável que é do tipo uint8_t

    *value = (uint8_t) temp;          // converte-se o valor de temp, obtido pela função sys_inb, para o tipo uint8_t, e atribui-se
                                      // esse valor a value

    return 0;
}
