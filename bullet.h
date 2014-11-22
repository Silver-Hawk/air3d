#ifndef __BULLET_CLASS_H__
#define __BULLET_CLASS_H__

#define DEG_TO_RAD 0.0174532925

#include "sprite.h"
#include "angular.h"

class bullet {
	public:
	angular a;
	sprite s;
	float speed;

	bullet(sprite spr, float angle, float x, float y, float speed){
		a = angular();
		a.setPos(x, y, 0.0f);
		a.setAngle(angle);

		this->speed = speed;
		s = spr;
	}

	void update(){
		a.addX(speed * cos(DEG_TO_RAD * a.getAngle()));
		a.addY(speed * sin(DEG_TO_RAD * a.getAngle()));
	}

	void draw(){
		a.bindBulletMat();
		s.draw();
	}
};


#endif