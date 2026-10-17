#include "menu.h"
#include "Bitmap.h"
#include "video_gr.h"
#include "utils.h"
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>
#include "kbd.h"

struct Menu
{
    Bitmap *menu_image;
    option_st option;
};

menu_t* createMenu()
{
    menu_t *menu = (menu_t *) malloc(sizeof(menu_t));
    return menu;
}

int selectionHandler()
{
    menu_t *menu = createMenu();
    menu->option = PLAY;
    menu->menu_image = loadBitmap("/home/lcom/proj/src/resources/main_menu_play.bmp");

    int ipc_status, r, aux;
    message msg;

    int timer_irq_set = BIT(TIMER_NOTIFICATION);
    timer_subscribe_int();
    int kbd_irq_set = BIT(KBD_NOTIFICATION);
    kbd_subscribe_int();

    while (1)
    {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
        {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status))
        {
            if (_ENDPOINT_P(msg.m_source) == HARDWARE)
            {
                if (msg.NOTIFY_ARG & timer_irq_set)
                {
                    memset(double_buffer, 0, getHRES() * getVRES() * getBitsPerPixel() / 8);

                    drawBitmap(menu->menu_image, 0, 0, ALIGN_LEFT);

                    changeVideoMem();
                }

                if (msg.NOTIFY_ARG & kbd_irq_set)
                {
                    unsigned long scancode = kbd_handler();

                    if (scancode == ENTER_MAKECODE)
                    {
                        if (menu->option == PLAY)
                        {
                            aux = 1;
                            break;
                        } else if (menu->option == _EXIT)
                        {
                            aux = 2;
                            break;
                        }
                    } else
                    {
                        if (menu->option == PLAY && scancode == ARROW_UP_MAKECODE)
                        {
                            menu->option = _EXIT;
                            menu->menu_image = loadBitmap("/home/lcom/proj/src/resources/main_menu_exit.bmp");
                        } else if (menu->option == PLAY && scancode == ARROW_DOWN_MAKECODE)
                        {
                            menu->option = _EXIT;
                            menu->menu_image = loadBitmap("/home/lcom/proj/src/resources/main_menu_exit.bmp");
                        } else if (menu->option == _EXIT && scancode == ARROW_UP_MAKECODE)
                        {
                            menu->option = PLAY;
                            menu->menu_image = loadBitmap("/home/lcom/proj/src/resources/main_menu_play.bmp");
                        } else if (menu->option == _EXIT && scancode == ARROW_DOWN_MAKECODE)
                        {
                            menu->option = PLAY;
                            menu->menu_image = loadBitmap("/home/lcom/proj/src/resources/main_menu_play.bmp");
                        }
                    }
                }
            }
        }
    }

    kbd_unsubscribe_int();
    timer_unsubscribe_int();
    deleteBitmap(menu->menu_image);
    return aux;
}

int pauseMenu()
{
    menu_t *menu = createMenu();
    menu->option = RESUME;
    menu->menu_image = loadBitmap("/home/lcom/proj/src/resources/pause_menu_resume.bmp");

    int ipc_status, r, aux;
    message msg;

    int timer_irq_set = BIT(TIMER_NOTIFICATION);
    timer_subscribe_int();
    int kbd_irq_set = BIT(KBD_NOTIFICATION);
    kbd_subscribe_int();

    while (1)
    {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
        {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status))
        {
            if (_ENDPOINT_P(msg.m_source) == HARDWARE)
            {
                if (msg.NOTIFY_ARG & timer_irq_set)
                {
                    memset(double_buffer, 0, getHRES() * getVRES() * getBitsPerPixel() / 8);
                    drawBitmap(menu->menu_image, 0, 0, ALIGN_LEFT);
                    changeVideoMem();
                }

                if (msg.NOTIFY_ARG & kbd_irq_set)
                {
                    unsigned long scancode = kbd_handler();

                    if (scancode == ENTER_MAKECODE)
                    {
                        if (menu->option == RESUME)
                        {
                            aux = 1;
                            break;
                        } else if (menu->option == BACK_MENU)
                        {
                            aux = 2;
                            break;
                        }
                    } else
                    {
                        if (menu->option == RESUME && scancode == ARROW_UP_MAKECODE)
                        {
                            menu->option = BACK_MENU;
                            menu->menu_image = loadBitmap("/home/lcom/proj/src/resources/pause_menu_backmenu.bmp");
                        }
                        else if (menu->option == RESUME && scancode == ARROW_DOWN_MAKECODE)
                        {
                            menu->option = BACK_MENU;
                            menu->menu_image = loadBitmap("/home/lcom/proj/src/resources/pause_menu_backmenu.bmp");
                        }
                        else if (menu->option == BACK_MENU && scancode == ARROW_DOWN_MAKECODE)
                        {
                            menu->option = RESUME;
                            menu->menu_image = loadBitmap("/home/lcom/proj/src/resources/pause_menu_resume.bmp");
                        }
                        else if (menu->option == BACK_MENU && scancode == ARROW_UP_MAKECODE)
                        {
                            menu->option = RESUME;
                            menu->menu_image = loadBitmap("/home/lcom/proj/src/resources/pause_menu_resume.bmp");
                        }
                    }
                }
            }
        }
    }

    free(menu);
    return aux;
}
