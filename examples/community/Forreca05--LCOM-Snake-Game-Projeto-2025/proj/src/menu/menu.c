#include "menu.h"
#include "../assets/xpm/all.xpm"

extern uint8_t scancode, irq_set_keyboard, irq_set_mouse, irq_set_timer, byte_index;
extern int mouse_x, mouse_y ,timer_count;
extern struct packet mouse_packet;
int ipc_status, r;
message msg;

static void drawMenu(Sprite* backgroundSprite, DynamicSprite* singlePlayerButton, DynamicSprite* multiPlayerButton, DynamicSprite* exitButton, Sprite* cursorSprite) {
    clear_screen();
    backgroundSprite->draw(backgroundSprite);
    singlePlayerButton->draw(singlePlayerButton);
    multiPlayerButton->draw(multiPlayerButton);
    exitButton->draw(exitButton);
    cursorSprite->draw(cursorSprite);
    draw();
}

static void destroyMenu(Sprite* backgroundSprite, DynamicSprite* singlePlayerButton, DynamicSprite* multiPlayerButton, DynamicSprite* exitButton, Sprite* cursorSprite) {
    backgroundSprite->destroy(backgroundSprite);
    singlePlayerButton->destroy(singlePlayerButton);
    multiPlayerButton->destroy(multiPlayerButton);
    exitButton->destroy(exitButton);
    cursorSprite->destroy(cursorSprite);
}

int run_menu(bool* isMultiplayer) {
    message msg;
    int ipc_status, r;

    if (vg_init(0x14C) == NULL) {
        printf("Failed to initialize video mode\n");
        return STATE_EXIT;
    }

    Sprite* backgroundSprite = create_sprite(menuBackground, 0, 0, 0, 0);
    DynamicSprite* singlePlayerButton = create_dynamicSprite(singlePlayerButton_xpm, 305,486, 0, 0,0x000000,0xffffff);
    DynamicSprite* multiPlayerButton = create_dynamicSprite(multiPlayerButton_xpm, 305,612, 0, 0,0x000000,0xffffff);
    DynamicSprite* exitButton = create_dynamicSprite(exitButton_xpm, 305,738, 0, 0,0x000000,0xffffff);
    Sprite* cursorSprite = create_sprite(cursor, mouse_x, mouse_y, 0, 0);
    
    drawMenu(backgroundSprite, singlePlayerButton, multiPlayerButton, exitButton, cursorSprite);

    while (true) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed: %d\n", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & irq_set_keyboard) {
                        kbc_ih();
                            
                        if (scancode == BREAK_ESC) {
                            destroyMenu(backgroundSprite, singlePlayerButton, multiPlayerButton, exitButton, cursorSprite);
                            return STATE_EXIT;
                        }
                    }
                    if (msg.m_notify.interrupts & irq_set_mouse) {
                        mouse_ih();
                        mouse_sync_bytes();
                        if (byte_index == 3) {
                            byte_index = 0;
                            mouse_bytes_to_packet();
                            update_mouse_position();
                        }
                        
                        singlePlayerButton->setHover(singlePlayerButton, false);
                        multiPlayerButton->setHover(multiPlayerButton, false);
                        exitButton->setHover(exitButton, false);
                        cursorSprite->changeXPM(cursorSprite, cursor);

                        if (singlePlayerButton->isInside(singlePlayerButton, mouse_x, mouse_y)) {
                            singlePlayerButton->setHover(singlePlayerButton, true);
                            cursorSprite->changeXPM(cursorSprite, pointer);
                            if (mouse_packet.lb) {
                                destroyMenu(backgroundSprite, singlePlayerButton, multiPlayerButton, exitButton, cursorSprite);
                                *isMultiplayer = false;
                                return STATE_GAME_SINGLEPLAYER;
                            }
                        } else if (multiPlayerButton->isInside(multiPlayerButton, mouse_x, mouse_y)) {
                            multiPlayerButton->setHover(multiPlayerButton, true);
                            cursorSprite->changeXPM(cursorSprite, pointer);
                            if (mouse_packet.lb) {
                                destroyMenu(backgroundSprite, singlePlayerButton, multiPlayerButton, exitButton, cursorSprite);
                                *isMultiplayer = true;
                                return STATE_GAME_MULTIPLAYER;
                            }
                            
                        } else if (exitButton->isInside(exitButton, mouse_x, mouse_y)) {
                            exitButton->setHover(exitButton, true);
                            cursorSprite->changeXPM(cursorSprite, pointer);
                            if (mouse_packet.lb) {
                                destroyMenu(backgroundSprite, singlePlayerButton, multiPlayerButton, exitButton, cursorSprite);
                                return STATE_EXIT;
                            }
                        }
                        
                    }
                    if (msg.m_notify.interrupts & irq_set_timer) {
                        timer_int_handler();
                        if (timer_count % 2 == 0) {
                            cursorSprite->x = mouse_x;
                            cursorSprite->y = mouse_y;
                            drawMenu(backgroundSprite, singlePlayerButton, multiPlayerButton, exitButton, cursorSprite);
                        }
                    }
                    
            }
        }
    }
    return STATE_EXIT;
}

