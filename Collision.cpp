#include "Collision.h"
#include <algorithm>

Collision::Collision() :
	loadCounterX{ 0 }, loadCounterY{ 0 }
{
	loadCollisionMap("level1_collision.txt");
}

Collision::~Collision()
{

}

void Collision::update(Player &player)
{
	platformCollision(player);
}

void Collision::update(Enemy &enemy)
{
	platformCollision(enemy);
}

// open collision map file to extract values to our collMapFile 2d array
void Collision::loadCollisionMap(const char *filename)
{
	std::ifstream openfile(filename);
	if (openfile.is_open())
	{
		openfile >> currentLevel >> mapSizeX >> mapSizeY;
		while (!openfile.eof())
		{
			openfile >> collMapFile[loadCounterX][loadCounterY];
			loadCounterX++;
			if (loadCounterX >= mapSizeX)
			{
				loadCounterX = 0;
				loadCounterY++;
			}
		}
		loadCounterX = loadCounterY = 0;
	}
	else
	{
		fprintf(stderr, "unable to load collision map!\n");
	}
}

// handles player-world collision by stepping x-axis first, then y-axis
void Collision::platformCollision(Player &player)
{
	for (int i = 0;i < mapSizeX;i++)
	{
		for (int j = 0;j < mapSizeY;j++)
		{
			if (collMapFile[i][j] == 1)
			{
				if ((player.box_x1 + player.velocity_x) >= i*TILE_SIZE + TILE_SIZE || (player.box_y1 + player.velocity_y) >= j*TILE_SIZE + TILE_SIZE || 
					(player.box_x2 + player.velocity_x) <= i*TILE_SIZE || (player.box_y2 + player.velocity_y) <= j*TILE_SIZE)
				{
					/*if (player.direction == KEY_RIGHT)
					{*/
						if (collMapFile[(player.box_x1 + 1) / TILE_SIZE][(player.box_y2 / TILE_SIZE)] == 0
							&& collMapFile[(player.box_x2 - 1) / TILE_SIZE][(player.box_y2 / TILE_SIZE)] == 0)
						{
							player.on_platform = false;
							player.current_vertical_state = FALLING;
						}
					//}
					/*else if (player.direction == KEY_LEFT)
					{
						if (collMapFile[(player.box_x2 - 1) / TILE_SIZE][(player.box_y2 / TILE_SIZE)] == 0)
						{
							player.on_platform = false;
							player.current_vertical_state = FALLING;
						}
					}*/
				}
				else
				{
					// stepping the x-axis, depending on which direction the character is moving towards
					/*if (player.direction == KEY_RIGHT)
					{*/
						if (((j * TILE_SIZE < player.box_y2) && !(j * TILE_SIZE < player.box_y1)) || 
							((j * TILE_SIZE + TILE_SIZE > player.box_y1) && !(j * TILE_SIZE + TILE_SIZE > player.box_y2)))
						{
							if ((player.box_x2 + player.velocity_x >= i*TILE_SIZE) &&
								!(player.box_x2 + player.velocity_x >= (i + 1)*TILE_SIZE))
							{
								player.setPositionByBoxX2(std::fmin((i * TILE_SIZE), (player.box_x2 + player.velocity_x)));
								player.velocity_x = 0;
							}
						}
					/*}
					else if (player.direction == KEY_LEFT)
					{*/
						if (((j * TILE_SIZE < player.box_y2) && !(j * TILE_SIZE < player.box_y1)) ||
							((j * TILE_SIZE + TILE_SIZE > player.box_y1) && !(j * TILE_SIZE + TILE_SIZE > player.box_y2)))
						{
							if ((player.box_x1 + player.velocity_x <= i*TILE_SIZE + TILE_SIZE) &&
								!(player.box_x1 + player.velocity_x <= (i - 1)*TILE_SIZE + TILE_SIZE))
							{
								player.setPositionByBoxX1(std::fmax((i * TILE_SIZE + TILE_SIZE), (player.box_x1 + player.velocity_x)));
								player.velocity_x = 0;
							}
						}
					//}
					// stepping the y-axis, depending on whether the player is jumping or falling (v_direction 1 = jumping, 2 = falling)
					if (player.v_direction == 2)
					{
						if (((i * TILE_SIZE < player.box_x2) && !(i * TILE_SIZE < player.box_x1)) ||
							((i * TILE_SIZE + TILE_SIZE > player.box_x1) && !(i * TILE_SIZE + TILE_SIZE > player.box_x2)))
						{
							if (player.box_y2 + player.velocity_y >= j*TILE_SIZE)
							{
								player.velocity_y = 0;
								player.on_platform = true;
								player.current_vertical_state = PLATFORM;
								player.setPositionByBoxY2(j*TILE_SIZE);
								player.can_jump = true;
							}
						}
					}
					else if (player.v_direction == 1)
					{
						if (((i * TILE_SIZE < player.box_x2) && !(i * TILE_SIZE < player.box_x1)) ||
							((i * TILE_SIZE + TILE_SIZE > player.box_x1) && !(i * TILE_SIZE + TILE_SIZE > player.box_x2)))
						{
							if (player.box_y2 > j * TILE_SIZE + TILE_SIZE)
							{
								if (player.box_y1 + player.velocity_y < j*TILE_SIZE + TILE_SIZE)
								{
									//std::cout << "Jumping collision" << std::endl;
									player.velocity_y = 0;
									//player.velocity_x = 0;
									player.setPositionByBoxY1(j*TILE_SIZE + TILE_SIZE);
									player.on_platform = false;
									player.current_vertical_state = FALLING;
									player.v_direction = 2;
								}
							}
						}
					}
				}
			}
			else if (collMapFile[i][j] == 2)
			{
				if (player.box_y2 <= j * TILE_SIZE)
				{
					if ((player.box_y2 + player.velocity_y) < j * TILE_SIZE)
					{
						if (collMapFile[(player.box_x1 + 3) / TILE_SIZE][(player.box_y2 / TILE_SIZE)] == 0
							&& collMapFile[(player.box_x2 - 3) / TILE_SIZE][(player.box_y2 / TILE_SIZE)] == 0)
						{
							player.on_platform = false;
							player.current_vertical_state = FALLING;
						}
					}
					if (((i * TILE_SIZE < player.box_x2) && !(i * TILE_SIZE < player.box_x1)) ||
						((i * TILE_SIZE + TILE_SIZE > player.box_x1) && !(i * TILE_SIZE + TILE_SIZE > player.box_x2)))
					{
						if (player.box_y2 + player.velocity_y > j*TILE_SIZE)
						{
							player.velocity_y = 0;
							player.on_platform = true;
							player.current_vertical_state = PLATFORM;
							player.setPositionByBoxY2(j*TILE_SIZE);
							player.can_jump = true;
						}
					}
				}
			}
		}
	}
}

// handles enemy-world collision by stepping x-axis; also tells enemy to turn around when it reaches an edge
void Collision::platformCollision(Enemy &enemy)
{
	for (int i = 0;i < mapSizeX;i++)
	{
		for (int j = 0;j < mapSizeY;j++)
		{
			if (collMapFile[i][j] == 1 || collMapFile[i][j] == 2)
			{
				if ((enemy.box_x1 + enemy.velocity_x) >= i*TILE_SIZE + TILE_SIZE || (enemy.box_y1 + enemy.velocity_y) >= j*TILE_SIZE + TILE_SIZE ||
					(enemy.box_x2 + enemy.velocity_x) <= i*TILE_SIZE || (enemy.box_y2 + enemy.velocity_y) <= j*TILE_SIZE)
				{
					/*enemy.on_platform = false;
					if (collMapFile[enemy.bottom_center_pixel_x / TILE_SIZE][(enemy.bottom_center_pixel_y / TILE_SIZE) + 2] == 0)
					{
					enemy.current_vertical_state = FALLING;
					}*/
				}
				else
				{
					// stepping the x-axis, depending on which direction the character is moving towards
					if (enemy.direction == KEY_RIGHT)
					{
						if (((j * TILE_SIZE < enemy.box_y2) && !(j * TILE_SIZE < enemy.box_y1)) ||
							((j * TILE_SIZE + TILE_SIZE > enemy.box_y1) && !(j * TILE_SIZE + TILE_SIZE > enemy.box_y2)))
						{
							if ((enemy.box_x2 + enemy.velocity_x >= i*TILE_SIZE) &&
								!(enemy.box_x2 + enemy.velocity_x >= (i + 1)*TILE_SIZE))
							{
								enemy.setPositionByBoxX2(std::fmin((i * TILE_SIZE), (enemy.box_x2 + enemy.velocity_x)));
								enemy.velocity_x = 0;
								enemy.turnAround();
							}
						}
					}
					else if (enemy.direction == KEY_LEFT)
					{
						if (((j * TILE_SIZE < enemy.box_y2) && !(j * TILE_SIZE < enemy.box_y1)) ||
							((j * TILE_SIZE + TILE_SIZE > enemy.box_y1) && !(j * TILE_SIZE + TILE_SIZE > enemy.box_y2)))
						{
							if ((enemy.box_x1 + enemy.velocity_x <= i*TILE_SIZE + TILE_SIZE) &&
								!(enemy.box_x1 + enemy.velocity_x <= (i - 1)*TILE_SIZE + TILE_SIZE))
							{
								enemy.setPositionByBoxX1(std::fmax((i * TILE_SIZE + TILE_SIZE), (enemy.box_x1 + enemy.velocity_x)));
								enemy.velocity_x = 0;
								enemy.turnAround();
							}
						}
					}
					// stepping the y-axis, depending on whether the enemy is jumping or falling (v_direction 1 = jumping, 2 = falling)
					/*if (enemy.v_direction == 2)
					{
					if (((i * TILE_SIZE < enemy.box_x2) && !(i * TILE_SIZE < enemy.box_x1)) ||
					((i * TILE_SIZE + TILE_SIZE > enemy.box_x1) && !(i * TILE_SIZE + TILE_SIZE > enemy.box_x2)))
					{
					if (enemy.box_y2 + enemy.velocity_y >= j*TILE_SIZE)
					{
					enemy.velocity_y = 0;
					//enemy.on_platform = true;
					//enemy.current_vertical_state = PLATFORM;
					enemy.setPositionByBoxY2(j*TILE_SIZE);
					//enemy.can_jump = true;
					}
					}
					}
					else if (enemy.v_direction == 1)
					{
					if (((i * TILE_SIZE < enemy.box_x2) && !(i * TILE_SIZE < enemy.box_x1)) ||
					((i * TILE_SIZE + TILE_SIZE > enemy.box_x1) && !(i * TILE_SIZE + TILE_SIZE > enemy.box_x2)))
					{
					if (enemy.box_y1 + enemy.velocity_y <= j*TILE_SIZE + TILE_SIZE)
					{
					enemy.velocity_y = 0;
					enemy.setPositionByBoxY1(j*TILE_SIZE + TILE_SIZE + 1);
					//enemy.on_platform = false;
					//enemy.current_vertical_state = FALLING;
					enemy.v_direction = 2;
					}
					}
					}*/
				}
			}
			if (enemy.direction == KEY_RIGHT)
			{
				if (collMapFile[(enemy.box_x2 + 4) / TILE_SIZE][(enemy.box_y2 + 1) / TILE_SIZE] == 0)
				{
					enemy.turnAround();
				}
			}
			else if (enemy.direction == KEY_LEFT)
			{
				if (collMapFile[(enemy.box_x1 - 4) / TILE_SIZE][(enemy.box_y2 + 1) / TILE_SIZE] == 0)
				{
					enemy.turnAround();
				}
			}
		}
	}
}

// handles player-enemy collision and resolution, only checking within the player's immediate vicinity (3 x 3 tile square [48 x 48 pixels])
void Collision::playerToEnemyCollision(Player &player, std::vector<Enemy*> enemy_list)
{
	std::vector<int> checkTiles{ player.getCheckTiles() };
	std::vector<Enemy*> enemiesToCheck;
	for (int i = 0; i < (checkTiles.size() / 2); i++)
	{
		for (Enemy *enemy : enemy_list)
		{
			if ((enemy->getTileX() == checkTiles.at(2 * i)) && (enemy->getTileY() == checkTiles.at(2 * i + 1)))
			{
				enemiesToCheck.push_back(enemy);
			}
		}
	}
	//std::cout << "Enemies to check: " << enemiesToCheck.size() << std::endl;
	for (Enemy *enemy : enemiesToCheck)
	{
		if (!enemy->isDead)
		{
			if (boxesOverlapping(player, *enemy))
			{
				player.damage();
				//player.velocity_x = 0;
			}
			else if (((player.box_x1 >= enemy->box_x1 - 1) && (player.box_x1 <= enemy->box_x2 + 1)) ||
				((player.box_x2 >= enemy->box_x1 - 1) && (player.box_x2 <= enemy->box_x2 + 1)) ||
				(player.bottom_center_pixel_x == enemy->bottom_center_pixel_x))
			{
				if (player.box_y2 < enemy->box_y1)
				{
					if (player.box_y2 + player.velocity_y >= enemy->box_y1)
					{
						player.setPositionByBoxY2(enemy->box_y1 - 1);
						player.upwardsBoost();
						enemy->die();
					}
				}
			}
		}
	}
}

// checks whether player and enemy collision boxes are overlapping, returning the result
bool Collision::boxesOverlapping(Player &player, Enemy &enemy)
{
	//std::cout << "Player box_x1: " << player.box_x1 << 
	if (((player.box_x1 >= enemy.box_x1) && (player.box_x1 <= enemy.box_x2)) && ((player.box_y1 >= enemy.box_y1) && (player.box_y1 <= enemy.box_y2)))
	{
		return true;
	}
	else if (((player.box_x1 >= enemy.box_x1) && (player.box_x1 <= enemy.box_x2)) && ((player.box_y2 >= enemy.box_y1) && (player.box_y2 <= enemy.box_y2)))
	{
		return true;
	}
	else if (((player.box_x2 >= enemy.box_x1) && (player.box_x2 <= enemy.box_x2)) && ((player.box_y1 >= enemy.box_y1) && (player.box_y1 <= enemy.box_y2)))
	{
		return true;
	}
	else if (((player.box_x2 >= enemy.box_x1) && (player.box_x2 <= enemy.box_x2)) && ((player.box_y2 >= enemy.box_y1) && (player.box_y2 <= enemy.box_y2)))
	{
		return true;
	}
	else return false;
}