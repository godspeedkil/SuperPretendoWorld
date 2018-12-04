#include "Main.h"
#include "Player.h"
#include <iostream>

Player::Player() :
	pos_x{ PLAYER_START_POSX }, pos_y{ PLAYER_START_POSY }, is_facing_right{ true }, current_state{ IDLE }, current_vertical_state{ PLATFORM },
	player_image{ al_load_bitmap("mario.png") }, jump_sound{ al_load_sample("jump.wav") }, dead_sound{ al_load_sample("dead.wav") },
	direction{ KEY_RIGHT }, on_platform{ true }, can_jump{ true }, hp{ 1 }, lives{ DEFAULT_LIVES }, damage_sound{ al_load_sample("damage.wav") }
{
	updateBox();
	if (!player_image || !jump_sound || !dead_sound || !damage_sound)
	{
		fprintf(stderr, "failed to load player resource!\n");
		delete this;
	}
}

Player::~Player()
{
	al_destroy_bitmap(player_image);
	al_destroy_sample(jump_sound);
	al_destroy_sample(dead_sound);
	al_destroy_sample(damage_sound);
}

void Player::update()
{
	//std::cout << "Is pressing jump: " << is_pressing_jump << std::endl;
	//gravity(timer_count, timer_count - last_time);
	//std::cout << velocity_y << std::endl;
	updatePosition();
	//drawBox();
	checkIfOOB();
	isFalling();
	refreshInvincibility();
	refreshAnimation();
	if (isInvincible)
	{
		refreshDamageAnimation();
	}
	draw();
	updateLastPos();
	updateVelocity();
	//std::cout << "hp: " << hp << std::endl;
	//std::cout << "velocity x: " << velocity_x << " velocity y: " << velocity_y << std::endl;
}

// determines whether the player is currently falling
void Player::isFalling()
{
	if (!(current_vertical_state == PLATFORM))
	{
		if (pos_y >= last_posy)
		{
			v_direction = 2;
			acceleration_y = GRAVITY_WITH_B;
			current_vertical_state = FALLING;
			can_jump = false;
		}
	}
}

// store the player's last y coordinate (used to determine if player is falling, jumping, or neither)
void Player::updateLastPos()
{
	last_posy = pos_y;
}

// update the player's current x and y velocities according to his acceleration, also handles maximum velocity limits
void Player::updateVelocity()
{
	//std::cout << velocity_x << std::endl;
	if (current_state == WALKING)
	{
		if (direction == KEY_RIGHT)
		{
			if (velocity_x + acceleration_x >= MAX_WALK_VELOCITY)
			{
				velocity_x = MAX_WALK_VELOCITY;
			}
			else
			{
				if (velocity_x < MIN_VELOCITY)
				{
					velocity_x = 1;
				}
				else
				{
					velocity_x += acceleration_x;
				}
			}
		}
		else if (direction == KEY_LEFT)
		{
			if (velocity_x + acceleration_x <= -MAX_WALK_VELOCITY)
			{
				velocity_x = -MAX_WALK_VELOCITY + 0.25;
			}
			else
			{
				if (velocity_x > -0.5)
				{
					velocity_x = -0.5;
				}
				else
				{
					velocity_x += acceleration_x;
				}
			}
		}
	}
	else if (current_state == RUNNING)
	{
		if (direction == KEY_RIGHT)
		{
			if (velocity_x + acceleration_x >= MAX_RUN_VELOCITY)
			{
				velocity_x = MAX_RUN_VELOCITY;
			}
			else
			{
				if (velocity_x < MIN_VELOCITY)
				{
					velocity_x = 1;
				}
				else
				{
					velocity_x += acceleration_x;
				}
			}
		}
		else if (direction == KEY_LEFT)
		{
			if (velocity_x + acceleration_x <= -MAX_RUN_VELOCITY)
			{
				velocity_x = -MAX_RUN_VELOCITY + 1;
			}
			else
			{
				if (velocity_x > -0.2)
				{
					velocity_x = -0.2;
				}
				else
				{
					velocity_x += acceleration_x;
				}
			}
		}
	}
	else if (current_state == SPRINTING)
	{
		if (velocity_x + acceleration_x >= MAX_SPRINT_VELOCITY)
		{
			velocity_x = MAX_SPRINT_VELOCITY;
		}
		else
		{
			velocity_x += acceleration_x;
		}
	}
	else if (current_state == DECELERATING_HORIZONTAL || current_state == CROUCHING || current_state == LOOKING_UP)
	{
		if (direction == KEY_RIGHT)
		{
			if (velocity_x - STOPPING_DECELERATION <= 0)
			{
				velocity_x = 0;
				current_state = IDLE;
			}
			else
			{
				velocity_x -= STOPPING_DECELERATION;
			}
		}
		else if (direction == KEY_LEFT)
		{
			if (velocity_x + STOPPING_DECELERATION >= 0)
			{
				velocity_x = 0;
				current_state = IDLE;
			}
			else
			{
				velocity_x += STOPPING_DECELERATION;
			}
		}
	}
	if (v_direction == 2)
	{
		if (velocity_y + acceleration_y >= MAX_FALLING_VELOCITY)
		{
			velocity_y = MAX_FALLING_VELOCITY;
		}
		else
		{
			velocity_y += acceleration_y;
		}
	}
	else if (v_direction == 1)
	{
		if (is_pressing_jump)
		{
			velocity_y += GRAVITY_WITH_B;
		}
		else
		{
			velocity_y += GRAVITY_WITHOUT_B;
		}
	}
}

// called by key press, sets the values for player movement
void Player::move(bool run, int direction)
{
	if (direction == KEY_RIGHT)
	{
		if (!run)
		{
			is_facing_right = true;
			this->direction = KEY_RIGHT;
			current_state = WALKING;
			anim_refresh_rate = WALKING_REFRESH_RATE;
			acceleration_x = ACCELERATION;
		}
		else if (run)
		{
			is_facing_right = true;
			this->direction = KEY_RIGHT;
			current_state = RUNNING;
			anim_refresh_rate = RUNNING_REFRESH_RATE;
			acceleration_x = ACCELERATION;
		}
	}
	else if (direction == KEY_LEFT)
	{
		if (!run)
		{
			is_facing_right = false;
			this->direction = KEY_LEFT;
			current_state = WALKING;
			anim_refresh_rate = WALKING_REFRESH_RATE;
			acceleration_x = -ACCELERATION;
		}
		else if (run)
		{
			is_facing_right = false;
			this->direction = KEY_LEFT;
			current_state = RUNNING;
			anim_refresh_rate = RUNNING_REFRESH_RATE;
			acceleration_x = -ACCELERATION;
		}
	}	
	else if (direction == KEY_DOWN)
	{
		if (current_vertical_state == PLATFORM)
		{
			current_state = CROUCHING;
		}
	}
	else if (direction == KEY_UP)
	{
		if (current_vertical_state == PLATFORM)
		{
			current_state = LOOKING_UP;
		}
	}
}

// move the player according to his current velocity, first steps x, then y, finally updates the collision box
void Player::updatePosition()
{
	/*if (!(v_direction == 2))
	{
		velocity_y = 0;
	}*/
	pos_x += velocity_x;
	pos_y += velocity_y;
	updateBox();
}

// handles the player's jump routine
void Player::jump()
{
	is_pressing_jump = true;
	if (can_jump)
	{
		current_vertical_state = JUMPING;
		velocity_y = JUMP_VELOCITY;
		on_platform = false;
		can_jump = false;
		v_direction = 1;
		al_play_sample(jump_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	}
}

/*void Player::gravity(double timer_count, double dxtime)
{
	if (!on_platform)
	{
		velocity_y += GRAVITY * dxtime / 200;
	}
	else {
		last_time = timer_count;
	}
}*/

// update the values of the player's collision box coordinates according to his current position
void Player::updateBox()
{
	last_box_x1 = box_x1;
	last_box_x2 = box_x2;
	last_box_y1 = box_y1;
	last_box_y2 = box_y2;
	box_x1 = pos_x + SM_MARIO_XTO_BOX;
	box_x2 = pos_x + SM_MARIO_XTO_BOX + TILE_SIZE;
	box_y1 = pos_y + SM_MARIO_YTO_BOX + 1;
	box_y2 = pos_y + SM_MARIO_YTO_BOX + TILE_SIZE;
	bottom_center_pixel_x = static_cast<int>((box_x1 + box_x2) / 2);
	bottom_center_pixel_y = box_y2;
	center_pixel_y = (box_y1 + box_y2) / 2;
}

// handles the player's sprite, rendering according to his actions
void Player::draw()
{
	if (!isDead)
	{
		if (is_facing_right)
		{
			if (!do_not_draw)
			{
				if (v_direction == 1 && current_vertical_state != PLATFORM)
				{
					al_draw_bitmap_region(player_image, SM_MARIO_JUMP_SX, SM_MARIO_JUMP_SY, SM_MARIO_IMG_W, SM_MARIO_IMG_H, pos_x, pos_y, 0);
				}
				else if (v_direction == 2 && current_vertical_state != PLATFORM)
				{
					al_draw_bitmap_region(player_image, SM_MARIO_FALL_SX, SM_MARIO_FALL_SY, SM_MARIO_IMG_W, SM_MARIO_IMG_H, pos_x, pos_y, 0);
				}
				else
				{
					if (current_state == IDLE)
					{
						al_draw_bitmap_region(player_image, SM_MARIO_IDLE_SX, SM_MARIO_IDLE_SY, SM_MARIO_IMG_W, SM_MARIO_IMG_H, pos_x, pos_y, 0);
					}
					else if (current_state == WALKING || current_state == DECELERATING_HORIZONTAL)
					{
						if (alternate_anim)
						{
							al_draw_bitmap_region(player_image, SM_MARIO_IDLE_SX, SM_MARIO_IDLE_SY, SM_MARIO_IMG_W, SM_MARIO_IMG_H, pos_x, pos_y, 0);
						}
						else
						{
							al_draw_bitmap_region(player_image, SM_MARIO_WALK_SX, SM_MARIO_WALK_SY, SM_MARIO_IMG_W, SM_MARIO_IMG_H, pos_x, pos_y, 0);
						}
					}
					else if (current_state == RUNNING)
					{
						if (alternate_anim)
						{
							al_draw_bitmap_region(player_image, SM_MARIO_RUN1_SX, SM_MARIO_RUN1_SY, SM_MARIO_IMG_W, SM_MARIO_IMG_H, pos_x, pos_y, 0);
						}
						else
						{
							al_draw_bitmap_region(player_image, SM_MARIO_RUN2_SX, SM_MARIO_RUN2_SY, SM_MARIO_IMG_W, SM_MARIO_IMG_H, pos_x, pos_y, 0);
						}
					}
					else if (current_state == CROUCHING)
					{
						al_draw_bitmap_region(player_image, SM_MARIO_CROUCH_SX, SM_MARIO_CROUCH_SY, SM_MARIO_IMG_W, SM_MARIO_IMG_H, pos_x, pos_y + 1, 0);
					}
					else if (current_state == LOOKING_UP)
					{
						al_draw_bitmap_region(player_image, SM_MARIO_LOOKUP_SX, SM_MARIO_LOOKUP_SY, SM_MARIO_IMG_W, SM_MARIO_IMG_H, pos_x, pos_y + 1, 0);
					}
					else if (current_state == RUNNING_JUMPING)
					{
						al_draw_bitmap_region(player_image, SM_MARIO_RUNJUMP_SX, SM_MARIO_RUNJUMP_SY, SM_MARIO_IMG_W, SM_MARIO_IMG_H, pos_x, pos_y, 0);
					}
				}
			}
		}
		else
		{
			if (!do_not_draw)
			{
				if (v_direction == 1 && current_vertical_state != PLATFORM)
				{
					al_draw_bitmap_region(player_image, SM_MARIO_JUMP_SX, SM_MARIO_JUMP_SY, SM_MARIO_IMG_W, SM_MARIO_IMG_H, pos_x, pos_y, ALLEGRO_FLIP_HORIZONTAL);
				}
				else if (v_direction == 2 && current_vertical_state != PLATFORM)
				{
					al_draw_bitmap_region(player_image, SM_MARIO_FALL_SX, SM_MARIO_FALL_SY, SM_MARIO_IMG_W, SM_MARIO_IMG_H, pos_x, pos_y, ALLEGRO_FLIP_HORIZONTAL);
				}
				else
				{
					if (current_state == IDLE)
					{
						al_draw_bitmap_region(player_image, SM_MARIO_IDLE_SX, SM_MARIO_IDLE_SY, SM_MARIO_IMG_W, SM_MARIO_IMG_H, pos_x, pos_y, ALLEGRO_FLIP_HORIZONTAL);
					}
					else if (current_state == WALKING || current_state == DECELERATING_HORIZONTAL)
					{
						if (alternate_anim)
						{
							al_draw_bitmap_region(player_image, SM_MARIO_IDLE_SX, SM_MARIO_IDLE_SY, SM_MARIO_IMG_W, SM_MARIO_IMG_H, pos_x, pos_y, ALLEGRO_FLIP_HORIZONTAL);
						}
						else
						{
							al_draw_bitmap_region(player_image, SM_MARIO_WALK_SX, SM_MARIO_WALK_SY, SM_MARIO_IMG_W, SM_MARIO_IMG_H, pos_x, pos_y, ALLEGRO_FLIP_HORIZONTAL);
						}
					}
					else if (current_state == RUNNING)
					{
						if (alternate_anim)
						{
							al_draw_bitmap_region(player_image, SM_MARIO_RUN1_SX, SM_MARIO_RUN1_SY, SM_MARIO_IMG_W, SM_MARIO_IMG_H, pos_x, pos_y, ALLEGRO_FLIP_HORIZONTAL);
						}
						else
						{
							al_draw_bitmap_region(player_image, SM_MARIO_RUN2_SX, SM_MARIO_RUN2_SY, SM_MARIO_IMG_W, SM_MARIO_IMG_H, pos_x, pos_y, ALLEGRO_FLIP_HORIZONTAL);
						}
					}
					else if (current_state == CROUCHING)
					{
						al_draw_bitmap_region(player_image, SM_MARIO_CROUCH_SX, SM_MARIO_CROUCH_SY, SM_MARIO_IMG_W, SM_MARIO_IMG_H, pos_x, pos_y + 1, ALLEGRO_FLIP_HORIZONTAL);
					}
					else if (current_state == LOOKING_UP)
					{
						al_draw_bitmap_region(player_image, SM_MARIO_LOOKUP_SX, SM_MARIO_LOOKUP_SY, SM_MARIO_IMG_W, SM_MARIO_IMG_H, pos_x, pos_y + 1, ALLEGRO_FLIP_HORIZONTAL);
					}
					else if (current_state == RUNNING_JUMPING)
					{
						al_draw_bitmap_region(player_image, SM_MARIO_RUNJUMP_SX, SM_MARIO_RUNJUMP_SY, SM_MARIO_IMG_W, SM_MARIO_IMG_H, pos_x, pos_y, ALLEGRO_FLIP_HORIZONTAL);
					}
				}
			}
		}
	}
	else
	{
		al_draw_bitmap_region(player_image, SM_MARIO_DEAD_SX, SM_MARIO_DEAD_SY, SM_MARIO_IMG_W, SM_MARIO_IMG_H, pos_x, pos_y, 0);
	}
}

// set a new refresh rate for animations
void Player::setAnimRefreshRate(int new_rate)
{
	anim_refresh_rate = new_rate;
}

// cycle the player's animation rate to alternate between animations
void Player::refreshAnimation()
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

// sets player state to idle
void Player::idle()
{
	if (velocity_x == 0 && velocity_y == 0)
	{
		current_state = IDLE;
	}
	else
	{
		current_state = DECELERATING_HORIZONTAL;
	}
}

// move the player's position according to his x1 collision box coordinate
void Player::setPositionByBoxX1(int new_box_x1)
{
	pos_x = new_box_x1 - SM_MARIO_XTO_BOX;
	updateBox();
}

// move the player's position according to his x2 collision box coordinate
void Player::setPositionByBoxX2(int new_box_x2)
{
	pos_x = new_box_x2 - TILE_SIZE - SM_MARIO_XTO_BOX;
	updateBox();
}

// move the player's position according to his y1 collision box coordinate
void Player::setPositionByBoxY1(int new_box_y1)
{
	pos_y = new_box_y1 - SM_MARIO_YTO_BOX;
	updateBox();
}

// move the player's position according to his y2 collision box coordinate
void Player::setPositionByBoxY2(int new_box_y2)
{
	pos_y = new_box_y2 - TILE_SIZE - SM_MARIO_YTO_BOX;
	updateBox();
}

// check and prevent the player from going out of bounds
void Player::checkIfOOB()
{
	if (pos_x < 0 - SM_MARIO_XTO_BOX)
	{
		pos_x = 0 - SM_MARIO_XTO_BOX;
		velocity_x = 0;
		updateBox();
	}
	else if (pos_x > SCREEN_W - SM_MARIO_IMG_W + SM_MARIO_XTO_BOX)
	{
		pos_x = SCREEN_W - SM_MARIO_IMG_W + SM_MARIO_XTO_BOX;
		updateBox();
	}
	else if (pos_y > SCREEN_H)
	{
		die();
	}
	else if (pos_y < 0)
	{
		pos_y = 0;
		velocity_y = 0;
		isFalling();
		updateBox();
	}
}

// move the player's position according to his x coordinate
void Player::setPositionX(int new_pos_x)
{
	pos_x = new_pos_x;
	updateBox();
}

// move the player's position according to his y coordinate
void Player::setPositionY(int new_pos_y)
{
	pos_y = new_pos_y;
	updateBox();
}

// asserts the player is not pressing the jump key
void Player::notPressingJump()
{
	is_pressing_jump = false;
}

// return a vector with the tiles surrounding the player (for enemy collision purposes)
std::vector<int> Player::getCheckTiles()
{
	std::vector<int> check_tiles;
	for (int i = -1;i <= 1;i++) {
		for (int j = -1;j <= 1;j++) {
			if (!(getTileX() + i < 0) && !(getTileY() + j < 0) && !(getTileX() + i > (SCREEN_W / TILE_SIZE)) && !(getTileY() + j > (SCREEN_H / TILE_SIZE))) {
				check_tiles.push_back(getTileX() + i);
				check_tiles.push_back(getTileY() + j);
			}
		}
	}
	/*for (int i = 0; i < (check_tiles.size() / 2); i++)
	{
		std::cout << check_tiles.at(2 * i) << ", " << check_tiles.at(2 * i + 1) << std::endl;
	}*/
	return check_tiles;
}

// return the player's current tile column (x-axis)
int Player::getTileX()
{
	return bottom_center_pixel_x / TILE_SIZE;
}

// return the player's current tile row (y-axis)
int Player::getTileY()
{
	return center_pixel_y / TILE_SIZE;
}

// damages player, only if player is not currently invincible, if hp reaches 0, kill the player
void Player::damage()
{
	if (!isInvincible)
	{
		if (hp > 1)
		{
			al_play_sample(damage_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			--hp;
			isInvincible = true;
			current_iframe = 0;
			current_dmg_frame = 0;
		}
		if (hp == 1 && !isInvincible)
		{
			isInvincible = true;
			die();
		}
	}
}

// handles the player's death, setting the game over flag to true if they run out of lives
void Player::die()
{
	al_play_sample(dead_sound, 2.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	isDead = true;
	if (lives > 1)
	{
		lives--;
	}
	else
	{
		isGameOver = true;
	}
}

// for collision box checking (debugging)
void Player::drawBox()
{
	al_draw_line(box_x1, box_y1, box_x2, box_y1, al_map_rgb(0, 0, 0), 1.0f);
	al_draw_line(box_x1, box_y1, box_x1, box_y2, al_map_rgb(0, 0, 0), 1.0f);
	al_draw_line(box_x2, box_y1, box_x2, box_y2, al_map_rgb(0, 0, 0), 1.0f);
	al_draw_line(box_x1, box_y2, box_x2, box_y2, al_map_rgb(0, 0, 0), 1.0f);
}

// refreshes invincibility state based on variable 'iframe_refresh_rate'
void Player::refreshInvincibility()
{
	if (current_iframe < IFRAMES)
	{
		current_iframe++;
	}
	else
	{
		current_iframe = 0;
		isInvincible = false;
		do_not_draw = false;
	}
}
 
// gives player an upwards speed boost (used for bouncing on things/enemies)
void Player::upwardsBoost()
{
	if (!is_pressing_jump)
	{
		current_vertical_state = JUMPING;
		velocity_y = JUMP_VELOCITY + 1;
		on_platform = false;
		can_jump = false;
		v_direction = 1;
	}
	else
	{
		current_vertical_state = JUMPING;
		velocity_y = JUMP_VELOCITY - 1;
		on_platform = false;
		can_jump = false;
		v_direction = 1;
	}
}

// refreshes flashing animation when damaged
void Player::refreshDamageAnimation()
{
	if (current_dmg_frame < DMG_FLASH_INTERVAL)
	{
		current_dmg_frame++;
	}
	else
	{
		current_dmg_frame = 0;
		do_not_draw = !do_not_draw;
	}
}

// resets player values for start of new level/loss of a life
void Player::initializeForLevel()
{
	is_facing_right = true;
	is_pressing_jump = false;
	do_not_draw = false;
	current_state = IDLE;
	current_vertical_state = PLATFORM;
	hp = 1;
	pos_x = PLAYER_START_POSX;
	pos_y = PLAYER_START_POSY;
	last_posy = pos_y;
	direction = KEY_RIGHT;
	on_platform = true;
	can_jump = true;
	isDead = false;
	isInvincible = false;
	updateBox();
	velocity_x = velocity_y = acceleration_x = acceleration_y = v_direction = h_direction = current_iframe = current_dmg_frame = 0;
}

// specifies a spawn position
void Player::initializeForLevel(int startPositionX, int startPositionY)
{
	is_facing_right = true;
	is_pressing_jump = false;
	do_not_draw = false;
	current_state = IDLE;
	current_vertical_state = PLATFORM;
	hp = 1;
	pos_x = startPositionX;
	pos_y = startPositionY;
	last_posy = pos_y;
	direction = KEY_RIGHT;
	on_platform = true;
	can_jump = true;
	isDead = false;
	isInvincible = false;
	updateBox();
	velocity_x = velocity_y = acceleration_x = acceleration_y = v_direction = h_direction = current_iframe = current_dmg_frame = 0;
}

// resets player values for start of new game
void Player::reinitialize()
{
	is_facing_right = true;
	is_pressing_jump = false;
	do_not_draw = false;
	current_state = IDLE;
	current_vertical_state = PLATFORM;
	hp = 1;
	pos_x = PLAYER_START_POSX;
	pos_y = PLAYER_START_POSY;
	last_posy = pos_y;
	direction = KEY_RIGHT;
	on_platform = true;
	can_jump = true;
	isDead = false;
	isInvincible = false;
	updateBox();
	velocity_x = velocity_y = acceleration_x = acceleration_y = v_direction = h_direction = current_iframe = current_dmg_frame = 0;
	lives = 5;
	isGameOver = false;
}

// return number of lives left
int Player::getLives()
{
	return lives;
}