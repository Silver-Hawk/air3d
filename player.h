#ifndef __PLAYER_CLASS_H__
#define __PLAYER_CLASS_H__

#include "unit.h"

class player {
public:
	int left, right, up;
	unit *self;
	player(int left, int right, int up, unit *self) : left(left), right(right), up(up), self(self)  {
	}

	void update(float delta) {
		if (glfwGetKey (g_window, left)) {
			self->addAngle(self->getRotationSpeed() * delta);
		}
		if (glfwGetKey (g_window, right)) {
			self->addAngle(-self->getRotationSpeed() * delta);
		}
		//unit_angle = fmod(unit_angle, 360.0f);
		if (glfwGetKey(g_window, up)){
			printf("LOLOLOLOLO\n");
			self->addVelocity(delta);
		}
		self->applyVelocity();
	}
};

#endif