#include "src/include/controllers/kbc/KBC.h"
#include "../i8042.h"
#include "src/include/Macros.h"

static uint8_t address = 0;
static uint8_t data = 0;
static bool mouse = false;

int (kBCStatus)(uint8_t *st){
    CHECK_RETURN(util_sys_inb(KBC_STAT_REG,st));
    return 0;
}

static int (kBCTries)(int (*func)(uint8_t st)){
    uint8_t tries = MAX_TRIES;
    while(tries > 0){
        uint8_t st;
        CHECK_RETURN_ERROR(kBCStatus(&st),"Failure to read KBC status.\n","");
        int ret = func(st);
        if(ret!=-1) return ret;
        tickdelay(micros_to_ticks(KBC_WAIT_TIME));
        --tries;
    }
    return 1;
}

static void (resetKBC)(){
    address = 0;
    data = 0;
}

static int (readAux)(uint8_t st){
    if((st&KBC_OBF)!=0){
        CHECK_RETURN_ERROR(st&KBC_PARITY,"KBC has a Parity error.\n","");
        CHECK_RETURN_ERROR(st&KBC_TIMEOUT,"KBC has a Timeout error.\n","");
        CHECK_RETURN_ERROR(((st & KBC_AUX) && !mouse) || ((!(st & KBC_AUX)) && mouse),"KBC is reading wrong data.\n","");
        CHECK_RETURN_ERROR(util_sys_inb(KBC_OUT_BUF,&data),"Failure to read KBC output.\n","");
        return 0;
    }
    return -1;
}

static int (kBCRead)(uint8_t *out){
    resetKBC();
    CHECK_RETURN(kBCTries(readAux));
    *out = data;
    resetKBC();
    return 0;
}

int (kBCReadKeyboard)(uint8_t *out){
    mouse = false;
    CHECK_RETURN(kBCRead(out));
    return 0;
}

int (kBCReadMouse)(uint8_t *out){
    mouse = true;
    CHECK_RETURN(kBCRead(out));
    return 0;
}

static int (writeAux)(uint8_t st){
    if((st&KBC_IBF)==0){
        CHECK_RETURN_ERROR(sys_outb(address,data),"Could not write to port %#x.\n", address);
        return 0;
    }
    return -1;
}

static int (kBCWrite)(uint8_t addr, uint8_t in){
    address = addr;
    data = in;
    CHECK_RETURN(kBCTries(writeAux));
    resetKBC();
    return 0;
}

int (kBCCommand)(uint8_t cmd){
    CHECK_RETURN(kBCWrite(KBC_CMD_IN_BUF,cmd));
    return 0;
}

int (kBCArgument)(uint8_t arg){
    CHECK_RETURN(kBCWrite(KBC_ARG_IN_BUF,arg));
    return 0;
}
