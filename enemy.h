#ifndef __ENEMY_CLASS_H__
#define __ENEMY_CLASS_H__

#include "weaponcontroller.h"
#include "unit.h"
#include "bvec.h"

#define MOV_SEEKING 0

#define SPEED_MAX 0
#define SPEED_MIN 1
#define SPEED_NONE 2

#define DEAD_MOVE_RIGHT 0
#define DEAD_MOVE_LEFT 1
#define DEAD_MOVE_NONE 2

class enemy {
public:
	unit *self;
	unit *target;
	weaponcontroller *wc;
	
	enemy* prev;
	enemy* next;

	float health;

	//behavior variables
	int MOV;
	int SPEED_SETUP;
	int DEATH_SETUP;

	bool dead;

	enemy (int Movement_Behavior) {
		MOV = Movement_Behavior;

		wc = new weaponcontroller();
		prev = next = NULL;
		//either give shotgun or rifle
		/*int weapon = round(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX))); 
		wc->setBehavior(weapon);*/


		wc->setBehavior(WEAPON_SINGLE_SHOT);
		wc->setController(ENEMY_CONTROLLED);

		health = 30 * static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
		
		self = new unit(0.0f, 0.0f, 0.0f);

		dead = false;

		DEATH_SETUP = -1;
	}

	void setTarget(unit *tar){
		target = tar;
	}

	void update(float delta) {
		checkDead();

		if(!dead){
			bvec2<float> steer;
			if(MOV == MOV_SEEKING)
				steer = seeking();

			if(wc->reloading())
				steer = fleeing();

			wc->update(delta, self, 5.481503f);


			if(distanceToTarget() < 250)
				if(!wc->reloading())
					wc->fire(self);

			calculate_changes(delta, steer);
		}
		else
		{
			if(DEATH_SETUP < 0){
				DEATH_SETUP =  floor(4 * static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)));

				printf("I AM DEAD with setup %i\n", DEATH_SETUP);
			}

			if(DEATH_SETUP < 2){
				if(DEATH_SETUP == DEAD_MOVE_LEFT){
					self->addAngle(self->getRotationSpeed()*delta);
				}
				else
				{
					self->addAngle(-self->getRotationSpeed()*delta);
				}


			}

		}

		self->update();
	}

	void draw(){
		self->draw();
	}

	bool checkDead(){
		if(health < 0)
			dead = true;

		return dead;
	}

	bool checkNearWater(){
		if(self->pos[1] < 15.0f)
			return true;
		return false;
	}

	enemy* destroy(){
		int numbers = 5 + floor(7* static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)));
		for(int i = 0; i <= numbers; i++){
			float rot =	360 * static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
			float randomSize = 35 * static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
			float addx = -10 + 20 * static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
			float addy = -10 + 20 * static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
			PC->add(new particle(new sprite(EXPLOSION_TEXTURE, AC->getXscale(EXPLOSION_TEXTURE), AC->getYscale(EXPLOSION_TEXTURE), randomSize), 0.0f, self->pos[0] + addx, self->pos[1] + addy, 0.0f, rot, 3.0f, -0.01f));
			PC->last->s->startAnimation(25);
		}
		
		//update list
		if(next)
			next->prev = prev;
		if(prev)
			prev->next = next;

		free(this);
		return next;
	}

	//AI CHECKS
	float distanceToTarget(){
		bvec2<float>* selfpos = self->get2Dpos();
		bvec2<float>* targetpos = target->get2Dpos();

		return selfpos->dist(*targetpos);// selfpos->dist(targetpos);
	}


	//MOVEMENT BEHAVIORS
	bvec2<float> seeking() {
		bvec2<float>targetdir = *(target->get2Dpos());
		bvec2<float>selfdir = *(self->get2Dpos());
		bvec2<float>dir = targetdir - selfdir; 
		dir.normalize();
		SPEED_SETUP = SPEED_MAX;
		return dir;
	}

	bvec2<float> fleeing(){
		bvec2<float>dir = seeking();
		dir *= -1;
		return dir;
	}

	void calculate_changes(float delta, bvec2<float> steer){
		float minusr = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
		float plusr = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
		self->alignAngle(delta, atan2(steer.y, steer.x)*57.2957795+(plusr-minusr));
		
		if(SPEED_SETUP == SPEED_MAX){
			self->addVelocity(0.1f * delta);
		}
	}

	unit* getUnit(){
		return self;
	}

};

#endif