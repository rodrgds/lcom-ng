#include "enemies.h"
#include <stdio.h>
#include "game.h"
#include "util.h"

extern rgbImage EnemySprite[];
extern rgbImage MotherShieldSprite[];
extern rgbImage MotherLightBeamSprite[];

extern int WIDTH,HEIGHT;
extern entity* player;
extern entity* MotherShip;
extern listNode* entities;
extern int score;
extern int level;
extern int GameWon;

/*!
 * Da dano a um inimigo. Indicando-lhe que foi morto pelo jogador caso esse dano seja suficinte para o matar.
 * \param s pointer para o inimigo a quem dar o dano
 * \param damage quantidade de dano a dar
 * \return devolve a vida do inimigo depois do dano ser dado
 */
int damageEnemy(entity* s, int damage) {
	s->health = max(s->health-damage,0);
	if (s->health == 0)
		switch (s->type) {
		case ENEMY: ((Enemy*) s)->killedByPlayer = 1; break;
		case ENEMYCRUISER: ((EnemyCruiser*) s)->killedByPlayer = 1; break;
		case ENEMYMOTHERSHIP: ((EnemyMotherShip*) s)->killedByPlayer = 1; break;
		}
	s->hurtTimer = 16 * (FPS/60.0);
	return s->health;
}



/////// ENEMY //////
/*!
 * inicializa uma entidade do tipo enemy
 * \param s pointer para entidade a ser inicializada
 */
void initEnemy(entity* s) {
	Enemy* e = (Enemy *) s;
	initBase(s);
	s->type = ENEMY;
	s->sprite = EnemySprite;


	s->AnimLength = 3;
	s->AnimSpeed = FPS / 6;


	e->canShoot = 0;
	e->shootTimer = 1*FPS;
	e->shotDelay = 2*FPS;

	e->killedByPlayer = 0;
};

/*!
 * evento tick da entidade enemy
 * \param s pointer para a entidade onde efetuar o evento
 */
void tickEnemy(entity* s) {
	Enemy* e = (Enemy *) s;
	if ( s->y > HEIGHT+64) s->remove=1;
	tickBase(s);

	// Shooty Action

	if (e->canShoot ) {
			entity* b;
			b = createEntity(EBULLET);
			b->x = s->x-16;
			b->y = s->y+4;
			if (level == 1 && player != NULL){
			b->dx = lengthdir_x(2, point_direction(b->x,b->y, player->x, player->y));
			b->dy = lengthdir_y(2, point_direction(b->x,b->y, player->x, player->y));
			}
			else b->dx = s->dx;

			b = createEntity(EBULLET);
			b->x = s->x+16;
			b->y = s->y+4;
			b->dx = s->dx;


		e->canShoot = 0;
		e->shootTimer = e->shotDelay;
	} else {
		e->shootTimer = max(e->shootTimer-1, 0);
		if (e->shootTimer <= 0) e->canShoot = 1;
	}
};

/*!
 * desenha a entidade enemy
 * \param s pointer para a entidade a desenhar
 */
void drawEnemy(entity* s) {
	if (s->hurtTimer!=0)
		//Blend suave do sprite com vermelho à medida que apanha dano
		s->blend = makeColor(0xFF
					, (char)(0xFF*(1.0 - s->hurtTimer/(15.0 * FPS / 60.0)))
					, (char)(0xFF*(1.0 - s->hurtTimer/(15.0 * FPS / 60.0)))
					);
	else s->blend = 0xFFFFFF;

	drawBase(s);
};

/*!
 * destrutor da entidade enemy
 * \param s pointer para a entidade a destruir
 */
void destroyEnemy(entity* s) {
	Enemy* e = (Enemy*) s;

	if (e->killedByPlayer) {
		score += 10;
		if (rand()%10 == 0)
				createEntityAt(POWERUP,s->x,s->y);
	}

	createEntityAt(EXPLOSION, s->x, s->y);




	destroyBase(s);
};









/////// CRUISER ///////

static int CruiserW = 100, CruiserH = 156;
extern rgbImage EnemyCruiserSprite[];

/*!
 * inicializa uma entidade do tipo cruiser
 * \param s pointer para entidade a ser inicializada
 */
void initEnemyCruiser(entity* s) {
	EnemyCruiser* e = (EnemyCruiser *) s;
	initBase(s);
	s->type = ENEMYCRUISER;
	s->sprite = EnemyCruiserSprite;

	s->AnimLength = 2;
	s->AnimSpeed = FPS / 2;

	s->dy = 0.25 * (60.0/FPS);

	e->canShoot = 0;
	e->shootTimer = 1*FPS;
	e->shotDelay = 0.3*FPS;
	e->shotIndex = 0;

	s->mask.w = CruiserW;
	s->mask.h = CruiserH;
	s->mask.ox = -CruiserW/2;
	s->mask.oy = -CruiserH/2;

	s->health = 2000;

	e->killedByPlayer = 0;
};

/*!
 * evento tick da entidade cruiser
 * \param s pointer para a entidade onde efetuar o evento
 */
void tickEnemyCruiser(entity* s) {
	EnemyCruiser* e = (EnemyCruiser *) s;
	if ( s->y > (CruiserH/2  + HEIGHT) ) s->remove=1;
	tickBase(s);

	// Shooty Action

	if (e->canShoot && player != 0) {
		double bx = s->x, by = s->y; int delay = e->shotDelay;
		switch(e->shotIndex) {
		case 0: by += 30; break;
		case 1: by += 20; break;
		case 2: break;
		case 3: by -= 11; break;
		case 4: by -= 22; break;
		case 5: by -= 40; bx -= 10; break;
		case 6: by -= 40; bx += 10; delay+=FPS*2; break;
		}
		e->shotIndex = (e->shotIndex+1)%7;

		EBullet* b = createEntityAt(EBULLET, bx, by);
		b->type = CRUISER;

		//para as balas seguirem o jogador
		((entity*) b)->dx = lengthdir_x(2, point_direction(bx,by, player->x, player->y));
		((entity*) b)->dy = lengthdir_y(2, point_direction(bx,by, player->x, player->y));

		e->canShoot = 0;
		e->shootTimer = delay;
	} else {
		e->shootTimer = max(e->shootTimer-1, 0);
		if (e->shootTimer <= 0) e->canShoot = 1;
	}


};

/*!
 * desenha a entidade cruiser
 * \param s pointer para a entidade a desenhar
 */
void drawEnemyCruiser(entity* s) {
	if (s->hurtTimer!=0)
		//Blend suave do sprite com vermelho à medida que apanha dano
		s->blend = makeColor(0xFF
					, (char)(0xFF*(1.0 - s->hurtTimer/(15.0 * FPS / 60.0)))
					, (char)(0xFF*(1.0 - s->hurtTimer/(15.0 * FPS / 60.0)))
					);
	else s->blend = 0xFFFFFF;


	drawBase(s);
};

/*!
 * destrutor da entidade cruiser
 * \param s pointer para a entidade a destruir
 */
void destroyEnemyCruiser(entity* s) {
	EnemyCruiser* e = (EnemyCruiser*) s;

	if (e->killedByPlayer) score += 250;

	int i = 0;
	for(i = 0; i < 16; i++)
		createEntityAt(EXPLOSION,
						s->x + randomRange(-CruiserW/2,CruiserW/2),
						s->y + randomRange(-CruiserH/2,CruiserH/2));


	if (rand()%3 == 0) {
		entity* l = createEntity(POWERUP);
		l->x = s->x;
		l->y = s->y;
	}
	destroyBase(s);

};







////// MOTHER SHIP //////

static int MotherShipW = 512, MotherShipH = 320;
extern rgbImage EnemyMotherShipSprite[];
void initEnemyMotherShip(entity* s){
	EnemyMotherShip* e = (EnemyMotherShip *) s;
		initBase(s);
		s->type = ENEMYMOTHERSHIP;
		s->sprite = EnemyMotherShipSprite;

		s->AnimLength = 2;
		s->AnimSpeed = FPS / 2;

		s->dy = 0.25 * (60.0/FPS);

		e->canShoot = 0;
		e->shootTimerC = 0.5*FPS; e->shotDelayC = 0.1*FPS;
		e->shootTimerBF = 0.5*FPS; e->shotDelayBF = 0.7*FPS;
		e->shootTimerF = 1.5*FPS; e->shotDelayF = 0.3*FPS;
		e->shootTimerBC = 1*FPS; e->shotDelayBC = 0.5*FPS;
		e->shootTimerLB = 10*FPS; e->hasLightBeam = 0;
		e->shieldTimer = 15*FPS; e->hasShield = 0;

		e->shotIndex = 0;
		e->higherBullets = 0;

		e->killedByPlayer = 0;

		s->mask.w = MotherShipW;
		s->mask.h = MotherShipH;
		s->mask.ox = -MotherShipW/2;
		s->mask.oy = -MotherShipH/2;

		s->health = 10000;
}

void tickEnemyMotherShip(entity* s) {
	EnemyMotherShip* e = (EnemyMotherShip *) s;

	tickBase(s);

	if (s->y >= 155)
		s->dy = 0;


	//// Shield ////
	if (e->hasShield > 0){
		s->mask.ox = -MotherShipW;
		s->mask.oy = -MotherShipH/2;
		s->mask.w = MotherShipW*2;
		s->mask.h = MotherShipH  + 64;

		e->hasShield = max(e->hasShield -1, 0); //decrementa o tempo do escudo
		e->shieldTimer = 20*FPS; //inicializa novamente o contador ate ao proximo

	}
	else {
		s->mask.ox = -MotherShipW/2;
		s->mask.oy = -MotherShipH/2;
		s->mask.w = MotherShipW;
		s->mask.h = MotherShipH;
		e->shieldTimer = max(e->shieldTimer-1,0); //decrementa o contador ate ao proximo shield
	}

	if(e->shieldTimer <= 0) e->hasShield = 5*FPS; //se for altura de proteger, inicializa o tempo de escudo




	//////// Shooty Action //////////


	int delayBC = e->shotDelayBC;
	int delayF = e->shotDelayF;
	int delayBF = e->shotDelayBF;

	//Cruiser Bullets
	if (e->shootTimerC <= 0 && player != 0) { //se for altura de disparar
		//posicao das balas comeca inicialmente no centro da nave
		double Cbx = s->x, Cby = s->y; //cruiser left bullets / lower bullets
		double Crbx = s->x, Crby = s->y; //cruiser right bullets (higher bullets)
		int delayC = e->shotDelayC;

		switch (e->shotIndex) {
		//indice das balas
		case 0:
			if (!e->higherBullets) {
				Cby -= 69;
				Cbx -= 68;
			} else {
				Cby -= 108;
				Cbx -= 25;
				Crby -= 108;
				Crbx += 25;
			}
			break;
		case 1:
			if (!e->higherBullets) {
				Cby -= 77;
				Cbx -= 36;
			} else {
				Cby -= 94;
				Cbx -= 61;
				Crby -= 94;
				Crbx += 61;
			}
			break;
		case 2:
			if (!e->higherBullets)
				Cby -= 82;
			else {
				Cby -= 88;
				Cbx -= 87;
				Crby -= 85;
				Crbx += 88;
				delayC += FPS * 2;
			}
			break;

		case 3:
			Cby -= 81;
			Cbx += 36;
			break;
		case 4:
			Cby -= 69;
			Cbx += 68;
			delayC += FPS * 2;
			break;

		}

		EBullet* Cb = createEntityAt(EBULLET, Cbx, Cby);
		Cb->type = CRUISER;

		if (!e->higherBullets){
		((entity*) Cb)->dx = lengthdir_x(randomRange(2, 10), point_direction(Cbx,Cby, player->x, player->y));
		((entity*) Cb)->dy = lengthdir_y(randomRange(2, 10), point_direction(Cbx,Cby, player->x, player->y));
		}
		else{
			EBullet* Crb = createEntityAt(EBULLET, Crbx, Crby);
			Crb->type = CRUISER;

			//para as balas seguirem o jogador
			((entity*) Crb)->dx = lengthdir_x(10, point_direction(Crbx,Crby, player->x, player->y));
			((entity*) Crb)->dy = lengthdir_y(10, point_direction(Crbx,Crby, player->x, player->y));
			((entity*) Cb)->dx = lengthdir_x(10, point_direction(Cbx,Cby, player->x, player->y));
			((entity*) Cb)->dy = lengthdir_y(10, point_direction(Cbx,Cby, player->x, player->y));
		}



		//troca indices e indicador da fileira de balas
		if (!e->higherBullets) {
			if (e->shotIndex == 4){
				e->higherBullets = 1;
				e->shotIndex = -1;
			}
			e->shotIndex = (e->shotIndex + 1) % 5;
		} else {
			if (e->shotIndex == 2){
				e->higherBullets = 0;
				e->shotIndex = -1;
			}
			e->shotIndex = (e->shotIndex + 1) % 3;
		}

		e->shootTimerC = delayC;

	}

	else {

		e->shootTimerC = max(e->shootTimerC-1, 0);
	}

	//Big Cruiser Bullets
	if (e->shootTimerBC <= 0 && player != 0) {
		double BCbx = s->x, BCby = s->y; //big cruiser bullets
		BCby -= 66;
		BCbx += randomBetween(-115, 115);
		EBullet* BCb = createEntityAt(EBULLET, BCbx, BCby);
		BCb->type = BIGCRUISER;
		((entity*) BCb)->dy = 5 * (60.0 / FPS);
		delayBC += FPS * 2;

		((entity*) BCb)->dx = lengthdir_x(4, point_direction(BCbx,BCby, player->x, player->y));
		((entity*) BCb)->dy = lengthdir_y(4, point_direction(BCbx,BCby, player->x, player->y));
		((entity*) BCb)->mask.ox = -8;
		((entity*) BCb)->mask.oy = -8;
		((entity*) BCb)->mask.h = 16;
		((entity*) BCb)->mask.w = 16;

		e->shootTimerBC = delayBC;

	} else
		e->shootTimerBC = max(e->shootTimerBC-1, 0);

	//Fire Bullets

	if (e->shootTimerF <= 0) {
		double Fbx = s->x, Fby = s->y; //fire bullets
		Fby += 135;
		Fbx += randomBetween(-160, 160);
		EBullet* Fb = createEntityAt(EBULLET, Fbx, Fby);
		Fb->type = FIRE;
		((entity*) Fb)->dy = 5 * (60.0 / FPS);
		((entity*) Fb)->mask.ox = -8;
		((entity*) Fb)->mask.oy = -8;
		((entity*) Fb)->mask.h = 16;
		((entity*) Fb)->mask.w = 16;

		delayF += FPS * 2;
		e->shootTimerF = delayF;
	} else
		e->shootTimerF = max(e->shootTimerF-1, 0);

	//Big Fire Bullets
	if (e->shootTimerBF <= 0) {
		double BFbx = s->x, BFby = s->y; //big fire bullets
		BFby -= 10;
		EBullet* BFb = createEntityAt(EBULLET, BFbx, BFby);
		BFb->type = BIGFIRE;
		((entity*) BFb)->dy = 2 * (60.0 / FPS);
		((entity*) BFb)->mask.ox = -20;
		((entity*) BFb)->mask.oy = -20;
		((entity*) BFb)->mask.h = 40;
		((entity*) BFb)->mask.w = 40;

		delayBF += FPS * 2;
		e->shootTimerBF = delayBF;
	} else
		e->shootTimerBF = max(e->shootTimerBF-1, 0);


	//LightBeam

	if (e->hasLightBeam > 0) {
		listNode* i;
		rectangle beam1;
		beam1.ox = MotherShipW/2 - 128;
		beam1.oy = MotherShipH/2 - 64;
		beam1.w = 64;
		beam1.h = HEIGHT;

		rectangle beam2;
		beam2.ox = -MotherShipW/2 + 64;
		beam2.oy = MotherShipH/2 - 64;
		beam2.w = 64;
		beam2.h = HEIGHT;

		for (i = entities; i != NULL; i = i->next) {
			if ((i->elem)->type !=PLAYER)
				continue;
			if ((rectanglesCollision(beam1, s->x, s->y, (i->elem)->mask,
					(i->elem)->x, (i->elem)->y)) || (rectanglesCollision(beam2, s->x, s->y, (i->elem)->mask,
							(i->elem)->x, (i->elem)->y))) {
				(i->elem)->health = max((i->elem)->health-10,0);
				(i->elem)->hurtTimer = 16 * (FPS / 60.0);
			}
		}

		e->hasLightBeam = max(e->hasLightBeam-1, 0); //diminui o tempo de disparo
		e->shootTimerLB = 10*FPS; //volta a inicializar o contador ate ao proximo
		add_screenShake(2);
	} else e->shootTimerLB = max(e->shootTimerLB-1,0); //se nao estiver a disparar, decrementa o contador ate ao proximo

	if(e->shootTimerLB <= 0) e->hasLightBeam = 3*FPS; //se for altura de disparar, inicializa o tempo de disparo

}


void drawEnemyMotherShip(entity* s) {
	EnemyMotherShip* e = (EnemyMotherShip*) s;

	if (rand() % 500 == 0) createEntityAt(POWERUP, rand() % WIDTH, -128);

	unsigned char blendR = 0xFF, blendG = 0xFF, blendB = 0xFF;
	if (s->hurtTimer!=0) {
		blendR = 0xFF;
		blendG = 0xFF*(1.0 - s->hurtTimer/(15.0 * FPS / 60.0));
		blendB = 0xFF*(1.0 - s->hurtTimer/(15.0 * FPS / 60.0));
	}
	if (e->shootTimerLB < 2*FPS && e->shootTimerLB > 0) {
		blendR *= 1;
		blendG *= 1;
		blendB = blendB*(e->shootTimerLB/(2.0*FPS));
	}

	s->blend = makeColor(blendR,blendG,blendB);

	double i;

	//Light Beam
	if (e->hasLightBeam > 0){
			for(i = s->y + MotherShipH/2; i <= HEIGHT+64; i+=63)
				drawImage(MotherLightBeamSprite[0], s->x-160, i);
			for(i = s->y + MotherShipH/2; i <= HEIGHT+64; i+=63)
				drawImage(MotherLightBeamSprite[0], s->x+160, i);

	}

	drawBase(s);

	//Shield
	if (((EnemyMotherShip*)s)->hasShield > 0) {
		for(i = -64; i <= 864; i+=54)
			drawImage(MotherShieldSprite[0], i, s->y+200);
	}


}

void destroyEnemyMotherShip(entity* s) {
	EnemyMotherShip* e = (EnemyMotherShip*) s;
	int i = 0;

	MotherShip = NULL;

	if (e->killedByPlayer) {
		score += 5000;
		gameWin();
	}

		for(i = 0; i < 64; i++)
			createEntityAt(EXPLOSION,
							s->x + randomRange(-MotherShipW/2,MotherShipW/2),
							s->y + randomRange(-MotherShipH/2,MotherShipH/2));


		destroyBase(s);
}
