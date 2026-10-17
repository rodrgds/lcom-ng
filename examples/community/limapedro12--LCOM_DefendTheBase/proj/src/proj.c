#include <lcom/lcf.h>
#include <lcom/proj.h>
#include <stdint.h>
#include <stdio.h>
#include "game.h"
#include "our_framework/framework_essencials.h"

int main(int argc, char *argv[]){
  lcf_set_language("EN-US");
  if (lcf_start(argc, argv)) return 1;
  lcf_cleanup();
  return 0;
}

int(proj_main_loop)(int argc, char *argv[]) {
    
  before();

  run(game);
  
  after();

  return 0;
}
