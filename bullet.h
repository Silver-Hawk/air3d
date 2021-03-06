#ifndef __BULLET_CLASS_H__
#define __BULLET_CLASS_H__

#define PLAYER_CONTROLLED 0
#define ENEMY_CONTROLLED 1

#define DEG_TO_RAD 0.0174532925

#include "sprite.h"
#include "angular.h"
#include "bvec.h"

class bullet {
	public:
	angular a;
	sprite *s;

	float speed;
	float xspeed;
	float yspeed;

	//make it a double linked list
	bullet *prev;
	bullet *next;

	bool firstDraw;

	int controlledBy;

	float damage;

	bullet(sprite *spr, float angle, float x, float y, float speed, int controller){
		a = angular();
		a.setPos(x, y, 0.0f);
		a.setAngle(angle);

		this->speed = speed;
		s = spr;
		xspeed = speed * cos(DEG_TO_RAD * angle);
		yspeed = speed * sin(DEG_TO_RAD * angle);
		firstDraw = false;

		prev = NULL;
		next = NULL;
		controlledBy = controller;

		damage = 0.0f;
	}

	float getDmg(){
		if(damage == 0){
			return s->getScale() * 2.0f;
		}
		return damage;
	}

	/*bullet(sprite *spr, float angle, float x, float y, float speed, int id){
		a = angular();
		a.setPos(x, y, 0.0f);
		a.setAngle(angle);

		this->speed = speed;
		s = spr;
		xspeed = speed * cos(DEG_TO_RAD * angle);
		yspeed = speed * sin(DEG_TO_RAD * angle);
		firstDraw = false;

		prev = NULL;
		next = NULL;

		debugId = id;
	}*/

	void addPlaneSpeed(float dx, float dy){
		xspeed += dx;
		yspeed += dy;
	}

	angular getPos(){
		return a;
	}

	void setNext(bullet *b){
		next = b;
	}

	void setPrev(bullet *b){
		prev = b;
	}

	void destroy(){
		//update list
		if(next)
			next->prev = prev;
		if(prev)
			prev->next = next;

		free(this);
	}

	void applyGravity(){
		yspeed -= 0.09f;
	}

	void update(){
		if(firstDraw){
			a.addX(xspeed);
			a.addY(yspeed);

			applyGravity();
		}
	}

	bvec2<float>* get2Dpos(){
		return (new bvec2<float> (a.getX(), a.getY()));
	}

	void bindMatOnly(){
		a.bindBulletMat();
	}

	void draw(){
		firstDraw = true;
		a.bindBulletMat();
		s->draw();
	}

	//naively use sprite size
	float getRadius(){
		return s->getScale();
	}
};


#endif