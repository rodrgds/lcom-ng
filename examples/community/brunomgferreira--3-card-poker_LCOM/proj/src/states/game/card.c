#include <lcom/lcf.h>
#include "card.h"

void (create_deck)(Card *deck) {
    int i = 0;
    for (int suit = CLUBS; suit <= DIAMONDS; suit++) {
        for (int worth = TWO; worth <= ACE; worth++) {
            deck[i].suit = (suit_enum)suit;
            deck[i].worth = (worth_enum)worth;
            i++;
        }
    }
}

void (shuffle_deck)(Card *deck) {
    srand(time(NULL)); // Seed the random number generator
    for (int i = DECK_SIZE - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        // Swap deck[i] with deck[j]
        Card temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

void (print_card)(const Card *card) {
    const char *suits[] = {"Clubs", "Spades", "Hearts", "Diamonds"};
    const char *worths[] = {"Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Ace"};

    printf("%s of %s\n", worths[card->worth], suits[card->suit]);
}

void (print_deck)(const Card *deck) {
    for (int i = 0; i < DECK_SIZE; i++) {
        print_card(&deck[i]);
    }
}

Sprite *(find_card_sprite)(const Card *card) {
    const char *suits[] = {"clubs", "spades", "hearts", "diamonds"};
    const char *worths[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
    
    char cardName[50];
    snprintf(cardName, sizeof(cardName), "%s%s", suits[card->suit], worths[card->worth]);
    return find_sprite(cardName);
}

void (draw_card)(const Card *card, uint32_t x, uint32_t y) {
    Sprite *sprite = find_card_sprite(card);
    if(sprite == NULL) return;
    sprite->x = x;
    sprite->y = y;
    draw_sprite(sprite);
}




void (determine_hand)(Card cards[3], HandType *hand) {
    int values[3];
    int duplicates = 0;
    int duplicated = 0;
    bool flush = true;
    bool straight = true;
    suit_enum suit = cards[0].suit;

    for (uint8_t i = 0; i < 3; i++) {
        int value = (int)cards[i].worth + 2;

        // Checks if the hand contains any duplicate numbers.
        for (uint8_t j = 0; j < i; j++) {
            if (values[j] == value) {
                duplicates++;
                duplicated = value;
            }
        }

        // Checks if the hand is all of the same suit.
        if (cards[i].suit != suit) flush = false;

        values[i] = value;
    }

    // Sort values
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {
            if (values[i] > values[j]) {
                int temp = values[i];
                values[i] = values[j];
                values[j] = temp;
            }
        }
    }

    // Can not be a straight if there are duplicates
    if (duplicates == 0) {
        // Checks if the numbers are increasing by one for all 3.
        for (uint8_t i = 1; i < 3; i++) {
            if (values[i] - values[i - 1] != 1) straight = false;
        }

        // Checks if it is a straight using the ace as a ONE.
        if (values[0] == 2 && values[1] == 3 && values[2] == 14) {
            values[2] = 1;
            straight = true;
        }
    } else {
        straight = false;
    }

    evaluate_hand_type(hand, values, flush, straight, duplicates, duplicated);
}

void (evaluate_hand_type)(HandType *hand, int values[3], bool flush, bool straight, int duplicates, int duplicated) {
    // Three of a kind
    if (duplicates == 2) {
        create_hand_type(hand, THREE_OF_A_KIND, values, 1);
        return;
    }

    // Pair
    if (duplicates == 1) {
        *hand = create_pair_type(values);
        return;
    }

    if (straight) {
        // Straight flush
        if (flush) {
            create_hand_type(hand, STRAIGHT_FLUSH, values, 1);
            return;
        }

        // Straight
        create_hand_type(hand, STRAIGHT, values, 1);
        return;
    }

    // Flush
    if (flush) {
        create_hand_type(hand, FLUSH, values, 3);
        return;
    }

    // High
    create_hand_type(hand, HIGH, values, 3);
}

void (create_hand_type)(HandType *hand, hand_rank_enum rank, int values[3], int index) {
    int temp[3] = {-1, -1, -1}; // Initialize temp with -1
    for (uint8_t i = 0; i < index; i++) {
        temp[i] = values[3 - i - 1];
    }
    *hand = (HandType){rank, {temp[0], temp[1], temp[2]}};
}

struct HandType (create_pair_type)(int values[3]) {
    HandType hand;
    int pairValue = -1;

    for (uint8_t i = 0; i < 3; i++) {
        for (uint8_t j = i + 1; j < 3; j++) {
            if (values[i] == values[j]) {
                pairValue = values[i];
                break;
            }
        }
        if (pairValue != -1) // If pair is found, break the loop
            break;
    }

    hand.values[0] = pairValue; // Set the pair value
    hand.values[1] = pairValue; // Set the pair value
    hand.values[2] = values[3 - (pairValue == values[0]) - (pairValue == values[1])]; // Set the remaining value

    hand.rank = PAIR;
    return hand;
}

Outcome (winning_hand)(HandType playerHand, HandType houseHand) {
    int playerScore = hand_rank_score(&playerHand);
    int houseScore = hand_rank_score(&houseHand);

    if (playerScore > houseScore) {
        return WIN;
    } else if (playerScore < houseScore) {
        return LOSE;
    } else {
        return DRAW;
    }
}

int (hand_rank_score)(const HandType *hand) {
    int rank_score = 0;

    switch (hand->rank) {
        case HIGH:
            rank_score = 1;
            break;
        case PAIR:
            rank_score = 2;
            break;
        case FLUSH:
            rank_score = 3;
            break;
        case STRAIGHT:
            rank_score = 4;
            break;
        case THREE_OF_A_KIND:
            rank_score = 5;
            break;
        case STRAIGHT_FLUSH:
            rank_score = 6;
            break;
    }

    return rank_score * 10000 + hand->values[0] * 100 + hand->values[1] * 10 + hand->values[2];
}


char* (handtype_to_string)(HandType hand) {
    static char name[100];
    const char *worths[] = {"Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Ace"};
    
    switch (hand.rank) {
        case HIGH:
            sprintf(name, "%s High", worths[hand.values[0] - 2]);
            break;
        case PAIR: 
            sprintf(name, "Pair of %ss", worths[(hand.values[0] - 2)]);
            break;
        case FLUSH:
            strcpy(name, "Flush");
            break;
        case STRAIGHT:
            strcpy(name, "Straight");
            break;
        case THREE_OF_A_KIND:
            strcpy(name, "Three of a Kind");
            break;
        case STRAIGHT_FLUSH:
            strcpy(name, "Straight Flush");
            break;
        default:
            strcpy(name, "Unknown");
            break;
    }

    return name;
}
