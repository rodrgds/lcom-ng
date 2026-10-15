// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "graphics.h"
#include "keyboard.h"
#include <lcom/pixmap.h>
#include "timer.c"

extern vbe_mode_info_t vbe_mode_info;
extern bool ESC_key;
extern int counter_timer;

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

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  if(vg_init(mode) == NULL) return 1; //set the graphic mode
  sleep (delay); //wait the delay time
  if(vg_exit()) return 1; //return to text mode

  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
                            
  if(vg_init(mode) == NULL) return 1; //set the graphic mode

  if(vg_color_mode(&color)) return 1; //change way color is represented depending on mode

  if(vg_draw_rectangle(x, y, width, height, color)) return 1; //draw the rectangle

  if(await_ESC_break()) return 1; //wait to identify the esc key being released
  
  if(vg_exit()) return 1; //return to text mode

  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  if(vg_init(mode) == NULL) return 1;

  uint16_t width, height; //width and height of each rectangle

  width = vbe_mode_info.XResolution / no_rectangles;
  height = vbe_mode_info.YResolution / no_rectangles;

  bool indexed = (mode == 0x105); //is only indexed color on mode 0x105

  uint32_t red_first, green_first, blue_first;
  if(!indexed){
    red_first = ((1 << vbe_mode_info.RedMaskSize) -1) & (first >> vbe_mode_info.RedFieldPosition);
    green_first = ((1 << vbe_mode_info.GreenMaskSize) -1) & (first >> vbe_mode_info.GreenFieldPosition);
    blue_first = ((1 << vbe_mode_info.BlueMaskSize) -1) & (first >> vbe_mode_info.BlueFieldPosition);
  }

  for (unsigned int row = 0; row<no_rectangles; row++){ //iterate through rows of rectangles
    for (unsigned int col = 0; col<no_rectangles; col++){ //in each row, iterate through columns
      uint32_t color;

      if(indexed) color = (first + (row * no_rectangles + col) * step) % (1 << vbe_mode_info.BitsPerPixel);
      else{
        uint32_t red = (red_first + col * step) % (1 << vbe_mode_info.RedMaskSize);
        uint32_t green = (green_first + col * step) % (1 << vbe_mode_info.GreenMaskSize);
        uint32_t blue = (blue_first + col * step) % (1 << vbe_mode_info.BlueMaskSize);

        color = (red << vbe_mode_info.RedFieldPosition) | (green << vbe_mode_info.GreenFieldPosition) | (blue << vbe_mode_info.BlueFieldPosition);
      }

      if(vg_color_mode(&color)) return 1; //change way color is represented depending on mode

      if(vg_draw_rectangle(col*width, row*height, width, height, color)) return 1; //draw the rectangle
    }
  }

  if(await_ESC_break()) return 1; //wait for the break code of esc

  if(vg_exit()) return 1;

  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  if(vg_init(105) == NULL) return 1; //change to video mode 105

  xpm_image_t img;

  uint8_t* map = xpm_load(xpm, XPM_INDEXED, &img); //map is an array of the color of the pixels by order

  if(vg_draw_xpm(img, map, x, y)) return 1;

  if(await_ESC_break()) return 1; //wait for the break code of esc

  if(vg_exit()) return 1;

  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  if(vg_init(105) == NULL) return 1; //change to video mode 105

  int frames_left = 0; //for when the speed is negative (frames left for the next pixel movement)
  if(speed <0) frames_left = -speed;

  bool x_displacement = (xi!=xf); //true is the dispacement is horizontal, false if it is vertical
  uint16_t x = xi, y = yi; //keeps the current x and y values

  xpm_image_t img;
  uint8_t* map = xpm_load(xpm, XPM_INDEXED, &img); //map is an array of the color of the pixels by order

  int ipc_stat;
  uint8_t mask_timer, mask_keyboard;
  message msg;


  if(timer_subscribe_int(&mask_timer)) return 1; //subscribe timer interrupts
  if(keyboard_subscribe_int(&mask_keyboard)) return 1; //subscribe keyboard interrupts


  while(!ESC_key){
    if(driver_receive(ANY, &msg, &ipc_stat)) continue; //error on driver_receive, try again

    if(is_ipc_notify(ipc_stat)){ //if it was a notification
      switch(_ENDPOINT_P(msg.m_source)){ //check where the message comes from

        case HARDWARE: //it came from a hardware device (like a keyboard)
          if(msg.m_notify.interrupts & mask_timer){ //check if the interrupt was from the timer 0
            timer_int_handler(); //call the interruption handler (count++)

            if(counter_timer%(60/fr_rate) == 0){ //one more frame needs to be processed
              if(speed<0 && frames_left!=0) frames_left--; //one less frame left for moving one pixel
              
              else if (speed <0 && (x!=xf || y!=yf)){ //if it's the movement frame and the image is not yet in its final position
                //clean the area that will not be overwritten
                if(x_displacement){
                  if(vg_draw_rectangle(x, y, 1, img.height, 0xFFFFFF)) return 1;
                  x++;
                }
                else{
                  if(vg_draw_rectangle(x, y, img.width, 1, 0xFFFFFF)) return 1;
                  y++;
                }

                if(vg_draw_xpm(img, map, x, y)) return 1; //draw the image w the new values

                frames_left = -speed; //reset the frames left for the next movement
              }

              else if (x!=xf || y!=yf){ //the speed is not negative and the image is still not in its correct place
                //clean the area that will not be overwritten

                if(x_displacement){
                  if(vg_draw_rectangle(x, y, speed, img.height, 0xFFFFFF)) return 1;
                  if(x + speed > xf) x = xf;
                  else x += speed;
                }
                else {
                  if(vg_draw_rectangle(x, y, img.width, speed, 0xFFFFFF)) return 1;
                  if(y + speed > yf) y = yf;
                  else y += speed;
                }

                if(vg_draw_xpm(img, map, x, y)) return 1; //draw the image w the new values
              }

              change_display_buffer();
            }
          }
          if(msg.m_notify.interrupts & mask_keyboard){ //check if the interrupt was from the keyboard
            kbc_ih(); //call the interruption handler
          }
          break;

        default:
          break; //no other notifications expected
      }
    }
  }

  if(timer_unsubscribe_int()) return 1;
  if(keyboard_unsubscribe_int()) return 1;

  if(vg_exit()) return 1;

  return 0;
}

int(video_test_controller)() {
  /* This year you do not need to implement this */
  printf("%s(): under construction\n", __func__);

  return 1;
}
