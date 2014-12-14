#ifndef __COLLISION_CLASS_H__
#define __COLLISION_CLASS_H__

#include "unitcontroller.h"
#include "bulletController.h"
#include "enemy.h"
#include "bvec.h"
#include "particleSprite.h"

class collision {
public:
	
	collision()  {
	}

	//naive all x all check
	void checkAllAll() {
		enemy *e = UC->enemies;

		while(e){
			bullet *b = BC->start;
			while(b){
				if(b->controlledBy == ENEMY_CONTROLLED){
					b = b->next;
					continue;
				}

				if(checkCollision(b, e->getUnit())){
					e->health -= b->getDmg();
					b =	BC->destroyBullet(b);
				}
				else
				{
					b = b->next;
				}
			}
			e = e->next;
		}

		player *p = UC->players;
		while(p){
			bullet *b = BC->start;
			while(b){
				if(b->controlledBy == PLAYER_CONTROLLED){
					b = b->next;
					continue;
				}

				checkCollision(b, p->getUnit());
				b = b->next;
			}
			p = p->next;
		}
	}

	bool checkCollision(bullet* b, unit* u){
		if (b->get2Dpos()->dist(*u->get2Dpos()) - (u->collisionRadius) < b->getRadius()){
			printf("Got hit : %s\n", (b->controlledBy==ENEMY_CONTROLLED)?"player":"enemy");
			
			float rot =	360 * static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
			float randomSize = 35 * static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
			PC->add(new particle(new sprite(EXPLOSION_TEXTURE, AC->getXscale(EXPLOSION_TEXTURE), AC->getYscale(EXPLOSION_TEXTURE), randomSize), 0.0f, u->pos[0], u->pos[1], 0.0f, rot, 3.0f, -0.01f));
			PC->last->s->startAnimation(25);

			//PC->add(new particle(new sprite(POW_TEXTURE, AC->getXscale(POW_TEXTURE), AC->getYscale(POW_TEXTURE), 15.0f), 0.0f, u->pos[0], u->pos[1], 0.0f, ANGLE_UP, 3.0f, -0.01f));
			return true;
		}
		return false;
	}
};

#endif