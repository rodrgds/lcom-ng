#include <lcom/lcf.h>
#include "violetchi.h"
#include "../images/violetchi4.xpm"
#include "../images/violetchi4_angry.xpm"
#include "../images/violetchi4_sad.xpm"
#include "../images/violetchi4_neutral.xpm"

/*static Violetchi* happy_violetchi;
static Violetchi* sad_violetchi;
static Violetchi* angry_violetchi;
static Violetchi* sleeping_violetchi;
static Violetchi* bath_violetchi;*/

Violetchi*(create_violetchi)(xpm_map_t xpm){
    Violetchi* violetchi = (Violetchi*)malloc(sizeof(Violetchi));

    uint16_t initial_x = 300;
    uint16_t initial_y = 250;
    uint16_t x_speed = 5, y_speed = 0; //it can be changed

    violetchi->sprite = create_sprite(xpm, initial_x, initial_y, x_speed, y_speed);
    start_violetchi_needs(violetchi);
    set_violetchi_mood(violetchi);
    return violetchi;
}

void(start_violetchi_needs)(Violetchi* violetchi){
    violetchi->needs.hunger = HIGH;
    violetchi->needs.to_sleep = HIGH;
    violetchi->needs.hygiene = HIGH;
    violetchi->needs.love = HIGH;
    violetchi->needs.play = HIGH;
}

void(update_violetchi_need)(Violetchi* violetchi, int button){
    switch (button){
    case 0:
        violetchi->needs.hunger = HIGH;
        break;
    case 1:
        violetchi->needs.to_sleep = HIGH;
        break;
    case 2:
        violetchi->needs.hygiene = HIGH;
        break;
    case 3:
        violetchi->needs.love = HIGH;
        break;
    case 4:
        violetchi->needs.play = HIGH;
        break;
    default:
        break;
    }
}


int 
check_num_of_lows(Violetchi* violetchi){
    int num_of_lows = 0;
    if(violetchi->needs.hunger == LOW){
        num_of_lows++;
    }

    if(violetchi->needs.to_sleep == LOW){
        num_of_lows++;
    }

    if(violetchi->needs.hygiene == LOW){
        num_of_lows++;
    }

    if(violetchi->needs.love == LOW){
        num_of_lows++;
        violetchi->mood = SAD;
    }
    if(violetchi->needs.play == LOW){
        num_of_lows++;
    }
    return num_of_lows;
}

int check_num_of_mediums(Violetchi* violetchi){
    int num_of_mediums = 0;
    if(violetchi->needs.hunger == MEDIUM){
        num_of_mediums++;
    }

    if(violetchi->needs.to_sleep == MEDIUM){
        num_of_mediums++;
    }

    if(violetchi->needs.hygiene == MEDIUM){
        num_of_mediums++;
    }

    if(violetchi->needs.love == MEDIUM){
        num_of_mediums++;
    }
    if(violetchi->needs.play == MEDIUM){
        num_of_mediums++;
    }
    return num_of_mediums;
}

int check_num_of_highs(Violetchi* violetchi){
    int num_of_highs = 0;
    if(violetchi->needs.hunger == HIGH){
        num_of_highs++;
    }

    if(violetchi->needs.to_sleep == HIGH){
        num_of_highs++;
    }

    if(violetchi->needs.hygiene == HIGH){
        num_of_highs++;
    }

    if(violetchi->needs.love == HIGH){
        num_of_highs++;
    }
    if(violetchi->needs.play == HIGH){
        num_of_highs++;
    }
    return num_of_highs;
}

void(set_violetchi_mood)(Violetchi* violetchi){
    int num_of_lows = check_num_of_lows(violetchi);

    if (num_of_lows == 5){
        violetchi->mood = DEAD;
        return;
    }

    if(violetchi->mood == SAD) {
        if (violetchi->needs.love != HIGH)
            return;
    }

    if(num_of_lows != 0){
        violetchi->mood = NEUTRAL;

        if(num_of_lows >= 3){
            violetchi->mood = ANGRY;
        }
        return;
    }

    int num_of_highs = check_num_of_highs(violetchi);

    if(num_of_highs == 5){
        violetchi->mood = HAPPY;
    }

    violetchi->mood = HAPPY;
}

void(update_violetchi_position)(Violetchi* violetchi){
    update_sprite(violetchi->sprite);
}

void(release_violet)(Violetchi* violetchi){
    release_sprite(violetchi->sprite);
    free(violetchi);
}

void(update_violetchi)(Violetchi* violetchi)
{
    if(violetchi->mood == HAPPY){
        update_xpm(violetchi->sprite, violetchi4);
    }

    else if(violetchi->mood == SAD){
        update_xpm(violetchi->sprite, violetchi4_sad);
    }

    else if(violetchi->mood == ANGRY){
        update_xpm(violetchi->sprite, violetchi4_angry);
    }

    else if(violetchi->mood == NEUTRAL){
        update_xpm(violetchi->sprite, violetchi4_neutral);
    }

    else if (violetchi->mood == DEAD){
        update_xpm(violetchi->sprite, violetchi4);
    }
}   

