#ifndef __PARTICLE_CONTROLLER_H__
#define __PARTICLE_CONTROLLER_H__

#include "particleSprite.h"
#include "cam.h"

class particlecontroller {
public:
	//positions and rotations and velocity
	particle *start;
	particle *last;

	int count;
	//should divide space for collision
	particlecontroller() {
		start = NULL;
		last = NULL;

		count = 0;
	}

	void add(particle *p){
		if(start)
		{
			last->setNext(p);
			p->setPrev(last);
			
			last = p;
		}
		else
		{
			start = p;
			last = p;
		}

		count++;
	}

	void update(){
		if(start && count){
			int *bounds = getWorldBounds();
			particle* p = start;
			while(p && count){
				p->update();

				if(!checkInsideWorld(bounds, p) || p->alp < 0 || p->s->animationDone()){
					//TODO make removal of particles work
					p = destroyParticle(p);
					//p = p->next;
				}
				else
				{
					p = p->next;
				}
			}
			if(count == 0){
				start = NULL;
				last = NULL;
			}
		}

	}

	void draw(cam camera){
		particle* p = start;
		while(p){
			if(camera.PointInFrustum(p->a.getX(), p->a.getY(), 0.0f)){
				p->draw();
			}
			p = p->next;
		}
	}

	particle* destroyParticle(particle* p){
		count --;
		particle* tmp = NULL;
		
		printf("D 1\n");
		if(p->next)
			tmp = p->next;
		printf("D 2\n");
		
		if(p == last)
			if(p->prev)
				last = p->prev;
		printf("D 3\n");

		if(start == p)
			if(p->next)
				start = p->next;

		printf("D 4\n");

		p->destroy();

		printf("D 5\n");

		return tmp;
	}

	static bool checkInsideWorld(int* worldBounds, particle *p){
		printf("check inside world\n");
		angular a = p->getPos();
		if(a.getX() < worldBounds[0] || a.getY() < worldBounds[1] || a.getX() > worldBounds[2] || a.getY() > worldBounds[3])
			return false;
		return true;
	}
	
};


#endif