#include "Card.h"

// Initializes a card with its fruit type and position, and sets default state
void card_init(Card* card, Fruta fruit, int x, int y) {
    card->fruit = fruit;
    card->xPos = x;
    card->yPos = y;
    card->isFaceUp = false;
    card->isAnimating = false;
    card->isMatched = false;

    card->sprite = c9;    // default sprite
}

// Draws the card at its current position
void card_draw(const Card* card) {
    sprite_desenhar(card->sprite, card->xPos, card->yPos);
}

// Returns true if the card is face-up
bool card_is_turned_over(const Card* card) {
    return card->isFaceUp;
}

// Flips the card (face-up becomes face-down and vice versa)
void card_turn(Card* card) {
    card->isFaceUp = !card->isFaceUp;
}

// Updates the animation frame of the card, and ends animation if complete
void animation_update(Card* card) {
    if (!card->anim.active || !card->anim.frames || !card) return;

    // Set current frame sprite
    card->sprite = card->anim.frames[card->anim.frame];
    
    // Advance to next frame
    card->anim.frame += card->anim.step;
    
    // Check if animation reached its end
    if ((card->anim.step > 0 && card->anim.frame > card->anim.end) ||
        (card->anim.step < 0 && card->anim.frame < card->anim.end)) {
        card->anim.active = false;
        card->isAnimating = false;
        card->isFaceUp = !card->isFaceUp;
        return;
    }
}

// Starts the flip animation for the card in the given direction (forward or reverse)
void animation_start(Card* card, int frame_count, bool forward) {
    //carregar_cartas_fruta(card->fruit);
    
    // Select animation frames based on fruit type
    switch (card->fruit) {
        case(ABACATE):
            card->anim.frames = abacate_frames;
            break;
        case(BANANA):
            card->anim.frames = banana_frames;
            break;
        case(CEREIJA):
            card->anim.frames = cereija_frames;
            break;
        case(COCO):
            card->anim.frames = coco_frames;
            break;
        case(LARANJA):
            card->anim.frames = laranja_frames;
            break;
        case(MACA):
            card->anim.frames = maca_frames;
            break;
        case(MELANCIA):
            card->anim.frames = melancia_frames;
            break;
        case(MORANGO):
            card->anim.frames = morango_frames;
            break;
        default:
            return;            

    }
    // Initialize animation parameters
    card->anim.fruta = card->fruit;
    card->anim.start = forward ? 0 : frame_count - 1;
    card->anim.end = forward ? frame_count - 1 : 0;
    card->anim.step = forward ? 1 : -1;
    card->anim.frame = card->anim.start;
    card->anim.last_tick = 0;
    card->anim.active = true;

    card->isAnimating = true;
    card->sprite = card->anim.frames[card->anim.start]; // Set initial animation frame
}
