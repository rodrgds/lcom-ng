#include <minix/drivers.h>
#include <minix/syslib.h>

#include "jogo.h"
#include "stdio.h"

int main(){

  sef_startup();
  

  Jogo jogo;
  start(&jogo);
  printf("ITS ALIVE!\n");


  return 0;
}
