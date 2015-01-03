#ifndef __UNIT_CONTROLLER_CLASS_H__
#define __UNIT_CONTROLLER_CLASS_H__

#include "enemy.h"
#include "player.h"

class unitcontroller {
public:
	enemy *enemies;
	enemy *last_enemy;

	player *players;
	player *last_player;


	unitcontroller()  {
		enemies = NULL;
		last_enemy = NULL;
		players = NULL;
		last_player = NULL;	
	}

	void addEnemy(enemy *e) {
		if(!enemies){
			enemies = e;
			last_enemy = e;
		}
		else
		{
			last_enemy->next = e;
			e->prev = last_enemy;

			last_enemy = e;
		}
	}

	void addPlayer(player *p) {
		if(!enemies){
			players = p;
			last_player = p;
		}
		else
		{
			last_player->next = p;
			p->prev = last_player;

			last_player = p;
		}
	}

	enemy* getEnemy(int index){
		enemy *e = enemies;
		while(index > 0){
			e = e->next;
			index--;
		}

		return e;
	}

	unit* getEnemyUnit(int index){
		return (getEnemy(index))->getUnit();
	}

	player* getPlayer(int index){
		player *p = players;
		while(index > 0){
			p = p->next;
			index--;
		}

		return p;
	}

	unit* getPlayerUnit(int index){
		return (getPlayer(index))->getUnit();
	}

	void update(float dt){
		player *p = players;
		while(p){
			p->update(dt);
			p = p->next;
		}

		enemy *e = enemies;
		while(e){
			e->update(dt);
			if(e->checkDead() && e->checkNearWater()){
				e->destroy();
			}

			e = e->next;
		}
	}

	void draw(){
		player *p = players;
		while(p){
			p->draw();
			p = p->next;
		}

		enemy *e = enemies;
		while(e){
			e->draw();
			e = e->next;		
		}
	}

};

#endif