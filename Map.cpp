#include "Main.h"
#include "Map.h"

Map::Map() : 
	loadCounterX{ 0 }, loadCounterY{ 0 }, tile_image{ al_load_bitmap("tiles.png") }, level_clear_sound{ al_load_sample("level_clear.wav") },
	final_level_clear{ al_load_sample("final_flag_music.wav") }
{
	if (!tile_image || !level_clear_sound)
	{
		fprintf(stderr, "Failed to load Map resource!");
		delete this;
	}
	loadMap("level1_map.txt");
	setClearFlagPosition();
}

Map::~Map()
{
	al_destroy_bitmap(tile_image);
	al_destroy_sample(level_clear_sound);
	al_destroy_sample(final_level_clear);
}

void Map::update(Player &player)
{
	draw();
	playerWithinClearFlag(player);
}

// read the mapFile array to draw the appropriate tile on-screen
void Map::draw()
{
	for (int i = 0;i < mapSizeX;i++)
	{
		for (int j = 0;j < mapSizeY;j++)
		{
			if(mapFile[i][j] == T_EMPTY)
			{
			}
			else if (mapFile[i][j] == T_PLAINS_GRASS_TOP)
			{
				al_draw_bitmap_region(tile_image, TILE_PLAINS_NO_SIDE_GRASS_SX, TILE_PLAINS_GRASS_TOP_SY, TILE_SIZE, TILE_SIZE, i*TILE_SIZE,
					j*TILE_SIZE, 0);
			}
			else if (mapFile[i][j] == T_PLAINS_NO_GRASS)
			{
				al_draw_bitmap_region(tile_image, TILE_PLAINS_NO_SIDE_GRASS_SX, TILE_PLAINS_NO_GRASS_TOPBOTTOM_SY, TILE_SIZE, TILE_SIZE, i*TILE_SIZE,
					j*TILE_SIZE, 0);
			}
			else if (mapFile[i][j] == T_PLAINS_GRASS_LEFT)
			{
				al_draw_bitmap_region(tile_image, TILE_PLAINS_GRASS_LEFT_SX, TILE_PLAINS_NO_GRASS_TOPBOTTOM_SY, TILE_SIZE, TILE_SIZE, i*TILE_SIZE,
					j*TILE_SIZE, 0);
			}
			else if (mapFile[i][j] == T_PLAINS_GRASS_RIGHT)
			{
				al_draw_bitmap_region(tile_image, TILE_PLAINS_GRASS_RIGHT_SX, TILE_PLAINS_NO_GRASS_TOPBOTTOM_SY, TILE_SIZE, TILE_SIZE, i*TILE_SIZE,
					j*TILE_SIZE, 0);
			}
			else if (mapFile[i][j] == T_PLAINS_GRASS_BOTTOM)
			{
				al_draw_bitmap_region(tile_image, TILE_PLAINS_NO_SIDE_GRASS_SX, TILE_PLAINS_GRASS_BOTTOM_SY, TILE_SIZE, TILE_SIZE, i*TILE_SIZE,
					j*TILE_SIZE, 0);
			}
			else if (mapFile[i][j] == T_PLAINS_GRASS_TOPLEFT)
			{
				al_draw_bitmap_region(tile_image, TILE_PLAINS_GRASS_LEFT_SX, TILE_PLAINS_GRASS_TOP_SY, TILE_SIZE, TILE_SIZE, i*TILE_SIZE,
					j*TILE_SIZE, 0);
			}
			else if (mapFile[i][j] == T_PLAINS_GRASS_TOPRIGHT)
			{
				al_draw_bitmap_region(tile_image, TILE_PLAINS_GRASS_RIGHT_SX, TILE_PLAINS_GRASS_TOP_SY, TILE_SIZE, TILE_SIZE, i*TILE_SIZE,
					j*TILE_SIZE, 0);
			}
			else if (mapFile[i][j] == T_PLAINS_GRASS_BOTTOMLEFT)
			{
				al_draw_bitmap_region(tile_image, TILE_PLAINS_GRASS_LEFT_SX, TILE_PLAINS_GRASS_BOTTOM_SY, TILE_SIZE, TILE_SIZE, i*TILE_SIZE,
					j*TILE_SIZE, 0);
			}
			else if (mapFile[i][j] == T_PLAINS_GRASS_BOTTOMRIGHT)
			{
				al_draw_bitmap_region(tile_image, TILE_PLAINS_GRASS_RIGHT_SX, TILE_PLAINS_GRASS_BOTTOM_SY, TILE_SIZE, TILE_SIZE, i*TILE_SIZE,
					j*TILE_SIZE, 0);
			}
			else if (mapFile[i][j] == T_PLAINS_GRASS_INNERBOTTOMRIGHT)
			{
				al_draw_bitmap_region(tile_image, TILE_PLAINS_GRASS_INNERRIGHT_SX, TILE_PLAINS_GRASS_INNERBOTTOM_SY, TILE_SIZE, TILE_SIZE, i*TILE_SIZE,
					j*TILE_SIZE, 0);
			}
			else if (mapFile[i][j] == T_PLAINS_GRASS_INNERBOTTOMLEFT)
			{
				al_draw_bitmap_region(tile_image, TILE_PLAINS_GRASS_INNERLEFT_SX, TILE_PLAINS_GRASS_INNERBOTTOM_SY, TILE_SIZE, TILE_SIZE, i*TILE_SIZE,
					j*TILE_SIZE, 0);
			}
			else if (mapFile[i][j] == T_PLAINS_GRASS_INNERTOPRIGHT)
			{
				al_draw_bitmap_region(tile_image, TILE_PLAINS_GRASS_INNERRIGHT_SX, TILE_PLAINS_GRASS_INNERTOP_SY, TILE_SIZE, TILE_SIZE, i*TILE_SIZE,
					j*TILE_SIZE, 0);
			}
			else if (mapFile[i][j] == T_PLAINS_GRASS_INNERTOPLEFT)
			{
				al_draw_bitmap_region(tile_image, TILE_PLAINS_GRASS_INNERLEFT_SX, TILE_PLAINS_GRASS_INNERTOP_SY, TILE_SIZE, TILE_SIZE, i*TILE_SIZE,
					j*TILE_SIZE, 0);
			}
			else if (mapFile[i][j] == T_MUSHROOM_PLATFORM_LEFT)
			{
				al_draw_bitmap_region(tile_image, TILE_MUSHROOM_PLATFORM_LEFT_SX, TILE_MUSHROOM_PLATFORM_LEFT_SY, TILE_SIZE, TILE_SIZE, i*TILE_SIZE,
					j*TILE_SIZE, 0);
			}
			else if (mapFile[i][j] == T_MUSHROOM_PLATFORM_MIDDLE)
			{
				al_draw_bitmap_region(tile_image, TILE_MUSHROOM_PLATFORM_MIDDLE_SX, TILE_MUSHROOM_PLATFORM_MIDDLE_SY, TILE_SIZE, TILE_SIZE, i*TILE_SIZE,
					j*TILE_SIZE, 0);
			}
			else if (mapFile[i][j] == T_MUSHROOM_PLATFORM_RIGHT)
			{
				al_draw_bitmap_region(tile_image, TILE_MUSHROOM_PLATFORM_RIGHT_SX, TILE_MUSHROOM_PLATFORM_RIGHT_SY, TILE_SIZE, TILE_SIZE, i*TILE_SIZE,
					j*TILE_SIZE, 0);
			}
			else if (mapFile[i][j] == T_MUSHROOM_PLATFORM_BASE_LEFT)
			{
				al_draw_bitmap_region(tile_image, TILE_MUSHROOM_PLATFORM_BASE_LEFT_SX, TILE_MUSHROOM_PLATFORM_BASE_LEFT_SY, TILE_SIZE, TILE_SIZE, i*TILE_SIZE,
					j*TILE_SIZE, 0);
			}
			else if (mapFile[i][j] == T_MUSHROOM_PLATFORM_BASE_MIDDLE)
			{
				al_draw_bitmap_region(tile_image, TILE_MUSHROOM_PLATFORM_BASE_MIDDLE_SX, TILE_MUSHROOM_PLATFORM_BASE_MIDDLE_SY, TILE_SIZE, TILE_SIZE, i*TILE_SIZE,
					j*TILE_SIZE, 0);
			}
			else if (mapFile[i][j] == T_MUSHROOM_PLATFORM_BASE_RIGHT)
			{
				al_draw_bitmap_region(tile_image, TILE_MUSHROOM_PLATFORM_BASE_RIGHT_SX, TILE_MUSHROOM_PLATFORM_BASE_RIGHT_SY, TILE_SIZE, TILE_SIZE, i*TILE_SIZE,
					j*TILE_SIZE, 0);
			}
			else if (mapFile[i][j] == T_MUSHROOM_PLATFORM_BASE_SINGLE)
			{
				al_draw_bitmap_region(tile_image, TILE_MUSHROOM_PLATFORM_BASE_SINGLE_SX, TILE_MUSHROOM_PLATFORM_BASE_SINGLE_SY, TILE_SIZE, TILE_SIZE, i*TILE_SIZE,
					j*TILE_SIZE, 0);
			}
			else if (mapFile[i][j] == T_MUSHROOM_PLATFORM_SINGLE)
			{
				al_draw_bitmap_region(tile_image, TILE_MUSHROOM_PLATFORM_SINGLE_SX, TILE_MUSHROOM_PLATFORM_SINGLE_SY, TILE_SIZE, TILE_SIZE, i*TILE_SIZE,
					j*TILE_SIZE, 0);
			}
			else if (mapFile[i][j] == T_STONE_BLOCK)
			{
				al_draw_bitmap_region(tile_image, TILE_STONE_BLOCK_SX, TILE_STONE_BLOCK_SY, TILE_SIZE, TILE_SIZE, i*TILE_SIZE,
					j*TILE_SIZE, 0);
			}
			else if (mapFile[i][j] == T_BROWN_BOX)
			{
				al_draw_bitmap_region(tile_image, TILE_BROWN_BOX_SX, TILE_BROWN_BOX_SY, TILE_SIZE, TILE_SIZE, i*TILE_SIZE, j*TILE_SIZE, 0);
			}
			else if (mapFile[i][j] == T_CLEAR_FLAG)
			{
				al_draw_bitmap_region(tile_image, TILE_CLEAR_FLAG_SX, TILE_CLEAR_FLAG_SY, CLEAR_FLAG_W, CLEAR_FLAG_H, i*TILE_SIZE, j*TILE_SIZE, 0);
			}
		}
	}
}

// open the level text file to populate the tile array (mapFile) that holds the map images
void Map::loadMap(const char *filename)
{
	std::ifstream openfile(filename);
	if (openfile.is_open())
	{
		openfile >> currentLevel >> mapSizeX >> mapSizeY;
		while (!openfile.eof())
		{
			openfile >> mapFile[loadCounterX][loadCounterY];
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
		fprintf(stderr, "map file not found!\n");
	}
}

// examine the tile array (mapFile) to determine the position of the clear flag
void Map::setClearFlagPosition()
{
	clearFlagX = SCREEN_W;
	clearFlagY = SCREEN_H;
	for (int i = 0;i < mapSizeX;i++)
	{
		for (int j = 0;j < mapSizeY;j++)
		{
			if (mapFile[i][j] == 69)
			{
				clearFlagX = i * TILE_SIZE;
				clearFlagY = j * TILE_SIZE;
			}
		}
	}
}

// returns true if the level has been cleared
bool Map::getLevelClear()
{
	return levelClear;
}

// set levelClear flag to true the player is positioned within the clear level area
void Map::playerWithinClearFlag(Player &player)
{
	if (player.bottom_center_pixel_x > (clearFlagX + TILE_SIZE) && player.bottom_center_pixel_x < (clearFlagX + CLEAR_FLAG_W - TILE_SIZE)
		&& player.bottom_center_pixel_y >= clearFlagY && player.bottom_center_pixel_y <= (clearFlagY + CLEAR_FLAG_H))
	{
		if (currentLevel != FINAL_LEVEL)
		{
			levelClear = true;
			al_play_sample(level_clear_sound, 1.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		}
		else
		{
			levelClear = true;
			al_play_sample(final_level_clear, 1.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		}
	}
}

void Map::reinitialize(const char *filename)
{
	loadMap(filename);
	setClearFlagPosition();
	levelClear = false;
}