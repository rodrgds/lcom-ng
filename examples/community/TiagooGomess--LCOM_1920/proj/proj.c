
#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>
#include "words.h"
#include "scancodes.h"
#include "timer.h"
#include "keyboard.h"
#include "video_card.h"
#include "mouse.h"
#include "game.h"

int irq_set_keyboard, irq_set_timer, irq_set_rtc,irq_set_mouse;
struct packet pacote;
extern uint8_t scancode; // de keyboard.c
extern bool kbc_ih_valid; // de keyboard.c
extern bool mouse_ih_valid; // de mouse.c
extern int size;  // de keyboard.c
extern int counter;  // de timer.c
extern uint8_t* secondBuffer; // de video_card.c
extern uint32_t bitsPerPixel; // de video_card.c
int y0_word = 0;
int x0_word = 10;
bool exitGame = false;

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

    init_all_sprites();

    timer_set_frequency(0, 120); // freq = 120 Hz

    uint8_t* initial_address = vg_init(0x11A);  // mapeamento da memória e mudança do modo gráfico para 0x11A

    char* topPlayerName = (char*) calloc(256, sizeof(char));

    char *word = getFirstWord();

    int ipc_status;
    uint8_t r;
    message msg;
    if (subscribe_and_enable_everything() != 0)
        return 1;

    while ((scancode != ESC_BREAK) && !exitGame) {  // Ciclo que só termina quando scancode for um esc_break(0x81) ou quando o utilizador sair do jogo
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {  //Verifica o tipo de notificação
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: // Notificação de interrupt de hardware

                    if (msg.m_notify.interrupts & irq_set_keyboard) {  // interrupçaõ do keyboard

                        kbc_ih();
                        
                        if (kbc_ih_valid) {

                            pauseGameHandlerKeyboard();

                            topPlayerName = askNameIfRecordHandlerKeyboard(topPlayerName);

                            gameWordsHandlerKeyboard();

                            size = 0; // de keyboard.c
                        }
                        else
                            break;
                        
                            
                    }

                    if ((msg.m_notify.interrupts & irq_set_mouse)) { // interrupção do rato
                        mouse_ih();
                        if (!mouse_ih_valid) 
                            break;
                        packet_parsing(&pacote);

                        if (mainMenuHandlerMouse())
                            break;

                        if (leaderboardHandlerMouse())
                            break;

                        if (instructionsHandlerMouse())
                            break;
                        
                        if (askNameHandlerMouse(topPlayerName))
                            break;

                        if(gamerOverHandlerMouse()) {
                            word = get_word();
                            y0_word = 0;
                            topPlayerName = (char*) calloc(256, sizeof(char)); // quando faço topPlayerName = "", o programa encrava    
                            break;
                        }
                        
                    }

                    if ((msg.m_notify.interrupts & irq_set_timer)) {  // se for uma interrupção do timer

                        timer_int_handler();

                        if (gameHandler())
                            break;

                        pausedGameHanler();

                        askNameHandler(topPlayerName);

                        menuHandler();

                        leaderboardHandler();

                        instructionsHandler();

                        gameOverHandler();

                        memcpy(initial_address, secondBuffer, h_res * v_res * (bitsPerPixel / 8)); // double buffering technique

                    }


                default:
                    break;

            }

        }
    }

    vg_exit();

    if (unsubscribe_everything() != 0)
        return 1;

    free_everything();
    free(topPlayerName);

    return 0;

}



