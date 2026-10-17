#include <lcom/lcf.h>
#include "game.h"


/** @defgroup Game Game
 * @{
 *
 * Contais tha code for the game screen
 */

extern enum game_state game_state;

///If the game is paused or not
bool paused = false;

///Value of the player x in the last frame, to erease the image
int ox = 0;

///Player y velocity
float vel;

//Gravity aceleration
float acel = 1;

///Velocity the player bounces back from a platform
float bounce_vel = -20;

///Platform y velocity
float asteroid_vel;

///Platform aceleration 
float asteroid_acel = 0.001;

///Player score
int score;

///Number of platforms in screen
int asteroids_size = 10;

///Player object
Object alien;

///Platform vector
Object* asteroids;

extern xpm_image_t alien_sprite;

extern xpm_image_t asteroid_sprite;

extern xpm_image_t broken_asteroid_sprite;

extern xpm_image_t background_sprite;


///Initializes game variables
void game_init()
{
    vel = 0; 
    asteroid_vel = 2;
    score = 0;

    asteroids = malloc(sizeof(Object)*asteroids_size);

    for(int i = 0; i < asteroids_size; i++)
    {
    	int x = rand() % (SCREEN_WIDTH - asteroid_sprite.width);
    	int y = SCREEN_HEIGHT/asteroids_size*i;
        Object asteroid = newObject(x,y,asteroid_sprite,PLATFORM);
        asteroids[i] = asteroid;
        if(i == asteroids_size/2)
        {
            alien = newObject(asteroid.x,asteroid.y-alien_sprite.height,alien_sprite,PLAYER);
        }
    }

}

///Called once per frame in game screen, manages moviment, colision, actions
void game_frame()
{
    
    vg_draw_rectangle(ox,alien.y,alien.width,alien.height,0);
    clear_text("score: ",5,5);
    clear_number(score, 100, 3);

    alien.y +=  vel;
    vel += acel;
    asteroid_vel += asteroid_acel;
    for(int i = 0; i < asteroids_size; i++)
    {
        if(asteroids[i].y + asteroid_vel > SCREEN_HEIGHT)
        {
            int x = rand() % (SCREEN_WIDTH - asteroids[i].width);
            int random_to_100 = rand() % 100;

            if(random_to_100 < score/20)
            {
                asteroids[i].sprite = broken_asteroid_sprite;
                asteroids[i].object_tag = PLATFORM_BROKEN;
            }
            else if(asteroids[i].object_tag == PLATFORM_BROKEN)
            {
                asteroids[i].sprite = asteroid_sprite;
                asteroids[i].object_tag = PLATFORM;
            }

            asteroids[i].x = x;
            asteroids[i].y = -asteroids[i].height;
            asteroids[i].active = true;
        }

        vg_draw_rectangle(asteroids[i].x,asteroids[i].y,asteroids[i].width,asteroids[i].height,0);
    
        asteroids[i].y += asteroid_vel;

        //if(alien.y <= SCREEN_HEIGHT/4) asteroids[i].y += abs(vel);

        if(colides(alien,asteroids[i]) && vel > 0)
        {
            alien.y = asteroids[i].y - alien.height;
            vel = bounce_vel;
            if(asteroids[i].object_tag == PLATFORM_BROKEN) asteroids[i].active = false;
        }
        if(asteroids[i].active) draw_object(asteroids[i]);
    }
    
    if(alien.y > SCREEN_HEIGHT)
    {
        change_game_state(&game_state, GAME_OVER);
        game_exit();
        return;
    }
    if(alien.x + alien.width/2 > SCREEN_WIDTH)
    {
        alien.x = -alien.width/2;
    }
    else if(alien.x + alien.width/2 < 0)
    {
        alien.x = SCREEN_WIDTH - alien.width/2;
    }
    
    int oy = alien.y;
    //if(alien.y <= SCREEN_HEIGHT/4) alien.y = SCREEN_HEIGHT/4;
    draw_sprite(background_sprite,0,0);
    draw_object(alien);
    alien.y = oy;
    draw_text("score: ",5,5);
    draw_number(score, 100, 3);
    draw_frame();

    ox = alien.x;
}

///Adds the score of the player, twice per second
void addScore()
{
    score = (score + asteroid_vel > 999999999) ? 999999999 : score + asteroid_vel;
}

///Called once per mouse interupt, adds player position to mouse delta
void game_mouse(struct packet pckt)
{
	alien.x+= pckt.delta_x;	
}

///Dealocates memory allocated for platform vector
void game_exit()
{
    free(asteroids);
}

///Called once per keyboard interupt, manages pause and exit
void game_keyboard(uint8_t data)
{
    if(data == SPACE_UP)
    {
        paused = !paused;
        if(paused) draw_sprite_xpm(PAUSED, (SCREEN_WIDTH/2)-100, (SCREEN_HEIGHT/2)-75);
        else vg_draw_rectangle((SCREEN_WIDTH/2)-100,(SCREEN_HEIGHT/2)-75,200,75,0);
        draw_frame();
    }
    else if(data == ESCKEY)
    {
        change_game_state(&game_state, END);
        return;
    }
}
