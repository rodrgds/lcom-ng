#include "game.h"
#include <stdio.h>
#include <stdlib.h>

static int counter = 0;

//2 numeros maximo
int draw_score(int points,Sprite **numbers)
{
    
    int points_save = points;
    uint8_t size = 0;
    while(points_save)
    {
        size++;
        points_save /= 10;
    }

    int score[2];
    
    points_save = points;

    for(int i = 0; i < 2; i++)
    {
        if(i > size)
            score[i] = 0;
        else
        {
            score[i] = points_save % 10;
            points_save /= 10;
        } 
    }

    for(int i = 0; i < 2; i++)
    {
        switch(score[i])
        {
            case 0:
                numbers[0]->x = 400 - 64 * i;
                draw_sprite(numbers[0]);
                break;
            case 1:
                numbers[1]->x = 400 - 64 * i;
                draw_sprite(numbers[1]);
                break;
            case 2:
                numbers[2]->x = 400 - 64 * i;
                draw_sprite(numbers[2]);
                break;
            case 3:
                numbers[3]->x = 400 - 64 * i;
                draw_sprite(numbers[3]);
                break;
            case 4:
                numbers[4]->x = 400 - 64 * i;
                draw_sprite(numbers[4]);
                break;
            case 5:
                numbers[5]->x = 400 - 64 * i;
                draw_sprite(numbers[5]);
                break;
            case 6:
                numbers[6]->x = 400 - 64 * i;
                draw_sprite(numbers[6]);
                break;
            case 7:
                numbers[7]->x = 400 - 64 * i;
                draw_sprite(numbers[7]);
                break;
            case 8:
                numbers[8]->x = 400 - 64 * i;
                draw_sprite(numbers[8]);
                break;
            case 9:
                numbers[9]->x = 400 - 64 * i;
                draw_sprite(numbers[9]);
                break;
            default:
                return 1;    
        }
    }

    return 0;
}

void game_over(uint8_t won,Sprite *game_over_logo,Sprite *lose_logo, Sprite *win_logo,Sprite *continue_logo)
{
    counter++;
    counter %= 120;
    reset_to_background();
    
    if(won)
        draw_sprite(win_logo);
    else
        draw_sprite(lose_logo);

    if(counter <= 60)
        draw_sprite(continue_logo);
    draw_sprite(game_over_logo);
    
    double_buff();

    
}


int detect_colisions(Sprite* cursor,uint16_t x,uint16_t y, uint16_t width,uint16_t height)
{
    if(cursor->x < x || cursor->x > x + width)
        return 0;
    if(cursor->y < y || cursor->y > y + height)
        return 0;
    return 1;
}



int breakout(state* game_state)
{
    //interrupts stuff
    uint8_t keyboard_bit_no = IRQ1;
    int     keyboard_bit_mask = BIT(IRQ1);
    if (kbd_subscribe_int(&keyboard_bit_no))
        return 1;

    uint8_t timer_bit_no = TIMER0_IRQ;
    int     timer_bit_mask = BIT(TIMER0_IRQ);
    if(timer_subscribe_int(&timer_bit_no))
        return 1;
    
    //MENU------------
    int     mouse_bit_mask = BIT(12);
    if(send_cmd_mouse(ENABLE_DATA))
        return 1;
    if(mouse_subscribe_int())
        return 1;
    //MENU------------



    timer_set_frequency(0,60);

    //---------------------------------------

    //initializing game stuff
    uint8_t lives = 3;
    uint8_t end_game = 0;
    uint16_t score = 0;
    bool init = true;

    //standard entities
    Sprite* game_over_logo = create_sprite(game_over_xpm,200,100,0,0);
    Sprite* win_logo = create_sprite(win_xpm,220,230,0,0);
    Sprite* lose_logo = create_sprite(lose_xpm,220,230,0,0);
    Sprite* continue_logo = create_sprite(continue_xpm,200,450,0,0);
    Ball* ball = create_ball(rand() % 700,400);
    Sprite* platform = create_sprite(paddle_xpm,400, 550,0,0);

    Sprite* numbers[10];
    numbers[0] = create_sprite(zero_xpm,400,10,0,0);
    numbers[1] = create_sprite(one_xpm,400,10,0,0);
    numbers[2] = create_sprite(two_xpm,400,10,0,0);
    numbers[3] = create_sprite(three_xpm,400,10,0,0);
    numbers[4] = create_sprite(four_xpm,400,10,0,0);
    numbers[5] = create_sprite(five_xpm,400,10,0,0);
    numbers[6] = create_sprite(six_xpm,400,10,0,0);
    numbers[7] = create_sprite(seven_xpm,400,10,0,0);
    numbers[8] = create_sprite(eight_xpm,400,10,0,0);
    numbers[9] = create_sprite(nine_xpm,400,10,0,0);
    Sprite* lives_sprite[3];

    for (uint8_t i = 0; i < 3; i++)
    {
        lives_sprite[i] = create_sprite(heart00_xpm,vmi_p.XResolution - 210 + i * 32,16,0,0);
    }

    
 
    Brick* bricks[30];
    uint16_t x = 64;uint16_t y = 150;
    for(int i = 0; i < 3 ; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            bricks[10 * i + j] = create_brick(x * (j + 1),32 * i + y);
        }
    }

    AnimSprite* explosion[30];



    //MENU-----------------
    
    Sprite *cursor = create_sprite(cursor_xpm,400,300,0,0);
    Sprite *logo = create_sprite(logo_xpm,200,100,0,0);
    Sprite *play = create_sprite(play_xpm,300,230,0,0);
    Sprite *hovering_play = create_sprite(hovering_play_xpm,300,230,0,0);
    Sprite *exit = create_sprite(exit_xpm,300,320,0,0);
    Sprite *hovering_exit = create_sprite(hovering_exit_xpm,300,230,0,0);



    //save of the original xpm
    uint8_t* play_map = play->map;
    uint8_t* exit_map = exit->map;


    //MENU-----------------


    alloc_background();


    
    //---------------------------------------

    int ipc_status,r;
    message msg;
    while (!end_game) {
        // Get a request message
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
        printf("driver_receive failed with: %d", r);
        continue;
        }
        if (is_ipc_notify(ipc_status)) { // received notification
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: // hardware interrupt notification
            if (msg.m_notify.interrupts & keyboard_bit_mask) { // subscribed interrupt
                kbc_get_scancode();
                kbc_verify_scancode();
            }
            if (msg.m_notify.interrupts & mouse_bit_mask)
            {
                if(analize_buffer())
                    return 1;
                if(process_packet())
                    return 1;
                if(complete)
                {
                    parse_packet();
                    if(*game_state == MENU)
                    {
                        if(!(cursor->x + mouse_packets.delta_x <= 0))
                            cursor->x += mouse_packets.delta_x;
                        if(!(cursor->y - mouse_packets.delta_y <= 0))
                            cursor->y -= mouse_packets.delta_y;
                        if(cursor->x + cursor->width >= vmi_p.XResolution)
                            cursor->x = vmi_p.XResolution - cursor->width;
                        if(cursor->y + cursor->height >= vmi_p.YResolution)
                            cursor->y = vmi_p.YResolution - cursor->height;
                        if(detect_colisions(cursor,play->x,play->y,play->width,play->height))
                        {
                            play->map = hovering_play->map;    
                            if(mouse_packets.lb)
                            {
                                (*game_state) = GAME;
                            }
                        }
                        else if(detect_colisions(cursor,exit->x,exit->y,exit->width,exit->height))
                        {
                            exit->map = hovering_exit->map;
                            if(mouse_packets.lb)
                            {
                                (*game_state) = EXIT;
                                end_game = 1;
                            }
                        }
                        else
                        {
                            play->map = play_map;
                            exit->map = exit_map;                    
                        }
                    }
                }

            }
            if (msg.m_notify.interrupts & timer_bit_mask) {
                if(*game_state == MENU)
                {
                    if(init)
                    {
                        draw_background(true);
                        init = false;
                    }
                    reset_to_background();
                    draw_sprite(play);
                    draw_sprite(exit);
                    draw_sprite(logo);
                    draw_sprite(cursor);
                    double_buff();
                }
                else
                {
                    counter++;
                    if(!lives || score == 30)
                    {
                        game_over(score == 30,game_over_logo,lose_logo,win_logo,continue_logo);
                        if(complete_scancode)
                            if(scancode == ESC_KEY)
                            {
                                *game_state = MENU;
                                end_game = 1;
                            }
                    }
                    else
                    {
                        //painting the buffer with the background
                        if(init)
                        {
                            draw_background(true);
                            init = false;
                        }
                        
                        
                        //clearing the screen
                        reset_to_background();


                        //ball physics
                        ball->old_x = ball->x;
                        ball->old_y = ball->y;

                        ball->x += ball->dx;
                        ball->y += ball->dy;

                        if(ball->y >= vmi_p.YResolution)
                            ball->y = 0;
                        if(ball->x >= vmi_p.XResolution)
                            ball->x = 0;

                        //-----------


                        //making the paddle move
                        if (complete_scancode){
                            switch(scancode)
                            {
                                case A_KEY:
                                    //if x <= origin + 5
                                    if(!(platform->x - 5 <= 0))
                                        platform->x -= 6.0f;
                                    break;
                                case D_KEY:
                                    if(!(platform->x + 5 + platform->width >= vmi_p.XResolution))
                                        platform->x += 6.0f;
                                    break;
                            }
                        }
                        //-----------

                        //borders colisions
                        if(ball->x + ball->width >= vmi_p.XResolution || ball->x <= 0)
                            ball->dx *= -1;
                        if(ball->y + ball->height >= vmi_p.YResolution || ball->y <= 0)
                            ball->dy *= -1;
                        //-----------
                        
                        //paddle colisions
                        
                        if((ball->x >= platform->x && ball->x <= platform->x + platform->width && ball->y >= platform->y && ball->y <= platform->y + platform->height))
                        {
                            float magnitude = pow(((float)ball->dx * (float)ball->dx + (float)ball->dy * (float)ball->dy) ,(1/2));
                            float vx = (float)ball->dx / magnitude;
                            float vy = (float)ball->dy / magnitude;
                            float t = vy * (-1) + vx * 0;
                            if(t <= 0)
                            {
                                ball->dy *= -1;
                            }

                        }


                        if(ball->y >= platform->y + platform->height)
                        {
                            ball->y = 300;
                            ball->dx = 3;
                            ball->dy = 3;
                            lives--;
                        }
                        //-------------
                        

                        bool no_colisions = false;

                        //draw_ball(ball);
                        for(int i = 0; i < 30; i++)
                        {   
                            
                            if(!no_colisions && !bricks[i]->is_destroyed && (bricks[i]->x <= ball->x && bricks[i]->x + bricks[i]->width >= ball->x && bricks[i]->y <= ball->y && bricks[i]->y + bricks[i]->height >= ball->y))
                            {
                                
                                bricks[i]->is_destroyed = true;
                                no_colisions = true;

                                
                                //left side
                                if(ball->old_x <= bricks[i]->x && ball->x  >= bricks[i]->x)
                                    ball->dx *= -1;
                                //right side
                                else if(ball->old_x >= bricks[i]->x + bricks[i]->width && ball->x <= bricks[i]->x + bricks[i]->width)
                                    ball->dx *= -1;
                                //up side
                                else if(ball->old_y <= bricks[i]->y && ball->y  >= bricks[i]->y)
                                    ball->dy *= -1;
                                //down side
                                else if(ball->old_y > bricks[i]->y + bricks[i]->height && ball->y <= bricks[i]->y + bricks[i]->height)
                                    ball->dy *= -1;
                                
                                


                                score += 1;
                                explosion[i] = create_animSprite(bricks[i]->x,bricks[i]->y,8,explosao0,explosao1,explosao2,explosao3,explosao4,explosao5,explosao6,explosao7);
                            }
                            if(!bricks[i]->is_destroyed)
                            {
                                draw_brick(bricks[i]);
                            }
                            else
                            {
                                if(explosion[i]->cur_fig != 7)
                                    animate_sprite(explosion[i]);
                            }
                            
                        }

                        

                        for(uint8_t i = 0; i < lives; i++)
                            draw_sprite(lives_sprite[i]);

                        draw_sprite(platform);
                        draw_ball(ball);
                        draw_score(score,numbers);
                        
                        double_buff();
                    }
                }
            }
            break;

            default:
            break; // no other notifications expected: do nothing
        }
        }
        else { //received a standard message, not a notification
        // no standard messages expected: do nothing
        }
    }
    if (kbd_unsubscribe_int())
        return 1;
    if (timer_unsubscribe_int())
        return 1;
    
    //menu
    if (mouse_unsubscribe_int())
        return 1;
    if(send_cmd_mouse(DISABLE_DATA))
        return 1;
    //------

    for(int i = 0 ; i < 10; i++)
        destroy_sprite(numbers[i]); 
    destroy_sprite(platform);
    destroy_sprite(game_over_logo);
    destroy_sprite(win_logo);
    destroy_sprite(lose_logo);
    destroy_sprite(continue_logo);
    
    destroy_ball(ball);
    for(int i = 0; i < 30; i++)
        destroy_brick(bricks[i]);
    destroy_sprite(cursor);
    destroy_sprite(logo);
    destroy_sprite(play);
    destroy_sprite(hovering_play);
    destroy_sprite(exit);
    destroy_sprite(hovering_exit);
    


    return 0;
}
