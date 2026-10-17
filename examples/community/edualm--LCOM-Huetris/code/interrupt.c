#include <minix/drivers.h>

#include <stdio.h>

#include "interrupt.h"

#include "timer.h"
#include "board.h"

#include "i8254.h"
#include "kbc.h"
#include "mouse.h"

static int subscribed[MAX_INTERRUPTS];

static int handler_running;

static kInterruptMode irqm;

//  Private Calls

void _int_reset();

//  Code

void int_init() {
    int i = 0;
    
    handler_running = 0;
    
    for (; i < MAX_INTERRUPTS; ++i)
        _int_reset(i);
    
    irqm = kInterruptModeInvalid;
}

void int_setMode(kInterruptMode new_mode) {
    irqm = new_mode;
}

void _int_reset(int subpos) {
    subscribed[subpos] = 0;
    
    printf("Resetting %d...\n", subpos);
}

int int_subscribe(int irq_line, int policy, void (*callback)()) {
    unsigned int subpos = 0, found = 0;
    
    for (; subpos < MAX_INTERRUPTS; subpos++)
        if (!subscribed[subpos]) {
            found = 1;
            
            break;
        }
    
    if (!found) {
        printf("[int_subscribe] couldn't subscribe interrupt. all interrupts are subscribed. did you forget to call int_init()?");
        
        return 1;
    }
    
    int hook = (int)subpos;
    if (sys_irqsetpolicy(irq_line, policy, &hook))
        return 1;
    
    if (sys_irqenable(&hook)) {
        sys_irqrmpolicy(&hook);
        return 1;
    }
    subscribed[subpos] = hook;
    
/*#warning TODO fix callbacks
    
    subscribed[subpos].callback = callback;*/
    
    printf("Subscribed on position %d...\n", subpos);
    
    return subpos;
}

int int_unsubscribe(int subpos) {
    unsigned int errored = 0;
    if (subpos < 0 || subpos >= MAX_INTERRUPTS || !subscribed[subpos])
        return 3;
    int hook = subscribed[subpos];
    
    if (sys_irqdisable(&hook))
        errored++;
    
    if (sys_irqrmpolicy(&hook))
        errored += 2;
    else
        _int_reset(subpos);
    
    /*
     *  +---------------+
     *  | return values |
     *  +---------------+
     *
     *  0 - no errors
     *  1 - first call failed
     *  2 - second call failed
     *  3 - all calls failed
     */
    
    return errored;
}

static int int_handler() {
    message msg;
    int ipc_status;
    
    int subpos = 0, found = 0;
    
    for (; subpos < MAX_INTERRUPTS; subpos++)
        if (subscribed[subpos]) {
            found = 1;
            
            break;
        }
    
    if (!found) {
        printf("[int_handler] no interrupts are subscribed. terminating...\n");
        
        return 1;
    }
    
    while (handler_running) {
        int resp = driver_receive(ANY, &msg, &ipc_status);
        if (resp) {
            printf("[int_handler] driver_receive failed with: %d\n", resp);
            
            continue;
        }
        
        int i;
        
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: {
                    for (i = 0; i < MAX_INTERRUPTS; i++) {
                        if (bit_isset(msg.NOTIFY_ARG, i)) {
                            printf("got message for %d\n", i);
                            if (subscribed[i] != 0) {
                                if (i == 2 && irqm != kInterruptModeInGame) {
                                    unsigned long ignored;
                                    mouse_read(&ignored);
                                    continue;
                                }
                                switch (irqm) {
                                    case kInterruptModeMainMenu: {
                                        if (i == 1)
                                            sceneMainMenu_keyboardHandler();
                                        
                                        printf("mode mm (%d)\n", i);
                                        
                                        break;
                                    }
                                        
                                    case kInterruptModeEndGame: {
                                        if (i == 1)
                                            sceneEndGame_keyboardHandler();
                                        
                                        printf("mode eg (%d)\n", i);
                                        
                                        break;
                                    }
                                        
                                    case kInterruptModeInGame: {
                                        if (i == 0) {
                                            timer_int_handler();
                                            
                                            printf("timer ih\n");
                                        } else if (i == 1) {
                                            board_keyboardHandler();
                                            
                                            printf("kb ih\n");
                                        } else if (i == 2) {
                                            board_mouseHandler();
                                            
                                            printf("mouse ih\n");
                                        } else {
                                            printf("got int number %d", i);
                                        }
                                        
                                        printf("mode ig (%d)\n", i);
                                        
                                        break;
                                    }
                                        
                                    default:
                                        printf("mode unk\n");
                                        
                                        break;
                                }
                            }
                            printf("subscribed %d\n", i);
                        }
                    }
                    
                    break;
                }
            }
        }
    }
    
    return 0;
}

int int_start_handler() {
    handler_running = 1;
    
    return int_handler();
}

void int_stop_handler() {
    handler_running = 0;
}
