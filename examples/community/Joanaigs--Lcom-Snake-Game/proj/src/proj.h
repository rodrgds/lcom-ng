#ifndef PROJ_H
#define PROJ_H

enum BaseState{
    mainMenu,
    singlePlayer,
    multiPlayer,
    instructions,
    game_over,
    leave
};

enum BaseState baseState;

void single_player();
void multi_player();
void gameOver();
void main_Menu();
void instructions_Menu();
int(proj_main_loop)(int argc, char *argv[]);


#endif //PROJ_H
