#include "Player.h"
#include "stdio.h"
#include "input.h"
#include "util.h"
#include "game.h"

extern int WIDTH, HEIGHT;
extern entity* player;
extern listNode* entities;

// Constantes para movimento do jogador

static double maxspeedX = 6 *(60.0/FPS);
static double fricX = 0.5 *(60.0/FPS);
static double accX = (0.4 + 0.5) *(60.0/FPS);
static double maxspeedY = 3 *(60.0/FPS);
static double fricY = 0.3 *(60.0/FPS);
static double accY = (0.25 + 0.3) *(60.0/FPS);

static double weaponHeatTimeOut = FPS*5;

static int screenBorder = 16;

///////

extern rgbImage PlayerSprite[];
extern rgbImage ShieldSprite[];
extern rgbImage RedBeamSprite[];



/*!
 * Inicializa uma entidade do tipo Player
 * \param s pointer para entidade a ser inicializada
 */
void initPlayer(entity* s) {
	Player* p = (Player*) s;
	initBase(s);
	s->type = PLAYER;
	s->sprite = PlayerSprite;

	p->canShoot = 1;
	p->shootTimer = 0;
	p->shotDelay = FPS / 10;
	p->weaponHeat = 0;

	s->AnimLength = 3;
	s->AnimSpeed = FPS / 10;
	
	s->mask.w = 28;
	s->mask.h = 28;
	s->mask.ox = -28/2;
	s->mask.oy = -28/2;

	p->hasShield = 0;
	p->hasRedBeam = 0;
	p->isInvincible = 2*FPS;
};

/*!
 * Evento tick da entidade Player
 * \param s pointer para a entidade onde efetuar o evento
 */
void tickPlayer(entity* s) {
	Player* p = (Player*) s;
	Input in = get_Input();

	s->dx = stepTo(s->dx,0,fricX);
	s->dy = stepTo(s->dy,0,fricY);

	// Movement


	if (in.UP) s->dy = stepTo(s->dy, -maxspeedY, accY);
	else if (in.DOWN) s->dy = stepTo(s->dy, maxspeedY, accY);

	if (in.LEFT) s->dx = stepTo(s->dx, -maxspeedX, accX);
	else if (in.RIGHT) s->dx = stepTo(s->dx, maxspeedX, accX);

	tickBase(s);

	if (!isGameOver()) {
		s->x = max(min(s->x, WIDTH-screenBorder), screenBorder);
		s->y = max(min(s->y, HEIGHT-screenBorder), screenBorder);
	} else {
		if (s->y < -64) s->remove = 1;
		player = NULL;
		s->dy -= 0.5 * (60.0/FPS);
	}

	// Shooty Action


	if (p->canShoot && in.SPACE && p->weaponHeat >= 0) {
		if (p->hasRedBeam) {
			// Red Beam Trigger
			p->weaponHeat = -p->hasRedBeam;
			p->canShoot = 0;
		} else {
			// Regular Bullets
			entity* b;
			b = createEntityAt(BULLET, s->x-16, s->y-16);
			b = createEntityAt(BULLET, s->x+16, s->y-16);

			p->canShoot = 0;
			p->shootTimer = p->shotDelay;

			p->weaponHeat = min(p->weaponHeat + 3 + p->shotDelay, 100);
			if (p->weaponHeat == 100)
				p->weaponHeat = -weaponHeatTimeOut;
		}
	} else {
		p->shootTimer = max(p->shootTimer-1, 0);
		if (p->shootTimer <= 0 && p->weaponHeat >= 0) p->canShoot = 1;
		p->weaponHeat = stepTo(p->weaponHeat,0,1);
	}

	//Red Beam
	if (p->hasRedBeam && p->canShoot == 0) {
		listNode* i; rectangle beam;
		beam.ox = -16;
		beam.oy = -HEIGHT;
		beam.w = 32;
		beam.h = HEIGHT;
		for(i = entities; i!=NULL; i = i->next) {
			if (! ((i->elem)->type == ENEMY || (i->elem)->type == ENEMYCRUISER || (i->elem)->type == ENEMYMOTHERSHIP) ) continue;
			if (rectanglesCollision(beam,s->x,s->y, (i->elem)->mask, (i->elem)->x,(i->elem)->y) )
				damageEnemy((i->elem), 10);
		}
		p->hasRedBeam = max(p->hasRedBeam-1, 0);
		add_screenShake(2);
	}

	//Powerps
	p->hasShield = max(p->hasShield-1,0);
	p->isInvincible = max(p->isInvincible-1,0);

	// Misc
	if (s->hurtTimer != 0) {
		add_screenShake( (s->hurtTimer / 16.0)*8 );
	}
};


/*!
 * Desenha a entidade Player
 * \param s pointer para a entidade a desenhar
 */
void drawPlayer(entity* s) {
	Player* p = (Player*) s;

	if (s->hurtTimer!=0)
			//Blend suave do sprite com vermelho à medida que apanha dano
			s->blend = makeColor(0xFF
						, (char)(0xFF*(1.0 - s->hurtTimer/(16.0 * FPS / 60.0)))
						, (char)(0xFF*(1.0 - s->hurtTimer/(16.0 * FPS / 60.0)))
						);
	else s->blend = 0xFFFFFF;

	if (p->hasRedBeam && p->canShoot == 0) {
			double i;
			for(i = s->y-40; i > -32; i=i-32)
			drawImage(RedBeamSprite[0], s->x, i);
	}
	if (p->isInvincible > 0 && s->ImageIndex == 0) s->visible = 0;

	drawBase(s);

	s->visible = 1;

	if (p->hasShield>0 && !(p->hasShield < 2*FPS && ((p->hasShield % (FPS/2))<=(FPS/6))) )
		drawImage(ShieldSprite[s->ImageIndex%2], s->x, s->y);
};


/*!
 * Destrutor da entidade Player
 * \param s pointer para a entidade a destruir
 */
void destroyPlayer(entity* s) {
	Player* p = (Player*) s;
	createEntityAt(EXPLOSION, s->x, s->y);
	createEntityAt(EXPLOSION, s->x, s->y);
	createEntityAt(EXPLOSION, s->x, s->y);
	destroyBase(s);
};
