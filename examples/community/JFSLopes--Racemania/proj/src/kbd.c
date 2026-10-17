#include <lcom/lcf.h>
#include "kbd.h"
#include "i8042.h"

int hook_id_keyboard = 1;
uint8_t scancodes[2];
uint8_t position = 0;
bool scancode_ready = false;

int (keyboard_subscribe_int)(uint8_t *bit_no){
    if (bit_no == NULL){
		printf("Invalid pointer in timer_subscribe_int().\n");
		return ERROR;
	}
	*bit_no = (uint8_t) hook_id_keyboard;
	int ret = sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_keyboard);
	if (ret != 0){
		printf("Error subscribing the timer.\n");
		return ERROR;
	}
	return OK;
}

int (keyboard_unsubscribe_int)() {
  	int ret = sys_irqrmpolicy(&hook_id_keyboard);
	if (ret != 0){
		printf("Error unsubscribing.\n");
		return ERROR;
	}
	return OK;
}


int (read_scancode) (){
    uint8_t scancode;
    int ret = read_OBF(&scancode, false);

    if (ret == ERROR) return ERROR;
    else if (ret == NO_DATA_AVAIVABLE) return NO_DATA_AVAIVABLE;
    
    scancodes[position] = scancode;
    if (scancode == INVALID_SCANCODE){
        position = 0;
    }
    else if (scancode == SCANCODE_2BYTES){
        position = 1;
    }
    else{
        scancode_ready = true;
    }
    return OK;
}

int (read_OBF) (uint8_t* byte, bool from_mouse){
    /// Read status
    uint8_t status;
    int ret = util_sys_inb(KBC_STAT_REG, &status);
    if (ret != 0){
        printf("Error reading the status of the keyboard.\n");
        return ERROR;
    }

    if ((status & KBC_STATUS_OBF) == 0) return NO_DATA_AVAIVABLE;
    

    /// Check if there was an error
    bool invalid = false;
    if ((status & (KBC_STATUS_PARITY | KBC_STATUS_TIMEOUT)) != 0){
        printf("Status Register showed an error.\n");
        invalid = true;
    }

    /// Checks if data is from the correct hardware, mouse or keyboard
    if (from_mouse && ((status & KBC_STATUS_AUX) == 0)){
        printf("Expected data from mouse but received from keyboard.\n");
        return ERROR;
    }
    else if (!from_mouse && ((status & KBC_STATUS_AUX) != 0)){
        printf("Expected data from keyboard but received from mouse.\n");
        return ERROR;
    }

    /// In any case read the byte
    ret = util_sys_inb(KBC_OUT_BUF, byte);
    if (ret != 0){
        printf("Error reading the scan code.\n");
        return ERROR;
    }
    
    if (invalid) *byte = 0;
    return OK;
}


void (kbc_ih)(){
    int ret = read_scancode();
    if (ret != 0){
        printf("Error reading the scan code");
    }
}

int (write_command) (uint8_t address, uint8_t command){

    while (true){
        uint8_t status;
        int ret = util_sys_inb(KBC_STAT_REG, &status);

        if (ret != 0){
            printf("Error reading the status of the keyboard.\n");
            return ERROR;
        }

        /// Check if the IBF (input buffer full) is 0, meaning we can write
        if ((status & KBC_STATUS_IBF) == 0){
            ret = sys_outb(address, command);
            if (ret != 0){
                printf("Error writting the command.\n");
                return ERROR;
            }
            return OK;
        }
        tickdelay(micros_to_ticks(DELAY_US));
    }
}

int (enable_interrupts) (void){
    uint8_t command_byte;
    /// Tell minix that we want to read the command byte
    int ret = write_command(KBC_WRITE_COMMAND_CMD, KBC_READ_COMMAND_BYTE);
    if (ret != 0){
        printf("Error writting command byte for reading the current command byte.\n");
        return ERROR;
    }
    /// Reading command byte
    ret = util_sys_inb(KBC_OUT_BUF, &command_byte);
    if (ret != 0){
        printf("Error writting command byte for reading the current command byte.\n");
        return ERROR;
    }
    /// Reactivate the interrupts
    command_byte = command_byte | KBC_KEYBOARD_INT;

    /// Tell minix we will write a new command byte
    ret = write_command(KBC_WRITE_COMMAND_CMD, KBC_WRITE_COMMAND_BYTE);
    if (ret != 0){
        printf("Error writting command byte for writting the new command byte.\n");
        return ERROR;
    }

    ret = write_command(KBC_WRITE_COMMAND_ARG, command_byte);
    if (ret != 0){
        printf("Error writting command byte for writting the new command byte.\n");
        return ERROR;
    }
    return OK;
}

void (reset_scancodes) (){
    if (scancode_ready){
        scancode_ready = false;
        position = 0;
    }
}
