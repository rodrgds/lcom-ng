/**
 * @file proj.c
 * 
 *  @brief Project main loop
 */

#include <lcom/lcf.h>

#include <lcom/proj.h>
#include <game.h>


#define FRAME_RATE 60


// IRQ masks
uint8_t timer_irq_set_mask;
uint8_t keyboard_irq_set_mask;
uint8_t mouse_irq_set_mask;
uint8_t rtc_irq_set_mask;

// Device variables
extern bool keyboard_complete;
extern bool mouse_complete;
extern uint8_t scancode;
extern int timerIntCounter;

// Game related variables
extern Cursor* cursor;
extern Game game;




int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  //lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  //lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


/**
 * @brief Subscribes all the needed device's interrupts
 * 
 * Subscribes all the needed device's interrupts
 * @return Return 0 when successful, 1 otherwise
 */
int subscribe_interrupts() {

  if (timer_subscribe_int(&timer_irq_set_mask) != 0) return 1;
  if (keyboard_subscribe_int(&keyboard_irq_set_mask) != 0) return 1;
  if (mouse_enable_data_report() != 0) return 1;
  if (mouse_subscribe_int(&mouse_irq_set_mask) != 0) return 1;
  if (rtc_subscribe_int(&rtc_irq_set_mask) != 0) return 1;

  return 0;
}



/**
 * @brief Unsubscribes all the needed device's interrupts
 * 
 * Unsubscribes all the needed device's interrupts
 * @return Return 0 when successful, 1 otherwise
 */
int unsubscribe_interrupts() {

  if (timer_unsubscribe_int() != 0) return 1;
  if (keyboard_unsubscribe_int() != 0) return 1;
  if (mouse_unsubscribe_int() != 0) return 1;
  if (mouse_disable_data_report() != 0) return 1;
  if (rtc_unsubscribe_int() != 0) return 1;

  return 0;
}



/**
 * @brief Initializes the game and needed devices and handles interrupts
 * 
 * Prepares videocard, changes game state to go to the Main Menu, subscribes device's interrupts and handles them
 * @return Return 0 when successful, 1 otherwise
 */
int(proj_main_loop)(int argc, char *argv[]) {
  

  if (timer_set_frequency(0, FRAME_RATE) != 0) return 1;  //Sets the frame rate as 60 frames per second
  if (subscribe_interrupts() != 0) return 1; //Subscribe the needed device's Interrupts

   //Obtain initial time and date
  if (rtc_update_time() != 0) return 1;
  if (rtc_update_date() != 0) return 1;
  

  if (vg_set_graphics_mode(SUBMODE_14C) != 0) return 1;  //Configure videocard to work with 1152x864 resolution and Direct Color mode
  
  initialize_sprites();  //Initialize all the needed sprites

 

  //Handle Interrupts while the game doesn't reach EXITED state nor the ESC key is released
  int ipc_status, r;
  message msg;
 
 while( (game.currentState != EXITED) && (scancode != ESC_BREAK_CODE) ) { 
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
         printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) {
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: 		

                

                //Keyboard Interrupt ------------------------------------------
                 if (msg.m_notify.interrupts & keyboard_irq_set_mask) {  
                    kbc_ih();  //Read scancode
                    if (keyboard_complete) { //If received a complete scancode, handle it
                        handle_pressed_key();  
                    }
                    break;
                  
                }
        
                //Mouse Interrupt ------------------------------------------------
                if(msg.m_notify.interrupts & mouse_irq_set_mask) {   
                  mouse_ih();
                  if (mouse_complete) {   //If received a complete packet, handle it
                    mouse_parse_packet();
                    handle_mouse_move();
                  }
                  break; 
                }


                 //RTC Interrupt ------------------------------------------------
                if (msg.m_notify.interrupts & rtc_irq_set_mask) { 
                     rtc_ih();
                     if ((game.currentState == MAINMENU) || (game.currentState == LEADERBOARDMENU) || (game.currentState == INSTRUCTIONSMENU)) //draw_time();
                     //swap_buffer();
                     break;
              
                }

               

                //Timer 0 Interrupt ------------------------------------------------
                if (msg.m_notify.interrupts & timer_irq_set_mask) { 
                     timer_int_handler();
                     show_current_screen();
                     handle_game_loop(); //Handle game events
                     if ((game.currentState == MAINMENU) || (game.currentState == LEADERBOARDMENU) || (game.currentState == INSTRUCTIONSMENU)) {
                      draw_time();
                      drawCursor(cursor);
                     }
                     swap_buffer();
                     break;
              
                }

               default:
                  break;
                
              
        }
  }
    else {}
  }
  
  destroy_sprites();
  destroy_buffer(); //Destroy the temporary frame buffer
  if (vg_exit() != 0) return 1; //Return to text mode 

  if (unsubscribe_interrupts() != 0) return 1; //Unsubscribe interrupts before exiting game


return 0;
}




