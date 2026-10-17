#include "instructionsView.h"

void createInstructionScreen() {
    i_background = generate_Sprite((xpm_map_t) Instructions);
}

void setInstructionBackgorund() {
    loadBackground(i_background);
}
