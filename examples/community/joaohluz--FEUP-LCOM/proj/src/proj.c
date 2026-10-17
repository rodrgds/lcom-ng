
// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
// Any header files included below this line should have been created by you
#include "i8254.h"
#include "i8042.h"
#include "vbe_macros.h"
#include "graphics.h"
#include "keyboard.h"
#include "game.h"
#include "mouse.h"
#include "menu.h"
#include "file.h"
#include "RTC.h"



bool explosion= false;

int counterExplosion =0;

uint8_t byte, packet[3];

int byteCounter = 0;

int ship_counterExplosion = 0;

bool ship_explosion= false;

extern game_st game_state;

extern bool is_over;





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

int (interrupt_loop)(Jogo* jogo, Player* player, Alien* alien) {
  int ipc_status;
  message msg;
  unsigned int r;
  uint8_t bit_no_timer, bit_no_kb, bit_no_mouse;
  unsigned counter = 0, shots_fired=0;
  //uint8_t seconds = 0;

  enable_cmd_int();

  if(mouse_subscribe(&bit_no_mouse) != 0)
    printf("Erro na funcao mouse_subscribe\n");

  if(kb_subscribe(&bit_no_kb) != 0)
    printf("Erro na funcao kb_subscribe\n");

  if(timer_subscribe_int(&bit_no_timer) != 0)
    printf("Erro na funcao timer_subscribe_int\n");

  uint32_t irq_set_kb = BIT(bit_no_kb);
  uint32_t irq_set_timer = BIT(bit_no_timer);
  uint32_t irq_set_mouse = BIT(bit_no_mouse);


  drawJogo(jogo,player,alien);

  while(!is_over) {

    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 )
      { 
        printf("driver_receive failed with: %d", r);
        continue;
      } 
    if (is_ipc_notify(ipc_status))
      { /* received notification */
        switch (_ENDPOINT_P(msg.m_source))
        {
          case HARDWARE: /* hardware interrupt notification */      

          if (msg.m_notify.interrupts & irq_set_timer)
            {
              timer_int_handler();
              if (game_state == GAME)
              {
                check_player_fire(jogo,player);
                drawJogo(jogo,player,alien);
                double_buffering();
              }
              else if (game_state == PAUSE)
              {
                drawPause(jogo);
                double_buffering();
              }
              else if (game_state == GAME_OVER)
              {
                drawGameOver(jogo);
                double_buffering();
              }
              else if (game_state == WON)
              {
                drawWon(jogo);
                double_buffering();
                
              }

            }

            if (msg.m_notify.interrupts & irq_set_kb)
              {
                    kbd_read();
                    if (game_state == GAME || game_state == PAUSE)
                    {
                      move_ship(jogo, player);
                    }
              }
            
            if (msg.m_notify.interrupts & irq_set_mouse)
              {
                 if(game_state == GAME)
                 {
                  mouse_ih();
                 }
              }
            
         }
     } else { /* received a standard message, not a notification */
         /* no standard messages expected: do nothing */}

    if (globalCounter % sys_hz() == 0){
      counter++;
     // printf("Score = %d \n", player->score);
      
      
      if(explosion)
        {counterExplosion++;}

      if(ship_explosion)
      {
        ship_counterExplosion++;
      }
    }
    if(counter == 2 && !alien->shot){
      alien->shot = 1;
      alien_shot_init(jogo,alien);
      shots_fired++;
      counter = 0;
    }
    else if(counter == 2){
      counter=0;
      alien->shot = 0;
    }

    if(counterExplosion == 1)
    {

      counterExplosion =0;
      explosion = false;
    }

    if(ship_counterExplosion ==1)
    {
      ship_counterExplosion = 0;
      ship_explosion = false;
    }


      
 }
  if(timer_unsubscribe_int() != 0)
    printf("Erro na funcao timer_unsubscribe_int\n");
  if(kb_unsubscribe() != 0)
    printf("Erro na funcao kb_unsubscribe\n");

  if(mouse_unsubscribe() != 0)
    printf("Erro na funcao mouse_unsubscribe\n");

  disable_cmd_int();

  game_state = MAIN_MENU;
  is_over = false;

  return 0;
}


int menu_interrupt_loop(Jogo* jogo, Mouse* mouse, Users users, char* name, char* date){
  int ipc_status;
  message msg;
  unsigned int r;
  uint8_t bit_no, bit_no_timer, bit_no_mouse;
  int letterCounter = 0;
  int dia, mes, ano;

  if (game_state == NAME)
  {
    dia = get_Day();
    mes = get_Month();
    ano = get_Year();

    dateIntToString(date, dia, mes, ano);
  }

  enable_cmd_int();

  if(mouse_subscribe(&bit_no_mouse) != 0)
    printf("Erro na funcao mouse_subscribe\n");
  if(kb_subscribe(&bit_no) != 0)
    printf("Erro na funcao mouse_subscribe\n");
  if(timer_subscribe_int(&bit_no_timer) != 0)
    printf("Erro na funcao timer_subscribe_int\n");

 uint32_t irq_set_kb = BIT(bit_no);
 uint32_t irq_set_timer = BIT(bit_no_timer);
 uint32_t irq_set_mouse = BIT(bit_no_mouse);

  while(game_state == MAIN_MENU || game_state == INSTRUCTIONS || game_state == NAME || game_state == SCORE)
   {
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 )
      { 
        printf("driver_receive failed with: %d", r);
        continue;
      } 
    if (is_ipc_notify(ipc_status))
      { /* received notification */
        switch (_ENDPOINT_P(msg.m_source))
        {
          case HARDWARE: /* hardware interrupt notification */      

          if (msg.m_notify.interrupts & irq_set_timer)
            {
              timer_int_handler();
              
              if (game_state == MAIN_MENU)
              {
                drawMenu(jogo, mouse);
                double_buffering();
              }
              else if (game_state == INSTRUCTIONS)
              {
                drawInstructions(jogo);
                double_buffering();
              }
              else if (game_state == NAME)
              {
                vg_draw_xpm(jogo->name_pic, &jogo->name_info, 0, 0); // Desenha o background;                
                //printf("X = %d Y = %d \n",mouse->x, mouse->y );
                int n = 0, letterX;
               

                while(n < letterCounter){
                  //printf("n = %d  LC = %d \n", n, letterCounter);
                   switch(n)
                {
                  case 0:
                  letterX = 319;
                  break;
                  case 1:
                  letterX= 472;
                  break;

                  case 2:
                   letterX= 626;
                   break;

                   default:
                    break;
                }
                  show_letter_file(jogo,name[n],letterX, 465);
                  n++;
                }
                double_buffering();
              }
              else if (game_state == SCORE)
              {
                vg_draw_xpm(jogo->highscores_pic, &jogo->highscores_info, 0, 0); // Desenha o background;
                display_score(jogo,users);
                double_buffering();
              }

            } 
            if (msg.m_notify.interrupts & irq_set_kb)
              {
                  kbd_read();
                  if (game_state == NAME && make)
                  {
                    if(byte != 0x1c)
                    {
                      if(byte == 0x0e && letterCounter != 0)
                      {
                        letterCounter-= 2;
                      }
                      show_letter_byte(name,letterCounter);
                      if (letterCounter < 3 && make)
                    {
                      letterCounter++;
                      make = false;
                    }

                  }
                    else
                    {
                      game_state = MAIN_MENU;
                    }
                    
                  }
                  else{
                    menu_kb_ih();
                  }
              }
            if (msg.m_notify.interrupts & irq_set_mouse)//&& game_state == MAIN_MENU
            {
          
                mouse_menu_ih(mouse);
              
            }
         }
     }
 }

 

  if(kb_unsubscribe() != 0)
    printf("Erro na funcao mouse_unsubscribe\n");
  if(timer_unsubscribe_int() != 0)
    printf("Erro na funcao timer_unsubscribe_int\n");
  if(mouse_unsubscribe() != 0)
    printf("Erro na funcao mouse_unsubscribe\n");

  disable_cmd_int();

  return 0;
}



int (proj_main_loop)(int argc, char *argv[]){

  vg_init(0x117);

  Jogo* jogo = (Jogo*) inicio();
  Player* player = (Player*) playerInit(jogo);
  Alien* alien = (Alien*) alienInit(jogo);
  Mouse* mouse = (Mouse*) mouseInit();

  char file[50];

  if (argc != 0)
  {
    strcpy(file,*argv);
  }

  Users users = usersInit();
  read_from_file(users, file);

  char name[3], score[3], date[8];
  

  while(1){
    switch(game_state){
   case NAME:
      reset_mouse(mouse);
      menu_interrupt_loop(jogo,mouse,users,name,date);
      break;
    case MAIN_MENU:
       reset_mouse(mouse);
       menu_interrupt_loop(jogo,mouse,users,name,date);
       break;
    case GAME:
      reset_player(jogo, player);
      reset_alien(jogo, alien, 1,6, 0);
      interrupt_loop(jogo, player, alien);
      game_state = MAIN_MENU;
      break;
    case COMP:
      vg_exit();
      scoreIntToString(score,player->score);
      addUser(users,name,date,score);
      write_to_file(users, file);
      freeUsers(users);
      playerDelete(player);
      fim(jogo);
      return 0;

    default:
      break;
    }
  }

  vg_exit();
  scoreIntToString(score, player->score);
  addUser(users,name,date,score);
  write_to_file(users, file);
  freeUsers(users);
  playerDelete(player);
  fim(jogo);
  return 0;
}
