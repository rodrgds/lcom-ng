#include <lcom/lcf.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include "main.h"

extern uint32_t timer_counter;

extern uint8_t scancode[];
extern int scancode_size;
extern bool second_byte;
extern uint8_t data;

extern struct packet mPacket;
extern struct mouse_position mPosition;
extern int byteIndex;

extern time_info rtc_info;

main_state_enum main_state = MENU;

struct Player players[MAX_PLAYERS];
int num_players = 0;
struct Player *selected_player;
char file_path[1024];

bool mouse_clickable = false;

void (keyboard_input_controller)() {
    // Check if the scancode corresponds to a printable character
    // printf("scancode: %x\n", scancode[scancode_size - 1]);
    
    switch (main_state) {
        case MENU:
            handle_kbd_menu();
            break;

        case CHOOSE_PLAYER:
            handle_kbd_choose_player();
            break;

        case CREATE_PLAYER:
            handle_kbd_create_player();
            break;

        case GAME:
            handle_kbd_game();
            break;

        default:
            break;
    }
}

void (timer_int_controller)() {

    switch (main_state) {
        case MENU:
            handle_timer_menu();
            break;

        case CREATE_PLAYER:
            handle_timer_create_player();
            break;

        case CHOOSE_PLAYER:
            handle_timer_choose_player();
            break;

        case GAME:
            handle_timer_game();
            break;

        case EXIT:
            break;

        default:
            break;
    }

    draw_mouse();
    swap_buffers(); // Troca os buffers para atualizar a tela

    if (timer_counter % 60 == 0) { 
        rtc_increment_time_by_one_second();
    }
}

void (mouse_input_controller)() {
    // if (mPacket.lb == 1) {
    //     printf("%d - %d\n", mPosition.x, mPosition.y);
    // }

    switch (main_state) {
        case MENU:
            handle_mouse_menu();
            break;

        case CHOOSE_PLAYER:
            handle_mouse_choose_player();
            break;

        case CREATE_PLAYER:
            handle_mouse_create_player();
            break;
        
        case GAME:
            handle_mouse_game();
            break;

        default:
            break;
    }
}

bool (is_on_close_button)() {
    return mPosition.x >= 10 && mPosition.x <= 55 && mPosition.y >= 10 && mPosition.y <= 55;
}

void (draw_mouse)() {
    if(mouse_clickable) {
        mouse_cursor_click->x = mPosition.x;
        mouse_cursor_click->y = mPosition.y;
        draw_sprite(mouse_cursor_click);
    }
    else {
        mouse_cursor->x = mPosition.x;
        mouse_cursor->y = mPosition.y;
        draw_sprite(mouse_cursor);
    }
}

void (read_kbd_input_str)(uint8_t scancode, char input_string[], uint32_t *string_length, uint32_t max_string_length) {
    if (is_printable(scancode)) {
        // Add the character to the input string buffer if it's not full
        if (*string_length < max_string_length - 1) {
            input_string[*string_length] = scancode_to_ascii(scancode);
            (*string_length)++;
            input_string[*string_length] = '\0'; // Null-terminate the string
            printf("%s\n", input_string);
        }
    } else if (scancode == 0x0E) { // Check for backspace
        if (*string_length > 0) {
            (*string_length)--;
            input_string[*string_length] = '\0'; // Null-terminate the string
        }
    }
}

bool (is_printable)(uint8_t scancode) {
    return (scancode >= 0x02 && scancode <= 0x39 && scancode != 0x0E && scancode != 0x2a && scancode != 0x1C);
}

char (scancode_to_ascii)(uint8_t scancode) {
    const char ascii_table[] = {
        '\0', '\0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\0', '\0',
        'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', '\0', 'a', 's',
        'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', '\0', '\\', 'z', 'x', 'c', 'v',
        'b', 'n', 'm', ',', '.', '/', '\0', '*', '\0', ' ', '\0'
    };
    
    if (scancode < sizeof(ascii_table)) {
        return ascii_table[scancode];
    } else {
        return '\0';
    }
}

void (wordDecoder)(char* word, uint32_t x, uint32_t y) {
    for (int i = 0; word[i] != '\0'; i++) {
        char letter = tolower((unsigned char) word[i]);
        char name[2] = {letter, '\0'};
        Sprite *s = find_sprite(name);
        if (s == NULL) return;
        s->x = x;
        s->y = y;
        draw_sprite(s);
        x += 20;
    }
}

void (draw_text_container)(int x, int y, int width, int height, uint32_t color) {
    vg_draw_rectangle(x, y, width, height, color);
    vg_draw_rectangle(x, y, width, 4, 0x000000);
    vg_draw_rectangle(x, y + height - 4, width, 4, 0x000000);
    vg_draw_rectangle(x, y, 4, height, 0x000000);
    vg_draw_rectangle(x + width - 4, y, 4, height, 0x000000);
}

void (extract_directory_path)(char *dirpath) {
    char *lastSlash = strrchr(__FILE__, '/');
    if (lastSlash == NULL) {
        printf("Error: Unable to determine directory path!\n");
        return;
    }
    size_t dirLength = lastSlash - __FILE__ + 1;
    strncpy(dirpath, __FILE__, dirLength);
    dirpath[dirLength] = '\0'; // Null-terminate the directory path
}

void (write_player_data_to_file)() {
    FILE *filePtr = fopen(file_path, "w");
    if (filePtr == NULL) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(filePtr, "Name, Coins, Next Reward Data Time\n");

    for (int i = 0; i < num_players; ++i) {
        fprintf(filePtr, "%s,%d,%d,%d,%d,%d,%d,%d\n", 
                players[i].name, 
                players[i].coins, 
                players[i].nextRewardDataTime.day,
                players[i].nextRewardDataTime.month,
                players[i].nextRewardDataTime.year,
                players[i].nextRewardDataTime.hours,
                players[i].nextRewardDataTime.minutes,
                players[i].nextRewardDataTime.seconds);
    }

    fclose(filePtr);
}

void (read_player_data_from_file)() {
    FILE *filePtr = fopen(file_path, "r");
    if (filePtr == NULL) {
        printf("Error opening file for reading!\n");
        return;
    }

    char buffer[1024]; // Buffer to store each line

    // Read and discard the first line
    if (fgets(buffer, sizeof(buffer), filePtr) == NULL) {
        printf("Error reading the first line!\n");
        fclose(filePtr);
        return;
    }

    int index = 0;
    while (fgets(buffer, sizeof(buffer), filePtr) != NULL && index < MAX_PLAYERS) {
        // Tokenize the line using comma as delimiter
        char *token = strtok(buffer, ",");
        if (token != NULL) {
            // Copy player name
            strcpy(players[index].name, token);
            // Move to next token
            token = strtok(NULL, ",");
            if (token != NULL) {
                // Convert coins to integer
                players[index].coins = atoi(token);
                // Move to next token
                token = strtok(NULL, ",");
                if (token != NULL) {
                    // Convert day to integer
                    players[index].nextRewardDataTime.day = atoi(token);
                    token = strtok(NULL, ",");
                    if (token != NULL) {
                        // Convert month to integer
                        players[index].nextRewardDataTime.month = atoi(token);
                        token = strtok(NULL, ",");
                        if (token != NULL) {
                            // Convert year to integer
                            players[index].nextRewardDataTime.year = atoi(token);
                            token = strtok(NULL, ",");
                            if (token != NULL) {
                                // Convert hours to integer
                                players[index].nextRewardDataTime.hours = atoi(token);
                                token = strtok(NULL, ",");
                                if (token != NULL) {
                                    // Convert minutes to integer
                                    players[index].nextRewardDataTime.minutes = atoi(token);
                                    token = strtok(NULL, ",");
                                    if (token != NULL) {
                                        // Convert seconds to integer
                                        players[index].nextRewardDataTime.seconds = atoi(token);
                                        // Move to the next player
                                        index++;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    num_players = index;

    fclose(filePtr);
}
