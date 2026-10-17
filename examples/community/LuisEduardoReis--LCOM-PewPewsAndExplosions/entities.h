#ifndef __ENTITIES_H_
#define __ENTITIES_H_

#include "graphics.h"
#include "sprites.h"


enum entityType {BASE, PLAYER, ENEMY, ENEMYCRUISER, ENEMYMOTHERSHIP, BULLET, EBULLET, EXPLOSION, POWERUP};

typedef struct {
	int ox,oy,w,h;
} rectangle;


typedef struct {
	enum entityType type; //tipo de entidade
	int remove; //indica se a entidade esta para ser removida
	double x,y, dx,dy, xp, yp; //posicao e velocidade
	int visible, solid;
	int health;

	rgbImage* sprite;
	/*
	 * imageindex - indice do sprite
	 * animTimer - contador de transicao de imagem (quando chega a 0, incrementa o imageIndex
	 * animLenght - numero de imagens no sprite
	 * animSpeed - delay entre cada troca
	 * */

	int ImageIndex,AnimTimer,AnimLength,AnimSpeed;
	unsigned int blend; //cor com que o sprite e misturado
	int hurtTimer;

	rectangle mask;
} entity;

typedef struct {
	entity base;

	// Shooty Action
	/*
	 * canShoot - indica se pode disparar
	 * shootTimer - timer para quando o proximo disparo pode ser feito
	 * shotDelay - intervalo entre disparos
	 * weaponHeat - aquecimento da arma
	 * */
	int canShoot, shootTimer, shotDelay, weaponHeat;
	
	//PowerUps
	int hasShield;
	int hasRedBeam;
	int isInvincible;

} Player;

//bala do jogador
typedef struct {
	entity base;

	char type;
	int damage;
} Bullet;


//balas dos inimigos
enum EBulletType {NORMAL, CRUISER, FIRE, BIGCRUISER, BIGFIRE, LIGHTBEAM};
typedef struct {
	entity base;

	enum EBulletType type;
	int damage;
} EBullet;


typedef struct {
	entity base;
} Explosion;

typedef struct {
	entity base;

	// Shooty Action
	int canShoot, shootTimer, shotDelay;

	int killedByPlayer;
} Enemy;

typedef struct {
	entity base;

	// Shooty Action
	int canShoot, shootTimer, shotDelay;
	int shotIndex;

	int killedByPlayer;
} EnemyCruiser;


typedef struct{
	entity base;

	//Shooty Action
	int canShoot;
	int shootTimerC, shotDelayC; //timer para as balas tipo cruiser
	int shootTimerBF, shotDelayBF; //timer para as balas tipo big fire
	int shootTimerF, shotDelayF; //timer para as balas tipo fire
	int shootTimerBC, shotDelayBC; //timer para as balas tipo big cruiser
	int shootTimerLB, hasLightBeam; //timer para o raio de luz
	int shieldTimer, shieldDelay;
	int shotIndex;
	int higherBullets;
	int hasShield;

	int killedByPlayer;
} EnemyMotherShip;



enum PowerUpType {SHIELD, LIFE, REDBEAM};
typedef struct {
	entity base;

	enum PowerUpType type;

} PowerUp;

void tickEntity(entity* e);
void drawEntity(entity* e);
void destroyEntity(entity* e);

void tickEntities();
void drawEntities();
void destroyEntities();

void* createEntity(enum entityType e);
void* createEntityAt(enum entityType e, double x, double y);

void initBase(entity* self);
void tickBase(entity* self);
void drawBase(entity* self);
void destroyBase(entity* self);


typedef struct listNode listNode;

/*!lista de entidades
 */
struct listNode {
	entity* elem;
	listNode* next;
};

void entitiesPush(entity* elem);

void entitiesFree();

int entityCount();

#endif
