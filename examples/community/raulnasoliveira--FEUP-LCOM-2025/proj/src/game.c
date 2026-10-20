#include <lcom/lcf.h>
#include <ctype.h>
#include "game.h"
#include "alphabet.h"
#include "cube_sprite.h"
#include "mouse.h"
#include "progress_bar.h"

#define MAX_SEQUENCE 26

extern vbe_mode_info_t information;
extern uint32_t counter_KBC;
extern uint32_t counter_timer;
extern uint8_t scancode;

char sequence[MAX_SEQUENCE];
int level = 1;
uint8_t keyboard_bit_no;
uint8_t timer_bit_no;
uint8_t mouse_bit_no;  /* hook do mouse */


int start_game (){
    if (set_frame_buffer(VBE_600p_DC) != 0) return 1;
    if (graphics_mode(VBE_600p_DC) != 0) return 1;
    return 0;

}

void draw_square(int x, int y, int size, uint32_t color) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            vg_draw_pixel_buffer(double_buffer, x + i, y + j, color);
        }
    }
}

void buffer_swap() {
    memcpy(frame_buffer, double_buffer, frame_size);
}

uint32_t new_color(uint32_t color) {
    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;
    uint32_t new_color = direct_mode(r, g, b);
    return new_color;
}

void draw_screen(u_int32_t color, uint8_t* buffer) {
    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;
    u_int32_t new_color = direct_mode(r, g, b);
    my_draw_rectangle(0, 0, information.XResolution, information.YResolution, new_color, buffer);
}


int show_main_menu(void) {
    int selected = 0;
    int last_selected = -1; 

    Sprite *cube = create_cube_sprite(
        information.XResolution / 2, 
        information.YResolution / 2,
        3, 2, 
        0.1 
    );


    draw_screen(0xADD8E6, double_buffer);

    bool running = true;
    message msg;
    int ipc_status;

    while (running) {
        bool redraw = false;
    
        if (driver_receive(ANY, &msg, &ipc_status) != 0) continue;
    
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & keyboard_bit_no) {
                        kbc_ih();
                        if (!(scancode & MAKE_CODE)) {
                            switch (scancode) {
                                case 0x48:
                                    selected = (selected + 2) % 3;
                                    redraw = true;
                                    break;
                                case 0x50:
                                    selected = (selected + 1) % 3;
                                    redraw = true;
                                    break;
                                case 0x1C:
                                    running = false;
                                    break;
                            }
                        }
                    }
                    if (msg.m_notify.interrupts & timer_bit_no) {
                        update_cube_sprite(cube);
                        redraw = true;
                    }
                    break;
            }
        }
    
        if (redraw || selected != last_selected) {
            memset(double_buffer, 0, frame_size);
            draw_screen(0xADD8E6, double_buffer);
            print_xpm_string_centered_highlight("PLAY REACTION SPEED TEST",
                                                information.YResolution / 2 - 80,
                                                double_buffer, information.XResolution,
                                                selected == 0 ? 0 : 1); 
            print_xpm_string_centered_highlight("PLAY MEMORY GAME", information.YResolution / 2 - 20, double_buffer, information.XResolution,
                                                selected == 1 ? 0 : 1);
            print_xpm_string_centered_highlight("PLAY VERBAL MEMORY TEST", information.YResolution / 2 + 40, double_buffer, information.XResolution,
                                                selected == 2 ? 0 : 1);       

            
            update_cube_sprite(cube);
            draw_cube_sprite(cube, double_buffer);
            buffer_swap();
            last_selected = selected;
        }
        
    }
    return selected;
}




int (video_test_reaction)() {
    uint32_t frequency = 250;
    srand(counter_timer);
    int attempt = 1;
    uint32_t total_time = 0;
    uint32_t reaction_time = 0;
    uint32_t random_delay = 0, start_time = 0;
    int tickforbar=0;
    int sprite_case;

    mouse_event.lb = false;
    mouse_event.rb = false;
    mouse_event.mb = false;

    Sprite *progress_bar = create_pro_sprite(10,10,0);

    enum { STATE_WAIT_GREEN, STATE_GREEN, STATE_TOO_SOON, STATE_RESULT, STATE_AVERAGE } state = STATE_WAIT_GREEN;
    int last_state = -1;
    bool running = true;
    message msg;
    int ipc_status;

    counter_timer = 0;
    random_delay = rand() % (3*frequency) + frequency;

    memset(double_buffer, 0, frame_size);
    draw_screen(0xADD8E6, double_buffer);
    draw_pro_bar(progress_bar,double_buffer,0);
    buffer_swap();
    if (mouse_subscribe_int(&mouse_bit_no) != 0) return 1;
    if (mouse_enable_data_report() != 0) return 1;

    int flush_ticks = 0;
while (mouse_event.lb || mouse_event.mb || mouse_event.rb || (scancode & MAKE_CODE)) {
    if (++flush_ticks > 125) break;
    message flush_msg;
    int flush_ipc;
    if (driver_receive(ANY, &flush_msg, &flush_ipc) == 0 && is_ipc_notify(flush_ipc)) {
        switch (_ENDPOINT_P(flush_msg.m_source)) {
            case HARDWARE:
                if (flush_msg.m_notify.interrupts & mouse_bit_no) {
                    mouse_ih();
                    packet_ready = false;
                }
                if (flush_msg.m_notify.interrupts & keyboard_bit_no) {
                    kbc_ih();
                }
                break;
        }
    }
}
mouse_event.lb = mouse_event.mb = mouse_event.rb = false;
scancode = 0;

    while (running) {
        bool redraw = false;

        if (driver_receive(ANY, &msg, &ipc_status) != 0) continue;
        

        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & timer_bit_no) {
                        timer_int_handler();
                        tickforbar++;
                        
                        if (state == STATE_WAIT_GREEN && counter_timer >= random_delay) {
                            state = STATE_GREEN;
                            start_time = counter_timer;
                            redraw = true;
                        }

                         if((tickforbar % (250 / 3) == 0) && state != STATE_RESULT && state != STATE_AVERAGE) {
                                sprite_case = attempt - 1;
                                if (sprite_case > 4) sprite_case = 4;
                                draw_pro_bar(progress_bar, double_buffer, sprite_case);
                                buffer_swap();
                            }

                    }
                    /* tecla */
                    if (msg.m_notify.interrupts & keyboard_bit_no) {
                        kbc_ih();
                        if (!(scancode & MAKE_CODE)) { 
                            redraw = true;

                            if (state == STATE_WAIT_GREEN) {
                                state = STATE_TOO_SOON;
                            }
                            else if (state == STATE_GREEN) {
                                
                                reaction_time = (counter_timer - start_time) * 4;
                                total_time += reaction_time;
                                state = STATE_RESULT;
                            }
                            else if (state == STATE_TOO_SOON) {
                               
                                counter_timer = 0;
                                random_delay = rand() % (3*frequency) + frequency;
                                state = STATE_WAIT_GREEN;
                            }
                            else if (state == STATE_RESULT && attempt<5) {
                                    attempt++;
                                    counter_timer = 0;
                                    tickforbar = 0;
                                    random_delay = rand() % (3*frequency) + frequency;
                                    state = STATE_WAIT_GREEN;
                                
                            }
                            else if (state == STATE_RESULT && attempt >= 5) {
                                state = STATE_AVERAGE;
                            }
                            else if (state == STATE_AVERAGE) {
                                if (scancode == 0x01 || scancode == BREAK_ESC) {
                                    mouse_disable_data_report();
                                    mouse_unsubscribe_int();
                                    running = false;
                                    scancode = 0;
                                }
                            }
                        }
                    }

                    /* rato – click esquerdo como "enter" */
                    if (msg.m_notify.interrupts & mouse_bit_no) {
                        mouse_ih();
                        if (packet_ready) {
                            packet_ready = false;
                            redraw = true;
                            
                            if (state == STATE_WAIT_GREEN && mouse_event.lb) {
                                state = STATE_TOO_SOON;
                            }
                            else if (state == STATE_GREEN && mouse_event.lb) {
                                reaction_time = (counter_timer - start_time) * 4;
                                total_time += reaction_time;
                                state = STATE_RESULT;
                            }
                            else if (state == STATE_TOO_SOON && mouse_event.lb) {
                                counter_timer = 0;
                                random_delay = rand() % (3*frequency) + frequency;
                                state = STATE_WAIT_GREEN;
                            }
                            else if (state == STATE_RESULT && mouse_event.lb) {
                                if (attempt < 5) {
                                    attempt++;
                                    counter_timer = 0;
                                    random_delay = rand() % (3*frequency) + frequency;
                                    state = STATE_WAIT_GREEN;
                                } else {
                                    state = STATE_AVERAGE;
                                }
                            }
                            else if (state == STATE_AVERAGE && (mouse_event.rb)) {
                                mouse_disable_data_report();
                                mouse_unsubscribe_int();
                                running = false;
                            }
                        }
                    }
                    break;
            }
        }

        if (state != last_state) {
            redraw = true;
            last_state = state;
        }

        if (redraw) {
            memset(double_buffer, 0, frame_size);
            switch (state) {
                case STATE_WAIT_GREEN:
                    draw_screen(0xFF0000, double_buffer);
                    print_xpm_string_centered_highlight("WAIT FOR GREEN", information.YResolution / 2 - 20, double_buffer, information.XResolution, 1);
                    sprite_case = attempt - 1;
                    if (sprite_case > 4) sprite_case = 4;
                    draw_pro_bar(progress_bar, double_buffer, sprite_case);
                    
                    break;
                case STATE_GREEN:
                    draw_screen(0x00FF00, double_buffer);
                    print_xpm_string_centered_highlight("PRESS ANY KEY!", information.YResolution / 2 - 20, double_buffer, information.XResolution, 1);
                    sprite_case = attempt - 1;
                    if (sprite_case > 4) sprite_case = 4;
                    draw_pro_bar(progress_bar, double_buffer, sprite_case);
                    
                    
                    
                    break;
                case STATE_TOO_SOON:
                    draw_screen(0x00008B, double_buffer);
                    draw_text_centered("TOO SOON!", information.YResolution / 2 - 20, 0xFFFFFF, double_buffer);
                    draw_text_centered("PRESS ANY KEY OR CLICK", information.YResolution / 2 + 20, 0xFFFFFF, double_buffer);
                    sprite_case = attempt - 1;
                    if (sprite_case > 4) sprite_case = 4;
                    draw_pro_bar(progress_bar, double_buffer, sprite_case);
                    
                    
                    break;
                case STATE_RESULT: {
                    sprite_case = attempt - 1;
                    if (sprite_case > 4) sprite_case = 4;
                    show_result_screen(reaction_time, sprite_case, progress_bar);
                           
                    break;
                }
                case STATE_AVERAGE: {
                    show_ending_screen(total_time,progress_bar);
                    
                    break;
                }
            }
            buffer_swap();
        }
    }
    destroy_pro_bar(progress_bar);
    return 0;
}




void show_result_screen(uint32_t reaction_time,int times,Sprite *progress_bar) {
  
    draw_screen(0x0000FF, double_buffer);// Blue background
    char msg[64];
    sprintf(msg, "ATTEMPT %d OUT OF 5", times+1);
    draw_text_centered(msg, information.YResolution / 2 - 40, 0xFFFFFF, double_buffer);
    sprintf(msg, "YOUR REACTION TIME WAS %d MS", reaction_time);
    draw_text_centered(msg, information.YResolution / 2, 0xFFFFFF, double_buffer);
    draw_text_centered("PRESS ANY KEY TO CONTINUE", information.YResolution / 2 + 40, 0xFFFFFF, double_buffer);
    draw_pro_result(progress_bar, double_buffer, times );
    buffer_swap();
}

void show_ending_screen(uint32_t total,Sprite *progress_bar) {
    draw_screen(0x0000FF, double_buffer);
    char msg[64];
    uint32_t avg = total / 5;
    sprintf(msg, "YOUR AVERAGE REACTION TIME WAS %d MS", avg);
    draw_text_centered(msg, information.YResolution / 2 - 20, 0xFFFFFF, double_buffer);
    draw_text_centered("PRESS ESC TO GO BACK TO MENU", information.YResolution / 2 + 20, 0xFFFFFF, double_buffer);
    draw_pro_result(progress_bar, double_buffer, 4);        
    buffer_swap();

}

void generate_sequence(int length) {
    for (int i = 0; i < length; i++) {
        sequence[i] = 'A' + rand() % 26;
    }
}

void display_sequence(int length) {
    draw_screen(0xADD8E6, double_buffer); // Light blue
    char buf[64];
    for (int i = 0; i < length; i++) {
        buf[i] = sequence[i];
    }
    buf[length] = '\0';
    print_xpm_string_centered_highlight(buf, information.YResolution / 2 - 20, double_buffer, information.XResolution, 1);
    buffer_swap();
    tickdelay(micros_to_ticks(2000000));

    draw_screen(0xADD8E6, double_buffer);
    buffer_swap();
}


void show_sequence(const char* answer, int length) {
    draw_screen(0xADD8E6, double_buffer); 
    char buf[64];
    for (int i = 0; i < length; i++) {
        buf[i] = answer[i];
    }
    buf[length] = '\0';
    print_xpm_string_centered_highlight(buf, information.YResolution / 2 - 20, double_buffer, information.XResolution, 1);
    buffer_swap();
}
bool get_user_input(int length) {
    int index = 0;
    char user_input[length + 1]; 
    message msg;
    int ipc_status;

    while (true) {
        if (driver_receive(ANY, &msg, &ipc_status) != 0) continue;

        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & keyboard_bit_no) {
                        kbc_ih(); 

                        if (scancode == 0x9C && index > 0) { 
                            user_input[index] = '\0'; 
                            if (index != length) return false;

                            if (compare_answer(user_input, length)) {
                                return true; 
                            } else {
                                return false;
                            }
                            return true;
                        } else if (scancode == 0x8E ) {
                            if (index > 0) {
                                index--;
                                user_input[index] = '\0'; 
                            }
                            show_sequence(user_input,index);
                        } else if(scancode & MAKE_CODE)  {
                            uint8_t clean_scancode = scancode & 0x7F;
                            char input = convert_scancode_to_char(clean_scancode);
                            if (input != 0 && index < length) {
                                user_input[index++] = input;
                            }
                            show_sequence(user_input,index);
                        }
                    }
                    break;
            }
        }
    }
}
bool compare_answer(const char* answer, int length) {
    for (int i = 0; i < length; i++) {
        if (toupper((unsigned char)answer[i]) != sequence[i]) {
            return false;
        }
    }
    return true;
}

char convert_scancode_to_char(uint8_t sc) {
    switch (sc) {
        case 0x1E: return 'A';
        case 0x30: return 'B';
        case 0x2E: return 'C';
        case 0x20: return 'D';
        case 0x12: return 'E';
        case 0x21: return 'F';
        case 0x22: return 'G';
        case 0x23: return 'H';
        case 0x17: return 'I';
        case 0x24: return 'J';
        case 0x25: return 'K';
        case 0x26: return 'L';
        case 0x32: return 'M';
        case 0x31: return 'N';
        case 0x18: return 'O';
        case 0x19: return 'P';
        case 0x10: return 'Q';
        case 0x13: return 'R';
        case 0x1F: return 'S';
        case 0x14: return 'T';
        case 0x16: return 'U';
        case 0x2F: return 'V';
        case 0x11: return 'W';
        case 0x2D: return 'X';
        case 0x15: return 'Y';
        case 0x2C: return 'Z';
        default:   return 0;  // not a letter
    }
}


void show_message_screen(const char* msg) {
    draw_screen(0x808080, double_buffer); // Dark background
    print_xpm_string_centered_highlight(msg, information.YResolution / 2 - 20, double_buffer, information.XResolution, 1);
    buffer_swap();
}

void show_countdown_screen() {
    draw_text_centered("GET READY FOR NEXT LEVEL", information.YResolution / 2 - 40, 0x0F, double_buffer);
    for (int i = 5; i >= 1; i--) {
        draw_screen(0x808080, double_buffer); // Dark background

        char numbuf[4];
        sprintf(numbuf, "%d", i);
        print_xpm_string_centered_highlight(numbuf, information.YResolution / 2 - 20, double_buffer, information.XResolution, 1);
        buffer_swap();
        tickdelay(micros_to_ticks(4000000)); // 1 second
    }
}

void show_lost_screen_and_wait_for_esc() {
    draw_screen(0x808080, double_buffer); // Dark background
    print_xpm_string_centered_highlight("YOU LOST", information.YResolution / 2 - 20, double_buffer, information.XResolution, 1);
    print_xpm_string_centered_highlight("PRESS ESC TO GO", information.YResolution / 2 + 60, double_buffer, information.XResolution, 1);
    print_xpm_string_centered_highlight("BACK TO MAIN MENU", information.YResolution / 2 + 120, double_buffer, information.XResolution, 1);
    buffer_swap();

    message msg;
    int ipc_status;
    while (true) {
        if (driver_receive(ANY, &msg, &ipc_status) != 0) continue;
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE:
                if (msg.m_notify.interrupts & keyboard_bit_no) {
                    kbc_ih();
                    if (scancode == BREAK_ESC) { // ESC break code
                        scancode = 0;
                        return;
                    }
                }
                break;
            }
        }
    }
}



void play_memory_game() {

    level = 1;
    while (true) {
        generate_sequence(level);
        display_sequence(level);

        draw_screen(0xADD8E6, double_buffer);
        if (!get_user_input(level)) {
            show_lost_screen_and_wait_for_esc();
            break;
        }
        show_countdown_screen();
        level++;
        if (level > MAX_SEQUENCE) level = MAX_SEQUENCE;
    }

}

static const char* word_list[] = {
    "TIE", "LUNGS", "KIT", "MOON", "BOOK", "CHAIR", "STAR", "PLANE", "FISH", "TREE",
    "DOOR", "CLOCK", "PEN", "SHOE", "LEAF", "BALL", "CAT", "DOG", "CAR", "HAT",
    "SUN", "CUP", "ROAD", "LEAF", "RAIN", "SNOW", "WIND", "FIRE", "ROCK", "SAND",
    "WAVE", "BOAT", "GRASS", "HILL", "FOG", "ICE", "MUD", "NEST", "PARK", "SEED",
    "SKY", "SOIL", "VASE", "WALL", "WIND", "YARD", "ZOO", "BEE", "FOX", "ANT"
};
int total_words = sizeof(word_list) / sizeof(word_list[0]);

bool word_already_shown(const char* word, const char* seen_words[], int seen_count) {
    for(int i = 0; i < seen_count; i++) {
        if (strcmp(word, seen_words[i]) == 0) return true;
    }
    return false;
}

void draw_verbal_memory_screen(const char* word, int lives, int selected) {
    draw_screen(0xADD8E6, double_buffer); // light blue background

    draw_text_centered(word, information.YResolution / 2, 0x000000, double_buffer); // black text

    char lives_message[30];
    sprintf(lives_message, "LIVES: %d", lives);
    draw_text_centered(lives_message, information.YResolution - 60, 0x000000, double_buffer); // black text

    if (selected == 0) {
        draw_text_centered("NEW", 20, 0xFFFFFF, double_buffer);  // white highlight
        draw_text_centered("SEEN", 60, 0x000000, double_buffer); // black text
    } else {
        draw_text_centered("NEW", 20, 0x000000, double_buffer);  // black text
        draw_text_centered("SEEN", 60, 0xFFFFFF, double_buffer); // white highlight
    }

    buffer_swap(); 
}


void play_verbal_memory_test() {
    int lives = 3;
    bool user_says_seen;
    const char* seen_words[total_words];
    int seen_count = 0;
    int selected = 0;

    while (lives > 0) {
        int idx = rand() % total_words;
        const char* current_word = word_list[idx];
        bool seen = word_already_shown(current_word, seen_words, seen_count);

        bool answered = false;
        draw_verbal_memory_screen(current_word, lives, selected);

        message msg;
        int ipc_status;

        while (!answered) {
            if (driver_receive(ANY, &msg, &ipc_status) != 0) continue;

            if (is_ipc_notify(ipc_status)) {
                switch (_ENDPOINT_P(msg.m_source)) {
                    case HARDWARE:
                        if (msg.m_notify.interrupts & keyboard_bit_no) {
                            kbc_ih();
                                switch (scancode) {
                                    case 0x48:
                                        selected = 0;
                                        draw_verbal_memory_screen(current_word, lives, selected);
                                        break;
                                        case 0x50:
                                        selected = 1;
                                        draw_verbal_memory_screen(current_word, lives, selected);
                                        break;
                                    case 0x1C:
                                        
                                        user_says_seen = (selected == 1);
                                        if (user_says_seen == seen) {
                                            if (!seen && seen_count < total_words) {
                                                seen_words[seen_count++] = current_word;
                                            }
                                        }
                                        else lives--;
                                        answered = true;
                                        break;
                                }
                            
                        }
                        break;
                }
            }
        }
    }

    show_lost_screen_and_wait_for_esc();
}


