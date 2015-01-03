#ifndef __ENEMY_SPAWNER_CLASS_H__
#define __ENEMY_SPAWNER_CLASS_H__

#include "enemy.h"
#include "player.h"
#include "bufferHelper.h"
#include "texturehelper.h"


class enemyspawner {
public:
	float timer;
	float last_time;
	float right, left; 
	bufferhelper* b;
	texturehelper* t;

	enemyspawner(bufferhelper* buf, texturehelper* tex)  {
		timer = 0;
		last_time = timer;

		b=buf;
		t=tex;

		right = WC.xend;
		left = WC.xstart;

		initspawn();
	}

	void initspawn(){
		int enemies = 5 * static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));

		for(int i = 0; i < enemies; i++){
			spawnEnemy();
		}
	}

	void update(float dt){
		timer += dt;

		if(timer > last_time+20){
			last_time = timer;
			printf("Spawned enemies\n");

			for(int i = 0; i < 10; i++){
				spawnEnemy();
			}

		}


	}

	void spawnEnemy(){
		int side = floor(2 * static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)));

		float xplace = 0.0f;

		if(side < 1)
			xplace = left;
		else
			xplace = right;

		UC->addEnemy(new enemy(MOV_SEEKING));
		UC->last_enemy->setTarget(UC->getPlayerUnit(0));
		UC->last_enemy->getUnit()->setBuffers(b);
		UC->last_enemy->getUnit()->setTex(t);
		UC->last_enemy->getUnit()->pos[0] = xplace;

		printf("spawned enemy at %f\n", xplace);
	}
};

#endif