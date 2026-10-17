#include <lcom/lcf.h>
#include <lcom/timer.h>
#include "i8042.h"

#include <stdint.h>
#include "kbc.h"
#include "timer/i8254.h"

int (read_KBC_status)(uint8_t* status){
    if (util_sys_inb(KBC_STATUS_REG, status)) return 1;
    return 0;
}

    


