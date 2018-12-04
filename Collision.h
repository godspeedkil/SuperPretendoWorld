#ifndef COLLISION_H
#define COLLISION_H
#include "Player.h"

class Collision
{
private:
	int loadCounterX, loadCounterY, mapSizeX, mapSizeY, collMapFile[100][100], currentLevel;
public:
	Collision();
	~Collision();
	void loadCollisionMap(const char *filename);
	void update(Player &player);
	void update(Enemy &enemy);
	void platformCollision(Player &player);
	void platformCollision(Enemy &enemy);
	void playerToEnemyCollision(Player &player, std::vector<Enemy*> enemy_list);
	bool boxesOverlapping(Player &player, Enemy &enemy);
};

#endif