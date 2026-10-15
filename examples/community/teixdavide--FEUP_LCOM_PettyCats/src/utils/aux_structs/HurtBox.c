#include "HurtBox.h"
#include <stdlib.h>



HurtBox *newHurtBox(uint16_t deltaX, uint16_t deltaY) {
    HurtBox* hurtBox = (HurtBox *) malloc(sizeof(HurtBox));
    hurtBox->deltaX = deltaX;
    hurtBox->deltaY = deltaY;
    return hurtBox;
}


void deleteHurtBox(HurtBox* hurtBox) {
    free(hurtBox);
}


