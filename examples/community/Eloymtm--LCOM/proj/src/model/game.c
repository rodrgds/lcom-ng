#include "game.h"

extern int count;
extern uint8_t scancode;
extern uint8_t byte_count;
extern struct packet pp;


int x1 = 10;
int y1 = 10;
int *xx = &x1;
int *yy = &y1;

Game* (StartGame)(STATE state){
    Game* game = (Game*)malloc(sizeof(Game));
    if (game == NULL) {
        return NULL; // Falha na alocação de memória
    }

    printf("start game");

    //paintBoard(*board);

    game->board1 = initializeBoard();
    game->board2 = initializeBoard();

    game->state = state;
    game->isPlayer1 = true;
    
    return game;
}

int GameLoop(Game *game){
    uint8_t irq_key;
    uint8_t irq_mouse;
    uint8_t irq_timer;

    int ipc_status,r;
    message msg;

    int time = 0;
    //int *time = time;

    write_command(0x64, 0xF3);
    write_command(0x60, 32);
    
    if(timer_subscribe_int(&irq_timer)!=0)return 1;
    if(mouse_subscribe_int(&irq_mouse)!=0)return 1;
    if(keyboard_subscribe_int(&irq_key)!=0)return 1;

    while(game->state != END){//scancode do esc { /* You may want to use a different condition */
        /* Get a request message. */
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: /* hardware interrupt notification */
                    if (msg.m_notify.interrupts & irq_key) { /* subscribed interrupt */
                        kbc_ih();
                        keyController(game, scancode, &time);
                        //printf("%x",scancode);
                    }

                    if (msg.m_notify.interrupts & irq_timer) { /* subscribed interrupt */
                        timer_int_handler();
                        swapBuff();
                        if(count == 60){
                          time++;
                          timerController(game, &time);

                          count=0;
                        }	
                    }

                    if (msg.m_notify.interrupts & irq_mouse) { /* subscribed interrupt */
                        mouse_ih();
                        synchronize_driver();
                        if(byte_count==3){
                        createPacket();
                        mouseController(game, pp,xx, yy);
                        byte_count=0;
                     }	
                    }


                    break;
                default:
                    break; /* no other notifications expected: do nothing */	
            }
        }
    }
    if(timer_unsubscribe_int()!=0)return 1;
    if(mouse_unsubscribe_int()!=0)return 1;
    if(keyboard_unsubscribe_int()!=0)return 1;
    return 0;
}
