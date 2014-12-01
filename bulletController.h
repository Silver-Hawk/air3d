#ifndef __BULLET_CONTROLLER_H__
#define __BULLET_CONTROLLER_H__

#include "bullet.h"

class bulletController {
public:
	//positions and rotations and velocity
	bullet *start;
	bullet *last;
	int bullet_count;


	//should divide space for collision
	bulletController() {
		bullet_count = 0;
		start = NULL;
		last = NULL;
	}

	void add(bullet *b){
		if(bullet_count > 0)
		{
			last->setNext(b);
			b->setPrev(last);
			
			last = b;
		}
		else
		{
			start = b;
			last = b;
		}

		bullet_count++;
	}

	void update(){
		int count = 0;
		printf("bullet_count %i\n", bullet_count);
		if(start){
			int *bounds = getWorldBounds();
			bullet* b = start;
			while(b && bullet_count){
				count++;
				b->update();
				if(!checkInsideWorld(bounds, b)){
					bullet_count--;
					
					bullet* tmp = NULL;
					if(b->next)
						tmp = b->next;
					
					if(b == last)
						if(b->prev)
							last = b->prev;

					if(start == b)
						if(b->next)
							start = b->next;
					printf("wow\n" );
					b->destroy();

					printf("wow2\n");
					b = tmp;
				}
				else
				{
					b = b->next;
				}
			}
			if(bullet_count == 0){
				start = NULL;
				last = NULL;
			}

		}
	}

	void draw(cam camera){
		bullet* b = start;
		while(b){
			if(camera.PointInFrustum(b->a.getX(), b->a.getY(), 0.0f)){
				b->draw();
			}
			b = b->next;
		}
	}

	static bool checkInsideWorld(int* worldBounds, bullet *b){
		angular a = b->getPos();
		if(a.getX() < worldBounds[0] || a.getY() < worldBounds[1] || a.getX() > worldBounds[2] || a.getY() > worldBounds[3])
			return false;
		return true;
	}
	
};


#endif