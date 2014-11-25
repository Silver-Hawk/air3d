#ifndef __BULLET_CONTROLLER_H__
#define __BULLET_CONTROLLER_H__

#include "bullet.h"

class bulletController {
public:
	//positions and rotations and velocity
	bullet *start;
	bullet *last;
	bool hasBullets;


	//should divide space for collision

	bulletController() {
		hasBullets = false;
	}

	void add(bullet *b){
		if(hasBullets)
			last->next = b;
		else
			start = b; 
	}

	void update(){

	}
	
};


#endif