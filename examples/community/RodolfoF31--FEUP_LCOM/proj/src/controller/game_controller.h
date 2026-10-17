#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "../Modules/vg.h"
#include <stdint.h>
#include <stdio.h>
#include "../Modules/timer.h"
#include "../Modules/keyboard.h"
#include "../view/draw_functions.h"

#define TIMEOUT 5

#define EASY_PHRASES_COUNT (sizeof(easy_phrases) / sizeof(easy_phrases[0]))
#define HARD_PHRASES_COUNT (sizeof(hard_phrase) / sizeof(hard_phrase[0]))


static char *easy_phrases[] = { 
    "believe in yourself even when the path ahead seems unclear.",
    "success comes from staying patient and never giving up hope.",
    "every great journey begins with one small, brave step forward.",
    "the more you practice, the more confident and skilled you become.",
    "kindness and hard work can open doors you never imagined existed.",
    "never stop learning because every day brings a new chance to grow and become better than you were yesterday.",
    "the world is full of opportunities waiting for those who dare to chase their dreams.",
    "when you face challenges, remember that each one is a chance to learn and improve.",
    "take a deep breath focus your mind and keep moving forward even when things feel too hard to handle.",
    "surround yourself with positive people who inspire and uplift you to reach your full potential."
};

static char *hard_phrase[] = {
    "When the clock struck midnight, silence filled the room. Shadows danced across the walls, and everyone held their breath, wondering what would happen next. Was it fear, excitement, or both? No one moved, no one spoke. The air was thick with anticipation and mystery.",
    "Success is not only about hard work, dedication, or talent. It also depends on timing, opportunity, and the courage to take risks! Have you ever wondered how far you could go if you stopped doubting yourself? Start now, push forward, and surprise everyone with your strength.",
    "She stepped onto the stage, heart pounding, hands shaking. The spotlight hit her face, and the audience went quiet. Would she remember the lines? Would her voice fail? Taking a deep breath, she began. Every word, every gesture, felt like a victory. The crowd cheered wildly!",
    "They traveled far and wide, crossing rivers, climbing mountains, and navigating dark forests. Danger was everywhere, yet they never gave up! Was it madness or bravery that kept them going? With every obstacle conquered, their resolve grew stronger. At journey's end, they had become legends.",
    "Machines roared, engines screamed, and the wind lashed against his face. This was no ordinary race. Around sharp corners, across deadly straights, he pushed harder, faster! Could he win? Would the risk pay off? One final turn, a burst of speed, and then... victory!",
    "The rain fell harder, soaking his coat as thunder echoed in the distance. Lights flickered in the windows, and a dog barked nervously. Was someone watching? He turned quickly, scanning the empty street. Nothing. Still, he could not shake the feeling that something was terribly wrong.",
    "Knowledge is power, but without wisdom, it can become dangerous. People may chase facts, numbers, and theories, yet forget the importance of empathy and understanding. So, ask yourself: Are you learning just to win, or to grow? The answer might define your journey far more than success.",
    "Explorers mapped the stars, charted new worlds, and pushed beyond the edge of the known. What drove them forward? Curiosity, ambition, or pure madness? With each mission, they risked everything. And still, they smiled, whispering to the void, We are not afraid. The future belongs to us!",
    "Her footsteps echoed down the corridor, each one louder than the last. The walls were lined with paintings, eyes that seemed to follow. A sudden breeze, a faint whisper, then silence. She paused, heart racing. Was it her imagination, or had something just moved in the shadows?",
    "Time moves quickly, yet we often act as if it will wait for us. We plan, delay, and hesitate, fearing mistakes more than missed chances. But what if we dared to act? Could we change everything? Take a breath, take a step, and take control now!"

};


typedef struct {
    char *word;
    Pixel coordinates; 
} WordPosition;

//  "success comes from staying patient and never giving up hope.",
//wordPositions[0].word = "success";
//wordPositions[0].coordinates = (Pixel){10, 20}; // Example coordinates


typedef struct {
    const char *difficulty;  
    int seconds_per_round;
    bool case_sensitive;
    bool has_commas;
} GameConfig;

typedef struct {
    Pixel position;
    int width;
    int height;
    int color; 
} Rectangle;

typedef struct {
    Rectangle writting_rectangle_position;
    WordPosition word_positions[MAX_WORDS];
    int current_word_index;
    int word_count;
    WordPosition *input;
    int correct_words;
    int incorrect_words;
    int time_taken;
    bool lost;
} GameState;


void start_game(GameConfig *config);
void check_input(GameState *game_state);
void game_loop(int8_t kbd_irq, int8_t timer_irq, GameConfig *config, GameState *game_state);
void end_game(GameState *game_state, GameConfig *config, int8_t kbd_irq, int8_t timer_irq);
void display_phrase(const char *phrase);
char* get_random_easy_phrase();
char* get_random_hard_phrase();
int map_words_in_sentence(const char* sentence, int x, int y, WordPosition* word_positions, int max_words);


#endif
