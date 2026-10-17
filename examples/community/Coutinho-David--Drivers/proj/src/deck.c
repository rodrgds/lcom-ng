#include <lcom/lcf.h>
#include <stdio.h>
#include <stdlib.h>

const char* suits[] = { "C", "D", "H", "S" };
const char* cards[] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "A", "J", "K", "Q" };

int player_cards[5];
int player_c_count = 0;
int dealer_cards[5];
int dealer_c_count = 0;

void getCard(int* score, int who) {
    int suit = rand() % 4;
    int card = rand() % 13;
    
    int val = card + 2; 
    if (val > 11) val = 10;
    
    if (val == 11 && (*score + val > 21)) val = 1;
    
    *score += val;
    if (who == 0) {
        player_cards[player_c_count] = (4 * card) + suit;
        player_c_count++;
        //printf("Player card: %d, new card: %s%s, score: %d\n", player_c_count, cards[card], suits[suit], *score);
    }
    else {
        dealer_cards[dealer_c_count] = (4 * card) + suit;
        dealer_c_count++;
        /*
        if (dealer_c_count == 2) //printf("Second dealer card is hidden\n");
        else //printf("Dealer card: %d, new card: %s%s, score: %d\n", dealer_c_count, cards[card], suits[suit], *score);
        */
        
    }
    
}
