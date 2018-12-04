#ifndef MAIN_H
#define MAIN_H
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <iterator>
#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"
#include "allegro5/allegro_audio.h"
#include "allegro5/allegro_acodec.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro_primitives.h"

const char	* const FILE_LEVEL_1_ENEMIES{ "level1_enemies.txt" },
			* const FILE_LEVEL_1_COLLISION{ "level1_collision.txt" },
			* const FILE_LEVEL_1_MAP{ "level1_map.txt" },
			* const FILE_LEVEL_2_ENEMIES{ "level2_enemies.txt" },
			* const FILE_LEVEL_2_COLLISION{ "level2_collision.txt" },
			* const FILE_LEVEL_2_MAP{ "level2_map.txt" },
			* const FILE_LEVEL_3_ENEMIES{ "level3_enemies.txt" },
			* const FILE_LEVEL_3_COLLISION{ "level3_collision.txt" },
			* const FILE_LEVEL_3_MAP{ "level3_map.txt" },
			* const FILE_LEVEL_4_ENEMIES{ "level4_enemies.txt" },
			* const FILE_LEVEL_4_COLLISION{ "level4_collision.txt" },
			* const FILE_LEVEL_4_MAP{ "level4_map.txt" },
			* const FILE_LEVEL_5_ENEMIES{ "level5_enemies.txt" },
			* const FILE_LEVEL_5_COLLISION{ "level5_collision.txt" },
			* const FILE_LEVEL_5_MAP{ "level5_map.txt" },
			* const FILE_FINAL_LEVEL_ENEMIES{ "final_level_enemies.txt" },
			* const FILE_FINAL_LEVEL_COLLISION{ "final_level_collision.txt" },
			* const FILE_FINAL_LEVEL_MAP{ "final_level_map.txt" },
			* const FILE_SAMPLE_OVERWORLD{ "overworld.ogg" },
			* const FILE_SAMPLE_TITLE{ "title.wav" },
			* const FILE_SAMPLE_GAMEOVER{ "game_over.wav" },
			* const FILE_SAMPLE_ENDING{ "ending_theme.wav" },
			* const FILE_SAMPLE_STOMP{ "stomp.wav" },
			* const FILE_BITMAP_BACKGROUND{ "plains_bg_extended.png" },
			* const FILE_BITMAP_TITLE{ "title.png" },
			* const FILE_BITMAP_GAMEOVER{ "game_over_screen.png" },
			* const FILE_BITMAP_ENDING{ "ending_screen.png" },
			* const FILE_BITMAP_ENEMY{ "enemies.png" },
			* const FILE_FONT_DEFAULT{ "font.ttf" };

const float FPS{ 60.0f };

const int	TILE_SIZE{ 16 }, 
			MAP_W{ 50 }, 
			MAP_H{ 38 }, 
			COLOR_PLAINS_SKY_R{ 248 }, 
			COLOR_PLAINS_SKY_G{ 224 }, 
			COLOR_PLAINS_SKY_B{ 176 }, 
			TILE_PLAINS_GRASS_LEFT_SX{ 427 },
			TILE_PLAINS_NO_SIDE_GRASS_SX{ 444 }, 
			TILE_PLAINS_GRASS_RIGHT_SX{ 461 }, 
			TILE_PLAINS_GRASS_TOP_SY{ 202 }, 
			TILE_PLAINS_NO_GRASS_TOPBOTTOM_SY{ 219 },
			TILE_PLAINS_GRASS_BOTTOM_SY{ 236 }, 
			TILE_PLAINS_GRASS_INNERRIGHT_SX{ 357 }, 
			TILE_PLAINS_GRASS_INNERLEFT_SX{ 374 },
			TILE_PLAINS_GRASS_INNERBOTTOM_SY{ 365 }, 
			TILE_PLAINS_GRASS_INNERTOP_SY{ 382 }, 
			TILE_MUSHROOM_PLATFORM_LEFT_SX{ 5 },
			TILE_MUSHROOM_PLATFORM_LEFT_SY{ 480 },
			TILE_MUSHROOM_PLATFORM_MIDDLE_SX{ 39 },
			TILE_MUSHROOM_PLATFORM_MIDDLE_SY{ 480 },
			TILE_MUSHROOM_PLATFORM_RIGHT_SX{ 73 },
			TILE_MUSHROOM_PLATFORM_RIGHT_SY{ 480 },
			TILE_MUSHROOM_PLATFORM_SINGLE_SX{ 90 },
			TILE_MUSHROOM_PLATFORM_SINGLE_SY{ 480 },
			TILE_MUSHROOM_PLATFORM_BASE_LEFT_SX{ 22 },
			TILE_MUSHROOM_PLATFORM_BASE_LEFT_SY{ 497 },
			TILE_MUSHROOM_PLATFORM_BASE_MIDDLE_SX{ 39 },
			TILE_MUSHROOM_PLATFORM_BASE_MIDDLE_SY{ 497 },
			TILE_MUSHROOM_PLATFORM_BASE_RIGHT_SX{ 56 },
			TILE_MUSHROOM_PLATFORM_BASE_RIGHT_SY{ 497 },
			TILE_MUSHROOM_PLATFORM_BASE_SINGLE_SX{ 107 },
			TILE_MUSHROOM_PLATFORM_BASE_SINGLE_SY{ 480 },
			TILE_BROWN_BOX_SX{ 257 },
			TILE_BROWN_BOX_SY{ 97 },
			TILE_STONE_BLOCK_SX{ 275 },
			TILE_STONE_BLOCK_SY{ 80 },
			TILE_CLEAR_FLAG_SX{ 480 },
			TILE_CLEAR_FLAG_SY{ 510 },
			CLEAR_FLAG_W{ 64 },
			CLEAR_FLAG_H{ 64 },
			SCREEN_W{ 800 }, 
			SCREEN_H{ 608 },
			FONT_SIZE{ 12 },
			SM_MARIO_IMG_W{ 30 },
			SM_MARIO_IMG_H{ 30 };

const double GRAVITY{ 9.8 };

enum MYKEYS {
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_Z, KEY_X, NO_KEY
};

enum TILE_DEFINITIONS {
	T_EMPTY								= 0, 
	T_PLAINS_GRASS_TOP					= 1, 
	T_PLAINS_NO_GRASS					= 2, 
	T_PLAINS_GRASS_LEFT					= 3, 
	T_PLAINS_GRASS_RIGHT				= 4, 
	T_PLAINS_GRASS_BOTTOM				= 5, 
	T_PLAINS_GRASS_TOPLEFT				= 6, 
	T_PLAINS_GRASS_TOPRIGHT				= 7, 
	T_PLAINS_GRASS_BOTTOMLEFT			= 8, 
	T_PLAINS_GRASS_BOTTOMRIGHT			= 9,
	T_PLAINS_GRASS_INNERBOTTOMRIGHT		= 10,
	T_PLAINS_GRASS_INNERBOTTOMLEFT		= 11,
	T_PLAINS_GRASS_INNERTOPRIGHT		= 12,
	T_PLAINS_GRASS_INNERTOPLEFT			= 13,
	T_MUSHROOM_PLATFORM_LEFT			= 20,
	T_MUSHROOM_PLATFORM_MIDDLE			= 21,
	T_MUSHROOM_PLATFORM_RIGHT			= 22,
	T_MUSHROOM_PLATFORM_BASE_LEFT		= 23,
	T_MUSHROOM_PLATFORM_BASE_MIDDLE		= 24,
	T_MUSHROOM_PLATFORM_BASE_RIGHT		= 25,
	T_MUSHROOM_PLATFORM_BASE_SINGLE		= 26,
	T_MUSHROOM_PLATFORM_SINGLE			= 27,
	T_STONE_BLOCK						= 67,
	T_BROWN_BOX							= 68,
	T_CLEAR_FLAG						= 69
};

enum ENEMIES {
	ENEMY_GOOMBA = 0,
	ENEMY_KOOPA = 1
};

enum SCENE_NUMBERS {
	MAIN_MENU,
	LEVEL_1,
	LEVEL_2,
	LEVEL_3,
	LEVEL_4,
	LEVEL_5,
	FINAL_LEVEL,
	GAME_OVER,
	START_SCENE,
	ENDING
};

#endif