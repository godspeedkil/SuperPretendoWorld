#ifndef MAP_H
#define MAP_H
#include "Player.h"
#include <fstream>

class Map
{
private:
	int loadCounterX, loadCounterY, mapSizeX, mapSizeY, clearFlagX, clearFlagY, mapFile[100][100], currentLevel;
	bool levelClear;
	ALLEGRO_BITMAP *tile_image;
	ALLEGRO_SAMPLE *level_clear_sound, *final_level_clear;
public:
	Map();
	~Map();
	void update(Player &player);
	void draw();
	void loadMap(const char *filename);
	void setClearFlagPosition();
	bool getLevelClear();
	void playerWithinClearFlag(Player &player);
	void reinitialize(const char *filename);
};

#endif