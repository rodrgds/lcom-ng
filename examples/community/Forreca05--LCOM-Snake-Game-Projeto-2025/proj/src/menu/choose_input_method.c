#include "choose_input_method.h"
#include "../assets/xpm/all.xpm"

extern uint8_t scancode, irq_set_keyboard, irq_set_timer, irq_set_mouse, byte_index;
extern int mouse_x, mouse_y;
extern struct packet mouse_packet;
extern int timer_count;
int ipc_status, r;
message msg;

uint32_t selectColor=0x00ff00;

int run_choose_input_method(int *option, bool isMultiplayer, bool isClassic)
{
    if (isClassic) {
        *option = 2;
        return STATE_GAME_SINGLEPLAYER_CLASSIC;
    }

    Sprite *instructionBackground = create_sprite(instructionBackground_xpm, 0, 0, 0, 0);

    Text *chooseText = create_Text("CHOOSE", 359, 152, 10);
    Sprite *cursorSprite = create_sprite(cursor, mouse_x, mouse_y, 0, 0);
    DynamicSprite *mouseSprite=create_dynamicSprite(mouse_xpm,203,280,0,0,0xffffff,selectColor);
    DynamicSprite *keyboardSprite=create_dynamicSprite(keyboard_xpm,705,357,0,0,0xffffff,selectColor);
    Text * option1Text=create_Text("1 Mouse",64,748,0);    
    Text * option2Text=create_Text("2 Keys",672,748,0);
    instructionBackground->draw(instructionBackground);
    chooseText->draw(chooseText);
    mouseSprite->draw(mouseSprite);
    keyboardSprite->draw(keyboardSprite);
    option1Text->draw(option1Text);
    option2Text->draw(option2Text);
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
                    if (scancode == KEY_1)
                    {
                        instructionBackground->destroy(instructionBackground);
                        cursorSprite->destroy(cursorSprite);
                        chooseText->destroy(chooseText);
                        option1Text->destroy(option1Text);
                        option2Text->destroy(option2Text); 
                        *option = 1;
                        return (isClassic ? STATE_GAME_SINGLEPLAYER_CLASSIC : STATE_GAME_SINGLEPLAYER_IO);
                    }
                    else if (scancode == KEY_2)
                    {
                        instructionBackground->destroy(instructionBackground);
                        cursorSprite->destroy(cursorSprite);
                        chooseText->destroy(chooseText);
                        option1Text->destroy(option1Text);
                        option2Text->destroy(option2Text);

                        *option = 2;
                        return (isClassic ? STATE_GAME_SINGLEPLAYER_CLASSIC : STATE_GAME_SINGLEPLAYER_IO);
                    }
                    else if (scancode == BREAK_ESC)
                    {
                        instructionBackground->destroy(instructionBackground);
                        chooseText->destroy(chooseText);
                        cursorSprite->destroy(cursorSprite);
                        option1Text->destroy(option1Text);
                        option2Text->destroy(option2Text);
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
                        if(mouseSprite->isInside(mouseSprite,mouse_x,mouse_y)){
                            mouseSprite->setHover(mouseSprite,true);
                            xpm_image_t img;
                            cursorSprite->map = (uint32_t *)xpm_load(pointer, XPM_8_8_8_8, &img);
                            if (mouse_packet.lb)
                            {
                                instructionBackground->destroy(instructionBackground);
                                chooseText->destroy(chooseText);
                                mouseSprite->destroy(mouseSprite);
                                keyboardSprite->destroy(keyboardSprite);
                                cursorSprite->destroy(cursorSprite);
                                option1Text->destroy(option1Text);
                                option2Text->destroy(option2Text);
                                *option = 1;
                                return (isClassic ? STATE_GAME_SINGLEPLAYER_CLASSIC : STATE_GAME_SINGLEPLAYER_IO);
                            }
                        }
                        else if(keyboardSprite->isInside(keyboardSprite,mouse_x,mouse_y)){
                            keyboardSprite->setHover(keyboardSprite,true);
                            xpm_image_t img;
                            cursorSprite->map = (uint32_t *)xpm_load(pointer, XPM_8_8_8_8, &img);
                            if (mouse_packet.lb)
                            {
                                instructionBackground->destroy(instructionBackground);
                                chooseText->destroy(chooseText);
                                mouseSprite->destroy(mouseSprite);
                                keyboardSprite->destroy(keyboardSprite);
                                cursorSprite->destroy(cursorSprite);
                                option1Text->destroy(option1Text);
                                option2Text->destroy(option2Text);
                                *option = 2;
                                return (isClassic ? STATE_GAME_SINGLEPLAYER_CLASSIC : STATE_GAME_SINGLEPLAYER_IO);
                            }

                        }else
                        {
                            xpm_image_t img;
                            cursorSprite->map = (uint32_t *)xpm_load(cursor, XPM_8_8_8_8, &img);
                            mouseSprite->setHover(mouseSprite,false);
                            keyboardSprite->setHover(keyboardSprite,false);
                        }
                    }
                }
                if (msg.m_notify.interrupts & irq_set_timer)
                {
                    timer_int_handler();
                    if (timer_count % 2 == 0) {
                    cursorSprite->x = mouse_x;
                    cursorSprite->y = mouse_y;
                    clear_screen();
                    instructionBackground->draw(instructionBackground);
                    chooseText->draw(chooseText);
                    mouseSprite->draw(mouseSprite);
                    keyboardSprite->draw(keyboardSprite);
                    option1Text->draw(option1Text);
                    option2Text->draw(option2Text);
                    cursorSprite->draw(cursorSprite);
                    draw();
                    timer_count = 0;
                    }
                }
            }
        }
    }
    instructionBackground->destroy(instructionBackground);
    chooseText->destroy(chooseText);
    cursorSprite->destroy(cursorSprite);
    return STATE_EXIT;
}
