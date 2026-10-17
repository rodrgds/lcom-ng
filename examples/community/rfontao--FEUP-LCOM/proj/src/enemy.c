#include "enemy.h"

#include "resources/enemy1.xpm"
#include "resources/enemyBar.xpm"

extern uint16_t vres;
extern uint16_t hres;

extern Bullet* bullets[];

Enemy* enemies[MAX_ENEMY_COUNT];
Sprite* enemyBars[MAX_ENEMY_COUNT];

extern int score;

int enemy_hp = ENEMY_HP_INITIAL;
unsigned int enemy_count = INITIAL_ENEMY_COUNT;

extern bool multi;

int spawnPoints[27][2] = { {0, 756}, {0, 700} , {1939, 658}, {1939, 680}, {500, 1939}, {700, 1939},{710, 1939},{730, 1939},{760, 1939},{800, 1939},{830, 1939},{900, 1939},{960, 1939},{1100, 1939}, {1170, 1939},{1250, 1939},{1300, 1939},{1370, 1939}, {1200, 1939},{1400, 1939},{1470, 1939},
{1550, 1939},{1600, 1939}, {1500, 1939} , {270, 1939}, {1640, 1939} };

void spawnEnemy()
{
        Sprite *sprite = loadSprite(enemy1);
	Sprite *enemyHp = loadSprite(enemyBar);
	unsigned int point;
	static size_t i = 0;
	if(multi)
	{
      	point = i;
		i++;
		if(i > 26)
			i = 0;
	}
	else
	      point = rand() % 26;

	
        sprite->xspeed = 0;
        sprite->yspeed = 0;	
	sprite->x = spawnPoints[point][0];
	sprite->y = spawnPoints[point][1];
        Enemy *enemy = (Enemy*)malloc(sizeof(Enemy));
        enemy->sprite = sprite;
        enemy->shootingCar = false;
        enemy->shootingObj = false;
	enemy->gun_cooldown = ENEMY_GUN_COOLDOWN;
        enemy->hp = enemy_hp;
        if(enemy != NULL)
        {
                for(size_t i = 0; i < enemy_count; i++)
                {
                        if(enemies[i] == NULL)
                        {
                                enemy->id = i;
                                enemies[i] = enemy;
				enemyBars[i] = enemyHp;
                                return;
                        }
                }
        }
        destroy_sprite(sprite);
        free(enemy);
}

void drawEnemies(Car* car, Objective* obj)
{
        for(size_t i = 0; i < enemy_count; i++)
        {
                if(enemies[i] != NULL)
                {
                        drawSprite(enemies[i]->sprite,enemies[i]->sprite->x,enemies[i]->sprite->y);
                        drawCollidible(enemies[i]->sprite, ENEMYGROUP, enemies[i]->id, car, obj);
			enemyBars[i]->width = (ENEMY_HP_INITIAL*enemies[i]->hp)/enemy_hp;
			drawSprite(enemyBars[i], enemies[i]->sprite->x,enemies[i]->sprite->y - 15);
                }
        }
}

void animEnemy(Enemy * enemy, Sprite * enemyAnim[])
{
	if(enemy->sprite->xspeed == 0 && enemy->sprite->yspeed < 0)
	{
		enemy->sprite->height = enemyAnim[0]->height;
		enemy->sprite->width = enemyAnim[0]->width;
		enemy->sprite->map = enemyAnim[0]->map;
	}
	else if(enemy->sprite->xspeed > 0 && enemy->sprite->yspeed<0 && enemy->sprite->xspeed < -enemy->sprite->yspeed)
	{
		enemy->sprite->height = enemyAnim[1]->height;
		enemy->sprite->width = enemyAnim[1]->width;
		enemy->sprite->map = enemyAnim[1]->map;
	}
	else if(enemy->sprite->xspeed > 0 && enemy->sprite->yspeed<0 && enemy->sprite->xspeed == -enemy->sprite->yspeed)
	{
		enemy->sprite->height = enemyAnim[2]->height;
		enemy->sprite->width = enemyAnim[2]->width;
		enemy->sprite->map = enemyAnim[2]->map;
	}
	else if(enemy->sprite->xspeed > 0 && enemy->sprite->yspeed<0 && enemy->sprite->xspeed > -enemy->sprite->yspeed)
	{
		enemy->sprite->height = enemyAnim[3]->height;
		enemy->sprite->width = enemyAnim[3]->width;
		enemy->sprite->map = enemyAnim[3]->map;
	}
	else if(enemy->sprite->xspeed > 0 && enemy->sprite->yspeed == 0)
	{
		enemy->sprite->height = enemyAnim[4]->height;
		enemy->sprite->width = enemyAnim[4]->width;
		enemy->sprite->map = enemyAnim[4]->map;
	}
	else if(enemy->sprite->xspeed > 0 && enemy->sprite->yspeed > 0 && enemy->sprite->xspeed > enemy->sprite->yspeed)
	{
		enemy->sprite->height = enemyAnim[5]->height;
		enemy->sprite->width = enemyAnim[5]->width;
		enemy->sprite->map = enemyAnim[5]->map;
	}
	else if(enemy->sprite->xspeed > 0 && enemy->sprite->yspeed > 0 && enemy->sprite->xspeed == enemy->sprite->yspeed)
	{
		enemy->sprite->height = enemyAnim[6]->height;
		enemy->sprite->width = enemyAnim[6]->width;
		enemy->sprite->map = enemyAnim[6]->map;
	}
	else if(enemy->sprite->xspeed > 0 && enemy->sprite->yspeed > 0 && enemy->sprite->xspeed < enemy->sprite->yspeed)
	{
		enemy->sprite->height = enemyAnim[7]->height;
		enemy->sprite->width = enemyAnim[7]->width;
		enemy->sprite->map = enemyAnim[7]->map;
	}
	else if(enemy->sprite->xspeed == 0 && enemy->sprite->yspeed > 0)
	{
		enemy->sprite->height = enemyAnim[8]->height;
		enemy->sprite->width = enemyAnim[8]->width;
		enemy->sprite->map = enemyAnim[8]->map;
	}
	else if(enemy->sprite->xspeed < 0 && enemy->sprite->yspeed > 0 && -enemy->sprite->xspeed < enemy->sprite->yspeed)
	{
		enemy->sprite->height = enemyAnim[9]->height;
		enemy->sprite->width = enemyAnim[9]->width;
		enemy->sprite->map = enemyAnim[9]->map;
	}
	else if(enemy->sprite->xspeed < 0 && enemy->sprite->yspeed > 0 && -enemy->sprite->xspeed == enemy->sprite->yspeed)
	{
		enemy->sprite->height = enemyAnim[10]->height;
		enemy->sprite->width = enemyAnim[10]->width;
		enemy->sprite->map = enemyAnim[10]->map;
	}
	else if(enemy->sprite->xspeed < 0 && enemy->sprite->yspeed > 0 && -enemy->sprite->xspeed > enemy->sprite->yspeed)
	{
		enemy->sprite->height = enemyAnim[11]->height;
		enemy->sprite->width = enemyAnim[11]->width;
		enemy->sprite->map = enemyAnim[11]->map;
	}
	else if(enemy->sprite->xspeed < 0 && enemy->sprite->yspeed == 0)
	{
		enemy->sprite->height = enemyAnim[12]->height;
		enemy->sprite->width = enemyAnim[12]->width;
		enemy->sprite->map = enemyAnim[12]->map;
	}
	else if(enemy->sprite->xspeed < 0 && enemy->sprite->yspeed < 0 && enemy->sprite->xspeed < enemy->sprite->yspeed)
	{
		enemy->sprite->height = enemyAnim[13]->height;
		enemy->sprite->width = enemyAnim[13]->width;
		enemy->sprite->map = enemyAnim[13]->map;
	}
	else if(enemy->sprite->xspeed < 0 && enemy->sprite->yspeed < 0 && enemy->sprite->xspeed == enemy->sprite->yspeed)
	{
		enemy->sprite->height = enemyAnim[14]->height;
		enemy->sprite->width = enemyAnim[14]->width;
		enemy->sprite->map = enemyAnim[14]->map;
	}
	else if(enemy->sprite->xspeed < 0 && enemy->sprite->yspeed < 0 && enemy->sprite->xspeed > enemy->sprite->yspeed)
	{
		enemy->sprite->height = enemyAnim[15]->height;
		enemy->sprite->width = enemyAnim[15]->width;
		enemy->sprite->map = enemyAnim[15]->map;
	}
	
}

void killEnemy(unsigned short int enemy_id)
{
        //free(enemies[enemy_id]->sprite->map);
        //free(enemies[enemy_id]->sprite);
        free(enemies[enemy_id]);
        enemies[enemy_id] = NULL;
}

void updateEnemies(Car *car, Objective *obj, Sprite *enemyAnim[])
{
        for(size_t i = 0; i < enemy_count; i++)
        {
                if(enemies[i] != NULL)
                {
                        if(enemies[i]->hp <= 0)
                        {
                                killEnemy(i);
				score += 50;
                                continue;
                        }
                        if(!(enemies[i]->shootingCar || enemies[i]->shootingObj))
                        {
                                if(obj->sprite->x < enemies[i]->sprite->x)
				{
                                        enemies[i]->sprite->xspeed = -1;
				}
                                else if(obj->sprite->x > enemies[i]->sprite->x)
                                {
                                        enemies[i]->sprite->xspeed = 1;
                                }
                                else if(obj->sprite->x == enemies[i]->sprite->x)
                                {
                                        enemies[i]->sprite->xspeed = 0;
                                }
                                if(obj->sprite->y < enemies[i]->sprite->y)
				{
                                        enemies[i]->sprite->yspeed = -1;
				}
                                else if(obj->sprite->y > enemies[i]->sprite->y)
                                {
                                        enemies[i]->sprite->yspeed = 1;
                                }
                                else if(obj->sprite->y == enemies[i]->sprite->y)
                                {
                                        enemies[i]->sprite->yspeed = 0;
                                }
				if(enemies[i]->sprite->x <= 258 || enemies[i]->sprite->x >= 1739)
				{
					enemies[i]->sprite->yspeed = 0;
				}
				
                                enemies[i]->sprite->x += enemies[i]->sprite->xspeed;
                                enemies[i]->sprite->y += enemies[i]->sprite->yspeed;

                                if(enemies[i]->sprite->y < 0)
                                        enemies[i]->sprite->y = 0;
                                else if(enemies[i]->sprite->y + enemies[i]->sprite->height > YRES)
                                        enemies[i]->sprite->y = YRES - enemies[i]->sprite->height;

                                if(enemies[i]->sprite->x < 0)
                                        enemies[i]->sprite->x = 0;
                                else if(enemies[i]->sprite->x + enemies[i]->sprite->width > XRES)
                                        enemies[i]->sprite->x = XRES - enemies[i]->sprite->width;
				
				
				    
			}
			else if(enemies[i]->shootingCar)
			{
				enemies[i]->sprite->xspeed = car->sprite->x - enemies[i]->sprite->x;
				enemies[i]->sprite->yspeed = car->sprite->y - enemies[i]->sprite->y;
				if(enemies[i]->gun_cooldown == 0)
				{
					enemies[i]->gun_cooldown = ENEMY_GUN_COOLDOWN;
					shoot(bullets, enemies[i]->sprite->x + enemies[i]->sprite->width/2 - 10,enemies[i]->sprite->y + enemies[i]->sprite->height/2 - 10 ,car->sprite->x + car->sprite->width/2 - 10,car->sprite->y + car->sprite->height/2 - 10, ENEMYBULLETGROUP);
				}
				else
				{
					enemies[i]->gun_cooldown--;
				}
				
			}
			else if(enemies[i]->shootingObj)
			{
				if(enemies[i]->gun_cooldown == 0)
				{
					enemies[i]->gun_cooldown = ENEMY_GUN_COOLDOWN;
					shoot(bullets, enemies[i]->sprite->x + enemies[i]->sprite->width/2 - 10,enemies[i]->sprite->y + enemies[i]->sprite->height/2 - 10 ,obj->sprite->x + obj->sprite->width/2 - 10,obj->sprite->y + obj->sprite->height/2 - 10, ENEMYBULLETGROUP);
				}
				else
				{
					enemies[i]->gun_cooldown--;
				}
			}
			if(abs(car->sprite->x - enemies[i]->sprite->x) <= 200 && abs(car->sprite->y - enemies[i]->sprite->y) <= 200)
			{
				enemies[i]->shootingCar = true;
				enemies[i]->shootingObj = false;
			}
			else if(abs(obj->sprite->y - enemies[i]->sprite->y) <= 200 && abs(obj->sprite->y - enemies[i]->sprite->y) <= 200) 
			{
				enemies[i]->shootingObj = true;
				enemies[i]->shootingCar = false;
			}    
			else
			{
				enemies[i]->shootingObj = false;
				enemies[i]->shootingCar = false;
			}
                        animEnemy(enemies[i], enemyAnim);
                        
                }
        }
}
