#include "helper.h"

void (printBinary)(uint8_t value) {
    int i;
    uint8_t mask = 0x80; // comecar no bit + significativo

    for (i = 0; i < 8; i++) {
        // 0 ou 1
        printf("%d", (value & mask) ? 1 : 0);

        // tudo um bit p direita (msbit ja demos prit)
        mask >>= 1;
    }

    printf("\n");
}
