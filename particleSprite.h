#ifndef __PARTICLE_CLASS_H__
#define __PARTICLE_CLASS_H__

#define DEG_TO_RAD 0.0174532925

#define ANGLE_UP 90 

#include "sprite.h"
#include "angular.h"
#include "bvec.h"

class particle {
	public:
	angular a;
	sprite *s;

	float speed;
	float xspeed;
	float yspeed;

	//make it a double linked list
	particle *prev;
	particle *next;

	bool firstDraw;

	float alp;
	float alpDiff;

	particle(sprite *spr, float angle, float x, float y, float speed, float imageAngle, float alpha, float alphaChange){
		a = angular();
		a.setPos(x, y, 0.0f);
		a.setAngle(imageAngle);

		this->speed = speed;
		s = spr;
		xspeed = speed * cos(DEG_TO_RAD * angle);
		yspeed = speed * sin(DEG_TO_RAD * angle);
		firstDraw = false;

		prev = NULL;
		next = NULL;

		alp = alpha;
		alpDiff = alphaChange;
	}

	angular getPos(){
		return a;
	}

	void setNext(particle *b){
		next = b;
	}

	void setPrev(particle *b){
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

			alp += alpDiff;
		}
	}

	bvec2<float>* get2Dpos(){
		return (new bvec2<float> (a.getX(), a.getY()));
	}

	void bindMatOnly(){
		a.bindBulletMat();
	}

	void updateAlpha(){
		SC->getShader(SPRITE_SHADER).bindLocationFloat(alp, 4);
	}

	void draw(){
		firstDraw = true;
		a.bindBulletMat();
		updateAlpha();
		s->draw();
	}

	//naively use sprite size
	float getRadius(){
		return s->getScale();
	}
};


#endif