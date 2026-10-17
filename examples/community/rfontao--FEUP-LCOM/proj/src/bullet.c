#include "bullet.h"

extern uint16_t hres;
extern uint16_t vres;


Bullet * bullets[MAX_BULLET_COUNT];


Bullet * createBullet(int initial_x, int initial_y, int initial_vx,int initial_vy, unsigned short int group){

        Sprite* sprite = loadSprite(bullet_xpm);


        sprite->x = initial_x;
        sprite->y = initial_y;

        sprite->xspeed = initial_vx;
        sprite->yspeed = initial_vy;

        Bullet* bullet = (Bullet*)malloc(sizeof(Bullet));

        if(!bullet){
                return NULL;
        }

        bullet->sprite = sprite;
        bullet->group = group;

        return bullet;

}

void updateBullets(Bullet * bullets[])
{
        removeBullets(bullets);
        for (size_t i = 0; i < MAX_BULLET_COUNT; ++i){
                if(bullets[i] != NULL){

                        move_sprite(bullets[i]->sprite);

                }
        }

}

void shoot(Bullet * bullets[], int xi,int yi, int xf,int yf, unsigned short int group){


        float vx = xf - xi;
        float vy = yf - yi;
        float size = sqrt(vx * vx + vy * vy);

        vx = BULLET_VELOCITY * (vx / size);
        vy = BULLET_VELOCITY * (vy / size);   

        Bullet * bullet = createBullet(xi,yi,(int)vx,(int)vy, group);

        if(bullet){

                bool foundSpace = false;
                for (size_t i = 0; i < MAX_BULLET_COUNT; ++i){
                        if(bullets[i] == NULL){
                                bullet->id = i; 
                                bullets[i] = bullet;
                                foundSpace = true;
                                break;
                        }
                }

                if(!foundSpace){
                        destroyBullet(bullet);
                }
        }

}

void drawBullets(Bullet * bullets[], Car* car, Objective* object){

        for (size_t i = 0; i < MAX_BULLET_COUNT; ++i){
                if(bullets[i] != NULL){

                        drawSprite(bullets[i]->sprite,bullets[i]->sprite->x,bullets[i]->sprite->y);
                        if(bullets[i]->group == ALLYBULLETGROUP)
                        {
                                drawCollidible(bullets[i]->sprite, ALLYBULLETGROUP,bullets[i]->id, car, object);
                        }
                        else if(bullets[i]->group == ENEMYBULLETGROUP)
                        {
                                drawCollidible(bullets[i]->sprite, ENEMYBULLETGROUP,bullets[i]->id, car, object);
                        }

                }
        }

}

void removeBullets(Bullet * bullets[]){

        for (size_t i = 0; i < MAX_BULLET_COUNT; ++i){
                if(bullets[i] != NULL){
                        if(bullets[i]->sprite->x < 0 || bullets[i]->sprite->x > XRES || bullets[i]->sprite->y < 0 || bullets[i]->sprite->y > YRES){
                                destroyBullet(bullets[i]);
                                bullets[i] = NULL;
                        }
                }
        }
}

void removeBulletById(Bullet * bullets[],unsigned short int bullet_id){

        //destroyBullet(bullets[bullet_id]);
        free(bullets[bullet_id]);
        bullets[bullet_id] = NULL;
}

void makeBulletsNull(Bullet * bullets[]){

        for (size_t i = 0; i < MAX_BULLET_COUNT;++i){
                if(bullets[i] != NULL){
                        destroyBullet(bullets[i]);
                }
                bullets[i] = NULL;
        }

}

void destroyBullet(Bullet * bullet){

        destroy_sprite(bullet->sprite);
        free(bullet);

}
