#ifndef __ENEMY_CLASS_H__
#define __ENEMY_CLASS_H__

#include "weaponcontroller.h"
#include "unit.h"
#include "bvec.h"

#define MOV_SEEKING 0

#define SPEED_MAX 0
#define SPEED_MIN 1
#define SPEED_NONE 2

class enemy {
public:
	unit *self;
	unit *target;
	weaponcontroller *wc;
	
	enemy* prev;
	enemy* next;

	//behavior variables
	int MOV;
	int SPEED_SETUP;

	enemy (int Movement_Behavior) {
		MOV = Movement_Behavior;

		wc = new weaponcontroller();

		prev = next = NULL;

		//either give shotgun or rifle
		int weapon = round(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX))); 

		wc->setBehavior(weapon);

		self = new unit(0.0f, 0.0f, 0.0f);
	}

	void setTarget(unit *tar){
		target = tar;
	}

	void update(float delta) {
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
		self->update();
	}

	void draw(){
		self->draw();
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