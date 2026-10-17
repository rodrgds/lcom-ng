

#include <lcom/lcf.h>

#include <stdio.h> 
#include <stdbool.h> 
#include <stdlib.h> 
#include <time.h> 

#include "libs/kbc/include/kbc.h"
#include "libs/kbc/include/i8042.h"
#include "libs/timer/include/i8254.h"
#include "libs/utils/include/utils.h"  
#include "libs/timer/include/timer.h" 
#include "libs/kbc/include/mouse.h" 
#include "libs/kbc/include/event_handler.h"
#include "libs/vbe/include/vbe_macros.h"
#include "libs/vbe/include/vbe_functions.h"
#include "xpm/numbers.h"
#include "xpm/highlighted_numbers.h"
#include "game/include/grid.h"
#include "xpm/menu.h"
#include "xpm/difficulties.h"
#include "xpm/game_elements.h"
#include <string.h>
#include <math.h>
extern uint32_t int_counter;
extern int time_count;

int game_diff = 0;          /* Variable used to set game difficulty */
int time_count = 0;         /* Variable used to set initial timer value */


int gridt[N][N];
int solution[N][N];
int grid[N][N] = {{0, 0, 0, 0, 0, 0, 0, 0, 0}, 
                 {0, 0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0, 0}};

int x_index = 0;
int y_index = 0;

int (randint)(int min, int max){
    return (rand() % (max - min + 1)) + min;
}

bool (SolveSudoku)(int grid[N][N], int ie, int je, int excep){ 
    
  int row, col;
  int i, j, t[9];
    
  t[0] = randint(1,9);
  i = 1;
  
  while (i < 9){
  
    t[i] = randint(1, 9);
  
    for (j = 0; j < i; j++)
  
      if (t[i] == t[j])
        i--;
  
      i++;
  }
    
  // If there is no unassigned location, we are done 
  if (!FindUnassignedLocation(grid, &row, &col)) 
    return true; // success! 
  
  int num;
  // consider digits 1 to 9 
  for (i = 0; i < 9; i++){ 
        
    num = t[i];
        
    // if looks promising 
    if (isSafe(grid, row, col, num, ie, je, excep)){ 
            
      // make tentative assignment 
      grid[row][col] = num; 
  
      // return, if success, yay! 
      if (SolveSudoku(grid, ie, je, excep)) 
        return true; 
  
      // failure, unmake & try again 
      grid[row][col] = UNASSIGNED; 
    } 
  }

  return false; // this triggers backtracking 
} 
  
bool (FindUnassignedLocation)(int grid[N][N], int *row, int *col){ 
    
  for (*row = 0; *row < N; (*row)++) 
      
    for (*col = 0; *col < N; (*col)++) 
        
      if (grid[*row][*col] == UNASSIGNED) 
        return true;

  return false; 
} 
  
bool (UsedInRow)(int grid[N][N], int row, int num){

  for (int col = 0; col < N; col++) 
      
    if (grid[row][col] == num) 
      return true; 

  return false; 
} 
  
bool (UsedInCol)(int grid[N][N], int col, int num){ 
  
  for (int row = 0; row < N; row++) 
    
    if (grid[row][col] == num) 
      return true; 

  return false; 
} 

bool (UsedinMDiag)(int grid[N][N], int row, int col, int num) 
{ 
  if(row == col)
  
    for (int row = 0; row < N; row++) 
    
      if (grid[row][row] == num) 
        return true; 

  return false; 
}  

bool (UsedinSDiag)(int grid[N][N], int row, int col, int num){

  if(row + col == 8)
    
    for (int row = 0; row < N; row++) 
    
      if (grid[row][8 - row] == num) 
        return true; 

  return false; 
}

bool (UsedInBox)(int grid[N][N], int boxStartRow, int boxStartCol, int num){ 

  for (int row = 0; row < 3; row++)

    for (int col = 0; col < 3; col++) 

      if (grid[row+boxStartRow][col+boxStartCol] == num) 
        return true; 

  return false; 
} 
  
bool (isSafe)(int grid[N][N], int row, int col, int num, int ie, int je, int excep){

  /* Check if 'num' is not already placed in current row, 
    current column and current 3x3 box */
  return !UsedInRow(grid, row, num) && 
          !UsedInCol(grid, col, num) &&
          !UsedinMDiag(grid, row, col, num) && 
          !UsedinSDiag(grid, row, col, num) &&
          !UsedInBox(grid, row - row%3 , col - col%3, num)&&
          !(row==ie && col==je && num==excep)&&
          grid[row][col]==UNASSIGNED; 
} 
  
void (printGrid)(int grid[N][N]){ 

  for (int row = 0; row < N; row++){
       
    for (int col = 0; col < N; col++) 
      printf("%2d", grid[row][col]); 
    
    printf("\n"); 
  } 
} 
  
/* Driver Program to test above functions */
void sudoku_generator(int ncasas){
  
  int i, j, cont, excep;
  srand(time(NULL));

  // 0 means unassigned cells 
  /*int grid[N][N] = {{4, 0, 0, 0, 7, 0, 0, 0, 0}, 
                      {0, 2, 0, 0, 9, 4, 0, 0, 0}, 
                      {6, 0, 0, 2, 0, 8, 4, 0, 0}, 
                      {0, 0, 0, 0, 0, 0, 0, 5, 1}, 
                      {0, 0, 7, 0, 1, 0, 3, 0, 0}, 
                      {0, 0, 0, 5, 0, 7, 0, 0, 0}, 
                      {0, 0, 0, 1, 4, 0, 0, 0, 3}, 
                      {0, 0, 0, 8, 0, 0, 0, 0, 0}, 
                      {0, 3, 4, 0, 0, 0, 0, 0, 5}}; */
    
  if (SolveSudoku(grid, -1, -1, -1) == true){ 
    
    for (i = 0; i < 9; i++)
      
      for (j = 0; j < 9; j++)
        solution[i][j] = grid[i][j];

    //printGrid(grid);

  }
          
  else{
    //printf("No solution exists\n");
  }
  cont = 1;    
  
  while (cont <= 81 - ncasas) {
        
    for (i = 0; i < 9; i++)
      
      for (j = 0; j < 9; j++)
        gridt[i][j] = grid[i][j];
        
        
    // gera aleatoriamente uma posição a retirar (ie, a colocar a zero)
    i = randint(0, 8);
    j = randint(0, 8);
    
    if (grid[i][j] != 0){
      
      excep=grid[i][j];
      gridt[i][j] = 0;
       
      //printf("\ni:%d j:%d grid:%d\n", i, j, grid[i][j]);

      if (SolveSudoku(gridt, i, j, excep) != true){

        //printf("numero pode ser retirado. cont=%d\n", cont);
        
        cont++;
        grid[i][j] = 0;
      } 
      
      else{
        //printf("mais do que uma solução\n");
      }
    }    
  }
  
  //printf("\npuzzle proposto:\n");
  //printGrid(grid); 
  
     /* Inicializa o gerador de números aleatórios */
 /*   srand(time(NULL));
    printf("\n");
    geratab (N,50,grid);
    printGrid(grid);
    return 0; */

} 

int mainmenu(){
  
  //int m1;
  //int m2;
  //int s1;
  //int s2;

  int menu_pos = 0;

  int cur_x=200;
  int cur_y=200;
  
  //inicia placa grafica

  if (get_mode_info(DIRC_1152x864))
    if (vg_exit())
      
      return 1;

  if (map_vram())
    return 1;

  if (set_graphics_mode(DIRC_1152x864)){
    
    vg_exit();

    if (lm_free(&mem_map))
      printf("failed to free memory\n");

    return 1;

  }

  //desenha menu inicial

  //inicia jogo

  init_menu();

  int ipc_status, r;
  message msg;
  
  //timer
  uint8_t timer_id = TIMER0_IRQ;
  int_counter = 0;

  int hook_id_t = 0;
  
  //keyboard
  uint8_t kb_id = KBC_IRQ;
  uint8_t kb_scancode[2];
  int hook_id_k = 0;
  uint8_t code_size = 1;
  //TO DO

  //mouse
  uint8_t mouse_id = MOUSE_IRQ;
  struct packet pkt;
  uint8_t counter = 1;
  uint8_t scancode[3];
  int hook_id_m = 0;

  
  //Timer
  if(timer_subscribe_int(&timer_id))
    return 1;
  hook_id_t = hook_id;

  //Keyboard
  if(kbc_subscribe_int(&kb_id))
    return 1;
  hook_id_k = hook_id;
  //TO DO


  //Mouse
  if (mouse_subscribe_int(&mouse_id))
    return 1;
  if (sys_irqdisable(&hook_id))
    return 1;
  if (mouse_set_data_report(1))
    return 1;
  if (sys_irqenable(&hook_id))
    return 1;
  hook_id_m = hook_id;

  
  int timer_irq = BIT(timer_id);
  int kb_irq = BIT(kb_id);
  int mouse_irq = BIT(mouse_id);

  int good = 1;
  
  while (good) { 
 
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) { 
      
      switch (_ENDPOINT_P(msg.m_source)) {
        
        case HARDWARE: 

          if (msg.m_notify.interrupts & timer_irq) { 
            
            timer_int_handler();
            if (int_counter % 2 == 0){
              buff3_to_2();
              draw_cursor(CURSOR, cur_x, cur_y);
            }
          }

          if (msg.m_notify.interrupts & mouse_irq) { 
            
            mouse_ih();
            
            if ((counter == 1) && (code & BIT1_IDENTFIER)){
              
              scancode[0] = code;
              counter++;
            }
            
            else if (counter == 2){
              
              scancode[1] = code;
              counter++;
            }

            else if (counter == 3){
              
              scancode[2] = code;
              counter++;
            }
            
            if (counter > 3){
              
              mouse_packet_builder(&pkt, scancode);
              mouse_print_packet(&pkt);
              counter = 1;          
            }
            
            //int px;
            //int py;
            
            if(get_event(&pkt, &ev))
              return 1;
            
  ////////////////////          
            if(ev.type == LB_PRESS){
              if(get_mouse_option_initmenu(cur_x,cur_y,&menu_pos)==0) {
                switch (menu_pos){

                  case 0:
                    draw_xpm(T_ATK, 426, 550);
                    draw_xpm(EXT, 802, 700);
                    draw_xpm(PRAC_HIGH, 50, 400);
                    good=0;
                    break;

                  case 1:
                    draw_xpm(PRAC, 50, 400);
                    draw_xpm(EXT, 802, 700);
                    draw_xpm(T_ATK_HIGH, 426, 550);
                    good=0;
                    break;

                  case 2:
                    draw_xpm(PRAC, 50, 400);
                    draw_xpm(T_ATK, 426, 550);
                    draw_xpm(EXT_HIGH, 802, 700);
                    good=0;
                    break;
                    
                  default:
                    break;

              //principal 
                }
              }
            }
/////////////////////// 
            if(ev.type == MOVE){
             
              cur_x = cur_x + ev.x_delta;
              cur_y = cur_y - ev.y_delta;
              
              if (cur_x < 0)
                
                cur_x = 0;
              
              else if (cur_x > 1127)
              //cur_x > 1135
                cur_x = 1127;
              
              if (cur_y < 0)
                
                cur_y = 0;
              
              else if (cur_y > 824)
                
                cur_y = 824;
              
              buff2_to_1();

            }


          }//FIM SECCAO MOUSE

          if (msg.m_notify.interrupts & kb_irq) { 
            
            kbc_int_handler();

            if (code == TWO_BYTE_CODE){
              
              kb_scancode[0] = code;
              code_size = 2;

            }

            else if (code_size == 2){
              
              kb_scancode[1] = code;

              // MOVEMENT HANDLER------------------------------------------------------------ 
              if ((menu_movement(&menu_pos, code)) == 0){
                
                switch (menu_pos){

                  case 0:
                    draw_xpm(T_ATK, 426, 550);
                    draw_xpm(EXT, 802, 700);
                    draw_xpm(PRAC_HIGH, 50, 400);
                    break;

                  case 1:
                    draw_xpm(PRAC, 50, 400);
                    draw_xpm(EXT, 802, 700);
                    draw_xpm(T_ATK_HIGH, 426, 550);
                    break;

                  case 2:
                    draw_xpm(PRAC, 50, 400);
                    draw_xpm(T_ATK, 426, 550);
                    draw_xpm(EXT_HIGH, 802, 700);
                    break;
                    
                  default:
                    break;
                }

                code_size = 1;
                
              }
              //------------------------------------------------------------------------------
            }

            else {

              kb_scancode[0] = code;
              code_size = 1;

              if (code == ESC_BRK_CODE){
                
                menu_pos = 2;
                draw_xpm(PRAC, 50, 400);
                draw_xpm(T_ATK, 426, 550);
                draw_xpm(EXT_HIGH, 802, 700);
                  
              }

              if (code == ENTER){
                
                good = 0;

              }

            }
          }
// fim interrupt kbc
          break;
         
        default:
          break;

      } 
    }
    else {      
    }
  }

  //Timer
  hook_id = hook_id_t;
  
  if(timer_unsubscribe_int())
    return 1;

  //Keyboard
  hook_id = hook_id_k;

  if (kbc_unsubscribe_int())
    return 1;
 
  //Mouse
  hook_id = hook_id_m;

  if (sys_irqdisable(&hook_id))
    return 1;

  if (mouse_set_data_report(0))
    return 1;

  if (sys_irqenable(&hook_id))
    return 1;

  if (kbc_unsubscribe_int())
    return 1;

  if(menu_pos == 2){
    
    if (vg_exit()){

        if (lm_free(&mem_map))
          printf("failed to free memory\n");

        return 1;
      }

    if (lm_free(&mem_map))
      printf("failed to free memory\n");
    
  }

  // DEBUX AUX-----------------------------------------------------
  //if (!timer_mode)
    //printf("%2d : %2d : %2d\n", time_count/3600, (time_count % 3600) / 60, time_count % 60);
  //---------------------------------------------------------------
  return menu_pos;
}

int diffmenu(){
  
  //int m1;
  //int m2;
  //int s1;
  //int s2;

  int diff_pos = 0;

  int cur_x=200;
  int cur_y=200;

  //desenha menu inicial

  //inicia jogo

  init_diff_menu();
  
  int ipc_status, r;
  message msg;
  
  //timer
  uint8_t timer_id = TIMER0_IRQ;
  int_counter = 0;

  int hook_id_t = 0;
  
  //keyboard
  uint8_t kb_id = KBC_IRQ;
  uint8_t kb_scancode[2];
  int hook_id_k = 0;
  uint8_t code_size = 1;
  //TO DO

  //mouse
  uint8_t mouse_id = MOUSE_IRQ;
  struct packet pkt;
  uint8_t counter = 1;
  uint8_t scancode[3];
  int hook_id_m = 0;

  
  //Timer
  if(timer_subscribe_int(&timer_id))
    return 1;
  hook_id_t = hook_id;

  //Keyboard
  if(kbc_subscribe_int(&kb_id))
    return 1;
  hook_id_k = hook_id;
  //TO DO


  //Mouse
  if (mouse_subscribe_int(&mouse_id))
    return 1;
  if (sys_irqdisable(&hook_id))
    return 1;
  if (mouse_set_data_report(1))
    return 1;
  if (sys_irqenable(&hook_id))
    return 1;
  hook_id_m = hook_id;

  
  int timer_irq = BIT(timer_id);
  int kb_irq = BIT(kb_id);
  int mouse_irq = BIT(mouse_id);

  int good = 1;
  
  while (good) { 
 
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) { 
      
      switch (_ENDPOINT_P(msg.m_source)) {
        
        case HARDWARE: 

          if (msg.m_notify.interrupts & timer_irq) { 
            
            timer_int_handler();
            if (int_counter % 2 == 0){
              buff3_to_2();
              draw_cursor(CURSOR, cur_x, cur_y);
            }

          }

          if (msg.m_notify.interrupts & mouse_irq) { 
            
            mouse_ih();
            
            if ((counter == 1) && (code & BIT1_IDENTFIER)){
              
              scancode[0] = code;
              counter++;
            }
            
            else if (counter == 2){
              
              scancode[1] = code;
              counter++;
            }

            else if (counter == 3){
              
              scancode[2] = code;
              counter++;
            }
            
            if (counter > 3){
              
              mouse_packet_builder(&pkt, scancode);
              mouse_print_packet(&pkt);
              counter = 1;          
            }
            
            //int px;
            //int py;
      
            if(get_event(&pkt, &ev))
              return 1;
           
              //////////////////
            if(ev.type == LB_PRESS){
              if(get_mouse_option_diffmenu(cur_x,cur_y,&diff_pos)==0) {
                switch (diff_pos){
                  case 0:
                    draw_xpm(ZEN_DIF_HIGH, 26, 32);
                    draw_xpm(CASUAL_DIF, 226, 182);
                    draw_xpm(SKILLED_DIF, 426, 382);
                    draw_xpm(MANIAC_DIF, 626, 582);
                    draw_xpm(EXT, 826, 732);
                    good=0;
                    break;

                  case 1:
                    draw_xpm(ZEN_DIF, 26, 32);
                    draw_xpm(CASUAL_DIF_HIGH, 226, 182);
                    draw_xpm(SKILLED_DIF, 426, 382);
                    draw_xpm(MANIAC_DIF, 626, 582);
                    draw_xpm(EXT, 826, 732);
                    good=0;
                    break;

                  case 2:
                    draw_xpm(ZEN_DIF, 26, 32);
                    draw_xpm(CASUAL_DIF, 226, 182);
                    draw_xpm(SKILLED_DIF_HIGH, 426, 382);
                    draw_xpm(MANIAC_DIF, 626, 582);
                    draw_xpm(EXT, 826, 732);
                    good=0;
                    break;

                  case 3:
                    draw_xpm(ZEN_DIF, 26, 32);
                    draw_xpm(CASUAL_DIF, 226, 182);
                    draw_xpm(SKILLED_DIF, 426, 382);
                    draw_xpm(MANIAC_DIF_HIGH, 626, 582);
                    draw_xpm(EXT, 826, 732);
                    good=0;
                    break;

                  case 4:
                    draw_xpm(ZEN_DIF, 26, 32);
                    draw_xpm(CASUAL_DIF, 226, 182);
                    draw_xpm(SKILLED_DIF, 426, 382);
                    draw_xpm(MANIAC_DIF, 626, 582);
                    draw_xpm(EXT_HIGH, 826, 732);
                    good=0;
                    break;
                    
                  default:
                    break;


                
    ///////////////// 
              //principal 
                }

                switch (diff_pos){

                  case 0:
                    game_diff = ZEN;
                    break;

                  case 1:
                    game_diff = CASUAL;
                    break;

                  case 2:
                    game_diff = SKILLED;
                    break;

                  case 3:
                    game_diff = MANIAC;
                    break;

                  case 4:
                    game_diff = 0;
                    break;
                    
                  default:
                    break;
                }
                good=0;
              }
                
            }

            if(ev.type == MOVE){
              
              cur_x = cur_x + ev.x_delta;
              cur_y = cur_y - ev.y_delta;
              
              if (cur_x < 0)
                
                cur_x = 0;
              
              else if (cur_x > 1127)
              //cur_x > 1135
                cur_x = 1127;
              
              if (cur_y < 0)
                
                cur_y = 0;
              
              else if (cur_y > 824)
                
                cur_y = 824;
              
              buff2_to_1();
            }


          }//FIM SECCAO MOUSE

          if (msg.m_notify.interrupts & kb_irq) { 
            
            kbc_int_handler();

            if (code == TWO_BYTE_CODE){
              
              kb_scancode[0] = code;
              code_size = 2;

            }

            else if (code_size == 2){
              
              kb_scancode[1] = code;

              // MOVEMENT HANDLER------------------------------------------------------------ 
              if ((diff_movement(&diff_pos, code)) == 0){
                
                switch (diff_pos){

                  case 0:
                    draw_xpm(ZEN_DIF_HIGH, 26, 32);
                    draw_xpm(CASUAL_DIF, 226, 182);
                    draw_xpm(SKILLED_DIF, 426, 382);
                    draw_xpm(MANIAC_DIF, 626, 582);
                    draw_xpm(EXT, 826, 732);
                    break;

                  case 1:
                    draw_xpm(ZEN_DIF, 26, 32);
                    draw_xpm(CASUAL_DIF_HIGH, 226, 182);
                    draw_xpm(SKILLED_DIF, 426, 382);
                    draw_xpm(MANIAC_DIF, 626, 582);
                    draw_xpm(EXT, 826, 732);
                    break;

                  case 2:
                    draw_xpm(ZEN_DIF, 26, 32);
                    draw_xpm(CASUAL_DIF, 226, 182);
                    draw_xpm(SKILLED_DIF_HIGH, 426, 382);
                    draw_xpm(MANIAC_DIF, 626, 582);
                    draw_xpm(EXT, 826, 732);
                    break;

                  case 3:
                    draw_xpm(ZEN_DIF, 26, 32);
                    draw_xpm(CASUAL_DIF, 226, 182);
                    draw_xpm(SKILLED_DIF, 426, 382);
                    draw_xpm(MANIAC_DIF_HIGH, 626, 582);
                    draw_xpm(EXT, 826, 732);
                    break;

                  case 4:
                    draw_xpm(ZEN_DIF, 26, 32);
                    draw_xpm(CASUAL_DIF, 226, 182);
                    draw_xpm(SKILLED_DIF, 426, 382);
                    draw_xpm(MANIAC_DIF, 626, 582);
                    draw_xpm(EXT_HIGH, 826, 732);
                    break;
                    
                  default:
                    break;
                }

                code_size = 1;
                
              }
              //------------------------------------------------------------------------------
            }

            else {

              kb_scancode[0] = code;
              code_size = 1;

              if (code == ESC_BRK_CODE){
                
                diff_pos = 4;
                draw_xpm(ZEN_DIF, 26, 32);
                draw_xpm(CASUAL_DIF, 226, 182);
                draw_xpm(SKILLED_DIF, 426, 382);
                draw_xpm(MANIAC_DIF, 626, 582);
                draw_xpm(EXT_HIGH, 826, 732);
                  
              }

              if (code == ENTER){
                
                 switch (diff_pos){

                  case 0:
                    game_diff = ZEN;
                    break;

                  case 1:
                    game_diff = CASUAL;
                    break;

                  case 2:
                    game_diff = SKILLED;
                    break;

                  case 3:
                    game_diff = MANIAC;
                    break;

                  case 4:
                    game_diff = 0;
                    break;
                    
                  default:
                    break;
                }
                
                good = 0;

              }

            }
          }

          break;
         
        default:
          break;

      } 
    }
    else {      
    }
  }

  //Timer
  hook_id = hook_id_t;
  
  if(timer_unsubscribe_int())
    return 1;

  //Keyboard
  hook_id = hook_id_k;

  if (kbc_unsubscribe_int())
    return 1;
 
  //Mouse
  hook_id = hook_id_m;

  if (sys_irqdisable(&hook_id))
    return 1;

  if (mouse_set_data_report(0))
    return 1;

  if (sys_irqenable(&hook_id))
    return 1;

  if (kbc_unsubscribe_int())
    return 1;

  // DEBUX AUX-----------------------------------------------------
  //if (!timer_mode)
    //printf("%2d : %2d : %2d\n", time_count/3600, (time_count % 3600) / 60, time_count % 60);
  //---------------------------------------------------------------
  return game_diff;
}

int game(int gamemode, int gamediff){
  
  int m1;
  int m2;
  int s1;
  int s2;
  
 
  
  int cur_x=200;
  int cur_y=200;

  int comp[N][N] = {{0, 0, 0, 0, 0, 0, 0, 0, 0}, 
                 {0, 0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0, 0}};

  

  //desenha menu inicial

  //inicia jogo

  timer_mode = gamemode;
  game_diff = gamediff;
  
  set_game(timer_mode, game_diff, &time_count);

  //init_grid(GRID_XPOS, GRID_YPOS, GRID_WIDTH, GRID_HEIGHT, GRID_COLOR);
  draw_xpm(GRID, 0, 0);

  for (int row = 0; row < N; row++)
       
    for (int col = 0; col < N; col++) 

      if (grid[row][col] != 0){

        comp[row][col] = grid[row][col];
        write_cell(row, col, high_num_selector(grid[row][col]));

      }

  highlight_cell(GRID_XPOS, GRID_YPOS, x_index, y_index, grid);

  if (grid[x_index][y_index] != 0)  
    write_cell(x_index, y_index, high_num_selector(grid[x_index][y_index]));

  draw_xpm(COUNTER, COUNTER_XPOS, COUNTER_YPOS);
  draw_xpm(DOTS, COUNTER_XPOS + 55, COUNTER_YPOS + 5);
  draw_xpm(CTRLS, 0, 0);
 
  buff1_to_3();
  buff3_to_2();
  
  //draw_xpm(CUR,cur_x,cur_y);
  int ipc_status, r;
  message msg;
  
  //timer
  uint8_t timer_id = TIMER0_IRQ;
  int_counter = 0;

  int hook_id_t = 0;
  
  //keyboard
  uint8_t kb_id = KBC_IRQ;
  uint8_t kb_scancode[2];
  int hook_id_k = 0;
  uint8_t code_size = 1;
  //TO DO

  //mouse
  uint8_t mouse_id = MOUSE_IRQ;
  struct packet pkt;
  uint8_t counter = 1;
  uint8_t scancode[3];
  int hook_id_m = 0;

  
  //Timer
  if(timer_subscribe_int(&timer_id))
    return 1;
  hook_id_t = hook_id;

  //Keyboard
  if(kbc_subscribe_int(&kb_id))
    return 1;
  hook_id_k = hook_id;
  //TO DO


  //Mouse
  if (mouse_subscribe_int(&mouse_id))
    return 1;
  if (sys_irqdisable(&hook_id))
    return 1;
  if (mouse_set_data_report(1))
    return 1;
  if (sys_irqenable(&hook_id))
    return 1;
  hook_id_m = hook_id;

  
  int timer_irq = BIT(timer_id);
  int kb_irq = BIT(kb_id);
  int mouse_irq = BIT(mouse_id);

  int good = 1; 
  
  while (code != ESC_BRK_CODE && good) { 
 
    
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) { 
      
      switch (_ENDPOINT_P(msg.m_source)) {
        
        case HARDWARE: 

          if (msg.m_notify.interrupts & timer_irq) { 
            
            timer_int_handler();

            m1 = (time_count / 60) / 10;
            m2 = (time_count / 60) % 10;
            s1 = (time_count % 60) / 10;
            s2 = (time_count % 60) % 10;
            
            if (time_count < 6000){
              
              draw_xpm(number_selector(m1), COUNTER_XPOS + 5, COUNTER_YPOS + 5);
              draw_xpm(number_selector(m2), COUNTER_XPOS + 30, COUNTER_YPOS + 5);
              draw_xpm(number_selector(s1), COUNTER_XPOS + 75, COUNTER_YPOS + 5);
              draw_xpm(number_selector(s2), COUNTER_XPOS + 100, COUNTER_YPOS + 5);
              
            }

            if (timer_mode){

              if (time_count == 0){

                draw_xpm(LOSE0, 327, 181);
                 
                tickdelay(micros_to_ticks(2000000));    
                good = 0;
              }

              if (int_counter % 60 == 0){
                
                timer_print_elapsed_time();
                time_count--;              
              }
            }
            
            else
              
              if (int_counter % 60 == 0){
                
                timer_print_elapsed_time();
                time_count++;
              }

            if (int_counter % 2 == 0){
              buff3_to_2();
              draw_cursor(CURSOR, cur_x, cur_y);
            }

          }

          if (msg.m_notify.interrupts & mouse_irq) { 
            
            mouse_ih();
            
            if ((counter == 1) && (code & BIT1_IDENTFIER)){
              
              scancode[0] = code;
              counter++;
            }
            
            else if (counter == 2){
              
              scancode[1] = code;
              counter++;
            }

            else if (counter == 3){
              
              scancode[2] = code;
              counter++;
            }
            
            if (counter > 3){
              
              mouse_packet_builder(&pkt, scancode);
              mouse_print_packet(&pkt);
              counter = 1;          
            }
            
            int px;
            int py;
            
            if(get_event(&pkt, &ev))
              return 1;
           
            if(ev.type == LB_PRESS && cur_x > 327 && cur_x < 822 && cur_y > 181 && cur_y < 676){

              clear_cell(GRID_XPOS, GRID_YPOS, x_index, y_index);

              if (grid[x_index][y_index] != 0 && grid[x_index][y_index] == comp[x_index][y_index])  
                write_cell(x_index, y_index, high_num_selector(grid[x_index][y_index]));

              else if (grid[x_index][y_index] != 0 && grid[x_index][y_index] != comp[x_index][y_index])
                write_cell(x_index, y_index, number_selector(grid[x_index][y_index]));
              
              if(get_mouse_pos(grid,cur_x,cur_y,&px,&py)==0){//dentro do grid
                
                x_index = py;
                y_index = px;    

                highlight_cell(GRID_XPOS, GRID_YPOS, x_index, y_index, grid);

                if (grid[x_index][y_index] != 0 && grid[x_index][y_index] == comp[x_index][y_index])  
                  write_cell(x_index, y_index, high_num_selector(grid[x_index][y_index]));

                else if (grid[x_index][y_index] != 0 && grid[x_index][y_index] != comp[x_index][y_index])
                  write_cell(x_index, y_index, number_selector(grid[x_index][y_index]));
              }
              if(get_mouse_pos(grid,cur_x,cur_y,&px,&py)!=0){
                if(x_index>32 && x_index<132 && y_index>20 &&y_index<320){
                  //draw_xpm(ZEN_DIF_HIGH, 20, 32);//Goback
                  good=0;
              }
              }
              
  /*draw_xpm(ZEN_DIF_HIGH, 830, 32);
  300 100 */
                
            }
            if(ev.type == MOVE){
              

              
              //draw_xpm(DCUR,cur_x,cur_y);
              cur_x = cur_x + ev.x_delta;
              cur_y = cur_y - ev.y_delta;
              
              if (cur_x < 0)
                
                cur_x = 0;
              
              else if (cur_x > 1127)
              //cur_x > 1135
                cur_x = 1127;
              
              if (cur_y < 0)
                
                cur_y = 0;
              
              else if (cur_y > 824)
                
                cur_y = 824;

              buff2_to_1();
            }


          }//FIM SECCAO MOUSE

          if (msg.m_notify.interrupts & kb_irq) { 
            
            kbc_int_handler();

            // INPUT HANDLER----------------------------------------------------------------
             
            num_aux = number_input(code);

            if (num_aux >= 0 && comp[x_index][y_index] == 0){
                
              grid[x_index][y_index] = num_aux;

              if (num_aux == 0)
                clear_cell(GRID_XPOS, GRID_YPOS, x_index, y_index);

              else
                write_cell(x_index, y_index, number_selector(grid[x_index][y_index]));

              if (finished(grid, solution) == 0){
                
                draw_xpm(WIN3, 327, 181);
                
                tickdelay(micros_to_ticks(2000000)); 

                good = 0;
              }
            }
            //-------------------------------------------------------------------------------
            
            if (code == TWO_BYTE_CODE){
              
              kb_scancode[0] = code;
              code_size = 2;

            }

            else if (code_size == 2){
              
              kb_scancode[1] = code;

              // MOVEMENT HANDLER------------------------------------------------------------ 
              if (!grid_movement(&x_index, &y_index, kb_scancode[1], grid, comp)){
                
                highlight_cell(GRID_XPOS, GRID_YPOS, x_index, y_index, grid);

                if (grid[x_index][y_index] != 0 && grid[x_index][y_index] == comp[x_index][y_index])  
                  write_cell(x_index, y_index, high_num_selector(grid[x_index][y_index]));

                else if (grid[x_index][y_index] != 0 && grid[x_index][y_index] != comp[x_index][y_index])  
                  write_cell(x_index, y_index, number_selector(grid[x_index][y_index]));
              }
              //------------------------------------------------------------------------------
            
              code_size = 1;

            }

            else {

              kb_scancode[0] = code;
              code_size = 1;
            }
          }

          break;
         
        default:
          break;

      } 
    }
    else {      
    }
  }

  //Timer
  hook_id = hook_id_t;
  
  if(timer_unsubscribe_int())
    return 1;

  //Keyboard
  hook_id = hook_id_k;

  if (kbc_unsubscribe_int())
    return 1;
 
  //Mouse
  hook_id = hook_id_m;

  if (sys_irqdisable(&hook_id))
    return 1;

  if (mouse_set_data_report(0))
    return 1;

  if (sys_irqenable(&hook_id))
    return 1;

  if (kbc_unsubscribe_int())
    return 1;

  // DEBUX AUX-----------------------------------------------------
  //if (!timer_mode)
    //printf("%2d : %2d : %2d\n", time_count/3600, (time_count % 3600) / 60, time_count % 60);
  //---------------------------------------------------------------
  return 0;
}

void set_game(int gamemode, int difficulty, int *counter){

  if(gamemode){

    switch (difficulty){
      
      case ZEN:
        
        *counter = ZEN;
        return;

      case CASUAL:
        
        *counter = CASUAL;
        return;

      case SKILLED:
        
        *counter = SKILLED;
        return;

      case MANIAC:
        
        *counter = MANIAC;
        return;

      case T_ATK_TEST:
        
        *counter = T_ATK_TEST;
        return;

    }
  }

  else

      *counter = 0;
}




 




