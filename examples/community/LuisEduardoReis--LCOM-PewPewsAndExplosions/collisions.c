#include "collisions.h"
#include "stdlib.h"
#include "game.h"
#include "util.h"

extern listNode* entities;

extern int life;

/*!
 * evento de colisao de uma entidade com outra (e chamado duas vezes para cada par de entidades que colide)
 * \param s entidade para a qual a colisao acontece
 * \param o entidade com a qual colide
 */
void collision(entity* s, entity* o) {

	enum entityType me = s->type, other = o->type;

	switch (me) {
	case ENEMY:
		switch (other) {
		case BULLET:
			s->health = max(s->health- ((Bullet*) o)->damage,0);
			if (s->health == 0) ((Enemy*) s)->killedByPlayer = 1;
			s->hurtTimer = 16 * (FPS/60.0);
			o->remove = 1;
			break;
		case PLAYER:
			s->remove = 1;
			break;
		}
		break;
	case ENEMYCRUISER:
		switch (other) {
		case BULLET:
			s->health = max(s->health- ((Bullet*) o)->damage,0);
			if (s->health == 0) ((EnemyCruiser*) s)->killedByPlayer = 1;
			s->hurtTimer = 16 * (FPS/60.0);
			o->remove = 1;
			break;
		}
		break;

	case ENEMYMOTHERSHIP:
		switch (other) {
		case BULLET:
			if (((EnemyMotherShip*) s)->hasShield <= 0) {
				s->health = max(s->health- ((Bullet*) o)->damage,0);
				if (s->health == 0) ((EnemyMotherShip*) s)->killedByPlayer = 1;
				s->hurtTimer = 16 * (FPS / 60.0);
			}
			o->remove = 1;
			break;
		}
		break;



		case PLAYER:
				switch (other){
				case ENEMY:
					if (((Player*) s)->isInvincible <= 0) {
						if (((Player*) s)->hasShield <= 0){
							s->health = max(s->health - 10,0);
							s->hurtTimer = 16 * (FPS/60.0);
						} else {
							o->health = max(s->health - 25,0);
							o->hurtTimer = 16 * (FPS/60.0);
							((Player*) s)->hasShield = max(((Player*) s)->hasShield - 3, 0);
						}
					}
					o->remove = 1;
					break;
				case EBULLET:
					if(((Player*) s)->hasShield <= 0 && ((Player*) s)->isInvincible <= 0) {
						s->health = max(s->health-5,0);
						s->hurtTimer = 16 * (FPS/60.0);
					}
					o->remove = 1;
					break;
				case POWERUP:
					if (((PowerUp*) o)->type == LIFE)
						{life++; o->remove = 1;}
					else if (((PowerUp*) o)->type == SHIELD)
						{((Player*) s)->hasShield = 10*FPS; o->remove = 1;}
					else if (((PowerUp*) o)->type == REDBEAM) {
						((Player*) s)->hasRedBeam = 3*FPS;
						((Player*) s)->canShoot = 1;
						o->remove = 1;
					}
					break;
				case ENEMYCRUISER:
					if (((Player*) s)->isInvincible <= 0) {
						s->health = max(s->health-5,0);
						s->hurtTimer = 16 * (FPS/60.0);
					}
					s->x += lengthdir_x(2,point_direction(o->x,o->y, s->x,s->y));
					s->y += lengthdir_y(2,point_direction(o->x,o->y, s->x,s->y));

					break;
				case ENEMYMOTHERSHIP:
					if (((Player*) s)->isInvincible <= 0) {
						s->health = max(s->health-5,0);
						s->hurtTimer = 16 * (FPS/60.0);
					}
					s->x += lengthdir_x(2,point_direction(o->x,o->y, s->x,s->y));
					s->y += lengthdir_y(2,point_direction(o->x,o->y, s->x,s->y));
				}


	}



};


/*!
 * verifica colisoes entre entidades
 */
void bruteForceCollisions() {

	listNode* i,* o;
	for(i = entities; i != NULL; i = i->next) {
		if (! (i->elem)->solid) continue;
		for(o = entities; o != NULL; o = o->next) {
				if (i->elem == o->elem || !(o->elem)->solid) continue;

				entity* me = (i->elem),*other = (o->elem);

				if (rectanglesCollision(me->mask,me->x,me->y,  other->mask,other->x,other->y))
					collision(me,other);
			}
	}
};


int rectanglesCollision(rectangle a, double ax, double ay, rectangle b, double bx, double by) {
	a.ox += ax; a.oy += ay;
	a.w += a.ox; a.h += a.oy;

	b.ox += bx; b.oy += by;
	b.w += b.ox; b.h += b.oy;

	return (a.ox <= b.w && b.ox <= a.w && a.oy <= b.h && b.oy <= a.h);
}

int pointToRectangleCollision(double px, double py, rectangle r, double rx, double ry) {
	r.ox += rx; r.oy += ry;
	r.w += r.ox; r.h += r.oy;

	return (px <= r.w && px >= r.ox && py <= r.h && py >= r.oy);
}
