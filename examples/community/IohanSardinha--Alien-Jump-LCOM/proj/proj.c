// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <stdlib.h>
#include <time.h>

#include "utils.h"
#include "graphics.h"
#include "timer.h"
#include "keyboard.h"
#include "mouse.h"
#include <stdint.h>
#include <stdio.h>
#include "i8042.h"
#include "sprites.h"
#include "letters.h"
#include "game.h"
#include "mainmenu.h"
#include "gameovermenu.h"
#include "scoreboardmenu.h"
#include "RTC.h"


/** @defgroup Proj Proj
 * @{
 *
 * Contais tha main of the project
 */

// Any header files included below this line should have been created by you

extern uint8_t data;
extern bool new_scancode;
extern uint32_t timer_count;
extern int score;
extern int asteroid_vel;
extern bool paused;
///Current game state
enum game_state game_state = MAIN_MENU;

///Game frame rate
int fr_rate = 60;

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

///Project main loop, basicly a state machine for each screen
int (proj_main_loop)()
{
    message msg;
    int ipc_status;
    int r;
    int irq_kbd, irq_mouse;
    uint8_t irq_timer;
    int mouseByte = 0;
    uint8_t data3bytes[3];

    srand(time(NULL));

    mouse_subscribe_int(&irq_mouse);
    setMouseIRQ(DISABLED);
    mouse_write_command(STREAM_MODE);
    mouse_write_command(ENABLE_DATA_REPORT);
    setMouseIRQ(ENABLED);    

    timer_subscribe_int(&irq_timer);

    keyboard_subscribe_int(&irq_kbd);
    
    vg_init(0x105);

    load_sprites();

    load_scores();

    while(game_state != END)
    {

        /* Get a request message. */
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 )
        {
            printf("driver_receive failed with: %d", r);
            continue;
        }

        if (is_ipc_notify(ipc_status)) { /* received notification */
            switch (_ENDPOINT_P(msg.m_source))
            {
                case HARDWARE: /* hardware interrupt notification */
                    if(msg.m_notify.interrupts & irq_timer)
                    {
                        timer_int_handler();
                        if(game_state == MAIN_MENU)
                        {
                           main_menu_frame();
                        }
                        else if(game_state == GAME && !paused)
                        {
                          if(timer_count%(60/fr_rate) == 0)
                          {
                              game_frame();
                          }
                          if(timer_count%30 == 0) addScore();
                        }
                        else if(game_state == GAME_OVER)
                        {
                           game_over_menu_frame();
                        }
                        else if(game_state == SCORE_BOARD)
                        {
                           score_board_menu_frame();
                        }

                    }
                    if (msg.m_notify.interrupts & irq_kbd) { /* subscribed interrupt */
                        keyboard_IH();
                        if(game_state == MAIN_MENU)
                        {
                            main_menu_keyboard(data);
                        }
                        else if(game_state == GAME_OVER)
                        {
                            game_over_menu_keyboard(data);
                        }
                        else if(game_state == GAME)
                        {
                            game_keyboard(data);
                        }
                        else if(game_state == SCORE_BOARD)
                        {
                            score_board_menu_keyboard(data);
                        }
                    }
                    if (msg.m_notify.interrupts & irq_mouse)/* subscribed interrupt */ 
                    { 
                      mouse_ih();
                      switch(mouseByte)
                      {
                        case 0:
                          data3bytes[0] = data;
                          mouseByte = 1;
                          break;
                        case 1:
                          data3bytes[1] = data;
                          mouseByte = 2;
                          break;
                        case 2:
                          data3bytes[2] = data;
                          mouseByte = 0;
                          if (!(data3bytes[0] & BIT(3)))
                            continue;
                          struct packet pckt;
                          createMousePacket(&pckt,data3bytes);
                          if(game_state == GAME && !paused)
                          {
                             game_mouse(pckt);
                          }
                          if(game_state == MAIN_MENU)
                          {
                             main_menu_mouse(pckt);
                          }
                          if(game_state == GAME_OVER)
                          {
                             game_over_menu_mouse(pckt);
                          }
                          if(game_state == SCORE_BOARD)
                          {
                             score_board_menu_mouse(pckt);
                          }
                          break;
                      }
                  }
                    break;
                default:
                    break; /* no other notifications expected: do nothing */
            }
        }
        else
        { /* received a standard message, not a notification */
            /* no standard messages expected: do nothing */
        }
    }
    
    setMouseIRQ(DISABLED);
    mouse_write_command(DISABLE_DATA_REPORT);
    setMouseIRQ(ENABLED);
    
    mouse_unsubscribe_int();
    
    timer_unsubscribe_int();

    keyboard_unsubscribe_int();

    vg_exit();
    
    free_buffer();

    return 0;
}

