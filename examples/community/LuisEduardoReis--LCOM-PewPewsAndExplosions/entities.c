#include "entities.h"
#include "stdlib.h"
#include "math.h"
#include "util.h"

#include "game.h"
#include "Player.h"
#include "projectiles.h"
#include "enemies.h"
#include "PowerUp.h"

listNode* entities = NULL;

static double TimePerFrame = 60.0/FPS;

/*!
 * Cria uma entidade de um certo tipo, alocando dinamicamente espaco para ela,
 * chamando o seu construtor e adicionado-a à lista de entidades.
 * \param e tipo da entidade a criar
 * \return pointer para a entidade criada
 */
void* createEntity(enum entityType e) {
	entity* s;
	switch(e) {
	case BASE:
		s = malloc(sizeof(entity));
		initBase(s);
		break;
	case PLAYER:
		s = (entity*) malloc(sizeof(Player));
		initPlayer(s);
		break;
	case ENEMY:
		s = (entity*) malloc(sizeof(Enemy));
		initEnemy(s);
		break;
	case ENEMYCRUISER:
		s = (entity*) malloc(sizeof(EnemyCruiser));
		initEnemyCruiser(s);
		break;
	case ENEMYMOTHERSHIP:
		s = (entity*) malloc(sizeof(EnemyMotherShip));
		initEnemyMotherShip(s);
		break;
	case BULLET:
		s = (entity*) malloc(sizeof(Bullet));
		initBullet(s);
		break;
	case EBULLET:
		s = (entity*) malloc(sizeof(EBullet));
		initEBullet(s);
		break;
	case EXPLOSION:
		s = (entity*) malloc(sizeof(Explosion));
		initExplosion(s);
		break;
	case POWERUP:
		s = (entity*) malloc(sizeof(PowerUp));
		initPowerUp(s);
		break;
	default:
		return NULL;
	}
	entitiesPush(s);
	return s;
};

/*!
 * Cria uma entidade de um certo tipo, alocando dinamicamente espaco para ela,
 * chamando o seu construtor e adicionado-a à lista de entidades.
 * \param e tipo da entidade a criar
 * \param x coordenada x do ponto onde criar a entidade
 * \param y coordenada y do ponto onde criar a entidade
 * \return pointer para a entidade criada
 */
void* createEntityAt(enum entityType e, double x, double y) {
	entity* s = createEntity(e);
	s->x = x;
	s->y = y;
	return s;
}






// BASE Entity

/*!
 * Contrutor generico de uma entidade. Inicializa os valores dos membros comuns a todas as entidades.
 * Este construtor e chamado por todos os outros.
 * \param s pointer para a entidade a inicializar
 */
void initBase(entity* s) {
	s->remove = 0;
	s->x = 0;
	s->y = 0;
	s->dx = 0;
	s->dy = 0;
	s->visible = 1;
	s->solid = 1;
	s->health = 100;
	s->type = BASE;

	//Animacao do sprite
	s->ImageIndex = 0;
	s->AnimTimer = 0;
	s->AnimLength = 1;
	s->AnimSpeed = 0;
	s->blend = 0xFFFFFF;
	s->hurtTimer = 0;

	//Mascara de colisao
	s->mask.ox = -32;
	s->mask.oy = -32;
	s->mask.w = 64;
	s->mask.h = 64;



}

/*!
 * Evento generico de tick para uma entidade. Altera a posicao de acordo com a velocidade, decrementa contadores e
 * remove a entidade caso esteja "morta". Este evento e chamado pelos eventos tick de cada entidade.
 * \param s pointer para a entidade onde efetuar o evento
 */
void tickBase(entity* s) {
	s->xp = s->x;
	s->yp = s->y;
	s->x += s->dx;
	s->y += s->dy;
	if (s->health <= 0) s->remove = 1;
	s->hurtTimer = max(s->hurtTimer-1,0);
};

/*!
 * Desenha uma entidade generica. Altera as variaveis de animacao e desenha o sprite apropriado a cada passo da animacao.
 * \param s pointer para a entidade a desenhar
 */
void drawBase(entity* s) {
	if (s->AnimSpeed > 0) {
		s->AnimTimer = max(s->AnimTimer-1, 0);
		if (s->AnimTimer <= 0) {
			s->ImageIndex = (s->ImageIndex+1)%(s->AnimLength);
			s->AnimTimer = s->AnimSpeed;
		}
	}
	if (s->visible) {
		if (s->blend == 0xFFFFFF)
			drawImage(s->sprite[s->ImageIndex],s->x,s->y);
		else{
			drawImageBlend(s->sprite[s->ImageIndex],s->x,s->y,s->blend);
		}
	}
};

/*!
 * Destrutor de uma entidade generica.
 * \param s pointer para a entidade a destruir
 */
void destroyBase(entity* s) {};






// ENTITY EVENTS

/*!
 * Chama o evento tick de cada entidade
 */
void tickEntities() {
	listNode* i = entities;
	while(i != NULL) {
		tickEntity(i->elem);
		i = i->next;
	}
};

/*!
 *	Desenha todas as entidades
 */
void drawEntities() {
	listNode* i = entities;
	while(i != NULL) {
		drawEntity(i->elem);
		i = i->next;
	}
};

/*!
 * Remove da lista de entidades todas as entidades com a flag remove a 1.
 * Chama o destrutor das entidades que remove.
 */
void destroyEntities() {
	listNode* i = entities, *l = NULL;
	while(i != NULL) {
		if ((i->elem)->remove) {
			if (l!=NULL) {
				l->next = i->next;
				destroyEntity(i->elem);
				free(i->elem);
				free(i);
				i = l->next;
			}
			else {
				entities = i->next;
				destroyEntity(i->elem);
				free(i->elem);
				free(i);
				i = entities;
			}

		}else {
			l = i;
			i = i->next;
		}
	}
};

/*!
 * Chama a funcao de tick de uma entidade consoante o seu tipo.
 * \param e entidade para o qual o evento e chamado
 */
void tickEntity(entity* e) {
	switch (e->type) {
	case BASE:	tickBase(e); break;
	case PLAYER: tickPlayer(e); break;
	case ENEMY: tickEnemy(e); break;
	case ENEMYCRUISER: tickEnemyCruiser(e); break;
	case ENEMYMOTHERSHIP: tickEnemyMotherShip(e); break;
	case BULLET: tickBullet(e); break;
	case EBULLET: tickEBullet(e); break;
	case EXPLOSION: tickExplosion(e); break;
	case POWERUP: tickPowerUp(e); break;
	default: tickBase(e); break;
	}
}
/*!
 * Chama a funcao para desenhar uma entidade consoante o seu tipo.
 * \param e entidade para o qual o evento e chamado
 */
void drawEntity(entity* e) {
	switch (e->type) {
	case BASE:	drawBase(e); break;
	case PLAYER: drawPlayer(e); break;
	case ENEMY: drawEnemy(e); break;
	case ENEMYCRUISER: drawEnemyCruiser(e); break;
	case ENEMYMOTHERSHIP: drawEnemyMotherShip(e); break;
	case BULLET: drawBullet(e); break;
	case EBULLET: drawEBullet(e); break;
	case EXPLOSION: drawExplosion(e); break;
	case POWERUP: drawPowerUp(e); break;
	default: drawBase(e); break;
	}
}
/*!
 * Chama o destrutor de uma entidade consoante o seu tipo.
 * \param e entidade para o qual o evento e chamado
 */
void destroyEntity(entity* e) {
	switch (e->type) {
	case BASE:	destroyBase(e); break;
	case PLAYER: destroyPlayer(e); break;
	case ENEMY: destroyEnemy(e); break;
	case ENEMYCRUISER: destroyEnemyCruiser(e); break;
	case ENEMYMOTHERSHIP: destroyEnemyMotherShip(e); break;
	case BULLET: destroyBullet(e); break;
	case EBULLET: destroyEBullet(e); break;
	case EXPLOSION: destroyExplosion(e); break;
	case POWERUP: destroyPowerUp(e); break;
	default: destroyBase(e); break;
	}
}




// LIST HANDLING


/*!
 * Insere uma entidade na lista ligada de entidades
 * \param elem entidade a inserir
 */
void entitiesPush(entity* elem) {
	listNode* new = malloc(sizeof(listNode));
	new->elem = elem;
	new->next = NULL;

	if (entities == NULL) {entities = new; return;}

	listNode* i = entities;
	while(i->next != NULL) {
		i = i->next;
	}
	i->next = new;
}

/*!
 * "Apaga" todas as entidades libertando a memoria que ocupam
 */
void entitiesFree() {
	listNode* i = entities,*r;
	while(i!=NULL) {
		r = i; i = i->next;
		free(r->elem);
		free(r);
	}
};

/*!
 * Conta o numero de entidades que existem.
 * \return numero de entidades que existem
 */
int entityCount() {
	listNode* i = entities; int c = 0;
		while(i!=NULL) {
			c++;
			i = i->next;
		}
	return c;
};

