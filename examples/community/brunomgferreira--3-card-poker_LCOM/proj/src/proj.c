#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "rtc/rtc.h"
#include "video/video.h"
#include "timer/timer.h"
#include "timer/i8254.h"
#include "keyboard_mouse/i8042.h"
#include "keyboard_mouse/kbc.h"
#include "keyboard_mouse/keyboard.h"
#include "keyboard_mouse/mouse.h"
#include "states/main.h"
#include "sprites/sprite.h"
#include "sprites/animSprite.h"

extern uint8_t *game_buffer;
extern uint8_t *menu_buffer;
extern uint8_t *create_player_buffer;
extern uint8_t *choose_player_buffer;

extern uint8_t scancode[];
extern int scancode_size;
extern bool second_byte;
extern uint8_t data;

extern struct packet mPacket;
extern struct mouse_position mPosition;
extern int byteIndex;
extern int timer_counter;

extern main_state_enum main_state;

extern char file_path[1024];

int main(int argc, char *argv[])
{
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  // lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  // lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (set_interrupts)() {
  if (mouse_write(ENABLE_DATA_REPORTING))
    return 1;

  if(timer_set_frequency(0, 60)) return 1;

  uint8_t irq_set_mouse, irq_set_keyboard, irq_set_timer;

  if (timer_subscribe_int(&irq_set_timer))
    return 1;

  if (kbd_subscribe_int(&irq_set_keyboard))
    return 1;

  if (mouse_subscribe_int(&irq_set_mouse))
    return 1;

  int ipc_status, r;
  message msg;

  while (main_state != EXIT)
  {
    if ((r = driver_receive(ANY, &msg, &ipc_status)))
    {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status))
    {
      switch (_ENDPOINT_P(msg.m_source))
      {
      case HARDWARE:
        if (msg.m_notify.interrupts & irq_set_timer)
        { 
          timer_int_handler();
          timer_int_controller();
        }
        if (msg.m_notify.interrupts & irq_set_mouse)
        { 
          mouse_ih();
          process_packet_byte();

          if (byteIndex == 3)
          {
            // printf("(%d, %d)\n", mPosition.x, mPosition.y);
            update_mouse_position();
            mouse_input_controller();            
            byteIndex = 0;
          }
        }
        if (msg.m_notify.interrupts & irq_set_keyboard)
        { 
          kbc_ih();

          process_scancode();

          if (!second_byte) {
            keyboard_input_controller();

            scancode_size = 1;
          }
        }
        
      }
    }
  }

  if (mouse_unsubscribe_int())
    return 1;

  if (kbd_unsubscribe_int())
    return 1;

  if (mouse_write(DISABLE_DATA_REPORTING))
    return 1;

  if (timer_unsubscribe_int())
    return 1;

  return 0;
}

void (initialize_buffers)() {
    game_buffer = (unsigned char *)malloc(XResolution * YResolution * BytesPerPixel);
    menu_buffer = (unsigned char *)malloc(XResolution * YResolution * BytesPerPixel);
    create_player_buffer = (unsigned char *)malloc(XResolution * YResolution * BytesPerPixel);
    choose_player_buffer = (unsigned char *)malloc(XResolution * YResolution * BytesPerPixel);
    draw_sprite_to_buffer(game_background, game_buffer);
    draw_sprite_to_buffer(menu_background, menu_buffer);
    draw_sprite_to_buffer(create_player_background, create_player_buffer);
    draw_sprite_to_buffer(choose_player_background, choose_player_buffer);
}

void (destroy_buffers)() {
    if (game_buffer) {
        free(game_buffer);
        game_buffer = NULL;
    }
    if (menu_buffer) {
        free(menu_buffer);
        menu_buffer = NULL;
    }
    if (create_player_buffer) {
        free(create_player_buffer);
        create_player_buffer = NULL;
    }
    if (choose_player_buffer) {
        free(choose_player_buffer);
        choose_player_buffer = NULL;
    }
}

int (proj_main_loop)(int argc, char *argv[]) {
    uint16_t mode = 0x14C;
    if (set_frame_buffer(mode)) return 1;
    if (set_video_mode(mode)) return 1;

    create_sprites();
    create_animSprites();
    initialize_buffers();

    char dir_path[1024];
    extract_directory_path(dir_path);
    sprintf(file_path, "%s%s", dir_path, "player_data.csv");
    read_player_data_from_file();

    rtc_setup();
    rtc_read_time();

    if (set_interrupts()) return 1;
    
    if (custom_vg_exit()) return 1; // Call custom_vg_exit instead of vg_exit
    destroy_buffers();
    destroy_sprites();
    destroy_animSprites();
    
    return 0;
}
