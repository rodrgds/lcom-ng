#include "name_controller.h"
#include "../drivers/keyboard/mkbd.h"
#include "../state/state.h"
#include "../model/name/name_input.h"

uint8_t last_scan_code = 0;

void (player_name_kbd)(uint8_t scan_code[2], int size){

  	if (size == 1){
		switch(scan_code[0]){
			case ESC:
				set_state(MENU);
				break;
			case ENTER:
				set_state(GAME);
				break;
			default:
				letter_input(scan_code[0]);
				break;
		}
	}
}

int (letter_input)(unsigned char code){
    
    if (last_scan_code == code) {
        return 1;
    }

    last_scan_code = code;

	char letter;

    switch (code) {
        case 0x10:
          letter = (char) 'Q';
          break;
        case 0x11:
          letter = (char) 'W';
          break;
        case 0x12:
          letter = (char) 'E';
          break;
        case 0x13:
          letter = (char) 'R';
          break;
        case 0x14:
          letter = (char) 'T';
          break;
        case 0x15:
          letter = (char) 'Y';
          break;
        case 0x16:
          letter = (char) 'U';
          break;
        case 0x17:
          letter = (char) 'I';
          break;
        case 0x18:
          letter = (char) 'O';
          break;
        case 0x19:
          letter = (char) 'P';
          break;
        case 0x1E:
          letter = (char) 'A';
          break;
        case 0x1F:
          letter = (char) 'S';
          break;
        case 0x20:
          letter = (char) 'D';
          break;
        case 0x21:
          letter = (char) 'F';
          break;
        case 0x22:
          letter = (char) 'G';
          break;
        case 0x23:
          letter = (char) 'H';
          break;
        case 0x24:
          letter = (char) 'J';
          break;
        case 0x25:
          letter = (char) 'K';
          break;
        case 0x26:
          letter = (char) 'L';
          break;
        case 0x2C:
          letter = (char) 'Z';
          break;
        case 0x2D:
          letter = (char) 'X';
          break;
        case 0x2E:
          letter = (char) 'C';
          break;
        case 0x2F:
          letter = (char) 'V';
          break;
        case 0x30:
          letter = (char) 'B';
          break;
        case 0x31:
          letter = (char) 'N';
          break;
        case 0x32:
          letter = (char) 'M';
          break;
        case 0xb9:
          letter = ' ';
          break;
        case 0x8e:  
          delete_letter();
          return 0;
        default:
          return 0;
    }
    
	add_letter(letter);

    return 0;
}
