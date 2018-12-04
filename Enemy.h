#ifndef ENEMY_H
#define ENEMY_H
#include "Main.h"

const double	MAX_VELOCITY{ 1.5 },
				THRESHOLD{ 1 };

class Enemy
{
protected:
	int ENEMY_IMG_W{ 16 },
		ENEMY_IMG_H{ 16 },
		ENEMY_WALK1_SX{ 12 },
		ENEMY_WALK1_SY{ 327 },
		ENEMY_WALK2_SX{ 52 },
		ENEMY_WALK2_SY{ 326 },
		ENEMY_WALKING_REFRESH_RATE{ 10 },
		ENEMY_DEATH_SX{ 0 },
		ENEMY_DEATH_SY{ 0 };
	int type, anim_refresh_rate, current_anim_frame, death_buffer, death_buffer_rate;
	double last_time, last_posx, last_posy, acceleration_x, acceleration_y;
	bool is_facing_right, alternate_anim;
	ALLEGRO_BITMAP *enemy_image;
	ALLEGRO_SAMPLE *enemy_stomped_sound;
public:
	int direction, v_direction, h_direction, box_x1, box_x2, box_y1, box_y2, pos_x, pos_y, bottom_center_pixel_x, bottom_center_pixel_y, center_pixel_y;
	double velocity_x, velocity_y;
	bool isDead, keepOnScreen;
	Enemy();
	Enemy(int type, int start_tile_x, int start_tile_y, int direction, ALLEGRO_BITMAP *enemy_image, ALLEGRO_SAMPLE *enemy_stomped_sound);
	~Enemy();
	void update();
	//void AI();
	void changePosition();
	void updateBox();
	void checkIfOOB();
	void refreshAnimation();
	virtual void draw();
	void updateLastPos();
	void updateVelocity();
	void setPositionByBoxX1(int new_box_x1);
	void setPositionByBoxX2(int new_box_x2);
	void setPositionByBoxY1(int new_box_y1);
	void setPositionByBoxY2(int new_box_y2);
	void turnAround();
	int getTileX();
	int getTileY();
	void drawBox();
	virtual void die();
	void waitAfterDeath();
};

#endif
