#ifndef __PLAYER_CLASS_H__
#define __PLAYER_CLASS_H__

#include "unit.h"
#include "weaponcontroller.h"

class player {
public:
	int left, right, up, shoot;
	unit *self;
	weaponcontroller *wc;

	player* prev;
	player* next;

	

	player(int left, int right, int up, int shoot) : left(left), right(right), up(up), shoot(shoot)  {
		wc = new weaponcontroller();
		wc->setBehavior(WEAPON_RIFLE);
		wc->setController(PLAYER_CONTROLLED);

		wc->shots = 40;
		wc->splits = 10; //amount of bullets when fire is run
		wc->spread = 15; //spread angle


		prev = next = NULL;

		self = new unit(0.0f, 0.0f, 0.0f);
	}

	void update(float delta) {
		float rotSpeed = (self->getRotationSpeed() * delta);

		//update weapon controller
		wc->update(delta, self, 5.481503f);

		if(glfwGetKey(g_window, up))
			rotSpeed *= 0.50;

		if (glfwGetKey (g_window, left)) {
			self->addAngle(rotSpeed);
		}
		if (glfwGetKey (g_window, right)) {
			self->addAngle(-rotSpeed);
		}
		//unit_angle = fmod(unit_angle, 360.0f);
		if (glfwGetKey(g_window, up)){
			self->addVelocity(delta);
		}

		if (glfwGetKey(g_window, shoot)){
			SC->use(SPRITE_SHADER);
			wc->fire(self);
		}
		self->update();
	}

	void draw(){
		self->draw();
	}

	unit *getUnit(){
		return self;
	}
};

#endif