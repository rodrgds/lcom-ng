// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>
#include "game/include/sudoku.h"

extern int grid[N][N];
extern int gridt[N][N];
extern int solution[N][N];

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

int(proj_main_loop)(int argc, char *argv[]) {
  
  int mode = 0;
  int diff = 0;
  int good = 1;
  int ncasas = 50;
  
  while (good){

    sudoku_generator(ncasas);
    
    //DEBUG AUX----------------------------------------------
    
    /*
      printGrid(grid);
      printf("\n");
      printGrid(solution);
    */
    
    //-------------------------------------------------------

    mode = mainmenu();

    switch (mode){
      
      case 0:  //pratica
        
        game(mode, 0);
        break;

      case 1:   //menu selecção grau dificuldade
        
        diff = diffmenu();
        
        if(diff == 0){  
          break;
        }
        
        else{

          game(mode, diff);
          break;
        }

      case 2:  //exit
        
        good = 0;
        break;

      default:
        
        break;
      }
  }

 return 0;
}
