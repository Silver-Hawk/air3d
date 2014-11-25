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
	float xspeed;
	float yspeed;

	//make it a double linked list
	bullet *prev;
	bullet *next;

	bool firstDraw;

	bullet(sprite spr, float angle, float x, float y, float speed){
		a = angular();
		a.setPos(x, y, 0.0f);
		a.setAngle(angle);

		this->speed = speed;
		s = spr;
		xspeed = speed * cos(DEG_TO_RAD * angle);
		yspeed = speed * sin(DEG_TO_RAD * angle);
		firstDraw = false;
	}

	void addPlaneSpeed(float dx, float dy){
		xspeed += dx;
		yspeed += dy;
	}

	void setNext(bullet *b){
		next = b;
	}

	void setPrev(bullet *b){
		prev = b;
	}

	void destroy(){
		//update list
		next->prev = prev;
		prev->next = next;

		free(this);
	}

	void update(){
		if(firstDraw){
			a.addX(xspeed);
			a.addY(yspeed);
		}
	}

	void bindMatOnly(){
		a.bindBulletMat();
	}

	void draw(){
		firstDraw = true;
		a.bindBulletMat();
		s.draw();
	}
};


#endif