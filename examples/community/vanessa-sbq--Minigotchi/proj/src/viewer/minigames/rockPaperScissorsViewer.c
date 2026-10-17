#include "rockPaperScissorsViewer.h"

static RockPaperScissors *rockPaperScissors;

void rockPaperScissorsViewer_setRockPaperScissors(RockPaperScissors* rps){
    rockPaperScissors = rps;
}

/**
 * @brief Calls draw function of each element the rock paper scissors game contains (also draws the GUI elements when the game terminates)
 */
void rockPaperScissorsViewer_draw(){
    vg_clear_screen();

    // Background elements
    wrapper_draw_background(0);

    // Foreground elements
    draw_button(rockPaperScissors_get_rockButton(rockPaperScissors));
    draw_button(rockPaperScissors_get_paperButton(rockPaperScissors));
    draw_button(rockPaperScissors_get_scissorsButton(rockPaperScissors));

    // Game finished
    if (rockPaperScissors_get_rpsState(rockPaperScissors) == RPS_WIN){
        drawSprite(guiDrawer_get_rpsWin_sprite(), 410, 200);
        drawSprite(guiDrawer_get_botChose_sprite(), 300, 600);
        
        Sprite* bot_choice = guiDrawer_get_button_sprite(rockPaperScissors_get_botChoice(rockPaperScissors) + 3);
        drawSprite(bot_choice, 800, (1280 - sprite_get_width(bot_choice)) / 2);

        Sprite* reward_sprite = item_get_sprite(rockPaperScissors_get_reward(rockPaperScissors));
        drawSprite(reward_sprite, (1152 - sprite_get_width(reward_sprite)) / 2, 400);
    }

    if (rockPaperScissors_get_rpsState(rockPaperScissors) == RPS_TIE){
        drawSprite(guiDrawer_get_rpsTie_sprite(), 500, 200);
        drawSprite(guiDrawer_get_botChose_sprite(), 300, 600);

        Sprite* bot_choice = guiDrawer_get_button_sprite(rockPaperScissors_get_botChoice(rockPaperScissors) + 3);
        drawSprite(bot_choice, 800, (1280 - sprite_get_width(bot_choice)) / 2);
    }

    if (rockPaperScissors_get_rpsState(rockPaperScissors) == RPS_LOSE){
        drawSprite(guiDrawer_get_rpsLose_sprite(), 383, 200);
        drawSprite(guiDrawer_get_botChose_sprite(), 300, 600);

        Sprite* bot_choice = guiDrawer_get_button_sprite(rockPaperScissors_get_botChoice(rockPaperScissors) + 3);
        drawSprite(bot_choice, 800, (1280 - sprite_get_width(bot_choice)) / 2);
    }

    draw_button(rockPaperScissors_get_quitButton(rockPaperScissors));
    draw_cursor(rockPaperScissors_get_cursor(rockPaperScissors));
    
    vg_page_flip();
}
