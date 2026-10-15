#include "record.h"

char winner_name[10];
unsigned int winner_name_length = 0;

bool init = false;

date_time_info dt_info;

static const char *filename = "/home/lcom/labs/proj/src/game/record/record.txt";

bool winner_menu_clicked = false;
bool record_menu_clicked = false;

extern int game_time;
extern unsigned int mode;

char kbc_letters[35] =  {
'q', // 0x90
'w', // 0x91
'e', // 0x92
'r', // 0x93
't', // 0x94
'y', // 0x95
'u', // 0x96
'i', // 0x97
'o', // 0x98
'p', // 0x99
' ',  // 0x9A
' ',  // 0x9B
' ',  // 0x9C
' ',  // 0x9D
'a', // 0x9E
's', // 0x9F
'd', // 0xA0
'f', // 0xA1
'g', // 0xA2
'h', // 0xA3
'j', // 0xA4
'k', // 0xA5
'l', // 0xA6
' ',  // 0xA7
' ',  // 0xA8
' ',  // 0xA9
' ',  // 0xAA
' ',  // 0xAB
'z', // 0xAC
'x', // 0xAD
'c', // 0xAE
'v', // 0xAF
'b', // 0xB0
'n', // 0xB1
'm'}; // 0xB2

void (winner_init)() {
    memset(winner_name, '\0', sizeof(winner_name));
    winner_name_length = 0;
    init = true;
}

int (display_winner_page)() {
    if (!init) {
        winner_init();
    }
    
    if (display_sprite(background, 0, 0)) {
        return 1;
    }

    if (display_sprite(congratulations, 200, 30)) {
        return 1;
    }

    if (display_string("Write your name:", 70, 420)) {
        return 1;
    }

    if (display_string(winner_name, 450, 420)) {
        return 1;
    }

    if (display_sprite(save_option, 290, 480)) {
        return 1;
    }

    if (display_sprite(x_option, 450, 480)) {
        return 1;
    }

    return 0;
}

int (display_record_page)() {
    if (display_sprite(background, 0, 0)) {
        return 1;
    }

    if (display_sprite(record_title, 300, 40)) {
        return 1;
    }

    if (display_winners()) {
        return 1;
    }

    if (display_sprite(x_option, 355, 480)) {
        return 1;
    }

    return 0;
}

int (save_winner)() {
    const char *tmp_filename = "/home/lcom/labs/proj/src/game/record/tmp.txt"; 
    FILE *file = fopen(filename, "r");
    FILE *tmp_file = fopen(tmp_filename, "w");

    if (file == NULL || tmp_file == NULL) {
        return 1;
    }

    char date_time[20];
    get_time_date_info(&dt_info);
    sprintf(date_time, "%02d/%02d/%02d %02d:%02d:%02d", dt_info.day, dt_info.month, dt_info.year, dt_info.hour, dt_info.min, dt_info.sec);

    fprintf(tmp_file, "%s,%d,%s\n", winner_name, game_time, date_time);

    char winner_copy[10];
    int game_time_copy;
    char date_time_copy[20];
    while (fscanf(file, "%[^,],%d,%[^\n]\n", winner_copy, &game_time_copy, date_time_copy) == 3) {
        fprintf(tmp_file, "%s,%d,%s\n", winner_copy, game_time_copy, date_time_copy);
    }

    fclose(file);
    fclose(tmp_file);

    remove(filename);
    rename(tmp_filename, filename);

    return 0;
}

int (display_winners)() {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        return 1;
    }

    char winner_record[10];
    int game_time_record;
    char date_time_record[20];
    int x = 60, y = 180, count = 0;

    while ((fscanf(file, "%[^,],%d,%[^\n]\n", winner_record, &game_time_record, date_time_record) == 3) && (count < 5)) {
        if (display_winner(winner_record, game_time_record, date_time_record, x, y)) {
            return 1;
        }

        y += 60;
        count++;
    }

    fclose(file);

    return 0;
}

int (display_winner)(char winner_record[10], int game_time_record, char date_time_record[20], int x, int y) {
    if (display_string(winner_record, x, y)) {
        return 1;
    }

    if (display_number(game_time_record, x + 280, y)) {
        return 1;
    } 

    if (display_string(date_time_record, x + 350, y)) {
        return 1;
    }

    return 0;
}

int (winner_control_kbc)(uint8_t *bytes) {
    if (init) {
        if (bytes[0] != FIRST_B_SCODE) {            
            switch (bytes[0]) {
                case ESC_BREAKCODE:
                    mode = MENU_MODE;
                    memset(winner_name, '\0', sizeof(winner_name));
                    winner_name_length = 0;
                    init = false;

                    break;
                case ENTER_BREAKCODE:
                    mode = MENU_MODE;
                    winner_name_length = 0;
                    init = false;

                    if (save_winner()) {
                        return 1;
                    }
                    
                    break;
                case DEL_BREAKCODE:
                    if (winner_name_length > 0) {
                        winner_name_length--;
                        winner_name[winner_name_length] = '\0';
                    }    

                    break;

                default:
                    if (winner_name_length < 9) {
                        if (bytes[0] == SPACE_BREAKCODE) {
                                winner_name[winner_name_length] = ' ';
                                winner_name_length++;
                        }
                        else if ((bytes[0] >= 0x90 && bytes[0] <= 0x99) || (bytes[0] >= 0x9E && bytes[0] <= 0xA6) || (bytes[0] >= 0xAC && bytes[0] <= 0xB2)) {
                            int aux =  (int) (bytes[0] - 0x90);
                            winner_name[winner_name_length] = kbc_letters[aux];
                            winner_name_length++; 
                        }
                    }
            }        
        }
    }

    return 0;
}

void (record_control_kbc)(uint8_t *bytes) {
    if (bytes[0] != FIRST_B_SCODE) {
        switch (bytes[0]) {
            case ESC_BREAKCODE:
                mode = MENU_MODE;
                break;
        }
    }
}

int (winner_control_mouse)(struct packet *pp, int x, int y) {
    if (x >= 290 && x < 380 && y >= 480 && y < 570) {
        if (!winner_menu_clicked) {
            if (pp->lb) {
                winner_menu_clicked = true;
            }
        }
        if (winner_menu_clicked) {
            if (!pp->lb) {
                mode = MENU_MODE;
                winner_name_length = 0;
                init = false;

                if (save_winner()) {
                    return 1;
                }

                winner_menu_clicked = false;
            }
        }
    } else if (x >= 450 && x < 540 && y >= 480 && y < 570) {
        if (!winner_menu_clicked) {
            if (pp->lb) {
                winner_menu_clicked = true;
            }
        }
        if (winner_menu_clicked) {
            if (!pp->lb) {
                mode = MENU_MODE;
                memset(winner_name, '\0', sizeof(winner_name));
                winner_name_length = 0;
                init = false;

                winner_menu_clicked = false;
            }
        }
    } else {
        winner_menu_clicked = false;
    }

    return 0;
}

void (record_control_mouse)(struct packet *pp, int x, int y) {
    if (x >= 355 && x < 445 && y >= 480 && y < 570) {
        if (!record_menu_clicked) {
            if (pp->lb) {
                record_menu_clicked = true;
            }
        }
        if (record_menu_clicked) {
            if (!pp->lb) {
                mode = MENU_MODE;

                record_menu_clicked = false;
            }
        }

    } else {
        record_menu_clicked = false;
    }

}

int (winner_display_mouse)(int x, int y) {
    if ((x >= 290 && x < 380 && y >= 480 && y < 570) || (x >= 450 && x < 540 && y >= 480 && y < 570)) {
        if (display_sprite(select_mouse_cursor, x, y)) {
            return 1;
        }
    } else {
        if (display_sprite(mouse_cursor, x, y)) {
            return 1;
        }
    }   

    return 0;
}

int (record_display_mouse)(int x, int y) {
    if ((x >= 355 && x < 445 && y >= 480 && y < 570)) {
        if (display_sprite(select_mouse_cursor, x, y)) {
            return 1;
        }
    } else {
        if (display_sprite(mouse_cursor, x, y)) {
            return 1;
        }
    }   

    return 0;
}
