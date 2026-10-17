#include "collisions.h"


extern uint16_t hres;
extern uint16_t vres;
extern int bitsPerPixel;

extern unsigned int round_time;

extern Bullet* bullets[];

extern Enemy* enemies[];

CollisionCell collision_matrix[YRES][XRES];

extern int score;

void cleanCollisionMatrix(){

        CollisionCell aux;
        aux.group = 0;
        aux.id = 0;

        for (size_t i = 0; i < YRES;++i){
                for(size_t j = 0;j < XRES;++j)
                        collision_matrix[i][j] = aux;
        }

}

void drawCollidible(Sprite * sprite, unsigned short int group, unsigned short int id, Car *car, Objective *object) { 

        CollisionCell colCell;
        colCell.group = group;
        colCell.id = id;

        int x = sprite->x;
        int y = sprite->y;

        uint8_t *aux = sprite->map;

	uint16_t copyX = x;
	
	bool breeak = false;

	for (uint16_t i = 0; i < sprite->height; i++) {
		if(y >= YRES || y < 0){
			y++;
			sprite->map += sprite->width;
			x = copyX;
		} else {
			for (uint16_t j = 0; j < sprite->width; j++) {

				if(x >= XRES || x < 0){
					sprite->map += (bitsPerPixel/8);
				} else {

					if(*(uint16_t*)(sprite->map) != xpm_transparency_color(XPM_5_6_5)){
						if(collision_matrix[y][x].group == 0){
                                                        collision_matrix[y][x] = colCell;
                                                } else {
							if(collisionHandler(collision_matrix[y][x], colCell, car, object))
							{
								breeak = true;
								break;
							}
                                                }

					}
					sprite->map += (bitsPerPixel/8);

					x++;
				}
			}
			if(breeak)
				break;

			y++;
			x = copyX;
		}
	}
	sprite->map = aux;

}

bool collisionHandler(CollisionCell collider1,CollisionCell collider2, Car* car, Objective* object){

        if(collider2.group == CARGROUP)
	{
		if(collider1.group == ENEMYGROUP)
		{
			enemies[collider1.id]->hp = 0;
			return false;
		}
	}
	else if(collider2.group == ALLYBULLETGROUP)
	{
		if(collider1.group == ENEMYGROUP)
		{
			removeBulletById(bullets, collider2.id);
			enemies[collider1.id]->hp -= 30;
			score += 2;
			return true;
		}
	} 
	else if(collider2.group == ENEMYBULLETGROUP)
	{
		if(collider1.group == CARGROUP)
		{
			removeBulletById(bullets,collider2.id);
			car->hp -= 5;
			score -= 5;
			return true;
		}
		else if(collider1.group == OBJECTIVEGROUP)
		{
			removeBulletById(bullets, collider2.id);
			object->hp -= 1;
			score -= 5;
			return true;
		}
	}
	return false;
}
