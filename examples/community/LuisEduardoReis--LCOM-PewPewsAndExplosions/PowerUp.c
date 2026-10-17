#include "PowerUp.h"
#include "game.h"

extern rgbImage PowerUpSprite[];
extern int WIDTH,HEIGHT;


/*!
 * Inicializa uma entidade do tipo PowerUp
 * \param s pointer para entidade a ser inicializada
 */
void initPowerUp(entity* s) {
	PowerUp* p = ((PowerUp*) s);
	initBase(s);
	s->type = POWERUP;
	s->sprite = PowerUpSprite;

	s->dy = 2.0 * (60.0/FPS);
	s->AnimLength = 2;
	s->AnimSpeed = FPS/3;
	
	s->mask.ox = -16;
	s->mask.oy = -16;
	s->mask.h = 32;
	s->mask.w = 32;
	
	p->type = rand()%3;

	if (p->type == LIFE)
		s->sprite = PowerUpSprite;
	else if (p->type == SHIELD)
		s->sprite = PowerUpSprite+2;
	else if (p->type == REDBEAM)
		s->sprite = PowerUpSprite+4;
};

/*!
 * Evento tick da entidade PowerUp
 * \param s pointer para a entidade onde efetuar o evento
 */
void tickPowerUp(entity* s) {
	if ( s->y > HEIGHT+64) s->remove=1;

	tickBase(s);
};

/*!
 * Desenha a entidade PowerUp
 * \param s pointer para a entidade a desenhar
 */
void drawPowerUp(entity* s) {drawBase(s);};


/*!
 * Destrutor da entidade PowerUp
 * \param s pointer para a entidade a destruir
 */
void destroyPowerUp(entity* s) {destroyBase(s);};


