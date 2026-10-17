#include <lcom/lcf.h>
#include "menu.h"

extern Cursor* cursor;

extern Sprite* background;
extern Sprite* gameName;
extern Sprite* gameOver;
extern Sprite* sun;

extern Sprite* playButton;
extern Sprite* leaderboardButton;
extern Sprite* instructionsButton;
extern Sprite* quitButton;
extern Sprite* quitButton2;
extern Sprite* playButton2;
extern Sprite* leaderboardButton2;
extern Sprite* instructionsButton2;
extern Sprite* backButton;
extern Sprite* backButton2;
extern Sprite* instructions;


extern Sprite* numberArray[10];
extern Sprite* twoDots;
extern Sprite* slash;


extern timeType timeData;
extern dateType dateData;

menuSelectedState selected_menu = INITIAL;

void draw_menu(menu menu) {
    draw_sprite(background);
    draw_time();

    //Draw Sun during the day
        if ((timeData.hours > 6) && (timeData.hours < 20)) {
            draw_sprite(sun);
        }

    if (menu == MAIN) {
        
        draw_sprite(gameName);

        

        switch (selected_menu)
        {
        case HOVER_PLAY:
            draw_sprite(playButton2);
            draw_sprite(leaderboardButton);
            draw_sprite(instructionsButton);
            draw_sprite(quitButton);
            break;
        case HOVER_LEADER_BOARD:
            draw_sprite(playButton);
            draw_sprite(leaderboardButton2);
            draw_sprite(instructionsButton);
            draw_sprite(quitButton);
            break;
        case HOVER_INSTRUCTIONS:
            draw_sprite(playButton);
            draw_sprite(leaderboardButton);
            draw_sprite(instructionsButton2);
            draw_sprite(quitButton);
            break;
        case HOVER_QUIT:
            draw_sprite(playButton);
            draw_sprite(leaderboardButton);
            draw_sprite(instructionsButton);
            draw_sprite(quitButton2);
            break;
        default:
            draw_sprite(quitButton);
            draw_sprite(playButton);
            draw_sprite(leaderboardButton);
            draw_sprite(instructionsButton);
            break;
        }
        return;
    }
    else if (menu == INSTRUCTIONS) {
        draw_sprite(instructions);
        switch (selected_menu) {
            case HOVER_BACK:
                draw_sprite(backButton2);
                break;
            default:
                 draw_sprite(backButton);
                 break;
        }
        return;
    }
    else if (menu == LEADERBOARD) {
       switch (selected_menu) {
            case HOVER_BACK:
                draw_sprite(backButton2);
                break;
            default:
                 draw_sprite(backButton);
                 break;
        }
        return;
    }
    

}

void draw_time() {
    rtc_update_time();
    rtc_update_date();
    
    int digit;
    uint16_t x=10;  //Initial position
    uint16_t y=10;

    //Monthday
    digit = (dateData.monthday / 10) %10;
    draw_digit(x,y,digit);
    x = x+20;
    digit = dateData.monthday % 10;
    draw_digit(x,y,digit);
    x = x+20;

    set_x_sprite(slash, x);
    set_y_sprite(slash, y);
    draw_sprite(slash);
    x=x+20;

    //Month
    digit = (dateData.month / 10) %10;
    draw_digit(x,y,digit);
    x = x+20;
    digit = dateData.month % 10;
    draw_digit(x,y,digit);
    x=x+20;

    set_x_sprite(slash, x);
    set_y_sprite(slash, y);
    draw_sprite(slash);
    x=x+20;

    //Year
    digit = (dateData.year / 10) %10;
    draw_digit(x,y,digit);
    x = x+20;
    digit = dateData.year % 10;
    draw_digit(x,y,digit);
    x=x+40;

    //Hours
    digit = (timeData.hours / 10) %10;
    draw_digit(x,y,digit);
    x = x+20;
    digit = timeData.hours % 10;
    draw_digit(x,y,digit);
    x=x+20;

    set_x_sprite(twoDots, x);
    set_y_sprite(twoDots, y);
    draw_sprite(twoDots);
    x=x+20;
    

    //Minutes
    digit = (timeData.minutes / 10) %10;
    draw_digit(x,y,digit);
    x = x+20;
    digit = timeData.minutes % 10;
    draw_digit(x,y,digit);
    x=x+20;

    set_x_sprite(twoDots, x);
    set_y_sprite(twoDots, y);
    draw_sprite(twoDots);
    x=x+20;

    //Seconds
    digit = (timeData.seconds / 10) %10;
    draw_digit(x,y,digit);
    x = x+20;
    digit = timeData.seconds % 10;
    draw_digit(x,y,digit);
    x=x+20;

    return;

}



