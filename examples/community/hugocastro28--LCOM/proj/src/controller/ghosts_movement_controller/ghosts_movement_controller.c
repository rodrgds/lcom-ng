#include <lcom/lcf.h>

#include "../../model/ghosts_model/ghost.h"
#include "../../model/pacman_model/pacman.h"
#include "../collisions_controller/collisions-controller.h"
#include "../game_controller.h"
#include "../../view/initialize_pixmaps.h"
#include "ghosts_movement_controller.h"

extern Pacman pacman;
extern Ghost ghosts[4];

extern bool game_is_on;

void (moveLeft)(int ghostIndex) {
    Position aux = ghosts[ghostIndex].pos;
    ghosts[ghostIndex].pos.left_x -= GHOST_STEP;
    ghosts[ghostIndex].pos.right_x -= GHOST_STEP;

    if (maze_collision(ghosts[ghostIndex].pos) || ghosts_collision(ghosts[ghostIndex].pos, ghostIndex))
        ghosts[ghostIndex].pos = aux;
    else if (ghost_pacman_collision(ghosts[ghostIndex].pos))
        game_is_on = false;
}

void (moveRight)(int ghostIndex) {
    Position aux = ghosts[ghostIndex].pos;
    ghosts[ghostIndex].pos.left_x += GHOST_STEP;
    ghosts[ghostIndex].pos.right_x += GHOST_STEP;

    if (maze_collision(ghosts[ghostIndex].pos) || ghosts_collision(ghosts[ghostIndex].pos, ghostIndex))
        ghosts[ghostIndex].pos = aux;
    else if (ghost_pacman_collision(ghosts[ghostIndex].pos))
        game_is_on = false;
}

void (moveUp)(int ghostIndex) {
    Position aux = ghosts[ghostIndex].pos;
    ghosts[ghostIndex].pos.bottom_y -= GHOST_STEP;
    ghosts[ghostIndex].pos.top_y -= GHOST_STEP;

    if (maze_collision(ghosts[ghostIndex].pos) || ghosts_collision(ghosts[ghostIndex].pos, ghostIndex))    
        ghosts[ghostIndex].pos = aux;
    else if (ghost_pacman_collision(ghosts[ghostIndex].pos))
        game_is_on = false;
}

void (moveDown)(int ghostIndex) {
    Position aux = ghosts[ghostIndex].pos;
    ghosts[ghostIndex].pos.bottom_y += GHOST_STEP;
    ghosts[ghostIndex].pos.top_y += GHOST_STEP;

    if (maze_collision(ghosts[ghostIndex].pos) || ghosts_collision(ghosts[ghostIndex].pos, ghostIndex))
        ghosts[ghostIndex].pos = aux;
    else if (ghost_pacman_collision(ghosts[ghostIndex].pos))
        game_is_on = false;
}

void (move_ghosts)() {
    for (int i=0; i < 4; i++ ) {        
        // Same line and ghost at the left of pacman
        if (ghosts[i].pos.bottom_y == pacman.pos.bottom_y && 
            ghosts[i].pos.right_x < pacman.pos.right_x) 
            moveRight(i);

        //Same line and ghost at the right of pacman
        else if (ghosts[i].pos.bottom_y == pacman.pos.bottom_y && 
            ghosts[i].pos.right_x > pacman.pos.right_x) 
            moveLeft(i);

        //Same column and ghost at the top of pacman
        else if (ghosts[i].pos.left_x == pacman.pos.left_x && 
            ghosts[i].pos.top_y < pacman.pos.top_y)
            moveDown(i);    

        //Same column and ghost at the bottom of pacman
        else if (ghosts[i].pos.left_x == pacman.pos.left_x && 
            ghosts[i].pos.top_y > pacman.pos.top_y)
            moveUp(i); 

        //Pacman at the top left diagonal
        else if (pacman.pos.top_y < ghosts[i].pos.top_y && 
            pacman.pos.right_x < ghosts[i].pos.right_x)
            moveLeft(i);
            
        //Pacman at the top right diagonal
        else if (pacman.pos.top_y < ghosts[i].pos.top_y &&
            pacman.pos.right_x > ghosts[i].pos.right_x)
            moveRight(i);
            
        //Pacman at the bottom left diagonal 
        else if (pacman.pos.top_y > ghosts[i].pos.top_y &&
            pacman.pos.right_x < ghosts[i].pos.right_x) 
            moveLeft(i);
            
        //Pacman at the bottom right diagonal
        else 
            moveRight(i);
    }
}
