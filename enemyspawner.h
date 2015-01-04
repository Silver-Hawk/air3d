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
	float max_height;  
	bufferhelper* b;
	texturehelper* t;

	enemyspawner(bufferhelper* buf, texturehelper* tex)  {
		timer = 0;
		last_time = timer;

		b=buf;
		t=tex;

		right = WC.xend;
		left = WC.xstart;

		max_height = 3000;

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

			//add one extra enemy for each step
			for(int i = 0; i < 7+(timer/10); i++){
				spawnEnemy();
			}

		}


	}

	void spawnEnemy(){
		int side = floor(2 * static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)));

		float xplace = 0.0f, yplace = 0.0f;

		if(side < 1)
			xplace = left;
		else
			xplace = right;

		yplace = max_height * static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));


		UC->addEnemy(new enemy(MOV_SEEKING));
		UC->last_enemy->setTarget(UC->getPlayerUnit(0));
		UC->last_enemy->getUnit()->setBuffers(b);
		UC->last_enemy->getUnit()->setTex(t);
		UC->last_enemy->getUnit()->pos[0] = xplace;
		UC->last_enemy->getUnit()->pos[1] = yplace;

		//upgrade enemy weapon
		UC->last_enemy->wc->shots = std::max(1, (int) ((timer/30) * static_cast <float> (rand()) / (static_cast <float> (RAND_MAX))));
		UC->last_enemy->wc->splits = std::max(1,(int)((timer/90) * static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)))); //amount of bullets when fire is run
		UC->last_enemy->wc->spread = 50 * static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)); //spread angle

		if(timer > 150){
			UC->last_enemy->wc->setBehavior(3 * static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)));
		}


		printf("spawned enemy at %f\n", xplace);
		printf("spawned enemy at %f\n", yplace);
	}
};

#endif