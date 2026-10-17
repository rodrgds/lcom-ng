#include <lcom/lcf.h>
#include "game.h"

// Game Levels
#define NUM_LEVELS 3
Level levels[NUM_LEVELS] = {
    {50, 3},    // Level 1
    {100, 2},   // Level 2
    {150, 1}    // Level 3
};


//Game Elements
Game game;  
PlayerShip* player;
Spaceship_Explosion* explosionPlayer;
Cursor* cursor;
Enemy* enemyArray[MAX_ENEMY];  
EnemyFire* enemyFireArray[MAX_ENEMY_FIRE];
EnemyExplosion* enemyExplosion[MAX_ENEMY];
Fire* fireArray[MAX_FIRE];

uint32_t enemyFireCounter = 0;
extern uint32_t fireCounter;
uint32_t fireReload = FIRE_RELOAD;
extern menuSelectedState selected_menu;


//Sprites
extern Sprite* background;
extern Sprite* gameOver;
extern Sprite* sun;
extern Sprite* backButton;
extern Sprite* backButton2;
extern Sprite* twoDots;
extern Sprite* slash;
extern Sprite* secondS;
extern Sprite* points;
extern Sprite* heart;
extern Sprite* levelSprite;
extern Sprite* fireShots;
extern Sprite* numberArray[10];


//Device Variables
extern int timerIntCounter;
extern uint8_t scancode; //Keyboard scancode
extern struct packet pp;
extern timeType timeData;
extern dateType dateData;


void init_level(int levelIndex) {
    game.currentLevel = levelIndex;
    game.levelCountdown = levels[levelIndex].maxTime;

    //Start with 5 enemies alive
    if (levelIndex == 0) {
        reset_game(); //Delete previous game
        timerIntCounter = 0;  //Reset int counter

        for (int i=0; i<5; i++) {
            spawn_enemy();
        }
        
    }
}


//Choose a random enemy to put alive
void spawn_enemy() {

    int attempts = 0;   
    while (attempts < MAX_ENEMY) { //Has maximum attempts so that when all enemies are alive it would be able to stop
        int random_index = rand() % MAX_ENEMY;
        if (!enemyArray[random_index]->alive) {
            enemyArray[random_index]->alive = true;
            return; 
        }
        attempts++;
    }
    return;  //All enemies were already alive, changed none
}

void reset_game() {

    game.currentState = MAINMENU;

    //Reset player 
    player->currLives= 3;
    player->currScore = 0;
    player->alive = true;
    player->x = 545;
    player->y = 800;
    set_x_sprite(player->curr_sprite, 545);
    set_y_sprite(player->curr_sprite, 800);
    player->curr_sprite->xspeed = 0;
    player->curr_sprite->yspeed = 0;

    //Reset player explosion
    explosionPlayer->alive = false;
    explosionPlayer->curr_sprite = (explosionPlayer->sprites[0]);


    //Reset enemies and explosions
    for (int i=0; i<MAX_ENEMY; i++) {
        enemyArray[i]->alive = false;
        enemyExplosion[i]->alive = false;
    }

    //reset spaceship fire

    for(int i = 0; i < MAX_FIRE;i++) {
        fireArray[i]->alive=false;
    }

    fireReload = FIRE_RELOAD;
    fireCounter = 0;

    //reset enemy fire
    for(int i = 0; i < MAX_ENEMY_FIRE;i++) {
        enemyFireArray[i]->alive=false;
    }
    

}



void handle_game_loop() {
    if (game.currentState == PLAYING) { 

        //If player looses all its lives, see it exploding before Game Over
        if (player->currLives == 0) { 
            player->alive = false;
            game.levelCountdown = 1;
            game.currentLevel=NUM_LEVELS-1;
            player->currLives = -1;
        }

        //1 second passed
        if (timerIntCounter % 60 == 0) {
            game.levelCountdown--;
              
            if (game.levelCountdown == 0) {  //If maximum time for a level ends, go to the next one or end game
                if (game.currentLevel == (NUM_LEVELS-1)) {
                    game.currentState = ENDSCREEN;
                }
                else {
                    init_level(game.currentLevel+1);
                }
                
            }
        }
        
        //Enemy spawn rate seconds passed
         if (timerIntCounter % (levels[game.currentLevel].enemySpawnRate * 60) == 0) {
            spawn_enemy();
         }

        //fires a enemy shot
        if(timerIntCounter % ENEMY_FIRE_RATE == 0) {
            
            // for(int i = 0; i < MAX_ENEMY; i++)
            int random_index = rand() % MAX_ENEMY;
            if(enemyArray[random_index]->alive==true) {
                createEnemiesFire(enemyArray[random_index]);
            }
        }

        // waiting timr after player spends all of "they" bullets
        if(fireCounter == MAX_FIRE) {
            if(fireReload == 0) {
                fireReload = FIRE_RELOAD;
                fireCounter = 0;
            }
            fireReload--;
        }
        
    }

}

//Updates game current state according to button clicks
void handle_mouse_move() {
    updateCursor(cursor, cursor->x+pp.delta_x, cursor->y-pp.delta_y);

    if (game.currentState == MAINMENU) { 
        drawCursor(cursor);

        if ((cursor->x >= 100) && (cursor->x <=514)) {
            if ((cursor->y >= 426) && (cursor->y <= 585)) { //Play Button
                selected_menu = HOVER_PLAY;
                if(pp.lb) {
                    init_level(0); //Initialize first level
                    game.currentState = PLAYING;
                    return;
                } 
                
            }  
            else if ((cursor->y >= 655) && (cursor->y <= 814)) { //Leaderboard Button
                selected_menu = HOVER_LEADER_BOARD;
                if(pp.lb) game.currentState = LEADERBOARDMENU;
                return;
            }
            else {
                selected_menu = INITIAL;
        }

        }
        else if ((cursor->x >= 638) && (cursor->x<=1052)) {
            if ((cursor->y >= 426) && (cursor->y <= 585)) { //Instructions Button
                selected_menu = HOVER_INSTRUCTIONS;
                if(pp.lb) {
                    game.currentState = INSTRUCTIONSMENU;
                    return;
                } 
            }  
            else if ((cursor->y >= 655) && (cursor->y <= 814)) { //Quit Button
                selected_menu = HOVER_QUIT;
                if(pp.lb) {
                    game.currentState = EXITED;
                    return;
                }
            }
            else {
            selected_menu = INITIAL;
        }

        }
        else {
            selected_menu = INITIAL;
            return;
        }
    }
    if ((game.currentState == INSTRUCTIONSMENU) || (game.currentState == LEADERBOARDMENU) || (game.currentState == ENDSCREEN)) {
         drawCursor(cursor);

         if ((cursor->x >= 638) && (cursor->x<=1052)) {
            if ((cursor->y >= 655) && (cursor->y <= 814)) { //Back Button
                selected_menu = HOVER_BACK;
                if(pp.lb) {
                    if (game.currentState == ENDSCREEN) { reset_game();}
                    game.currentState = MAINMENU;
                    return;
                }

            }
            else {
            selected_menu = INITIAL;
        }
        }
         else {
            selected_menu = INITIAL;
        }

    }
    else if(game.currentState == PLAYING) {
        if(pp.lb) {
            createSpaceShipFire(player);
            return;
        }
    }

    else {  
        return;
    }
    

}



void show_current_screen() {
    switch(game.currentState) {
        case MAINMENU:
            draw_menu(MAIN);
            break;
        case INSTRUCTIONSMENU:
            draw_menu(INSTRUCTIONS);
            break;
        case LEADERBOARDMENU:
            draw_menu(LEADERBOARD);
            break;
        case PLAYING:
             draw_game_screen();
             break;
        case ENDSCREEN:
             draw_end_screen(); 
             break;
        default:
            break;
    }

}

void draw_game_status() {
    uint16_t x=10;  //Initial position
    uint16_t y=10;

    //Draw level
    set_x_sprite(levelSprite, x);
    set_y_sprite(levelSprite, y);
    draw_sprite(levelSprite);
    x=x+65;
    draw_digit(x,y,game.currentLevel+1);
    x=x+40;

    //Draw countdown
    draw_three_digits(game.levelCountdown, x, y, 20);
    x=x+65;
    set_x_sprite(secondS, x);
    set_y_sprite(secondS, y);
    draw_sprite(secondS);
    x=x+30;

    //Draw Score
    draw_three_digits(player->currScore, x, y, 20);
    x=x+60;
    set_x_sprite(points, x);
    set_y_sprite(points, y);
    draw_sprite(points);
    x=x+80;

    draw_three_digits(MAX_FIRE-fireCounter, x, y, 20);

    x=x+65;

    set_x_sprite(fireShots, x);
    set_y_sprite(fireShots, y);
    draw_sprite(fireShots);

    x=x+80;

    //Draw Life Hearts
    for (int i=0; i<player->currLives; i++) {
        set_x_sprite(heart, x+i*45);
        set_y_sprite(heart, 5);
        draw_sprite(heart);
    }

}




void draw_game_screen() {
    draw_sprite(background);

    //Draw Sun during the day
    if ((timeData.hours > 6) && (timeData.hours < 20)) {
            draw_sprite(sun);
        }
    if ((player->currLives)>0) {
        draw_game_status();
    }
    
    move_sprite(player->curr_sprite);
    moveFire();
    updateSpaceshipExplosions();
    updateEnemiesAndExplosions();
    moveEnemyFire();

    //Draw player if it is not exploding
    if(player->alive) {
        drawPlayerShip(player);
    }


    //Draw all alive enemies
    for (int i=0; i<MAX_ENEMY; i++) {
        if (enemyArray[i]->alive) {
            drawCurrEnemySprite(enemyArray[i]);
        }
    }


}

void draw_end_screen() {
    draw_sprite(background);
    draw_sprite(gameOver);
    draw_time();
    
    //Draw Sun during the day
        if ((timeData.hours > 6) && (timeData.hours < 20)) {
            draw_sprite(sun);
        }
   
    draw_three_digits(player->currScore, 520, 550, 20);
     set_x_sprite(points, 590);
     set_y_sprite(points, 550);
     draw_sprite(points);
    switch (selected_menu) {
        case HOVER_BACK:
            draw_sprite(backButton2);
            break;
        default:
            draw_sprite(backButton);
            break;
        }
    drawCursor(cursor);
    
      
}
    




void handle_pressed_key() {
    if (game.currentState == MAINMENU) {  
        if (scancode == BACK_SPACE_MAKE) {
            game.currentState = EXITED;
        }
        if (scancode == Q_MAKE) {
            game.currentState = EXITED;
        }

    }
    if (game.currentState == INSTRUCTIONSMENU) {
       if (scancode == BACK_SPACE_MAKE) {
            game.currentState = MAINMENU;
        }

    }
    if (game.currentState == LEADERBOARDMENU) {
        if (scancode == BACK_SPACE_MAKE) {
            game.currentState = MAINMENU;
        }

    }
    if (game.currentState == ENDSCREEN) {
         if (scancode == BACK_SPACE_MAKE) {
            game.currentState = MAINMENU;
        }

    }
    

    if (game.currentState == PLAYING) {
        if (scancode == Q_MAKE) {
            game.currentState = MAINMENU;
        }
        
        if (scancode == BACK_SPACE_MAKE) {
           game.currentState = MAINMENU;
        }

        //Only move player while it's not exploding
         if(!explosionPlayer->alive) {
             switch (scancode)
        {
        case D_MAKE:
            player->curr_sprite->xspeed = SPACESHIP_SPEED;
            player->curr_sprite->yspeed = 0;
            break;
        case A_MAKE:
            player->curr_sprite->xspeed = -SPACESHIP_SPEED;
            player->curr_sprite->yspeed = 0;
            break;
        case S_MAKE:
            player->curr_sprite->xspeed = 0;
            player->curr_sprite->yspeed = 0;
            break;
        case SPACE_BAR_MAKE:
            createSpaceShipFire(player);
            break;
        default:
            break;
    }
         }
                    
    }

    }
   
    








