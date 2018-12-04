#ifndef GREENKOOPA_H
#define GREENKOOPA_H

#include "Enemy.h"

class GreenKoopa : public Enemy
{
protected:
int	ENEMY_IMG_W{ 16 },
	ENEMY_IMG_H{ 16 },
	ENEMY_WALK1_SX{ 92 },
	ENEMY_WALK1_SY{ 85 },
	ENEMY_WALK2_SX{ 132 },
	ENEMY_WALK2_SY{ 86 },
	ENEMY_DEATH_SX{ 132 },
	ENEMY_DEATH_SY{ 122 };
public:
	GreenKoopa();
	GreenKoopa(int type, int start_tile_x, int start_tile_y, int direction, ALLEGRO_BITMAP *enemy_image, ALLEGRO_SAMPLE *enemy_stomped_sound);
	virtual void draw();
};

#endif