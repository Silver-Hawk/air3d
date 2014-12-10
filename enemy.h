#ifndef __ENEMY_CLASS_H__
#define __ENEMY_CLASS_H__

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
	
	//behavior variables
	int MOV;
	int SPEED_SETUP;

	enemy (int Movement_Behavior, unit *me) {
		MOV = Movement_Behavior;
		self = me;
	}

	void setTarget(unit *tar){
		target = tar;
	}

	void update(float delta) {
		bvec2<float> steer;
		if(MOV == MOV_SEEKING)
			steer = seeking();

		calculate_changes(delta, steer);
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

	void calculate_changes(float delta, bvec2<float> steer){
		float minusr = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
		float plusr = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
		self->alignAngle(delta, atan2(steer.y, steer.x)*57.2957795+(plusr-minusr));
		
		if(SPEED_SETUP == SPEED_MAX){
			self->addVelocity(0.1f * delta);
		}
	}


};

#endif