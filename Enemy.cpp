#include "Main.h"
#include "Enemy.h"

Enemy::Enemy()
{

}

Enemy::Enemy(int type, int start_tile_x, int start_tile_y, int direction, ALLEGRO_BITMAP *enemy_image, ALLEGRO_SAMPLE *enemy_stomped_sound) :
	pos_x{ start_tile_x * TILE_SIZE }, pos_y{ start_tile_y * TILE_SIZE }, type{ type }, anim_refresh_rate{ 10 }, keepOnScreen{ true }, death_buffer_rate{ 30 }
{
	this->direction = direction;
	this->enemy_image = enemy_image;
	this->enemy_stomped_sound = enemy_stomped_sound;
	if (this->direction == KEY_RIGHT)
	{
		is_facing_right = true;
	}
	updateBox();
	if (!enemy_image || !enemy_stomped_sound)
	{
		fprintf(stderr, "failed to load enemy resource!\n");
		delete this;
	}
}

Enemy::~Enemy()
{
}

void Enemy::update()
{
	if (!isDead)
	{
		changePosition();
		//AI();
		updateBox();
		//drawBox();
		checkIfOOB();
		refreshAnimation();
	}
	draw();
	if (!isDead)
	{
		updateLastPos();
		updateVelocity();
	}
	if (isDead)
	{
		waitAfterDeath();
	}
}

/*void Enemy::AI()
{
}*/

// changes position of Enemy, based on current velocity values
void Enemy::changePosition()
{
	if (!(v_direction == 2))
	{
		velocity_y = 0;
	}
	pos_x += velocity_x;
	pos_y += velocity_y;
}

// update the Enemy's collision box
void Enemy::updateBox()
{
	box_x1 = pos_x + 1;
	box_x2 = pos_x + TILE_SIZE - 1;
	box_y1 = pos_y + 4;
	box_y2 = pos_y + TILE_SIZE;
	bottom_center_pixel_x = static_cast<int>((box_x1 + box_x2) / 2);
	bottom_center_pixel_y = box_y2;
	center_pixel_y = static_cast<int>((box_y1 + box_y2) / 2);
}

// make sure the enemy is not leaving boundaries
void Enemy::checkIfOOB()
{
	if (pos_x < 0)
	{
		pos_x = 0;
		turnAround();
	}
	else if (pos_x > SCREEN_W - ENEMY_IMG_W)
	{
		pos_x = SCREEN_W - ENEMY_IMG_W;
		turnAround();
	}
}

// refresh the animation, to alternate between animations
void Enemy::refreshAnimation()
{
	if (current_anim_frame < anim_refresh_rate)
	{
		current_anim_frame++;
	}
	else
	{
		current_anim_frame = 0;
		alternate_anim = !alternate_anim;
	}
}

// draw the Enemy's sprite, based on the Enemy's current conditions
void Enemy::draw()
{
	if (!isDead)
	{
		if (is_facing_right)
		{
			if (alternate_anim)
			{
				al_draw_bitmap_region(enemy_image, ENEMY_WALK1_SX, ENEMY_WALK1_SY, ENEMY_IMG_W, ENEMY_IMG_H, pos_x, pos_y, ALLEGRO_FLIP_HORIZONTAL);
			}
			else
			{
				al_draw_bitmap_region(enemy_image, ENEMY_WALK2_SX, ENEMY_WALK2_SY, ENEMY_IMG_W, ENEMY_IMG_H, pos_x, pos_y, ALLEGRO_FLIP_HORIZONTAL);
			}
		}
		else
		{
			if (alternate_anim)
			{
				al_draw_bitmap_region(enemy_image, ENEMY_WALK1_SX, ENEMY_WALK1_SY, ENEMY_IMG_W, ENEMY_IMG_H, pos_x, pos_y, 0);
			}
			else
			{
				al_draw_bitmap_region(enemy_image, ENEMY_WALK2_SX, ENEMY_WALK2_SY, ENEMY_IMG_W, ENEMY_IMG_H, pos_x, pos_y, 0);
			}
		}
	}
	else
	{
		if (is_facing_right)
		{
			al_draw_bitmap_region(enemy_image, ENEMY_DEATH_SX, ENEMY_DEATH_SY, ENEMY_IMG_W, ENEMY_IMG_H, pos_x, pos_y, 0);
		}
		else
		{
			al_draw_bitmap_region(enemy_image, ENEMY_DEATH_SX, ENEMY_DEATH_SY, ENEMY_IMG_W, ENEMY_IMG_H, pos_x, pos_y, ALLEGRO_FLIP_HORIZONTAL);
		}
	}
}

// update the values of last position
void Enemy::updateLastPos()
{
	last_posy = pos_y;
	last_posx = pos_x;
}

// update Enemy's velocity based on their current direction
void Enemy::updateVelocity()
{
	if (direction == KEY_RIGHT)
	{
		velocity_x = MAX_VELOCITY;
	}
	else if (direction == KEY_LEFT)
	{
		velocity_x = -MAX_VELOCITY + 0.50;
	}
}

// set Enemy's position by their box_x1 value
void Enemy::setPositionByBoxX1(int new_box_x1)
{
	pos_x = new_box_x1;
	updateBox();
}

// set Enemy's position by their box_x2 value
void Enemy::setPositionByBoxX2(int new_box_x2)
{
	pos_x = new_box_x2 - TILE_SIZE;
	updateBox();
}

// set Enemy's position by their box_y1 value
void Enemy::setPositionByBoxY1(int new_box_y1)
{
	pos_y = new_box_y1;
	updateBox();
}

// set Enemy's position by their box_y2 value
void Enemy::setPositionByBoxY2(int new_box_y2)
{
	pos_y = new_box_y2 - TILE_SIZE;
	updateBox();
}

// changes Enemy's current direction on the x-axis
void Enemy::turnAround()
{
	if (direction == KEY_RIGHT)
	{
		direction = KEY_LEFT;
		is_facing_right = false;
		updateBox();
	}
	else
	{
		direction = KEY_RIGHT;
		is_facing_right = true;
		updateBox();
	}
}

// return the Enemy's current x-axis position in the form of a tile value
int Enemy::getTileX()
{
	return bottom_center_pixel_x / TILE_SIZE;
}

// return the Enemy's current y-axis position int he form of a tile value
int Enemy::getTileY()
{
	return center_pixel_y / TILE_SIZE;
}

// FOR DEBUGGING: draw the Enemy's collision box on-screen
void Enemy::drawBox()
{
	al_draw_line(box_x1, box_y1, box_x2, box_y1, al_map_rgb(0, 0, 0), 1.0f);
	al_draw_line(box_x1, box_y1, box_x1, box_y2, al_map_rgb(0, 0, 0), 1.0f);
	al_draw_line(box_x2, box_y1, box_x2, box_y2, al_map_rgb(0, 0, 0), 1.0f);
	al_draw_line(box_x1, box_y2, box_x2, box_y2, al_map_rgb(0, 0, 0), 1.0f);
}

// plays death sound, sets isDead flag to true, and sets conditions to keep enemy dead on-screen for a time
void Enemy::die()
{
	al_play_sample(enemy_stomped_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	isDead = true;
}

// buffers a bit after Enemy death, to make sure it stays on-screen a while longer
void Enemy::waitAfterDeath()
{
	if (death_buffer < death_buffer_rate)
	{
		death_buffer++;
	}
	else
	{
		keepOnScreen = false;
	}
}