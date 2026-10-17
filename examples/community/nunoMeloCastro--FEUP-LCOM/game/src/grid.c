#include <lcom/lcf.h>

#include <stdio.h> 
#include <stdbool.h> 
#include <stdlib.h> 
#include <time.h>
#include <stdint.h> 

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

int (init_menu)(void){
  
  if (vg_draw_rectangle(0, 0, get_xres(), get_yres(), 0xABE1E7)){

    vg_exit();

    if (lm_free(&mem_map))
      printf("failed to free memory\n");

      return 1;
  }
  
  draw_xpm(MAINTITLE, 177, 100);
  draw_xpm(PRAC_HIGH, 50, 400);
  draw_xpm(T_ATK, 426, 550);
  draw_xpm(EXT, 802, 700);

  buff1_to_3();
  buff3_to_2();
  return 0;
}

int (init_diff_menu)(void){
  
  if (vg_draw_rectangle(0, 0, get_xres(), get_yres(), 0xABE1E7)){

    vg_exit();

    if (lm_free(&mem_map))
      printf("failed to free memory\n");

      return 1;
  }
  
  draw_xpm(ZEN_DIF_HIGH, 26, 32);
  draw_xpm(CASUAL_DIF, 226, 182);
  draw_xpm(SKILLED_DIF, 426, 382);
  draw_xpm(MANIAC_DIF, 626, 582);
  draw_xpm(EXT, 826, 732);

  buff1_to_3();
  buff3_to_2();
  return 0;
}

int (init_grid)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
  
  //1152x864 -> x = 327 || y = 181 || width = 500 || height = 500

  uint16_t w = (x + width > get_xres()) ? (get_xres() - x) : (width);
  
  uint16_t h = (y + height > get_yres()) ? (get_yres() - y) : (height);

  if (x < get_xres() && y < get_yres()){
    
    if (vg_draw_rectangle(x, y, w, h, color)){

      vg_exit();

      if (lm_free(&mem_map))
        printf("failed to free memory\n");

        return 1;
    }

    for (int i = 0; i < 9; i++)
      
      for (int j = 0; j < 9 ; j++)
      
        if (vg_draw_rectangle(x + 5 + 55 * i , y + 5 + 55 * j, 50, 50, 0xffffff)){

            vg_exit();

            if (lm_free(&mem_map))
              printf("failed to free memory\n");

            return 1;
        }

    for (int i = 1; i < 3; i++)

      if (vg_draw_rectangle(x + 55 * 3 * i , y + 5, 5, 490, 0)){

        vg_exit();

        if (lm_free(&mem_map))
          printf("failed to free memory\n");

        return 1;
      }
    
    for (int j = 1; j < 3; j++)

      if (vg_draw_rectangle(x + 5, y + 55 * 3 * j, 490, 5, 0)){

        vg_exit();

        if (lm_free(&mem_map))
          printf("failed to free memory\n");

        return 1;
      }
    
  }
  return 0;
}

int (highlight_cell)(int x, int y, int ind_x, int ind_y, int grid[N][N]){
  
  //1152x864 -> x = 327 || y = 181 || ind_x = {0, ..., 9} || ind_y = {0, ..., 9}

  if (vg_draw_rectangle(x + 5 + 55 * ind_y , y + 5 + 55 * ind_x, 50, 50, 0x0288d1)){

    vg_exit();

    if (lm_free(&mem_map))
      printf("failed to free memory\n");

    return 1;
  }

  //if (grid[ind_x][ind_y] != 0)
    //draw_xpm(number_selector(grid[ind_x][ind_y]), 344 + 55 * ind_y, 191 + 55 * ind_x);

  return 0;
}

void (write_cell)(int ind_x, int ind_y, xpm_map_t xpm){

  //1152x864 -> ind_x = {0, ..., 9} || ind_y = {0, ..., 9}

  draw_xpm(xpm, 344 + 55 * ind_y, 191 + 55 * ind_x);

}

int (clear_cell)(int x, int y, int ind_x, int ind_y){
  
  //1152x864 -> x = 327 || y = 181 || ind_x = {0, ..., 9} || ind_y = {0, ..., 9}

  if (vg_draw_rectangle(x + 5 + 55 * ind_y , y + 5 + 55 * ind_x, 50, 50, 0xFFFFFF)){

    vg_exit();

    if (lm_free(&mem_map))
      printf("failed to free memory\n");

    return 1;
  }

  return 0;
}

xpm_map_t (number_selector)(int num){
  
  switch (num){
    
    case 0:
      return N0;

    case 1:
      return N1;
    
    case 2:
      return N2;

    case 3:
      return N3;

    case 4:
      return N4;

    case 5:
      return N5;

    case 6:
      return N6;

    case 7:
      return N7;

    case 8:
      return N8;

    case 9:
      return N9;

  }
  return NULL;
}

xpm_map_t (high_num_selector)(int num){
  
  switch (num){

    case 0:
      return H0;

    case 1:
      return H1;
    
    case 2:
      return H2;

    case 3:
      return H3;

    case 4:
      return H4;

    case 5:
      return H5;

    case 6:
      return H6;

    case 7:
      return H7;

    case 8:
      return H8;

    case 9:
      return H9;

  }
  return NULL;
}

int (grid_movement)(int *x, int *y, uint8_t code, int grid[N][N], int comp[N][N]){

  if (code == LEFT_BREAK){

    clear_cell(GRID_XPOS, GRID_YPOS, *x, *y);

    if (grid[*x][*y] != 0 && grid[*x][*y] == comp[*x][*y])  
      write_cell(*x, *y, high_num_selector(grid[*x][*y]));

    else if (grid[*x][*y] != 0 && grid[*x][*y] != comp[*x][*y])
      write_cell(*x, *y, number_selector(grid[*x][*y]));
    
    if (*y == 0)

      *y = 8;
    
    else

      *y = *y - 1 ;

    return 0;
  }

  else if (code == RIGHT_BREAK){

    clear_cell(GRID_XPOS, GRID_YPOS, *x, *y);

    if (grid[*x][*y] != 0 && grid[*x][*y] == comp[*x][*y])  
      write_cell(*x, *y, high_num_selector(grid[*x][*y]));

    else if (grid[*x][*y] != 0 && grid[*x][*y] != comp[*x][*y])
      write_cell(*x, *y, number_selector(grid[*x][*y]));
    
    if (*y == 8)

      *y = 0;
    
    else

      *y = *y + 1 ;

    return 0;
  }

  else if (code == UP_BREAK){

    clear_cell(GRID_XPOS, GRID_YPOS, *x, *y);

    if (grid[*x][*y] != 0 && grid[*x][*y] == comp[*x][*y])  
      write_cell(*x, *y, high_num_selector(grid[*x][*y]));

    else if (grid[*x][*y] != 0 && grid[*x][*y] != comp[*x][*y])
      write_cell(*x, *y, number_selector(grid[*x][*y]));
    
    if (*x == 0)

      *x = 8;
    
    else

      *x = *x - 1;

    return 0;
  }

  else if (code == DOWN_BREAK){

    clear_cell(GRID_XPOS, GRID_YPOS, *x, *y);
    
    if (grid[*x][*y] != 0 && grid[*x][*y] == comp[*x][*y])  
      write_cell(*x, *y, high_num_selector(grid[*x][*y]));

    else if (grid[*x][*y] != 0 && grid[*x][*y] != comp[*x][*y])
      write_cell(*x, *y, number_selector(grid[*x][*y]));
    
    if (*x == 8)

      *x = 0;
    
    else

      *x = *x + 1;
  
    return 0;
  }

  else
    return 1;

}

int (menu_movement)(int *x, uint8_t code){

  if (code == LEFT_BREAK){
    
    if (*x == 0)

      *x = 2;
    
    else

      *x = *x - 1 ;

    return 0;
  }

  else if (code == RIGHT_BREAK){
    
    if (*x == 2)

      *x = 0;
    
    else

      *x = *x + 1 ;

    return 0;
  }

  else if (code == UP_BREAK){

    if (*x == 0)

      *x = 2;
    
    else

      *x = *x - 1;

    return 0;
  }

  else if (code == DOWN_BREAK){

    if (*x == 2)

      *x = 0;
    
    else

      *x = *x + 1;
  
    return 0;
  }

  else
    return 1;

}

int (diff_movement)(int *x, uint8_t code){

  if (code == LEFT_BREAK){
    
    if (*x == 0)

      *x = 4;
    
    else

      *x = *x - 1 ;

    return 0;
  }

  else if (code == RIGHT_BREAK){
    
    if (*x == 4)

      *x = 0;
    
    else

      *x = *x + 1 ;

    return 0;
  }

  else if (code == UP_BREAK){

    if (*x == 0)

      *x = 4;
    
    else

      *x = *x - 1;

    return 0;
  }

  else if (code == DOWN_BREAK){

    if (*x == 4)

      *x = 0;
    
    else

      *x = *x + 1;
  
    return 0;
  }

  else
    return 1;

}

int (number_input)(uint8_t code){

  switch (code){

    case KEY1:
      return 1;

    case KEY2:
      return 2;

    case KEY3:
      return 3;

    case KEY4:
      return 4;

    case KEY5:
      return 5;

    case KEY6:
      return 6;

    case KEY7:
      return 7;

    case KEY8:
      return 8;

    case KEY9:
      return 9;

    case BACKSPACE:
      return 0;
    
    default:
      return -1;
  }
}

int (finished)(int grid[N][N], int solution[N][N]){

  for (int i = 0; i < 9; i++)
      
      for (int j = 0; j < 9; j++)

        if(solution[i][j] != grid[i][j])
          return 1;
  
  return 0;
}

int (get_mouse_pos)(int grid[N][N],int x_cur,int y_cur,int *posx,int *posy){
    *posx=(abs((x_cur-327)/55));
    *posy=(abs((y_cur-181)/55));
    if(!(*posx>=0 && *posx<9 && *posy>=0 && *posy<9)){
        //FORA DO GRID
        return 1;
    }
    return 0;
}

int (get_mouse_option_initmenu)(int x_cur,int y_cur,int *option){
   
    if(x_cur>=50 && x_cur<=350 && y_cur>=400 && y_cur<=500)
        *option = 0; // pratica

    else
    {
        if(x_cur>=426 && x_cur<=726 && y_cur>=550 && y_cur<=650)
            *option = 1; //t_attk
        else
        {
            if(x_cur>=802 && x_cur<=1102 && y_cur>=700 && y_cur<=800)
                *option = 2;  //exit
            else
            {   //fora dos botões das opções
                return 1;
            }
            
        }
            
    }
        
    return 0;
}

int (get_mouse_option_diffmenu)(int x_cur,int y_cur,int *option){
   
    if(x_cur>=26 && x_cur<=326 && y_cur>=32 && y_cur<=132)
        *option = 0; // zen

    else if(x_cur>=226 && x_cur<=526 && y_cur>=182 && y_cur<=282)
        *option = 1; //casual
       
    else if(x_cur>=426 && x_cur<=726 && y_cur>=382 && y_cur<=482)
        *option = 2;  //skilled
    
    else if(x_cur>=626 && x_cur<=926 && y_cur>=582 && y_cur<=682)
        *option= 3;//maniac

    else if(x_cur>=826 && x_cur<=1126 && y_cur>=732 && y_cur<=832)
        *option= 4;//exit
                   
    else
        return 1;
        
    return 0;
}
