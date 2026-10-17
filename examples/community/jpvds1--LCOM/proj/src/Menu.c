#include "Menu.h"

int selected;
int dead_selected;

int draw_menu()
{
    clear_buffer();
    switch (selected)
    {
    case 0:
        draw_xpm(logo_sprite, logo_img, 0, 100);
        draw_xpm(levels_sel_sprite, levels_sel_img, 340, 350);
        draw_xpm(endless_sprite, endless_img, 340, 500);
        draw_xpm(instructions_sprite, instructions_img, 340, 650);
        draw_xpm(exit_sprite, exit_img, 340, 800);
        break;
    case 1:
        draw_xpm(logo_sprite, logo_img, 0, 100);
        draw_xpm(levels_sprite, levels_img, 340, 350);
        draw_xpm(endless_sel_sprite, endless_sel_img, 340, 500);
        draw_xpm(instructions_sprite, instructions_img, 340, 650);
        draw_xpm(exit_sprite, exit_img, 340, 800);
        break;
    case 2:
        draw_xpm(logo_sprite, logo_img, 0, 100);
        draw_xpm(levels_sprite, levels_img, 340, 350);
        draw_xpm(endless_sprite, endless_img, 340, 500);
        draw_xpm(instructions_sel_sprite, instructions_sel_img, 340, 650);
        draw_xpm(exit_sprite, exit_img, 340, 800);
        break;
    case 3:
        draw_xpm(logo_sprite, logo_img, 0, 100);
        draw_xpm(levels_sprite, levels_img, 340, 350);
        draw_xpm(endless_sprite, endless_img, 340, 500);
        draw_xpm(instructions_sprite, instructions_img, 340, 650);
        draw_xpm(exit_sel_sprite, exit_sel_img, 340, 800);
        break;
    default:
        return 1;
        break;
    }
    video_switch();
    return 0;
}

int menu_interrupt_loop()
{
    draw_menu();
    int r,i=0;
    uint8_t bytes[2];
    //interrupt loop
    int ipc_status;
    message msg;
    while(scancode != KBC_BRK_ESC_KEY) //only exits with ESC
    { /* You may want to use a different condition */
        /* Get a request message. */
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) 
        {
        case HARDWARE: /* hardware interrupt notification */	
            if (msg.m_notify.interrupts & irq_set_kbc) //keyboard interrupts
            { /* subscribed interrupt */
                //get the scanconde
                kbc_ih();
                if(scancode == KBC_2BYTE_CODE){
                    bytes[i] = scancode;
                    i++;
                    continue;
                }
                bytes[i] = scancode;
                i=0;
                
                if(scancode == KBC_W_MAKE_KEY){
                    if(selected != 0)
                    {
                        selected--;
                    }
                }
                else if(scancode == KBC_S_MAKE_KEY)
                {
                    if(selected < 3)
                    {
                        selected++;
                    }
                }
                else if(scancode == KBC_ENTER_MAKE_KEY)
                {
                    return call_level(selected);
                }
            }
            if (msg.m_notify.interrupts & irq_set_timer) {
                clear_buffer();
                draw_menu();
                if (vg_draw_rectangle(mouse_x, mouse_y, 10, 10, 0xffffff)) return 1;
                video_switch();
            }
            if (msg.m_notify.interrupts & irq_set_mouse) {
                mouse_ih();

                if (mouse_counter == 3)  {
                    mouse_counter = 0;
                    mouse_write_to_packet();
                    mouse_update_coords();
                    if(mouse_x > 200 && mouse_x < 1080)
                    {
                        if(mouse_y >= 350 && mouse_y <= 450)
                        {
                            selected = 0;
                        }
                        else if(mouse_y >= 500 && mouse_y <= 600)
                        {
                            selected = 1;
                        }
                        else if(mouse_y >= 650 && mouse_y <= 750)
                        {
                            selected = 2;
                        }
                        else if(mouse_y >= 800 && mouse_y <= 900)
                        {
                            selected = 3;
                        }
                    }
                    if (mousePacket.lb)  {return call_level(selected);}
                }
            }
            break;
        default:
            break; /* no other notifications expected: do nothing */	
        }
        } 
        else 
        { /* received a standard message, not a notification */
            /* no standard messages expected: do nothing */
        }
    }
    return 0;
}

int dead_draw_menu()
{
    clear_buffer();
    switch (dead_selected)
    {
    case 0:
        draw_xpm(menu_selected_sprite, menu_selected_img, 340, 200);
        draw_xpm(exit_sprite, exit_img, 340, 600);
        break;
    case 1:
        draw_xpm(menu_sprite, menu_img, 340, 200);
        draw_xpm(exit_sel_sprite, exit_sel_img, 340, 600);
        break;
    default:
        return 1;
        break;
    }
    video_switch();
    return 0;
}

int dead_menu_interrupt_loop()
{
    dead_draw_menu();
    int r,i=0;
    uint8_t bytes[2];
    //interrupt loop
    int ipc_status;
    message msg;
    while(scancode != KBC_BRK_ESC_KEY) //only exits with ESC
    { /* You may want to use a different condition */
        /* Get a request message. */
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) 
        {
        case HARDWARE: /* hardware interrupt notification */				
            if (msg.m_notify.interrupts & irq_set_kbc) //keyboard interrupts
            { /* subscribed interrupt */
                //get the scanconde
                kbc_ih();
                if(scancode == KBC_2BYTE_CODE){
                    bytes[i] = scancode;
                    i++;
                    continue;
                }
                bytes[i] = scancode;
                i=0;
                
                if(scancode == KBC_W_MAKE_KEY){
                    if(dead_selected != 0)
                    {
                        dead_selected--;
                    }
                }
                else if(scancode == KBC_S_MAKE_KEY)
                {
                    if(dead_selected < 1)
                    {
                        dead_selected++;
                    }
                }
                else if(scancode == KBC_ENTER_MAKE_KEY)
                {
                    if(dead_selected == 0)
                    {
                        return call_level(4);
                    }
                    else
                    {
                        return call_level(3);
                    }
                }
            }
            if (msg.m_notify.interrupts & irq_set_timer) {
                clear_buffer();
                dead_draw_menu();
                if (vg_draw_rectangle(mouse_x, mouse_y, 10, 10, 0xffffff)) return 1;
                video_switch();
            }
            if (msg.m_notify.interrupts & irq_set_mouse) {
                mouse_ih();

                if (mouse_counter == 3)  {
                    mouse_counter = 0;
                    mouse_write_to_packet();
                    mouse_update_coords();
                    if(mouse_x > 200 && mouse_x < 1080)
                    {
                        if(mouse_y >= 200 && mouse_y <= 300)
                        {
                            dead_selected = 0;
                        }
                        else if(mouse_y >= 600 && mouse_y <= 700)
                        {
                            dead_selected = 1;
                        }
                    }
                    if (mousePacket.lb)  {
                        if(dead_selected == 0)
                        {
                            return call_level(4);
                        }
                        else
                        {
                            return call_level(3);
                        }
                    }
                }
            }
            break;
        default:
            break; /* no other notifications expected: do nothing */	
        }
        } 
        else 
        { /* received a standard message, not a notification */
            /* no standard messages expected: do nothing */
        }
    }
    return 0;
}

int instructions_loop()
{
    bool end = false;
    draw_xpm(inside_instructions_sprite, inside_instructions_img, 0, 0);
    video_switch();
    int r,i=0;
    uint8_t bytes[2];
    //interrupt loop
    int ipc_status;
    message msg;
    while(scancode != KBC_BRK_ESC_KEY && !end) //only exits with ESC
    { /* You may want to use a different condition */
        /* Get a request message. */
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) 
        {
        case HARDWARE: /* hardware interrupt notification */				
            if (msg.m_notify.interrupts & irq_set_kbc) //keyboard interrupts
            { /* subscribed interrupt */
                //get the scanconde
                kbc_ih();
                if(scancode == KBC_2BYTE_CODE){
                    bytes[i] = scancode;
                    i++;
                    continue;
                }
                bytes[i] = scancode;
                i=0;
                if(scancode == KBC_Q_MAKE_KEY){
                    end = true;
                }
            }
            if (msg.m_notify.interrupts & irq_set_mouse) {
                mouse_ih();
                if (mouse_counter == 3)  {
                    mouse_counter = 0;
                    mouse_write_to_packet();
                    mouse_update_coords();
                }
            }
            break;
        default:
            break; /* no other notifications expected: do nothing */	
        }
        } 
        else 
        { /* received a standard message, not a notification */
            /* no standard messages expected: do nothing */
        }
    }
    return 0;
}
