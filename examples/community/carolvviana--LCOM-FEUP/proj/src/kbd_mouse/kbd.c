#include "kbd.h"

#include "i8042.h"
// #include "KBC.h"

#ifdef DEBUG
#  define DEBUG_MSG(str) printf(str)
#else
#  define DEBUG_MSG(str)
#endif


int kbd_hook_id = 1;
uint8_t scancode=0;


int (KBC_write_cmd)(uint8_t port, uint8_t cmdByte){
    
    uint8_t status;
    uint8_t count = 0;


    do{
        if (util_sys_inb(KBC_STATUS_REG, &status)!=0) { //le o status do kbc
            DEBUG_MSG("Error in KBC - Status not available");
            return 1;
        }
        
        if(!(status & KBC_INPUTB_FULL)){ //verificar se o input buffer nao esta vazio
            if(sys_outb(port, cmdByte)!=0){ // tentar escrever o comando no input buffer
                DEBUG_MSG("Error in KBC - Could not write command");
                return 1;
            }
            return 0; //se conseguir escrever o comando no input buffer, sucesso
        }

        tickdelay(micros_to_ticks(10000));
        count++;

    }while (count < 5);

    return 1; //se ultrapassar o numero de tentativas
}

int (KBC_read_output_keyboard)(uint8_t* output){
    
    uint8_t status;
    // uint8_t count = 0;

    if (util_sys_inb(KBC_STATUS_REG, &status) != 0) return 1;

    if ( status & KBC_OUTPUTB_FULL){
        if (util_sys_inb(KBD_OUT_CMD, output) != 0) {
                DEBUG_MSG("Could not read output buffer\n");
                return 1;
            }   
            if ((status & PARITY_ERR) || (status & TIMEOUT_ERR) ||(status & BIT(5))){
                DEBUG_MSG("Error\n");
                return 1;
            }
            return 0;

        }

    return 1; // se ultrapassar o numero de tentativas
}

int (kbd_subscribe_int)(uint8_t *bit){
    *bit = BIT(kbd_hook_id);
    if(sys_irqsetpolicy(IRQ_KBD, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id)!=0) return 1;
    
    #ifdef DEBUG
    printf("Keyboard subscribed\n");
    #endif

    return 0;
}

void (kbc_ih)(){
    if(KBC_read_output_keyboard(&scancode)!=0) DEBUG_MSG("Error in keyboard - Can't read scancode");
}

int (kbd_unsubscribe_int)(){
    if(sys_irqrmpolicy(&kbd_hook_id)!=0) return 1;
    return 0;
}
