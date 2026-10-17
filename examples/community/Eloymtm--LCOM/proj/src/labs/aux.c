#include "aux.h"

int (read_status)(uint8_t *status){
    if(util_sys_inb(0x64,status)!=0)return 1;
    return 0;
}



int(read_output_buffer)(uint8_t *outByte,bool IsMouse){
    uint8_t status;
    int attemps =10;

    while(attemps > 0){
    
        if(read_status(&status)!=0) return 1;

        if((status & BIT(0))!=0){
            if(util_sys_inb(0x60,outByte)!=0)return 1;

            switch (status & BIT(5))
            {
            case 0:
                if(IsMouse)return 1; // estamos à espera de dados do rato mas os dados disponiveis não são do rato
                break;
            
            case 1:
                if(!IsMouse) return 1; // estamos à espera de dados do teclado mas os dados disponiveis não são do teclado
                break;
            }      
            if((status & BIT(6)) !=0)return 1;

            if((status & BIT(7))!=0)return 1;
    
    return 0;
    }
    attemps --;
    tickdelay(micros_to_ticks(20000));
    }
    return 1;
}

int (write_command)(uint8_t port ,uint8_t commandByte){

    uint8_t status;
    int attemps =10;

    while(attemps>0 ){
        
        if(read_status(&status)!=0)return 1; // ler o status register

        if((status & BIT(1)) == 0){ // garantir que o input buffer nao esta cheio, bit 1 tem de ser 0

            if(sys_outb(port,commandByte) !=0){ // escrever o commandByte
                return 1;
            }
            return 0; // sucesso a escrever
        }

        attemps--;
        tickdelay(micros_to_ticks(20000));
    }
    return 1; // caso exceda as attemps
    
}
