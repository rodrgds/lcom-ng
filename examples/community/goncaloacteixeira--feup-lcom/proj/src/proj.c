// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>

// Any header files included below this line should have been created by you

#include "timer.h"
#include "tetris.h"

#include <lcom/xpm.h>


int main(int argc, char *argv[]) {
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");

    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need it]
    lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    lcf_log_output("/home/lcom/labs/proj/output.txt");

    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv))
        return 1;


    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();

    return 0;
}


int32_t kbd_id = 1;
uint32_t irq_set;   // keyboard
uint8_t irq_set_timer;
uint32_t mouseID = 12;
uint32_t irq_set_mouse;


/*
 * Mode needs to be 0x117
 * 1024x768
 * RGB 565
 */

int(proj_main_loop)(int argc, char *argv[]) {

    if (vg_init(0x117) == NULL) {
        printf("error initializing video in graphics mode\n");
    }

    
    menus_t menus = startMenus();
    
    uint32_t bit_no;
	if (keyboard_subscribe_int(&bit_no) != 0) {
        printf("error subs kbd\n");
        return 1;
    }
    irq_set = bit_no;
    if (timer_subscribe_int(&irq_set_timer) != 0) {
        printf("error subs timer\n");
        return 1;
    }
    mainTitle(&menus);

    if (mouse_issue_cmd(ENABLE) != 0) {
        printf("error::mouse_issue_cmd(ENABLE)\n");
        return 1;
    }
    if (mouse_subscribe_int(&mouseID) != 0) {
        printf("erro subs mouse\n");
        return 1;
    }
    irq_set_mouse = mouseID;
    while (diffMenu(&menus) == 0) {
        mouse_issue_cmd(DISABLE);
        mouse_unsubscribe_int();
        
        tetris game = startGame(menus.difficulty);
        tetris_start(&game,&menus);

        if (mouse_issue_cmd(ENABLE) != 0) {
            printf("error::mouse_issue_cmd(ENABLE)\n");
            return 1;
        }
        if (mouse_subscribe_int(&mouseID) != 0) {
            printf("erro subs mouse\n");
            return 1;
        }
    }

    mouse_issue_cmd(DISABLE);
    mouse_unsubscribe_int();
    timer_unsubscribe_int();
    keyboard_unsubscribe_int();
    
    vg_exit();

    return 0;
}
