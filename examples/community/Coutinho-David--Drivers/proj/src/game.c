#include "game.h" 
#include "deck.h"
#include <time.h>
#include <stdlib.h>

int score[2];
int fakeScore;

int money = 500;
int bet = 0;


void game_start() {
    if (bet <= 0) return;
    srand(time(NULL));
    score[0] = 0;
    score[1] = 0;
    player_c_count = 0;
    dealer_c_count = 0;
    getCard(&score[0], 0);
    getCard(&score[1], 1);
    getCard(&score[0], 0);
    fakeScore = score[1];
    getCard(&score[1], 1);
    
    if (score[0] == 21) game_end();
    else state = 7;

}

void change_bet() {
    if (state == 2 && money - 5 >= 0) {
        bet += 5;
        money -= 5;    
    }
    else if (state == 3 && money - 10 >= 0) {
        bet += 10;
        money -= 10;    
    }
    else if (state == 4 && money - 25 >= 0) {
        bet += 25;
        money -= 25;    
    }
    else if (state == 5 && money - 50 >= 0) {
        bet += 50;
        money -= 50;    
    }
    //printf("money: %d, bet: %d\n", money, bet);
}

void game_player_hit() { 
    getCard(&score[0], 0);
    if (score[0] > 21) {
        game_end();
    }
}

void game_player_stand() {
    //printf("Dealer reveals hidden card, new score: %d\n", score[1]); 
    game_dealer();
}

void game_player_double() {
    /* logica do double */
    if (player_c_count > 2) return;
    if (money - bet >= 0) {
        money -= bet;
        bet *= 2;
        getCard(&score[0], 0);
        if (score[0] > 21) game_end();
        else game_dealer();
    }
    
}

void game_dealer() {
    while (score[1] < 17) {
        getCard(&score[1], 1);
        if (score[1] > 21) {
            break;
        }
    }
    game_end();
}

void game_end() { 
    if (score[0] == 21 && player_c_count == 2) {
        money += bet * 1.5;
        state = 16;
        bet = 0;
    }
    else if (score[0] > 21) {
        //printf("You busted. Dealer wins.\n");
        state = 10;
    } 
    else if (score[1] > 21 || score[0] > score[1]) {
        //printf("You win! (%d vs %d)\n", score[0], score[1]);
        money += bet * 2;
        state = 12;
    } 
    else if (score[0] == score[1]) {
        //printf("Push (tie).\n");
        money += bet;
        state = 14;
    } 
    else {
        //printf("Dealer wins (%d vs %d)\n", score[1], score[0]);
        state = 10;
    }

    //printf("Restarting\n");
    bet = 0;
}

