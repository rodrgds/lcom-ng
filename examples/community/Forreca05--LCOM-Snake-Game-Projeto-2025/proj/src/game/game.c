#include "game.h"
#include "gamestate.h"
#include "../controllers/sp/sp.h"
#include "../assets/xpm/all.xpm"
#include "../input/input.h"

// este codigo é enviado de ambas as vms, quando as duas recebem começa o jogo
#define READY_CODE 0xFF

extern uint8_t scancode, irq_set_keyboard, irq_set_timer, irq_set_mouse;
extern int game_state;
uint8_t remote_scancode;
extern int goldenAppleCounter, whiteAppleCounter, darkAppleCounter;

int ipc_status, r;
message msg;

static void wait_for_ready() {
    bool ready = false;
    bool remote_ready = false;
    uint8_t lsr_content, response;
    
    Sprite* waitingSprite = create_sprite(waiting, 0, 0, 0, 0);
    waitingSprite->draw(waitingSprite);
    draw();
    
    while (!(ready && remote_ready)) {
        if (!ready) {
            sp_send_byte(READY_CODE);
            ready = true;
        }
        util_sys_inb(COM1_BASE_ADDR + LSR, &lsr_content);
        if (lsr_content & 0x01) {
            util_sys_inb(COM1_BASE_ADDR, &response);
            if (response == READY_CODE) {
                remote_ready = true;
            }
        }
    }
    
    clear_screen();
    waitingSprite->destroy(waitingSprite);
}

int run_game_multiplayer(int option, bool isRemote) {
    GameState* gameState = createGameState(option, true, false, false, isRemote);

    wait_for_ready();
    
    while (game_state == STATE_GAME_MULTIPLAYER_REMOTE) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed: %d\n", r);
            continue;
        }

        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & irq_set_keyboard) {
                        game_state = handleKeyboardInputMultiPlayer(gameState);
                    }
                    
                    if (msg.m_notify.interrupts & irq_set_mouse) {
                        game_state = handleMouseInputMultiPlayer(gameState);
                    }

                    if (msg.m_notify.interrupts & irq_set_timer) {
                        game_state = handleTimerInputMultiPlayer(gameState);
                    }

                    handleSerialPortInput();
                    break;
            }
        }
    }

    destroyGameState(gameState);
    return game_state;
}

int run_game(int option) {
    GameState* gameState = createGameState(option, false, false, false, false);
    printf("Running game with option: %d\n", option);
    while (game_state == STATE_GAME_SINGLEPLAYER_IO) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed: %d\n", r);
            continue;
        }

        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    printf("Handling interrupts in run_game\n");
                    if (msg.m_notify.interrupts & irq_set_keyboard) {
                        if (handleKeyboardInputSinglePlayerIO(gameState) != STATE_GAME_SINGLEPLAYER_IO) {
                            goldenAppleCounter = 0;
                            darkAppleCounter = 0;
                            whiteAppleCounter = 0;
                            return STATE_MENU;
                        }
                    }
                    
                    if (msg.m_notify.interrupts & irq_set_mouse) {
                        if (handleMouseInputSinglePlayerIO(gameState) != STATE_GAME_SINGLEPLAYER_IO) {
                            goldenAppleCounter = 0;
                            darkAppleCounter = 0;
                            whiteAppleCounter = 0;
                            return STATE_MENU;
                        }
                    }

                    if (msg.m_notify.interrupts & irq_set_timer) {
                        if (handleTimerInputSinglePlayerIO(gameState) != STATE_GAME_SINGLEPLAYER_IO) {
                            goldenAppleCounter = 0;
                            darkAppleCounter = 0;
                            whiteAppleCounter = 0;
                            return STATE_MENU;
                        }
                    }
                    break;
            }
        }
    }

    destroyGameState(gameState);
    return game_state;
}


int run_game_classic(int option) {
    GameState* gameState = createGameState(option, false, false, true, false);

    while (game_state == STATE_GAME_SINGLEPLAYER_CLASSIC) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed: %d\n", r);
            continue;
        }

        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & irq_set_keyboard) {
                        if (handleKeyboardInputSinglePlayerClassic(gameState) != STATE_GAME_SINGLEPLAYER_CLASSIC) {
                            goldenAppleCounter = 0;
                            darkAppleCounter = 0;
                            whiteAppleCounter = 0;
                            destroyGameState(gameState);
                            return STATE_MENU;
                        }
                    }
                    
                    if (msg.m_notify.interrupts & irq_set_mouse) {
                        if (handleMouseInputSinglePlayerClassic(gameState) != STATE_GAME_SINGLEPLAYER_CLASSIC) {
                            goldenAppleCounter = 0;
                            darkAppleCounter = 0;
                            whiteAppleCounter = 0;
                            destroyGameState(gameState);
                            return STATE_MENU;
                        }
                    }

                    if (msg.m_notify.interrupts & irq_set_timer) {
                        if (handleTimerInputSinglePlayerClassic(gameState) != STATE_GAME_SINGLEPLAYER_CLASSIC) {
                            goldenAppleCounter = 0;
                            darkAppleCounter = 0;
                            whiteAppleCounter = 0;
                            destroyGameState(gameState);
                            return STATE_MENU;
                        }
                    }
                    break;
            }
        }
    }

    destroyGameState(gameState);
    return game_state;
}

int run_game_multiplayer_local(int option) {
    GameState* gameState = createGameState(option, false, true, false, false);
    printf("Running local multiplayer game with option: %d\n", option);
    while (game_state == STATE_GAME_MULTIPLAYER_LOCAL) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed: %d\n", r);
            continue;
        }

        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    printf("Handling interrupts in run_game_multiplayer_local\n");
                    if (msg.m_notify.interrupts & irq_set_keyboard) {
                        printf("Keyboard interrupt\n");
                        if (handleKeyboardInputMultiPlayerLocal(gameState) != STATE_GAME_MULTIPLAYER_LOCAL) {
                            goldenAppleCounter = 0;
                            darkAppleCounter = 0;
                            whiteAppleCounter = 0;
                            return STATE_MENU;
                        }
                    }
                    
                    if (msg.m_notify.interrupts & irq_set_mouse) {
                        printf("Mouse interrupt\n");
                        if (handleMouseInputMultiPlayerLocal(gameState) != STATE_GAME_MULTIPLAYER_LOCAL) {
                            goldenAppleCounter = 0;
                            darkAppleCounter = 0;
                            whiteAppleCounter = 0;
                            return STATE_MENU;
                        }
                    }

                    if (msg.m_notify.interrupts & irq_set_timer) {
                        printf("Timer interrupt\n");
                        if (handleTimerInputMultiPlayerLocal(gameState) != STATE_GAME_MULTIPLAYER_LOCAL){
                            goldenAppleCounter = 0;
                            darkAppleCounter = 0;
                            whiteAppleCounter = 0;
                            return STATE_MENU;
                        }
                    }
                    break;
            }
        }
    }

    destroyGameState(gameState);
    printf("Exiting local multiplayer game\n");
    return game_state;
}

