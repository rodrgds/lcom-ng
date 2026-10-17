
#include "sprite.h"

typedef enum {LOW, MEDIUM, HIGH} Scale;
typedef enum {HAPPY, SAD, ANGRY, NEUTRAL, DEAD} Mood;

typedef struct {
    Scale hunger;
    Scale to_sleep;
    Scale hygiene;
    Scale love;
    Scale play;
} Needs;


typedef struct{
    Sprite* sprite;
    Needs needs;
    Mood mood;

} Violetchi;

void(initialize_all_violetchis)();

Violetchi*(create_violetchi)(xpm_map_t xpm);

void(start_violetchi_needs)(Violetchi* violetchi);

void(start_violetchi_mood)(Violetchi* violetchi);

void(set_violetchi_mood)(Violetchi* violetchi);

void(update_violetchi_position)(Violetchi* violetchi);

void(update_violetchi_need)(Violetchi* violetchi, int button);

void(release_violet)(Violetchi* violetchi);

void(update_violetchi)(Violetchi* violetchi);
