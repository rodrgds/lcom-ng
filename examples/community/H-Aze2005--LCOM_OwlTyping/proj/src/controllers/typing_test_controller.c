#include "typing_test_controller.h"

#define MAX_TYPED_CHARS 2048
#define NUM_KEYS 31

static char typed_chars[MAX_TYPED_CHARS];
static int typed_count = 0;

static int phrase_index = 0; // Which phrase line should be printed first
static int typing_phrase_index = 0; // Phrase line currently being written
static int char_index = 0;    // Overall char index

static int shuffled_indices[TOTAL_PHRASES];

static int remaining_time = 0;
static int initial_duration = 0;
static int start_frame = 0;

extern int timer_counter;

// Stats
int final_wpm = 0;
int final_accuracy = 0;

typedef struct {
    int total_typed;
    int correct_typed;
} KeyStats;

static KeyStats key_stats[NUM_KEYS];

KeyPerformance perf_map[NUM_KEYS];

static void shuffle_indices() {
    for (int i = 0; i < TOTAL_PHRASES; ++i) {
        shuffled_indices[i] = i;
    }

    for (int i = TOTAL_PHRASES - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int temp = shuffled_indices[i];
        shuffled_indices[i] = shuffled_indices[j];
        shuffled_indices[j] = temp;
    }
}

void start_typing_test(int time) {
    typed_count = 0;
    phrase_index = 0;
    typing_phrase_index = 0;
    char_index = 0;

    memset(typed_chars, 0, sizeof(typed_chars));
    for (int i = 0; i < NUM_KEYS; i++) {
        key_stats[i].total_typed = 0;
        key_stats[i].correct_typed = 0;
    }

    shuffle_indices();

    remaining_time = time;
    initial_duration = time;
    start_frame = timer_counter;

    draw_text(shuffled_indices, phrase_index);
    draw_typed_chars(typed_chars, typed_count, shuffled_indices, phrase_index);
    draw_countdown_timer(remaining_time);
}

void resume_typing_test() {
    start_frame = timer_counter - (initial_duration - remaining_time) * sys_hz();

    draw_text(shuffled_indices, phrase_index);
    draw_typed_chars(typed_chars, typed_count, shuffled_indices, phrase_index);
    draw_countdown_timer(remaining_time);
}

void go_to_next_line() {
    int current_len = strlen(phrases[shuffled_indices[typing_phrase_index]]);
    char_index += current_len;  // Move to next phrase's starting index

    if (typing_phrase_index != 0) phrase_index++;
    typing_phrase_index++;

    draw_text(shuffled_indices, phrase_index);
    draw_typed_chars(typed_chars, typed_count, shuffled_indices, phrase_index);
}

void type_char(char c) {
    if (c == '-') { // Backspace
        if (typed_count > 0) {
            typed_count--;
            typed_chars[typed_count] = '\0';
        }
        draw_text(shuffled_indices, phrase_index);
    } else {
        if (typed_count < MAX_TYPED_CHARS) {
            typed_chars[typed_count++] = c;

            int idx = get_key_index(c);
            if (idx != -1) {
                key_stats[idx].total_typed++;
            }

            int current_phrase_len = strlen(phrases[shuffled_indices[typing_phrase_index]]);

            if (typed_count >= char_index + current_phrase_len) {
                go_to_next_line();
                return;  // already redrawn
            }
        }
    }

    draw_typed_chars(typed_chars, typed_count, shuffled_indices, phrase_index);
}

void calculate_stats() {
    int correct_chars = 0;
    int total_chars = typed_count;
    int index_in_typed = 0;

    for (int i = 0; i <= typing_phrase_index; ++i) {
        const char* phrase = phrases[shuffled_indices[i]];
        int phrase_len = strlen(phrase);

        for (int j = 0; j < phrase_len && index_in_typed < typed_count; ++j, ++index_in_typed) {
            if (typed_chars[index_in_typed] == phrase[j]) {
                correct_chars++;
                int idx = get_key_index(typed_chars[index_in_typed]);
                if (idx != -1) {
                    key_stats[idx].correct_typed++;
                }
            } else {
                // Increment total of expected char so that accuracy of that letter decreases
                int idx = get_key_index(typed_chars[index_in_typed]);
                if (idx != -1) {
                    key_stats[idx].total_typed++;
                }
            }
        }
    }

    // Avoid divide-by-zero
    if (total_chars == 0) {
        final_accuracy = 0;
    } else {
        final_accuracy = (correct_chars * 100) / total_chars;
    }

    final_wpm = (int)((total_chars / 5.0f) / (initial_duration / 60.0f));

    for (int i = 0; i < NUM_KEYS; ++i) {    
        if (key_stats[i].total_typed == 0) {
            perf_map[i] = KEY_NO_DATA;
            continue;
        }
    
        float accuracy = (float) key_stats[i].correct_typed / key_stats[i].total_typed;
    
        if (accuracy >= 0.95f) {
            perf_map[i] = KEY_EXCELLENT;
        } else if (accuracy >= 0.90f) {
            perf_map[i] = KEY_GOOD;
        } else if (accuracy >= 0.80f) {
            perf_map[i] = KEY_AVERAGE;
        } else {
            perf_map[i] = KEY_BAD;
        }
    }
}

void countdown_tick() {
    int elapsed_seconds = (timer_counter - start_frame) / sys_hz();
    remaining_time = initial_duration - elapsed_seconds;

    draw_countdown_timer(remaining_time);

    if (remaining_time < 0) {
        calculate_stats();
        set_state(STATS);
    }
}
