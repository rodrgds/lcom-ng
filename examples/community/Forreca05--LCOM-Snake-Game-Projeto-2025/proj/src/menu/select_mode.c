#include "select_mode.h"
#include "../assets/xpm/all.xpm"

extern uint8_t scancode, irq_set_keyboard, irq_set_timer, irq_set_mouse, byte_index;
extern int mouse_x, mouse_y;
extern struct packet mouse_packet;
extern int timer_count;
int ipc_status, r;
message msg;

int run_select_mode(bool* isClassic)
{
    Sprite *backgroundSprite = create_sprite(menuBackground, 0, 0, 0, 0);
    if (!backgroundSprite)
    {
        printf("Failed to load background\n");
        return STATE_EXIT;
    }

    DynamicSprite *classicButtonSprite = create_dynamicSprite(classicButton, 305, 486, 0, 0, 0x000000, 0xffffff);
    if (!classicButtonSprite)
    {
        printf("Failed to load button\n");
        return STATE_EXIT;
    }

    DynamicSprite *minhoca_IoButtonSprite = create_dynamicSprite(minhoca_ioButton, 305, 612, 0, 0, 0x000000, 0xffffff);
    if (!minhoca_IoButtonSprite)
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
    classicButtonSprite->draw(classicButtonSprite);
    minhoca_IoButtonSprite->draw(minhoca_IoButtonSprite);
    cursorSprite->draw(cursorSprite);
    draw();

    while (true)
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
                        classicButtonSprite->destroy(classicButtonSprite);
                        minhoca_IoButtonSprite->destroy(minhoca_IoButtonSprite);
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

                        classicButtonSprite->setHover(classicButtonSprite, false);
                        minhoca_IoButtonSprite->setHover(minhoca_IoButtonSprite, false);
                        cursorSprite->changeXPM(cursorSprite, cursor);

                        if (classicButtonSprite->isInside(classicButtonSprite, mouse_x, mouse_y))
                        {
                            classicButtonSprite->setHover(classicButtonSprite, true);
                            cursorSprite->changeXPM(cursorSprite, pointer);
                            if (mouse_packet.lb)
                            {
                                backgroundSprite->destroy(backgroundSprite);
                                classicButtonSprite->destroy(classicButtonSprite);
                                minhoca_IoButtonSprite->destroy(minhoca_IoButtonSprite);
                                cursorSprite->destroy(cursorSprite);
                                *isClassic = true;
                                return STATE_SELECTMODE;
                            }
                        }
                        else if (minhoca_IoButtonSprite->isInside(minhoca_IoButtonSprite, mouse_x, mouse_y))
                        {
                            minhoca_IoButtonSprite->setHover(minhoca_IoButtonSprite, true);
                            cursorSprite->changeXPM(cursorSprite, pointer);
                            if (mouse_packet.lb)
                            {
                                backgroundSprite->destroy(backgroundSprite);
                                classicButtonSprite->destroy(classicButtonSprite);
                                minhoca_IoButtonSprite->destroy(minhoca_IoButtonSprite);
                                cursorSprite->destroy(cursorSprite);
                                *isClassic = false;
                            return STATE_SELECTMODE;
                            }
                        }
                    }
                }

                if (msg.m_notify.interrupts & irq_set_timer)
                {
                    timer_int_handler();
                    if (timer_count % 3 == 0)
                    {
                        cursorSprite->x = mouse_x;
                        cursorSprite->y = mouse_y;
                        clear_screen();
                        backgroundSprite->draw(backgroundSprite);
                        classicButtonSprite->draw(classicButtonSprite);
                        minhoca_IoButtonSprite->draw(minhoca_IoButtonSprite);
                        cursorSprite->draw(cursorSprite);
                        draw();
                        timer_count = 0;
                    }

                }
            }
        }
    }
    backgroundSprite->destroy(backgroundSprite);
    classicButtonSprite->destroy(classicButtonSprite);
    minhoca_IoButtonSprite->destroy(minhoca_IoButtonSprite);
    return STATE_EXIT;
}
