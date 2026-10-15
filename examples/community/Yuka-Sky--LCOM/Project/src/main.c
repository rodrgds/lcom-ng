#include <lcom/lcf.h>
#include <stdio.h>
#include <lcom/proj.h>
#include <stdlib.h>
#include <time.h>
#include "mousekb/keyboard.h"
#include "drawScreens.h"
#include "timer/timer.c"

#define FPS 60
#define MODE 0x14C
#define NUM_SUBJECTS 70 

char* subjects[NUM_SUBJECTS] = {
  "CAT", "CUP", "FORK", "APPLE", "FROG", "CHEESE", "HOUSE", "PENCIL", "BANANA", "AVOCADO",
  "BEACH", "GLASSES", "MOON", "BOMB", "PLANE", "HOT DOG", "CLOCK", "BALLOON", "CANDLE", "POTION",
  "HAT", "SNAIL", "OWL", "PYRAMID", "BACKPACK", "GUITAR", "MOUSTACHE", "WAND", "SUNGLASSES", "LEMON",
  "PINATA", "PINEAPPLE", "WINDOW", "TOWER", "CAR", "BICYCLE", "SUN", "FLOWER", "CHICKEN", "LEAF",
  "SATURN", "CHURCH", "PILLOW", "BEAR", "SNAKE", "BUTTERFLY", "TREE", "LAKE", "MOUNTAIN", "EYE",
  "STORM", "BOAT", "HAND", "PUMPKIN", "KEY", "SWORD", "CASTLE", "ORANGUTAN","LAMP", "CANE",
  "GHOST", "SPIDER", "PEN", "BOOK", "DRESS", "FLAMINGO", "PLATYPUS", "CHAIR", "FLAG", "RAINBOW"
};

typedef enum{
  MAIN_MENU,
  COUNTDOWN,
  GUESSING,
  WON,
  TIME_OVER
} GameState;

extern int byte_index;
extern struct packet mouse_packet;
extern vbe_mode_info_t vbe_mode_info;

GameState gameState = MAIN_MENU;

/**
 * @brief function main, initializes LCF
 * 
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return 0 on success.
 */
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
 * @brief Gets a random string from the subjects array.
 * 
 * @return the string that was selected.
 */
char* get_random_subject() {
  // Generate a random index between 0 and NUM_SUBJECTS-1
  int random_index = rand() % NUM_SUBJECTS;
  return subjects[random_index];
}

/**
 * @brief Draws the current game screen based on game state.
 *
 * @param mouse_map pointer to XPM pixel map of the mouse. 
 * @param img a struct cointaining the image's width, height and type information.
 * @param mouse_x int of mouse x coordinate.
 * @param mouse_y int of mouse y coordinate.
 * @param elapsed_time current elapsed time.
 * @param string input text.
 * @param size_str length of the input string.
 * @param drawingScreen array of pixels representing the drawing screen area.
 * @param c_mouse pointer for checking if mouse needs to be redrawn (c_mouse > 0)
 * @param c_text pointer for checking if text needs to be redrawn (c_text > 0)
 * @param c_timer pointer for checking if timer needs to be redrawn (c_timer > 0)
 * @param new_screen pointer to screen draw countdown.
 * @param subject word to be drawn/guessed.
 * @param history array with the MAX_HISTORY_SIZE previous tries submissions.
 * @param history_sizes array with the sizes of the MAX_HISTORY_SIZE previous tries.
 * @param history_count number of strings in history.
 *
 * @return 0 on success or 1 if it fails.
 */
int (update_vram)(uint8_t* mouse_map, xpm_image_t img, int mouse_x, int mouse_y, int elapsed_time, char string[], int size_str, uint32_t drawingScreen[], int* c_mouse, int* c_text, int* c_timer, int* new_submit, int* new_screen, char* subject, char history[MAX_HISTORY_SIZE][3 * MAX_STR_LEN], int history_sizes[MAX_HISTORY_SIZE], int history_count){
  switch (gameState){
  case MAIN_MENU:
    if(draw_main_menu(mouse_map, img, mouse_x, mouse_y, c_mouse, new_screen)) return 1;
    break;
    
  case COUNTDOWN:
    if (elapsed_time % 60 == 0){
      *new_screen = 2;
    }
    if(draw_countdown_screen(elapsed_time, subject, new_screen)) return 1;
    break;

  case GUESSING:
    if(draw_guessing_screen(drawingScreen, mouse_map, img, mouse_x, mouse_y, elapsed_time, string, size_str, c_mouse, c_text, c_timer, new_submit, new_screen, history, history_sizes, history_count)) return 1;
    break;

  case TIME_OVER:
    if(draw_lose_menu(new_screen)) return 1;
    break;

  case WON:
    if(draw_win_menu(new_screen)) return 1;
    break;
  
  default:
    break;
  }
  
  return 0;
}

/**
 * @brief Updates the game state based on input and time.
 *
 * @param elapsed_time current elapsed time.
 * @param mouse_x int of mouse x coordinate.
 * @param mouse_y int of mouse y coordinate.
 * @param try array for user input string.
 * @param str_size pointer to input string size.
 * @param answer array of current correct subject.
 * @param submit bool for checking if user submitted an answer.
 * @param drawingScreen array of pixels representing the drawing screen area.
 * @param new_screen pointer to screen draw countdown.
 * @param history array with the MAX_HISTORY_SIZE previous tries submissions.
 * @param history_count pointer to the variable with the number of strings in history.
 */
void (update_state)(int* elapsed_time, int* mouse_x, int* mouse_y, char try[], int *str_size, char answer[], bool submit, uint32_t drawingScreen[], int* new_screen, char history[MAX_HISTORY_SIZE][3 * MAX_STR_LEN], int* history_count){
  switch (gameState){
  case MAIN_MENU:
    if(mouse_packet.lb && !mouse_packet.mb && !mouse_packet.rb && *mouse_x >= 420 && *mouse_x <= 720 && *mouse_y >= 500 && *mouse_y <= 600){
      gameState = COUNTDOWN;
      *elapsed_time = 0;
      *new_screen = 2;
      
      // Get a new random subject
      memset(answer, 0, 3*MAX_STR_LEN);
      strcpy(answer, get_random_subject());
    }
    break;
    
  case COUNTDOWN:
    if(*elapsed_time >= 5 * 60){
      gameState = GUESSING;
      *elapsed_time = 0;
      *new_screen = 2;

      // reset mouse position
      *mouse_x = 325;
      *mouse_y = 410;

      // reset history
      memset(history, 0, 3 * MAX_STR_LEN * MAX_HISTORY_SIZE);
      *history_count = 0;

      // reset drawing area
      reset_drawing_area(drawingScreen);

      // reset text box
      memset(try, 0, 3*MAX_STR_LEN);
      *str_size = 0;
    }
    break;

  case GUESSING:
    if(*elapsed_time == TIMER_INITIAL * 60){
      gameState = TIME_OVER;
      *elapsed_time = 0;
      *new_screen = 2;
    }
    if(submit && !memcmp(try, answer, 3*MAX_STR_LEN)){
      gameState = WON;
      *elapsed_time = 0;
      *new_screen = 2;
    }
    break;

  case TIME_OVER:
    if(*elapsed_time == 3*60){
      gameState = MAIN_MENU;
      *elapsed_time = 0;
      *new_screen = 2;
    }
    break;

  case WON:
    if(*elapsed_time == 3*60){
      gameState = MAIN_MENU;
      *elapsed_time = 0;
      *new_screen = 2;
    }
    break;
  
  default:
    break;
  }
}


/**
 * @brief Updates the history array with the latest submitted string.
 * 
 * @param history array with the MAX_HISTORY_SIZE previous tries submissions.
 * @param history_sizes array with the sizes of the MAX_HISTORY_SIZE previous tries.
 * @param history_count pointer to the variable with the number of strings in history.
 * @param string string we want to append to the end of history.
 * @param size_str integer size of string.
 */
void (update_history)(char history[MAX_HISTORY_SIZE][3 * MAX_STR_LEN], int history_sizes[MAX_HISTORY_SIZE], int* history_count, char string[], int size_str){

  if (*history_count == MAX_HISTORY_SIZE) {
    for (int i = 0; i < MAX_HISTORY_SIZE - 1; i++) {
      memset(history[i], 0, 3 * MAX_STR_LEN);
      strncpy(history[i], history[i + 1], 3 * MAX_STR_LEN);
      history_sizes[i] = history_sizes[i+1];
    }
    (*history_count) --;
  }

  memset(history[*history_count], 0, 3 * MAX_STR_LEN);
  strncpy(history[*history_count], string, 3 * MAX_STR_LEN);
  history_sizes[*history_count] = size_str;

  (*history_count) ++;
}

/**
 * @brief Main loop for the game, handling input and game states.
 * 
 * @param argc argument count.
 * @param argv argument vector.
 * @return 0 on success or 1 if it fails.
 */
int (proj_main_loop)(int argc, char* argv[]){
  int c_mouse = 0, c_text = 0, c_timer = 0, new_submit = 0, new_screen = 2; //variables to track if certain elements were changed (and how many buffers still have to be changed)
  int counter = 0, mouse_x = 576, mouse_y = 432;

  //variables for the guessing history
  char history[MAX_HISTORY_SIZE][3 * MAX_STR_LEN];
  int history_sizes[MAX_HISTORY_SIZE];
  int history_count = 0;
  
  srand(time(NULL)); // used for random subject selection

  if(vg_init(MODE) == NULL) return 1; //set the graphic mode

  //mouse cursor map
  xpm_map_t m = mouse_xpm;
  xpm_image_t img;
  uint8_t* mouse_map = xpm_load(m, XPM_8_8_8_8, &img);

  load_char_xpms();

  //drawing screen
  uint32_t drawingScreen[600*814];

  //kbc reader
  char string[3 * MAX_STR_LEN] = "\0";
  int size_str = 0;
  bool submit = false;
  char answer[3 * MAX_STR_LEN] = "TABLE\0";
  bool ESC_key = false;
  
  if(img.type == INVALID_XPM) return 1;

  if(reset_drawing_area(drawingScreen)) return 1;
  
  int ipc_stat;
  uint8_t mouse_mask, kb_mask, timer_mask;
  message msg;

  if(keyboard_subscribe_int(&kb_mask)) return 1;
  if(mouse_write_command(ENABLE_DATA_REPORTING)) return 1; //enables data reporting
  if(mouse_subscribe_int(&mouse_mask)) return 1; //subscribe to mouse interruptions
  if(timer_subscribe_int(&timer_mask)) return 1;

  while (!ESC_key){
    if(driver_receive(ANY, &msg, &ipc_stat)) continue; //error on driver_receive, try again

    if(is_ipc_notify(ipc_stat)){ //if it was a notification
      switch(_ENDPOINT_P(msg.m_source)){ //check where the message comes from

        case HARDWARE: //it came from a hardware device
          if(msg.m_notify.interrupts & mouse_mask){ //check if the interrupt was from the mouse
            mouse_ih(); //call the interruption handler
            c_mouse = 2;

            if(byte_index == 3){ //we have read the three bytes of this packet
              byte_index = 0; //reset for next packet

              if(gameState == MAIN_MENU){
                update_state(&counter, &mouse_x, &mouse_y, string, &size_str, answer, submit, drawingScreen, &new_screen, history, &history_count);
                calculate_placement(&mouse_x, &mouse_y, vbe_mode_info.XResolution, vbe_mode_info.YResolution, 0, 0, mouse_packet);
              }
              else {
                if(mouse_event_handler_guessing(img, mouse_packet, &mouse_x, &mouse_y, drawingScreen, &c_mouse)) return 1; 
              }
            }
          }

          if(msg.m_notify.interrupts & kb_mask){ //check if the interrupt was from the keyboard
            kbc_ih1(string, &size_str, &ESC_key, &submit);
            c_text = 2;
          }

          if(msg.m_notify.interrupts & timer_mask){
            counter++;

            if(gameState != MAIN_MENU){
              update_state(&counter, &mouse_x, &mouse_y, string, &size_str, answer, submit, drawingScreen, &new_screen, history, &history_count);
              if (submit){
                update_history(history, history_sizes, &history_count, string, size_str);
                memset(string, 0, sizeof(string));
                size_str = 0;
                new_submit = 2;
              }
            }

            if(counter % 60 == 0){
              c_timer = 2;
            }

            if(counter % (60/FPS) == 0){
              if(update_vram(mouse_map, img, mouse_x, mouse_y, counter, string, size_str, drawingScreen, &c_mouse, &c_text, &c_timer, &new_submit, &new_screen, answer, history, history_sizes, history_count)) return 1;
              if(change_display_buffer()) return 1;
            }

            submit = false;
          }

          break;

        default:
          break; //if the notification is other than a hardware interrupt, do nothing
      }
    }
  }

  if(mouse_write_command(DISABLE_DATA_REPORTING)) return 1; //disable data reporting
  if(mouse_unsubscribe_int()) return 1;
  if(keyboard_unsubscribe_int()) return 1;
  
  if(vg_exit()) return 1;

  
  return 0;
}
