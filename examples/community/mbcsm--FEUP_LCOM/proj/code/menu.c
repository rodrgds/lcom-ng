#include <lcom/lcf.h>

#include "menu.h"
#include "xpm.h"
#include "font.h"
#include "cursor.h"
#include "score.h"


/* ------------- XPMs --------------- */

uint16_t *playbutton;
xpm_image_t imgPlayButton;

uint16_t *playbuttonover;
xpm_image_t imgPlayButtonOver;

uint16_t *highscoresbutton;
xpm_image_t imgHighscoresButton;

uint16_t *highscoresbuttonover;
xpm_image_t imgHighscoresButtonOver;

uint16_t *exitbutton;
xpm_image_t imgExitButton;

uint16_t *exitbuttonover;
xpm_image_t imgExitButtonOver;  

uint16_t *highscoresscreen;
xpm_image_t imgHighscoresScreen;

/*------------------------------------*/


Menu menuState = VOID;
LeftClick lc = NON;

int startMenu(){
    
    //board = (uint16_t*)xpm_load(board_pre_xpm, XPM_5_6_5, &imgBoard);
    playbutton = (uint16_t*)xpm_load(play_button_xpm, XPM_5_6_5, &imgPlayButton);
    playbuttonover = (uint16_t*)xpm_load(play_button_over_xpm, XPM_5_6_5, &imgPlayButtonOver);
    highscoresbutton = (uint16_t*)xpm_load(highscores_button_xpm, XPM_5_6_5, &imgHighscoresButton);
    highscoresbuttonover = (uint16_t*)xpm_load(highscores_button_over_xpm, XPM_5_6_5, &imgHighscoresButtonOver);
    exitbutton = (uint16_t*)xpm_load(exit_button_xpm, XPM_5_6_5, &imgExitButton);
    exitbuttonover = (uint16_t*)xpm_load(exit_button_over_xpm, XPM_5_6_5, &imgExitButtonOver);
    highscoresscreen = (uint16_t*)xpm_load(highscores_xpm, XPM_5_6_5, &imgHighscoresScreen);

    printf("Button xpms started");

    if (displayMenu() == 0)
        printf("Menu displaying");

    return 0;
}

int menuIH(bool mouse, bool kbd, struct packet* pp, uint32_t kbdData){
    if (mouse){
        int x, y;

        getPosition(&x, &y);

        if (menuState == VOID){
            if (x > align_x_center(imgPlayButton) && x < align_x_center(imgPlayButton) + imgPlayButton.width && y >= PLAY_BUTTON_Y && y < PLAY_BUTTON_Y + imgPlayButton.height){ // inside playbutton
                menuState = PLAY_BUTTON_OVER;
                displayMenu();
                //startCursor();
            }
            else if (x > align_x_center(imgHighscoresButton) && x < align_x_center(imgHighscoresButton) + imgHighscoresButton.width && y > HIGHSCORES_BUTTON_Y && y < HIGHSCORES_BUTTON_Y + imgHighscoresButton.height){ // inside hs button
                menuState = HIGHSCORES_BUTTON_OVER;
                displayMenu();
                //startCursor();
            }
            else if (x > align_x_center(imgExitButton) && x < align_x_center(imgExitButton) + imgExitButton.width && y > EXIT_BUTTON_Y && y < EXIT_BUTTON_Y + imgExitButton.height){ // inside exit button
                menuState = EXIT_BUTTON_OVER;
                displayMenu();
                //startCursor();
            }
        }
        else if (menuState == PLAY_BUTTON_OVER){
            if (x < align_x_center(imgPlayButton) || x > align_x_center(imgPlayButton) + imgPlayButton.width || y < PLAY_BUTTON_Y || y > PLAY_BUTTON_Y + imgPlayButton.height){ // outside playbutton
                menuState = VOID;
                displayMenu();
                //startCursor();
            }
        }
        else if (menuState == HIGHSCORES_BUTTON_OVER){
            if (x < align_x_center(imgHighscoresButton) || x > align_x_center(imgHighscoresButton) + imgHighscoresButton.width || y < HIGHSCORES_BUTTON_Y || y > HIGHSCORES_BUTTON_Y + imgHighscoresButton.height){ // outside hs button
                menuState = VOID;
                displayMenu();
                //startCursor();
            }
        }
        else if (menuState == EXIT_BUTTON_OVER){
            if (x < align_x_center(imgExitButton) || x > align_x_center(imgExitButton) + imgExitButton.width || y < EXIT_BUTTON_Y || y > EXIT_BUTTON_Y + imgExitButton.height){ // outside exit button
                menuState = VOID;
                displayMenu();
                //startCursor();
            }
        }

        bool lb = pp->lb;
        if (!lb && lc == NON){
            if (menuState == PLAY_BUTTON_OVER)
                lc = LB_UP;
            if (menuState == HIGHSCORES_BUTTON_OVER)
                lc = LB_UP;
            if (menuState == EXIT_BUTTON_OVER)
                lc = LB_UP;
            if (menuState == VOID)
                lc = NON;
        }

        if (lb && lc == LB_UP){
            if (menuState == PLAY_BUTTON_OVER)
                lc = LB_PRESS;
            if (menuState == HIGHSCORES_BUTTON_OVER)
                lc = LB_PRESS;
            if (menuState == EXIT_BUTTON_OVER)
                lc = LB_PRESS;
            if (menuState == VOID)
                lc = NON;
        }

        if (!lb && lc == LB_PRESS)
            lc = CLICKED;
        
        
        if (lc == CLICKED){
            lc = NON;
            if (menuState == PLAY_BUTTON_OVER)
                return 1; 
            else if (menuState == HIGHSCORES_BUTTON_OVER){
                menuState = HIGHSCORES;
                displayMenu();
            }
            else if (menuState == EXIT_BUTTON_OVER)
                return 2;
        }
        
    }
    else if (kbd){
        
        switch (kbdData)
        {
            case 0x9c: //ENTER BREAKCODE
                if (menuState == PLAY_BUTTON_OVER)
                    return 1; 
                else if (menuState == HIGHSCORES_BUTTON_OVER){
                    menuState = HIGHSCORES;
                    displayMenu();
                }
                else if (menuState == EXIT_BUTTON_OVER)
                    return 2;
                break;

            case 0x8e: // BACKSPACE BREAKCODE
                if (menuState == HIGHSCORES){
                    menuState = VOID;
                    displayMenu();
                }
                break;
            case 0x48: // NUMPAD 8 - (UP ARROW)
                if (menuState == PLAY_BUTTON_OVER){
                    menuState = EXIT_BUTTON_OVER;
                    displayMenu();
                }
                else if (menuState == HIGHSCORES_BUTTON_OVER){
                    menuState = PLAY_BUTTON_OVER;
                    displayMenu();
                }
                else if (menuState == EXIT_BUTTON_OVER){
                    menuState = HIGHSCORES_BUTTON_OVER;
                    displayMenu();
                }
                else if (menuState == VOID){
                    menuState = EXIT_BUTTON_OVER;
                    displayMenu();
                }
                break;

            case 0x50: // NUMPAD 2 - (DOWN ARROW)
                if (menuState == PLAY_BUTTON_OVER){
                    menuState = HIGHSCORES_BUTTON_OVER;
                    displayMenu();
                }
                else if (menuState == HIGHSCORES_BUTTON_OVER){
                    menuState = EXIT_BUTTON_OVER;
                    displayMenu();
                }
                else if (menuState == EXIT_BUTTON_OVER){
                    menuState = PLAY_BUTTON_OVER;
                    displayMenu();
                }
                else if (menuState == VOID){
                    menuState = PLAY_BUTTON_OVER;
                    displayMenu();
                }
                break;
            default:
                break;
        }
    }
    return 0;
}

int displayMenu(){
    int x, y;

    clearScreen();
    
    switch (menuState)
    {
        case VOID:
            x = align_x_center(imgPlayButton);
            y = PLAY_BUTTON_Y;
            draw_xpm(x, y, playbutton, imgPlayButton, getTransparency());
            
            y = HIGHSCORES_BUTTON_Y;
            draw_xpm(x, y, highscoresbutton, imgHighscoresButton, getTransparency());

            y = EXIT_BUTTON_Y;
            draw_xpm(x, y, exitbutton, imgExitButton, getTransparency());
            break;

        case PLAY_BUTTON_OVER:
            x = align_x_center(imgPlayButtonOver);
            y = PLAY_BUTTON_Y;
            draw_xpm(x, y, playbuttonover, imgPlayButtonOver, getTransparency());

            x = align_x_center(imgHighscoresButton);
            y = HIGHSCORES_BUTTON_Y;
            draw_xpm(x, y, highscoresbutton, imgHighscoresButton, getTransparency());
            
            x = align_x_center(imgExitButton);
            y = EXIT_BUTTON_Y;
            draw_xpm(x, y, exitbutton, imgExitButton, getTransparency());
            break;

        case HIGHSCORES_BUTTON_OVER:
            x = align_x_center(imgPlayButton);
            y = PLAY_BUTTON_Y;
            draw_xpm(x, y, playbutton, imgPlayButton, getTransparency());
            
            x = align_x_center(imgHighscoresButtonOver);
            y = HIGHSCORES_BUTTON_Y;
            draw_xpm(x, y, highscoresbuttonover, imgHighscoresButtonOver, getTransparency());

            x = align_x_center(imgExitButton);
            y = EXIT_BUTTON_Y;
            draw_xpm(x, y, exitbutton, imgExitButton, getTransparency());
            break;

        case EXIT_BUTTON_OVER:
            x = align_x_center(imgPlayButton);
            y = PLAY_BUTTON_Y;
            draw_xpm(x, y, playbutton, imgPlayButton, getTransparency());
            
            x = align_x_center(imgHighscoresButton);
            y = HIGHSCORES_BUTTON_Y;
            draw_xpm(x, y, highscoresbutton, imgHighscoresButton, getTransparency());

            x = align_x_center(imgExitButtonOver);
            y = EXIT_BUTTON_Y;
            draw_xpm(x, y, exitbuttonover, imgExitButtonOver, getTransparency());
            break;

        case HIGHSCORES:
            displayScores();
            break;
    
        default:
            break;
    }
    return 0;
}

void displayScores(){
    draw_xpm(0, 0, highscoresscreen, imgHighscoresScreen, getTransparency());

    if (s.scoreOne == 0 ){
        printstring("............", 12, "w", 200, 360);
        printstring("............", 12, "w", 200, 490);
        printstring("............", 12, "w", 200, 620);
        printstring("............", 12, "w", 200, 760);
        printstring("............", 12, "w", 200, 900);
        return;
    }
    
    printstring("............", 12, "w", 200, 370);
    printstring("............", 12, "w", 200, 500);


}
