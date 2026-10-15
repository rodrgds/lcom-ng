#include "local_or_remote.h"

message msg;
int ipc_status;
int r;
extern struct packet mouse_packet;

extern uint8_t scancode;
extern int mouse_x, mouse_y, byte_index;
extern uint8_t irq_set_keyboard, irq_set_mouse, irq_set_timer;
extern int timer_count;

int run_local_or_remote()
{
    Sprite *backgroundSprite = create_sprite(menuBackground, 0, 0, 0, 0);
    if (!backgroundSprite)
    {
        printf("Failed to load background\n");
        return STATE_EXIT;
    }

    DynamicSprite *localButtonSprite = create_dynamicSprite(localButton, 305, 486, 0, 0, 0x000000, 0xffffff);
    if (!localButtonSprite)
    {
        printf("Failed to load button\n");
        return STATE_EXIT;
    }

    DynamicSprite *remoteButtonSprite = create_dynamicSprite(remoteButton, 305, 612, 0, 0, 0x000000, 0xffffff);
    if (!remoteButtonSprite)
    {
        printf("Failed to load button\n");
        return STATE_EXIT;
    }

    Sprite *cursorSprite = create_sprite(cursor, mouse_x, mouse_y, 0, 0);
    if (!cursorSprite)
    {
        printf("Failed to load cursor\n");
        return STATE_EXIT;
    }

    clear_screen();
    backgroundSprite->draw(backgroundSprite);
    localButtonSprite->draw(localButtonSprite);
    remoteButtonSprite->draw(remoteButtonSprite);
    cursorSprite->draw(cursorSprite);
    draw();

    while (1)
    {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
        {
            printf("driver_receive failed: %d\n", r);
            continue;
        }

        if (is_ipc_notify(ipc_status))
        {
            switch (_ENDPOINT_P(msg.m_source))
            {
            case HARDWARE:
                if (msg.m_notify.interrupts & irq_set_keyboard)
                {
                    kbc_ih();
                    if (scancode == BREAK_ESC)
                    {
                        backgroundSprite->destroy(backgroundSprite);
                        localButtonSprite->destroy(localButtonSprite);
                        remoteButtonSprite->destroy(remoteButtonSprite);
                        cursorSprite->destroy(cursorSprite);
                        return STATE_MENU;
                    }
                }

                if (msg.m_notify.interrupts & irq_set_mouse)
                {
                    mouse_ih();
                    mouse_sync_bytes();
                    if (byte_index == 3)
                    {
                        byte_index = 0;
                        mouse_bytes_to_packet();
                        update_mouse_position();
                        cursorSprite->x = mouse_x;
                        cursorSprite->y = mouse_y;

                        localButtonSprite->setHover(localButtonSprite, false);
                        remoteButtonSprite->setHover(remoteButtonSprite, false);
                        cursorSprite->changeXPM(cursorSprite, cursor);

                        if (localButtonSprite->isInside(localButtonSprite, mouse_x, mouse_y))
                        {
                            localButtonSprite->setHover(localButtonSprite, true);
                            cursorSprite->changeXPM(cursorSprite, pointer);
                            if (mouse_packet.lb)
                            {
                                backgroundSprite->destroy(backgroundSprite);
                                localButtonSprite->destroy(localButtonSprite);
                                remoteButtonSprite->destroy(remoteButtonSprite);
                                cursorSprite->destroy(cursorSprite);
                                return STATE_GAME_MULTIPLAYER_LOCAL;
                            }
                        }
                        else if (remoteButtonSprite->isInside(remoteButtonSprite, mouse_x, mouse_y))
                        {
                            remoteButtonSprite->setHover(remoteButtonSprite, true);
                            cursorSprite->changeXPM(cursorSprite, pointer);
                            if (mouse_packet.lb)
                            {
                                backgroundSprite->destroy(backgroundSprite);
                                localButtonSprite->destroy(localButtonSprite);
                                remoteButtonSprite->destroy(remoteButtonSprite);
                                cursorSprite->destroy(cursorSprite);
                                return STATE_GAME_MULTIPLAYER_REMOTE;
                            }
                        }
                        
                    }
                }

                if (msg.m_notify.interrupts & irq_set_timer)
                {
                    timer_int_handler();
                    if (timer_count % 2 == 0)
                    {
                        cursorSprite->x = mouse_x;
                        cursorSprite->y = mouse_y;
                        clear_screen();
                        backgroundSprite->draw(backgroundSprite);
                        localButtonSprite->draw(localButtonSprite);
                        remoteButtonSprite->draw(remoteButtonSprite);
                        cursorSprite->draw(cursorSprite);
                        draw();
                        timer_count = 0;
                    }
                }

                break;
            }
        }
    }
    return STATE_EXIT;
}
