// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

// Any header files included below this line should have been created by you
#include "kbd.h"
#include "i8254.h"
#include "video.h"
#include "mouse.h"
#include "Entity.h"
#include "Player.h"
#include "GameState.h"
#include "Game.h"
#include "assets/assets.h"
#include "assets/background.h"
#include "Score.h"
#include "rtc.h"

extern uint8_t scancode[2];
extern vbe_mode_info_t vmi_p;
extern unsigned h_res;
extern unsigned v_res;
extern int counter;
extern uint8_t byte_no;
extern struct packet p;
extern bool pintou;
uint8_t kbd_irq_set;
uint8_t mouse_irq_set;
uint8_t timer_irq_set;
int rtc_irq_set;


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

/*PARA USAR XPM NOSSAS MUDAR O STATIC CHAR * PARA STATIC CONST CHAR **/

int (proj_main_loop)(int UNUSED(argc) , char * UNUSED(argv[])) {

  int ipc_status;
  message msg;
  int r;
  uint32_t trash;
  xpm_image_t x;
  xpm_load(background, XPM_8_8_8_8, &x);
  if(set_mode(0x14C) == 1) printf("ERRO");
  vg_init(0x14C);
  srand(time(NULL));
  Game g = game();
  Date date = NewDate();


  if ((r = kbd_subscribe_int(&kbd_irq_set)) != OK){
    printf("Error subscribing to kbd interrupts %d", r);
    return r;
  }
  if ((r = set_stream()) != OK) return r;

  if ((r = mouse_subscribe_int(&mouse_irq_set)) != OK){
    printf("Error subscribing to mouse interrupts %d", r);
    return r;
  }

  if((r = timer_subscribe_int(&timer_irq_set)) != OK){
    printf("Error subscribing to timer interrupts %d", r);
    return r;
  }

  if((r = rtc_subscribe_int(&rtc_irq_set)) != OK){
    printf("Error subscribing to rtc interrupts %d", r);
    return r;
  }
  rtc_enable();

  read_OB(&trash);
  readHighScoresFromFile();
  while(g.state != EXIT){
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
        printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) {
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: // hardware interrupt notification

                if(msg.m_notify.interrupts & kbd_irq_set) reactToKBDEvent(&g);

                if(msg.m_notify.interrupts & mouse_irq_set) reactToMouseEvent(&g);

                if(msg.m_notify.interrupts & timer_irq_set) reactToTimerEvent(&g);

                if(msg.m_notify.interrupts & rtc_irq_set) rtc_update(&date);

                break;
            default:
                break;
        }

        if (g.state == PLAYING){
          switch(g.gamestate.phase){
            case 1:
              if(g.gamestate.enemies == NULL) generateEnemies(&g.gamestate);
              break;
            case 2:
              if(g.gamestate.enemies == NULL) generateEnemies(&g.gamestate);
              break;
            case 3:
              if(g.gamestate.obstacles == NULL) generateAsteroids(&g.gamestate);
              if(g.gamestate.enemies == NULL) generateEnemies(&g.gamestate);
              break;
            case 4:
              if(g.gamestate.obstacles == NULL) generateAsteroids(&g.gamestate);
              if(g.gamestate.enemies == NULL) generateBoss(&g.gamestate);
              break;
            default:
              break;
            }
          dmgPlayer(&g.gamestate);
          if(!alive(&g.gamestate.player)){
             g.state = GAMEOVER;
             g.fs = score(g.gamestate.score);
             g.fs.data = date;
             if(newHS(&g.fs)) g.state = NEWHIGHSCORE;
          }
        }
    }
  }
  writeHighScoresToFile();
  read_OB(&trash);
  timer_unsubscribe_int();
  mouse_unsubscribe_int();
  kbd_unsubscribe_int();
  rtc_disable();
  rtc_unsubscribe_int();
  mouse_cmd(DIS_DATA_R);
  vg_exit();
  return 1;
}
