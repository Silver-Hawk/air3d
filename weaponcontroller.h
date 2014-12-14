#ifndef __WEAPON_CONTROLLER_CLASS_H__
#define __WEAPON_CONTROLLER_CLASS_H__

#include "sprite.h"

#define WEAPON_SHOTGUN 0//mutiple bullets at the same time
#define WEAPON_RIFLE 1//burst fire
#define WEAPON_SINGLE_SHOT 2 //normal enemy weapon

class weaponcontroller {
	public:
	//cooldown timers
	float reloadtimer;
	float firetimer;

	//reload time
	float reloadTime; 

	int shots;
	int splits; //amount of bullets when fire is run
	int spread; //spread angle

	int weaponBehavior;

	//bullet starting point
	float xstart, ystart;

	float *split_angles;

	//enemy or player
	int controller;

	weaponcontroller(){
		reloadtimer  = 0.0f;
		firetimer = 0.0f;
		shots = 4;
		splits = 1;
		spread = 0.0f;

		reloadTime = 1.5f;
	}

	void setController(int control){
		controller = control;
	}

	//f.x. can be WEAPON_SHOTGUN 
	void setBehavior(int type){
		weaponBehavior = type;
	}

	void fire(unit *u){
		if(reloadtimer <= 0){
			if(weaponBehavior == WEAPON_SINGLE_SHOT)
				fireSingle(new sprite(BULLET_TEXTURE, 5.0f), split_angles[(int) (splits/2)], xstart, ystart, 10.0f, u);

			if(weaponBehavior == WEAPON_SHOTGUN)
				fireShotgun(u);
			
			reloadtimer = reloadTime;
		}
	}

	bool reloading(){
		return reloadtimer > 0;
	}

	void fireShotgun(unit* u){
		//make random shotgun spread
		for(int j=0;j<splits;j++){
			for(int i=0; i<shots; i++){
				//shotgun spread
				float diffAngle = -30.0f + 60.0f * static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
				//make different size shots
				float scale = 2.0f + 3.0f * static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));

				fireSingle(new sprite(BULLET_TEXTURE, scale), split_angles[j]+diffAngle, xstart, ystart, 2.0f * scale, u);
			}
		}
	}

	void fireRifle(unit* u){
		for(int j=0;j<splits;j++){
			fireSingle(new sprite(BULLET_TEXTURE, 5.0f), split_angles[j], xstart, ystart, 10.0f, u);
		}

		firetimer = reloadTime/(shots*2);
	}

	void fireSingle(sprite* s, float angle, float x, float y, float speed, unit* u){
		BC->add(new bullet(s, angle, x, y, speed, controller));
		BC->last->addPlaneSpeed(u->velocity.x, u->velocity.y);	
	}

	void update(float dt, unit* u, float displacement){
		reloadtimer -= dt;

		float dx = cos(ONE_DEG_IN_RAD * u->getAngle());
		float dy = sin(ONE_DEG_IN_RAD * u->getAngle());
		
		split_angles = (float*) malloc(sizeof(float) * splits);

		for(int j=0;j<splits;j++){
			float splitAngle, diffAngle;
			int extradiff;
			
			//calculate split
			splitAngle = spread/2.0f;

			diffAngle = splitAngle / ((float) splits);
			if(j%2 == 0)
				extradiff = 0; 
			else
				extradiff = 1;
			
			split_angles[j] = -splitAngle + u->getAngle() + (diffAngle*(j+extradiff));

			xstart = u->pos[0] + (displacement * dx) + u->velocity.x;
			ystart = u->pos[1] + (displacement * dy) + u->velocity.y;
		}


		if(weaponBehavior == WEAPON_RIFLE){
			if(reloadtimer > reloadTime/2){
				if(firetimer <= 0){
					fireRifle(u);
				}
				firetimer-=dt;
			}
		}
	}
};


#endif