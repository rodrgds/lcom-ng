/**
 * \mainpage Tetris 
 * An adaptation of the worldwide known game Tetris, Within the scope of the curricular unit Laboratório de Computadores - 2º semester 2022/2023
 * 
 * @author Diogo Pintado, Gabriel Machado Jr., José Costa, Linda Rodrigues
 */

// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/proj.h>

#include <stdint.h>
#include <stdio.h>

// Created by the group
#include "board.h"
#include "game.h"
#include "game_state.h"
#include "i8042.h"
#include "keyboard.h"
#include "keys.h"
#include "menu.h"
#include "mouse.h"
#include "vbe_graphics.h"
#include "game_timer.h"

extern uint8_t scancode, stat;
extern GameState game_state;

extern int counter;
extern int count;

extern int game_timer;

extern struct packet pp;
extern uint8_t data, byte_index;
extern uint8_t packet[3];

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return EXIT_SUCCESS;
}

int(proj_main_loop)(int argc, char *argv[]) {
  int ipc_status;
  message msg;
  int r;

  uint8_t timer_bit_no = 0;
  uint8_t kbc_bit_no = 7;
  uint8_t mouse_bit_no = 6;

  // Subscribers
  if (kbc_subscribe_int(&kbc_bit_no) != 0) {
    printf("There was an error in kbd_subscribe_int function!\n");
    return 1;
  }

  if (timer_subscribe_int(&timer_bit_no) != 0) {
    printf("There was an error in timer_subscribe_int function!\n");
    return 1;
  }

  
  if (write_to_mouse(ENABLE_DATA_REPORTING) != 0) {
    printf("Error in enabling data reporting\n");
    return 1;
  }

  if (mouse_subscribe_int(&mouse_bit_no) != 0) {
    printf("There was an error in mouse_subscribe_int function!\n");
    return 1;
  }
  

  // Init graphic mode
  if (vg_init(0x115) == NULL) {
    printf("There was an error in vg_init function!\n");
    vg_exit();
    return 1;
  }

  clean_screen();
  clean_buffer();
  menu_files_init();
  draw_menu();
  video_flip();//memcopy(buffer,vram,size)
  instruction_file_init();
  load_score_xpm();
  timer_files_init();
  load_game_over();

  // board and pieces and score
  Board *board = new_board();
  Piece *piece = new_piece(board);
  extern int score;

  /*when scancode is equal to esc key, the loop ends*/
  while (scancode != 0x81) {

    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (game_state == MENU) {
      draw_menu();
      video_flip();
    }

    else if(game_state == GAME){
      draw_score();
      video_flip();
    }

    else if(game_state == INSTRUCTIONS){
      draw_instructions();
      video_flip();
    }

    else if(game_state == GAME_OVER){
      clean_screen();
      write_game_over();
      video_flip();
    }

    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:               /* hardware interrupt notification */
          if (msg.m_notify.interrupts & kbc_bit_no) {
            kbc_ih();
            if (navigate_menu(&scancode) && game_state == MENU) {
              draw_menu();
              //video_flip();
            }

            else if (scancode == KBD_Q && game_state == GAME) {
                clean_buffer();
                game_state = MENU;
            }

            else if(game_state == GAME){
              movement_input(&scancode,piece);
            }

            else if(game_state == INSTRUCTIONS){
              if(scancode == KBD_Q){
                clear_instructions();
                clean_buffer();
                game_state = MENU;
              }
            }
            counter = 0;
          }
          
          if (msg.m_notify.interrupts & timer_bit_no) {
            timer_int_handler();
            if (game_state == GAME) {
              if (counter % 20 == 0) {
                play_game(piece);
                video_flip();
              }
              if(counter % 60 == 0){
                  game_counter();
                  game_timer++;
              }
            }
          }

          if (msg.m_notify.interrupts & mouse_bit_no) {
                mouse_ih();
                sync_mouse_bytes();
                if(byte_index == 3) {
                    build_mouse_packet();
                    //mouse_print_packet(&pp);
                    byte_index = 0;
                    if(game_state == MENU) {
                        struct mouse_ev *cur_mouse_ev = mouse_detect_event(&pp);
                        if(menu_mouse_handler(&scancode, cur_mouse_ev)){
                            draw_menu();
                        }  
                    }         
                }
            }

          /*if (game_state == GAME) {
            draw_squares(board);
          }*/
        default:
          break; /* no other notifications expected: do nothing */
      }
    }
  }

  delete_piece(piece);

  // Unubscribers
  if (kbc_unsubscribe_int() != 0) {
    printf("There was an error in kbd_subscribe_int function!\n");
    return 1;
  }

  if (timer_unsubscribe_int() != 0) {
    printf("There was an error in timer_subscribe_int function!\n");
    return 1;
  }
  
  if (mouse_unsubscribe_int() != 0) {
    printf("There was an error in mouse_subscribe_int function!\n");
    return 1;
  }

  if (write_to_mouse(DISABLE_DATA_REPORTING) != 0) {
    printf("Error in enabling data reporting");
    return 1;
  }

  vg_exit();

  return 0;
}
