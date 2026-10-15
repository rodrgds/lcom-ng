#include "player2.h"
#include <stdlib.h> 

void updatePlayer2AI(Player *player2, Ball *ball, int counter, bool canHitAfterServe) {
    if (ball->x == 9999) {
        return; // Exit the function early if ball is in invalid position
    }

    int player2X = player2->x;
    int player2Y = player2->y;
    int ballX = ball->x;
    int ballY = ball->y;

    int deltaX = ballX - player2X;
    int deltaY = ballY - player2Y;

   int deltaStartY = START_Y_POS - player2Y;

    //the player2 is starting the game
    if(player2->state == CHOOSE_START_STOP){
        if(counter % 120 == 0){
            player2->state = START;
        }
        return;
    }

    //no special actions when the player is starting
    if(player2->state == START){
        updatePlayerMovementsTimer(player2, counter, canHitAfterServe);
        return;
    }


    // Prioritize movement along the axis with the largest difference. Every movement has a tolerance of two
    if(player2->state == MOVE || player2->state == STOP){
        player2->state = MOVE;

        //if ball is going into the opposite direction, try to get closer to starting Y pos while following ball movement
        if (ball->direction == DOWN_BALL) {
            if (abs(deltaX) > abs(deltaStartY)) {
                if (deltaX < -2) {
                    player2->movement = LEFT_PLAYER;
                    player2->direction = LEFTD;
                } 
                else if (deltaX > 2) { 
                    player2->movement = RIGHT_PLAYER;
                    player2->direction = RIGHTD;
                }
                else {
                    //the player is in the tolerance
                    player2->state = STOP;
                }
            }
            else {
                // Only move up if not already at the starting Y position
                if (deltaStartY < 0) {
                    player2->movement = UP_PLAYER;
                } else {
                    // Stop moving if already at the starting Y position
                    player2->state = STOP;
                }
            }
        }
        //try getting closer to the ball
        else {
            //player need to move in the x direction
            if ((abs(deltaX) > 0.3 * abs(deltaY)) ) {
                if (deltaX < -2) {
                    player2->movement = LEFT_PLAYER;
                    player2->direction = LEFTD;
                } else if (deltaX > 2) {
                    player2->movement = RIGHT_PLAYER;
                    player2->direction = RIGHTD;
                }
                else {
                    //the player is in the tolerance
                    player2->state = STOP;
                }

                //the player needs to move in the y direction
            } else if (ball->y  < MAX_Y_P2) {
                if (deltaY < -2) {
                    player2->movement = UP_PLAYER;
                } else if (deltaY > 2) {
                    player2->movement = DOWN_PLAYER;
                }
                else {
                    //the player is in the tolerance
                    player2->state = STOP;
                }
            }

            //the ball if on the player 1 side
            else if((ball->y  > MAX_Y_P2) && (abs(deltaX) > 6)){
                if (deltaX < -2) {
                    player2->movement = LEFT_PLAYER;
                    player2->direction = LEFTD;
                } else if (deltaX > 2) {
                    player2->movement = RIGHT_PLAYER;
                    player2->direction = RIGHTD;
                }
                else {
                    //the player is in the tolerance
                    player2->state = STOP;
                }
            }
            //the player doesn't need to move
            else{
                player2->state = STOP;
            }
        }
    }
    
    // Check for collision between player2 and the ball
    // Get the current hit limits of player2
    int x_min, x_max, y_min, y_max;
    get_current_hit_limits(player2, &x_min, &x_max, &y_min, &y_max);

   
    //corrects the position in order to hit the ball
    if((ball->x >= player2X - 20) && (ball-> x <= x_min) && (ballY <= y_max + 30) && (ballY >= y_min)){
            player2->direction = LEFTD;
        }
    if((ball->x >= x_min) && (ball-> x <= x_max + 20)  && (ballY <= y_max + 30) && (ballY >= y_min)){
            player2->direction = RIGHTD;
    }

    // Check if the ball is within the hit limits of player2
    if ((ballX + 25  >= x_min) && (ballX + 4 <= x_max) && (ballY + 30 >= y_min) && (ballY + 4 <= y_max )) {
            // If there is a collision and canHitAfterServe is true, trigger a hit animation for player2
            if (canHitAfterServe) {
                player2->state = HIT;
                hitAnim(player2);
            }
    }
    updatePlayerMovementsTimer(player2, counter, canHitAfterServe);
}
