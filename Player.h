#ifndef PLAYER_H
#define PLAYER_H
#include "Enemy.h"

enum STATES {
	IDLE, 
	WALKING, 
	RUNNING, 
	SPRINTING, 
	JUMPING, 
	FALLING, 
	PLATFORM,
	CROUCHING, 
	LOOKING_UP, 
	RUNNING_JUMPING, 
	DECELERATING_HORIZONTAL
};

const int	SM_MARIO_XTO_BOX{ 7 },
			SM_MARIO_YTO_BOX{ 14 },
			SM_MARIO_IDLE_SX{ 0 },
			SM_MARIO_IDLE_SY{ 0 },
			SM_MARIO_WALK_SX{ 30 },
			SM_MARIO_WALK_SY{ 0 },
			SM_MARIO_JUMP_SX{ 120 },
			SM_MARIO_JUMP_SY{ 0 },
			SM_MARIO_FALL_SX{ 150 },
			SM_MARIO_FALL_SY{ 0 },
			SM_MARIO_RUN1_SX{ 180 },
			SM_MARIO_RUN1_SY{ 0 },
			SM_MARIO_RUN2_SX{ 210 },
			SM_MARIO_RUN2_SY{ 0 },
			SM_MARIO_CROUCH_SX{ 240 },
			SM_MARIO_CROUCH_SY{ 0 },
			SM_MARIO_LOOKUP_SX{ 270 },
			SM_MARIO_LOOKUP_SY{ 0 },
			SM_MARIO_RUNJUMP_SX{ 60 },
			SM_MARIO_RUNJUMP_SY{ 30 },
			SM_MARIO_DEAD_SX{ 90 },
			SM_MARIO_DEAD_SY{ 30 },
			PLAYER_START_POSX{ TILE_SIZE }, 
			PLAYER_START_POSY{ SCREEN_H - TILE_SIZE - SM_MARIO_IMG_H },
			WALKING_REFRESH_RATE{ 10 }, 
			RUNNING_REFRESH_RATE{ 2 },
			DMG_FLASH_INTERVAL{ 5 },
			IFRAMES{ 180 },
			DEFAULT_LIVES{ 5 };

const double	MAX_WALK_VELOCITY{ 1.25 }, 
				MAX_RUN_VELOCITY{ 2.25 }, 
				MAX_SPRINT_VELOCITY{ 3.0 }, 
				MIN_VELOCITY{ 1.0 },	
				ACCELERATION{ 0.09375 }, 
				STOPPING_DECELERATION{ 0.0625 },
				SKID_DECELERATION_WALK{ 0.15625 }, 
				SKID_DECELERATION_RUN{ 0.3125 }, 
				//GRAVITY_WITH_B{ 0.1875 }, 
				GRAVITY_WITH_B{ 0.15 },
				GRAVITY_WITHOUT_B{ 0.375 }, 
				MAX_FALLING_VELOCITY{ 4.0 },
				AIR_ACCELERATION_FWD{ 0.09375 }, 
				AIR_ACCELERATION_BWD{ 0.15625 }, 
				AIR_ACCELERATION_RUN_AND_BWD{ 0.3125 }, 
				JUMP_VELOCITY{ -3.8125 };

class Player
{
private:
	bool is_facing_right, alternate_anim, is_pressing_jump, do_not_draw;
	double last_time, last_posy;
	int current_state, anim_refresh_rate, current_anim_frame, current_iframe, current_dmg_frame, hp, lives;
	ALLEGRO_BITMAP *player_image;
	ALLEGRO_SAMPLE *jump_sound, *dead_sound, *damage_sound;
public:
	bool can_jump, on_platform, isDead, isInvincible, isGameOver;
	int direction, v_direction, h_direction, box_x1, box_x2, box_y1, box_y2, pos_x, pos_y, last_box_x1, last_box_x2, last_box_y1, last_box_y2,
		bottom_center_pixel_x, bottom_center_pixel_y, center_pixel_y, current_vertical_state;
	double velocity_x, velocity_y, acceleration_x, acceleration_y;
	Player();
	~Player();
	void update();
	void updateBox();
	void draw();
	void move(bool run, int direction);
	//void setPositionX();
	//void setPositionY();
	void refreshAnimation();
	void setAnimRefreshRate(int new_rate);
	void idle();
	void jump();
	//void gravity(double timer_count, double dxtime);
	void isFalling();
	void updateLastPos();
	void updateVelocity();
	void updatePosition();
	void setPositionByBoxX1(int new_box_x1);
	void setPositionByBoxX2(int new_box_x2);
	void setPositionByBoxY1(int new_box_y1);
	void setPositionByBoxY2(int new_box_y2);
	void checkIfOOB();
	void setPositionX(int new_pos_x);
	void setPositionY(int new_pos_y);
	void notPressingJump();
	int getTileX();
	int getTileY();
	std::vector<int> getCheckTiles();
	void damage();
	void die();
	void drawBox();
	void refreshInvincibility();
	void upwardsBoost();
	void refreshDamageAnimation();
	void initializeForLevel();
	void initializeForLevel(int startPositionX, int startPositionY);
	void reinitialize();
	int getLives();
};

#endif
