// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include "keyboard.h"
#include "video.h"
#include "mouse.h"
#include "packet.h"

#include "cursor.h"
#include "game.h"

#include "pixmap/play_button.h"
#include "pixmap/title.h"

#include <i8042.h>
#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you


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

/*
void display_menu() {


  vg_start(0x11A);

  //int h_res = get_h_res();
  //int v_res = get_v_res();
  //int bits_per_pixel = get_bits_per_pixel();


  //xpm_image_t img;
  //uint16_t *sprite_button = (uint16_t*)xpm_load(play_button, XPM_5_6_5, &img);

  //const uint16_t transp = xpm_transparency_color(XPM_5_6_5);

  //int xi =  v_res/2 - img.height, yi = h_res/2 - img.width/2;
  //uint16_t *sprite_title = (uint16_t*)xpm_load(title_xpm, XPM_5_6_5, &img);

  //xi =  xi*(3/2) - img.height;
  //yi = h_res/2 - img.width/2;
  
  //draw_xpm(700, 980, sprite_title, img, transp);



  uint32_t byteCounter = 0;
  struct packet pp;

	int ipc_status;
	message msg;
	uint8_t bit_no_kbd, bit_no_mouse;
	int r;

  extern uint32_t mouseData;
  extern uint32_t kbdData;

  kbd_subscribe_int(&bit_no_kbd);
  uint64_t irq_set_kbd = BIT(bit_no_kbd);

  mouse_subscribe_int(&bit_no_mouse);
	uint64_t irq_set_mouse = BIT(bit_no_mouse);

  bool click = false;

  while (kbdData != ESC || click == true) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: 
          if (msg.m_notify.interrupts & irq_set_kbd) {
            kbdData = 0;
            kbd_ih();
          }else if (msg.m_notify.interrupts & irq_set_mouse) {
						mouseData = 0;   

            mouse_ih();                        
            byteCounter++;

            parseToPacket(byteCounter % 3, mouseData, &pp);
            if(pp.lb == 1){
              click = true;
            }

            if (byteCounter % 3 == 0){
              mouse_print_packet(&pp);
              resetPacket(&pp);
            }
					}
					break;
				default:
					break; 
      }
    }
  }
  kbd_unsubscribe_int();

  if(click == true){
    //TODO: call function to start game here!!!!!!!
  }

  if (vg_exit() != 0)
    return ;

  return ;
}
*/




int (proj_main_loop)(int argc, char *argv[]) {
  
  printf("%s(%d, %p): under construction\n", __func__, argc, argv);

  
	///display_menu();

	Game *game = Start();
  printf("Started");
  if (game != NULL)
    Handler(game);
  else printf(" ");
  
	End();


  return 0;
}
