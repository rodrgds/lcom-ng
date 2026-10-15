#include <lcom/lcf.h>
#include "graphics.h"
#include "sprite.h"
#include "keyboard.h"
#include "macros.h"
#include "mouse.h"
#include "menu.h"
#include "ps2.h"
#include "game.h"
#include "../assets/xpm.h"

Sprite *m_bg = NULL;
Sprite *m_title = NULL;
Sprite *m_play = NULL;
Sprite *m_instructions = NULL;
Sprite *m_about = NULL;
Sprite *m_aboutMenu = NULL;
Sprite *m_playHover = NULL;
Sprite *m_instructionsHover = NULL;
Sprite *m_instructionsMenu = NULL;
Sprite *m_aboutHover = NULL;
Sprite *m_mouse = NULL;
Mouse *mouse = NULL;
MenuObject *menu = NULL;
int interruptPerFrame = TIMER_DFLT_FREQ / FPS;
uint8_t kbd_bit = KEYBOARD_IRQ;
uint8_t timerBit = TIMER0_IRQ;
uint16_t mouseBit = MOUSE_IRQ;

MenuObject *createMenu()
{
    MenuObject *menu = (MenuObject *)malloc(sizeof(Menu));
    menu->mState = Selecting;
    menu->menuOption = 1;
    return menu;
}

int menuLoop()
{
    int ipc_status, rec;
    message msg;
    initMenu();
    scannedKey = 0;
    while (scannedKey != ESC)
    {
        if ((rec = driver_receive(ANY, &msg, &ipc_status)) != 0)
        {
            printf("driver_receive failed with: %d", rec);
            continue;
        }
        if (is_ipc_notify(ipc_status))
        {
            switch (_ENDPOINT_P(msg.m_source))
            {
            case HARDWARE:
                if (msg.m_notify.interrupts & kbd_bit)
                {
                    kbc_ih();
                    menuInput();
                    if (menu->mState == Go)
                    {
                        return Playing;
                    }
                }
                if (msg.m_notify.interrupts & timerBit)
                {
                    timer_int_handler();
                    updateMenuScreen();
                }
                if (msg.m_notify.interrupts & mouseBit)
                {
                    mouse_ih();
                    if (mouse_evaluate_bytes())
                    {
                        if (handleMouse())
                        {
                            menuInput();
                            if (menu->mState == Go)
                            {
                                return Playing;
                            }
                        }
                    }
                }
                break;
            default:
                break;
            }
        }
    }

    return 0;
}
int initMenu()
{
    menu = createMenu();
    m_bg = create_sprite(background, 800, 300);
    m_title = create_sprite(title, 400, 125);
    m_play = create_sprite(play, 400, 350);
    m_playHover = create_sprite(playHover, 400, 350);
    m_about = create_sprite(about, 400, 500);
    m_aboutMenu = create_sprite(aboutMenu, 400, 300);
    m_aboutHover = create_sprite(aboutHover, 400, 500);
    m_instructions = create_sprite(instructions, 400, 425);
    m_instructionsHover = create_sprite(instructionsHover, 400, 425);
    m_instructionsMenu = create_sprite(instructionsMenu, 400, 300);
    m_mouse = create_sprite(cursor, 400, 300);
    mouse = create_mouse(400, 300, m_mouse);
    set_sprite_speed(m_bg, 800, 300, 0, 300, -1);
    return 0;
}

void updateMenuScreen()
{
    if (interruptCounter % interruptPerFrame == 0)
    {
        clearScreen();
        if (m_bg->x != 0)
        {
            update_sprite(m_bg, 0, 300);
        }
        else
        {
            m_bg->x = 800;
        }
        if (menu->mState == Instructions)
        {
            drawInstructions();
        }
        else if (menu->mState == About)
        {
            drawAbout();
        }
        else
        {
            drawMenu();
            draw_mouse(mouse);
        }
        flipBuffer();
    }
}

int menuInput()
{

    if (scannedKey == ENTER || mouse->lb_status)
    {
        if (menu->mState == Selecting)
        {
            if (menu->menuOption == 1)
            {
                menu->mState = Go;
            }
            else if (menu->menuOption == 2)
            {
                menu->mState = Instructions;
            }
            else if (menu->menuOption == 3)
            {
                menu->mState = About;
            }
        }
        else
        {
            menu->mState = Selecting;
        }
    }
    else if (scannedKey == ARR_UP || scannedKey == ARR_DOWN)
    {
        kbd_menu_input(&menu->menuOption);
    }
    return 0;
}

int handleMouse()
{
    int x, y;
    initialize_packet(&pp, mouseBytes);
    
    mouse->lb_status = pp.lb;
    if (!pp.x_ov && !pp.y_ov)
    {
        setMouseX(mouse, mouse->sprite->x + pp.delta_x);
        setMouseY(mouse, mouse->sprite->y - pp.delta_y);
    }
    
    x = mouse->sprite->x;
    y = mouse->sprite->y;
    if (y >= 330 && y <= 520)
    {
        if ((y >= 370 && y <= 405) || (y >= 445 && y <= 480))
        {
            return 0;
        }
        else if (y < 370 && (x >= 325 && x <= 475))
        {
            menu->menuOption = 1;
            return 1;
        }
        else if (y < 445 && (x >= 166 && x <= 634))
        {
            menu->menuOption = 2;
            return 1;
        }
        else if (y <= 520 && (x >= 296 && x <= 403))
        {
            menu->menuOption = 3;

            return 1;
        }
    }
    if (mouse->lb_status)
    {
        return 1;
    }

    return 0;
}

void drawMenu()
{
    draw_sprite(m_bg);
    draw_sprite(m_title);
    draw_sprite(m_play);
    draw_sprite(m_instructions);
    draw_sprite(m_about);
    switch (menu->menuOption)
    {
    case 1:
        draw_sprite(m_playHover);
        break;
    case 2:
        draw_sprite(m_instructionsHover);
        break;
    case 3:
        draw_sprite(m_aboutHover);
        break;
    default:
        break;
    }
}

void drawInstructions()
{
    draw_sprite(m_bg);
    draw_sprite(m_instructionsMenu);
}
void drawAbout()
{
    draw_sprite(m_bg);
    draw_sprite(m_aboutMenu);
}
