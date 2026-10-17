#include "projectiles.h"
#include "util.h"
#include "game.h"

extern int HEIGHT, WIDTH;

static int bulletSpeed = 10;
static int bulletH = 20, bulletW = 8;

static int ebulletSpeed = 3;
static int ebulletH = 8, ebulletW = 8;

extern rgbImage ExplosionSprite[];
extern rgbImage BulletSprite[];
extern rgbImage EBulletSprite[];


/// BULLET

/*!
 * Inicializa uma entidade do tipo Bullet (Balas do jogador)
 * \param s pointer para entidade a ser inicializada
 */
void initBullet(entity* s) {
	Bullet* b = (Bullet*) s;
	initBase(s);
	s->type = BULLET;
	s->sprite = BulletSprite;

	s->dy = -bulletSpeed * (60.0/FPS);

	s->mask.ox = -bulletW/2;
	s->mask.oy = -bulletH/2;
	s->mask.h = bulletH;
	s->mask.w = bulletW;

	s->AnimLength = 3;
	s->AnimSpeed = FPS / 3;

	b-> damage = 25;
}


/*!
 * Evento tick da entidade Bullet
 * \param s pointer para a entidade onde efetuar o evento
 */
void tickBullet(entity* s) {
	tickBase(s);
	if (s->x < -100 || s->x > WIDTH+100 || s->y < -100 || s->y > HEIGHT+100)
		s->remove = 1;
}


/*!
 * Desenha a entidade Bullet
 * \param s pointer para a entidade a desenhar
 */
void drawBullet(entity* s) {drawBase(s);};


/*!
 * Destrutor da entidade Bullet
 * \param s pointer para a entidade a destruir
 */
void destroyBullet(entity* s) {destroyBase(s);};

//////////////////////




///// ENEMY BULLET

/*!
 * Inicializa uma entidade do tipo EBullet (Balas dos inimigos)
 * Contem uma variavel type que identifica o tipo de bala
 * \param s pointer para entidade a ser inicializada
 */
void initEBullet(entity* s) {
	EBullet* b = (EBullet*) s;
	initBase(s);
	s->type = EBULLET;
	s->sprite = EBulletSprite;

	s->dy = ebulletSpeed * (60.0/FPS);

	s->mask.ox = -ebulletW/2;
	s->mask.oy = -ebulletH/2;
	s->mask.h = ebulletH;
	s->mask.w = ebulletW;

	s->AnimLength = 2;
	s->AnimSpeed = FPS/8;

	b->type = NORMAL;
};

/*!
 * Evento tick da entidade EBullet
 * \param s pointer para a entidade onde efetuar o evento
 */
void tickEBullet(entity* s) {
	tickBase(s);
	if (s->x < -100 || s->x > WIDTH+100 || s->y < -100 || s->y > HEIGHT+100)
		s->remove = 1;
};

/*!
 * Desenha a entidade EBullet
 * \param s pointer para a entidade a desenhar
 */
void drawEBullet(entity* s) {
	EBullet* b = (EBullet*) s;
		switch(b->type) {
		case NORMAL: s->sprite = EBulletSprite; break;
		case CRUISER: s->sprite = EBulletSprite+2; break;
		case FIRE: s->sprite = EBulletSprite+8; break;
		case BIGFIRE: s->sprite = EBulletSprite+4; break;
		case BIGCRUISER: s->sprite = EBulletSprite+6; break;
		}
	drawBase(s);
};

/*!
 * Destrutor da entidade EBullet
 * \param s pointer para a entidade a destruir
 */
void destroyEBullet(entity* s) {destroyBase(s);};

//////////////////////////




//// EXPLOSION

/*!
 * Inicializa uma entidade do tipo Explosion
 * \param s pointer para entidade a ser inicializada
 */
void initExplosion(entity* s) {
	initBase(s);
	s->type = EXPLOSION;
	s->sprite = ExplosionSprite;

	s->AnimLength = 6;
	s->AnimSpeed = 4 * (FPS/60.0);
	s->AnimTimer = s->AnimSpeed;
};

/*!
 * Evento tick da entidade Explosion
 * \param s pointer para a entidade onde efetuar o evento
 */
void tickExplosion(entity* s) {}

/*!
 * Desenha a entidade Explosion
 * \param s pointer para a entidade a desenhar
 */
void drawExplosion(entity* s) {
		if (s->visible) {
			if (s->AnimSpeed > 0) {
				s->AnimTimer = max(s->AnimTimer-1, 0);
				if (s->AnimTimer <= 0) {
					s->ImageIndex = (s->ImageIndex+1);
					if (s->ImageIndex >= s->AnimLength) {
						s->remove = 1;
						return;
					}
					s->AnimTimer = s->AnimSpeed;
				}
			}
			drawImage(s->sprite[s->ImageIndex],s->x,s->y);
		}
};

/*!
 * Destrutor da entidade Explosion
 * \param s pointer para a entidade a destruir
 */
void destroyExplosion(entity* s) { destroyBase(s); };
