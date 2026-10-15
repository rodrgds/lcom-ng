/**
 * @file main.c
 * @brief Main file for the project
 */


#include "devices/graphics/graphics.h"
#include "devices/keyboard/keyboard.h"
#include "devices/mouse/mouse.h"
#include "game/classes/enemy.h"
#include "game/classes/mouse_cursor.h"
#include "game/classes/player.h"
#include "game/logic/game_logic.h"
#include "game/menu/menu.h"
#include "game/sprite/sprite.h"
#include "lcom/timer.h"
#include "game/classes/bullet.h"
#include "game/classes/bullet_node.h"



#define PLAYER_LIFE 5
#define PLAYER_DAMAGE 5
#define MONSTER_LIFE 5
#define MONSTER_DAMAGE 5
#define DEFAULT_PLAYER_X 400
#define DEFAULT_PLAYER_Y 571
#define DEFAULT_MOUSE_X 400
#define DEFAULT_MOUSE_Y 350
#define ONE_SEC 30
#define SCAN_CODE_A_PRESSED   0x1E
#define SCAN_CODE_A_RELEASED  0x9E
#define SCAN_CODE_D_PRESSED   0x20
#define SCAN_CODE_D_RELEASED  0xA0
#define SCAN_CODE_W_PRESSED   0x11
#define SCAN_CODE_W_RELEASED  0x91
#define SCAN_CODE_ENTER 0x1C

extern int hook_id_kbd;
extern int hook_id_mouse;
extern int hook_id_timer;
extern int hook_id_rtc;
extern int hook_id_graphics;
extern int counter_timer;
extern uint8_t scancode;
extern uint8_t scancode_mouse;
extern struct packet mouse_packet;
extern int idx;
extern vbe_mode_info_t info;
extern GameState currentState;
extern int renew;
int score=0;
double multiplier = 1.0;


uint8_t kbd_irq_set;
uint8_t mouse_irq_set;
uint8_t timer_irq_set;
uint16_t mode = 0x110;

/**
 * @brief Opens and initializes necessary devices for the game.
 *
 * Initializes the mouse, keyboard, timer, and graphics mode.
 *
 * @return Returns 0 if all devices are successfully opened, 1 otherwise.
 */
int open_devices() {
  if (mouse_write_cmd(MOUSE_ENABLE_STREAM_MODE) != 0)
    return 1;
  if (mouse_write_cmd(MOUSE_ENABLE_DATA_REPORTING) != 0)
    return 1;


  if (timer_set_frequency(0, ONE_SEC) != 0)
    return 1;

  if (keyboard_subscribe_int(&kbd_irq_set) != 0)
    return 1;
  if (mouse_subscribe_int(&mouse_irq_set) != 0)
    return 1;

  
  if (timer_subscribe_int(&timer_irq_set) != 0)
    return 1;

  if (set_buffer(mode) != 0)
    return 1;

  if (allocate_write_buffer() != 0)
    return 1;

  if (set_graphic_mode(mode) != 0)
    return 1;
  return 0;

}

/**
 * @brief Closes and cleans up necessary devices for the game.
 *
 * Frees all sprites, unsubscribes from interrupts, and exits graphics mode.
 *
 * @return Returns 0 if all devices are successfully closed, 1 otherwise.
 */
int close_devices() {
  freeAllSprites();
  if (keyboard_unsubscribe_int() != 0)
    return 1;
  if (mouse_unsubscribe_int() != 0)
    return 1;
  if (mouse_write_cmd(MOUSE_DISABLE_DATA_REPORTING) != 0)
    return 1;
  if (timer_unsubscribe_int() != 0)
    return 1;

  /*
  if(rtc_unsubscribe_int() != 0)
      return;
  */
  if (vg_exit() != 0)
    return 1;

  return 0;

}

/**
 * @brief Changes the graphics mode.
 *
 * Exits the current graphics mode, sets up the buffer and new graphics mode, and reloads sprites.
 *
 * @param new_mode The new graphics mode to be set.
 *
 * @return Returns 0 if the mode is successfully changed, 1 otherwise.
 */
int change_mode(uint16_t new_mode){
  mode=new_mode;
  
  if (vg_exit() != 0)
    return 1;
  
  
  if (set_buffer(mode) != 0)
    return 1;

  if (allocate_write_buffer() != 0)
    return 1;

  if (set_graphic_mode(mode) != 0)
    return 1;
  freeInitialSprites();
  loadAllSprites(mode);
  
  return 0;
  
}

/**
 * @brief Main loop of the project.
 *
 * This function represents the main loop of the project. It initializes necessary components, such as devices and sprites,
 * and enters a loop where it continuously processes hardware interrupts and updates the game state accordingly.
 * 
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * 
 * @return Returns 0 upon successful execution, non-zero otherwise.
 */
int(proj_main_loop)(int argc, char *argv[]) {

  if(open_devices()!=0)
    return 1;
  loadInitialSprites();
  int ipc_status;
  message msg;
  MouseCursor *mouse;
  mouse = createMouseCursor(DEFAULT_MOUSE_X*info.XResolution/DEFAULT_X_RESOLUTION_14C, DEFAULT_MOUSE_Y*info.YResolution/DEFAULT_Y_RESOLUTION_14C, mouse_cursor);
  player *player;
  bullet_node *bullets = NULL;
  bool flag=true;
 while (flag) {
    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      printf("driver_receive failed");
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & timer_irq_set) {
            timer_int_handler();
            if(currentState==RESOLUTION){
              drawResolution();
              
            }
            draw_sprite(mouse->sprite, mouse->x, mouse->y);
            switch_buffers();
          }
          if (msg.m_notify.interrupts & kbd_irq_set) {
            kbc_ih();
          }
          if (msg.m_notify.interrupts & mouse_irq_set) {
            mouse_ih();
            mouse_bytes_sync();

            if (idx == 3) {
              idx = 0;
              mouse_generate_packet();
              if (!((mouse_packet.delta_x + mouse->x) <= 0 || (mouse_packet.delta_x + mouse->x + 7) >= (info.XResolution) || (-mouse_packet.delta_y + mouse->y) <= 0 || (-mouse_packet.delta_y + mouse->y) > (info.YResolution))) {
                mouse->x += mouse_packet.delta_x;
                mouse->y -= mouse_packet.delta_y;
              }
            if(currentState==RESOLUTION){
                  if(res14C(mouse->x, mouse->y)==0){
                    change_mode(0x14C);
                    player = createPlayer(PLAYER_LIFE,PLAYER_DAMAGE, DEFAULT_PLAYER_X*info.XResolution/DEFAULT_X_RESOLUTION_14C, DEFAULT_PLAYER_Y*info.YResolution/DEFAULT_Y_RESOLUTION_14C, cuphead1);
                    mouse = createMouseCursor(DEFAULT_MOUSE_X*info.XResolution/DEFAULT_X_RESOLUTION_14C, DEFAULT_MOUSE_Y*info.YResolution/DEFAULT_Y_RESOLUTION_14C, mouse_cursor);
                    extern enemy monsters[10];
                    int v = 10;
                    for (int i = 0; i < 10; i++) {
                      monsters[i] = *createEnemy(MONSTER_LIFE,MONSTER_DAMAGE, 5 + i + v, 5 + i + v, 3, 3, monster1, false);
                    }

                    flag=false;
                  }
                  else if(res115(mouse->x, mouse->y)==0){

                    change_mode(0x115);
                    player = createPlayer(PLAYER_LIFE,PLAYER_DAMAGE, DEFAULT_PLAYER_X*info.XResolution/DEFAULT_X_RESOLUTION_14C, DEFAULT_PLAYER_Y*info.YResolution/DEFAULT_Y_RESOLUTION_14C, cuphead1);
                    mouse = createMouseCursor(DEFAULT_MOUSE_X*info.XResolution/DEFAULT_X_RESOLUTION_14C, DEFAULT_MOUSE_Y*info.YResolution/DEFAULT_Y_RESOLUTION_14C, mouse_cursor);
                    extern enemy monsters[10];
                    int v = 10;
                    for (int i = 0; i < 10; i++) {
                      monsters[i] = *createEnemy(MONSTER_LIFE,MONSTER_DAMAGE, 5 + i + v, 5 + i + v, 3, 3, monster1, false);
                    }
                    flag=false;
                  }
                  else if(res110(mouse->x, mouse->y)==0){
                    change_mode(0x110);
                    
                    player = createPlayer(PLAYER_LIFE,PLAYER_DAMAGE, DEFAULT_PLAYER_X*info.XResolution/DEFAULT_X_RESOLUTION_14C, DEFAULT_PLAYER_Y*info.YResolution/DEFAULT_Y_RESOLUTION_14C, cuphead1);
                    
                    mouse = createMouseCursor(DEFAULT_MOUSE_X*info.XResolution/DEFAULT_X_RESOLUTION_14C, DEFAULT_MOUSE_Y*info.YResolution/DEFAULT_Y_RESOLUTION_14C, mouse_cursor);
                    extern enemy monsters[10];
                    int v = 10;
                    for (int i = 0; i < 10; i++) {
                      monsters[i] = *createEnemy(MONSTER_LIFE,MONSTER_DAMAGE, 5 + i + v, 5 + i + v, 3, 3, monster1, false);
                    }
                    flag=false;
                  }
                  else if(res11A(mouse->x, mouse->y)==0){
                    change_mode(0x11A);
                    player = createPlayer(PLAYER_LIFE,PLAYER_DAMAGE, DEFAULT_PLAYER_X*info.XResolution/DEFAULT_X_RESOLUTION_14C, DEFAULT_PLAYER_Y*info.YResolution/DEFAULT_Y_RESOLUTION_14C, cuphead1);
                    mouse = createMouseCursor(DEFAULT_MOUSE_X*info.XResolution/DEFAULT_X_RESOLUTION_14C, DEFAULT_MOUSE_Y*info.YResolution/DEFAULT_Y_RESOLUTION_14C, mouse_cursor);
                    extern enemy monsters[10];
                    int v = 10;
                    for (int i = 0; i < 10; i++) {
                      monsters[i] = *createEnemy(MONSTER_LIFE,MONSTER_DAMAGE, 5 + i + v, 5 + i + v, 3, 3, monster1, false);
                    }
                    flag=false;
                  }

                }
              }
            }
              
      }
    }
 }




  draw_sprite(instructions, 0, 0);
  switch_buffers();
  sleep(3);


  int8_t speed_x = 0;
  int8_t speed_y = 0;
  extern enemy monsters_fly[2];
  int bullet_cooldown=0;
  bool key_a_pressed = false;
  bool key_d_pressed = false;
  bool key_w_pressed = false;
  bool create_enemy = false;
  int unvulnerability = 0;
  while (scancode != ESC_BREAKCODE) {
    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      printf("driver_receive failed");
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & timer_irq_set) {
            timer_int_handler();
            
            if (currentState == MENU) {
              
              drawMenu();

            }
            else if (currentState == GAME) {

              unvulnerability++;
              bullet_cooldown++;
              

              drawGame(player, score);

              update_bullet_logic(&bullets);


              update_player_logic(player, mouse, key_a_pressed, key_d_pressed, key_w_pressed, &speed_x, &speed_y, &unvulnerability);
              
              if (counter_timer % ONE_SEC*2 == 0) {
                create_enemy = true;
              }
              if (counter_timer % ONE_SEC == 0) {
                renew++;
              }
              if(renew>=5){
                spawn_dead_enemies();
                renew=0;
              }
              if (currentState == GAME) {
                if (counter_timer % ONE_SEC*2 == 0) {
                  multiplier+=0.1;
                }
                if(counter_timer%20 == 0) {
                  score+=10*multiplier;
                }
              }
              
              update_enemy_logic(mouse, create_enemy,player);
              create_enemy = false;
            }
            else if (currentState == LEADERBOARD) {
              drawLeaderBoard();
            }
            else if (currentState == SCOREBOARD) {
              drawScoreBoard(score);
            }
            
            else if (currentState == EXIT) {
              if (close_devices() != 0)
                return 1;
              return 0;
            }
            draw_sprite(mouse->sprite, mouse->x, mouse->y);

            switch_buffers();
            
          }
          
          if (msg.m_notify.interrupts & kbd_irq_set) {
            kbc_ih();
            if (currentState == SCOREBOARD) {
              processScanCode(scancode);
            }
            if (scancode == SCAN_CODE_A_PRESSED) {
              key_a_pressed = true;
            }
            else if (scancode == SCAN_CODE_A_RELEASED) {
              key_a_pressed = false;
            }
            else if (scancode == SCAN_CODE_D_PRESSED) {
              key_d_pressed = true;
            }
            else if (scancode == SCAN_CODE_D_RELEASED) {
              key_d_pressed = false;
            }
            else if (scancode == SCAN_CODE_W_PRESSED) {
              key_w_pressed = true;
            }
            else if (scancode == SCAN_CODE_W_RELEASED) {
              key_w_pressed = false;
            }
          }

          if (msg.m_notify.interrupts & mouse_irq_set) {
            mouse_ih();
            mouse_bytes_sync();

            if (idx == 3) {
              idx = 0;
              mouse_generate_packet();
              if (!((mouse_packet.delta_x + mouse->x) <= 0 || (mouse_packet.delta_x + mouse->x + 7) >= (info.XResolution) || (-mouse_packet.delta_y + mouse->y) <= 0 || (-mouse_packet.delta_y + mouse->y) > (info.YResolution))) {
                mouse->x += mouse_packet.delta_x;
                mouse->y -= mouse_packet.delta_y;
              }
              if (currentState == MENU) {
                playButton(mouse->x, mouse->y);
                leaderboardButton(mouse->x, mouse->y);
                exitButton(mouse->x, mouse->y);
              }
              if (currentState == LEADERBOARD) {
                menuButtonLeader(mouse->x, mouse->y);
                
              }
              if (currentState == SCOREBOARD) {
                menuButton(mouse->x, mouse->y, &score, &multiplier);
                
              }
              else if(currentState == GAME){
                if (mouse_packet.lb) {
                  if(bullet_cooldown > 30){
                    bullet_cooldown = 0;
                
                    bullet *new_shot = createBullet(player->x+75, player->y+50, mouse->x, mouse->y, 3, bala);
                    addBullet(&bullets, new_shot);
                  }
                }
              }

            }
          }
          break;

        default:
          break;
      }
    }
  }

  if (close_devices() != 0)
    return 1;
  return 0;
}


/**
 * @brief Main function of the project.
 *
 * Sets up language and logging, then hands control to LCF.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 *
 * @return Returns 0 upon successful execution, non-zero otherwise.
 */
int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  //lcf_trace_calls("/home/lcom/labs/proj/src/trace.txt");
  //lcf_log_output("/home/lcom/labs/proj/src/output.txt");
  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}
