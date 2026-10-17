#include <lcom/lcf.h>

#include "controller/game.h"


int main(int argc, char *argv[]) {
    // Define a linguagem das mensagens do LCF
    lcf_set_language("EN-US");
  
    // Habilita o rastreamento de chamadas de função
    // lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");
  
    // Habilita a gravação da saída de printf
    // lcf_log_output("/home/lcom/labs/lab5/output.txt");
  
    // Executa a função desejada (pode ser kbd_test_scan ou outra)
    if (lcf_start(argc, argv))
      return 1;
  
    // Finaliza a execução do LCF
    lcf_cleanup();
  
    return 0;
}

int(proj_main_loop)(int argc, char* argv[]) {
    /* your main loop code here */

    if (game_controller() != 0) return 1;

    return 0;
}
