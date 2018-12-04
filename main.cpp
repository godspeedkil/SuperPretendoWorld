// Programmed by: Edgar Diaz
// Contact me: gettingschwifty@gmail.com
// Built with Allegro 5.1.12 (MSVC 2015 x86 distribution) site:http://download.gna.org/allegro/allegro-unstable-bin/5.1.12/, 
//		check project properties for correct paths...

#include "Main.h"
#include "Player.h"
#include "Enemy.h"
#include "GreenKoopa.h"
#include "Map.h"
#include "Collision.h"

void flushEnemyList(std::vector<Enemy*> &enemy_list);
void loadEnemyList(std::vector<Enemy*> &enemy_list, const char *filename, ALLEGRO_BITMAP *enemy_image, ALLEGRO_SAMPLE *enemy_stomped_sound);
void levelLoop(Player &player, std::vector<Enemy*> &enemy_list, Collision &collision, Map &map, ALLEGRO_EVENT_QUEUE *&event_queue, ALLEGRO_EVENT &ev,
	bool(&key)[6], bool &redraw, bool &doexit, ALLEGRO_BITMAP *&background, ALLEGRO_FONT *&font, int &nextScene, int &currentScene, bool &levelExit,
	ALLEGRO_SAMPLE_ID &level_music_id);
void startLevelProcedure(Player &player, std::vector<Enemy*> &enemy_list, Collision &collision, Map &map, ALLEGRO_SAMPLE *&level_theme,
	ALLEGRO_SAMPLE_ID *&level_music_id, int &currentScene, int &nextScene, bool &levelExit, bool(&key)[6], const char * const enemy_file, const char * const collision_file,
	const char * const map_file, ALLEGRO_BITMAP *enemy_image, ALLEGRO_SAMPLE *enemy_stomped_sound);
void startSceneProcedure(std::vector<Enemy*> &enemy_list, int &currentScene, int &nextScene, Player &player, Collision &collision, Map &map,
	std::vector<ALLEGRO_SAMPLE*> sample_list, std::vector<ALLEGRO_SAMPLE_ID*> sample_id_list, bool &levelExit, bool(&key)[6],
	ALLEGRO_BITMAP *enemy_image, ALLEGRO_SAMPLE *enemy_stomped_sound);
void gameOverProcedure(Player &player, int &currentScene, int &nextScene, ALLEGRO_BITMAP *game_over_screen);
void endingProcedure(ALLEGRO_BITMAP *&ending_screen, ALLEGRO_EVENT_QUEUE *&event_queue, ALLEGRO_EVENT &ev, ALLEGRO_SAMPLE_ID &ending_id,
	int &currentScene, int &nextScene, bool &doExit);
void mainMenuProcedure(Player &player, ALLEGRO_EVENT &ev, ALLEGRO_SAMPLE_ID &title_id, int &currentScene, int &nextScene, bool &doExit,
	ALLEGRO_BITMAP *&title_image);

int main(int argc, char **argv)
{
	if (!al_init()) {
		fprintf(stderr, "Failed to initialize Allegro!\n");
		return -1;
	}
	al_init_font_addon(); // never returns false, this is a build bug of Allegro 5.0
	if (!al_init_ttf_addon() || !al_init_primitives_addon() || !al_install_keyboard() || !al_install_audio()
		|| !al_init_acodec_addon() || !al_reserve_samples(10) || !al_init_image_addon())
	{
		fprintf(stderr, "Failed to initialize an Allegro module!\n");
		return -1;
	}
	ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);				//destroy
	al_append_path_component(path, "resources");
	al_change_directory(al_path_cstr(path, '/'));									// change the working directory
	ALLEGRO_DISPLAY *display = al_create_display(SCREEN_W, SCREEN_H);				// destroy
	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();						// destroy
	ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);								// destroy
	ALLEGRO_SAMPLE	*overworld_theme{ al_load_sample(FILE_SAMPLE_OVERWORLD) },
					*title_theme{ al_load_sample(FILE_SAMPLE_TITLE) },
					*game_over_music{ al_load_sample(FILE_SAMPLE_GAMEOVER) },
					*ending_theme{ al_load_sample(FILE_SAMPLE_ENDING) },
					*enemy_stomped_sound{ al_load_sample(FILE_SAMPLE_STOMP) };		// destroy
	ALLEGRO_BITMAP	*background = al_load_bitmap(FILE_BITMAP_BACKGROUND),
					*title_image{ al_load_bitmap(FILE_BITMAP_TITLE) },
					*game_over_screen{ al_load_bitmap(FILE_BITMAP_GAMEOVER) },
					*ending_screen{ al_load_bitmap(FILE_BITMAP_ENDING) },
					*enemy_image{ al_load_bitmap(FILE_BITMAP_ENEMY) };				// destroy
	ALLEGRO_FONT *font = al_load_ttf_font(FILE_FONT_DEFAULT, FONT_SIZE, 0);			// destroy
	ALLEGRO_SAMPLE_ID title_id, overworld_id, ending_id;
	std::vector<ALLEGRO_SAMPLE*> sample_list{ overworld_theme, title_theme, ending_theme, game_over_music, enemy_stomped_sound };
	std::vector<ALLEGRO_BITMAP*> bitmap_list{ background, title_image, game_over_screen, ending_screen, enemy_image };
	std::vector<ALLEGRO_SAMPLE_ID*> sample_id_list{ &overworld_id, &title_id, &ending_id };
	bool key[6] = { false, false, false, false, false, false };
	bool redraw{ true }, doExit{ false }, levelClear{ false }, levelExit{ false };
	int currentScene{ START_SCENE }, nextScene{ MAIN_MENU };
	if (!display || !event_queue || !timer || !overworld_theme || !title_theme || !game_over_music || !ending_theme || !enemy_stomped_sound || !background || 
		!title_image || !game_over_screen || !ending_screen || !enemy_image || !font)
	{
		al_destroy_display(display);
		al_destroy_event_queue(event_queue);
		al_destroy_timer(timer);
		al_destroy_sample(overworld_theme);
		al_destroy_sample(title_theme);
		al_destroy_sample(game_over_music);
		al_destroy_sample(ending_theme);
		al_destroy_sample(enemy_stomped_sound);
		al_destroy_bitmap(background);
		al_destroy_bitmap(title_image);
		al_destroy_bitmap(game_over_screen);
		al_destroy_bitmap(ending_screen);
		al_destroy_bitmap(enemy_image);
		al_destroy_font(font);
		fprintf(stderr, "Failed to load a general resource!\n");
		return -1;
	}
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_start_timer(timer);
	Collision collision;
	Map map;
	Player player;
	std::vector<Enemy*> enemy_list;
	while (!doExit)
	{
		switch (currentScene) {
		case MAIN_MENU:
			ALLEGRO_EVENT ev;
			al_wait_for_event(event_queue, &ev);
			mainMenuProcedure(player, ev, title_id, currentScene, nextScene, doExit, title_image);
			break;
		case START_SCENE:
			startSceneProcedure(enemy_list, currentScene, nextScene, player, collision, map, sample_list, sample_id_list, levelExit, key,
				enemy_image, enemy_stomped_sound);
			break;
		case GAME_OVER:
			gameOverProcedure(player, currentScene, nextScene, game_over_screen);
			break;
		case ENDING:
			endingProcedure(ending_screen, event_queue, ev, ending_id, currentScene, nextScene, doExit);
			break;
		default:
			levelLoop(player, enemy_list, collision, map, event_queue, ev, key, redraw, doExit, background, font, currentScene, nextScene, levelExit,
				overworld_id);
			break;
		}
	}
	sample_list.clear();
	bitmap_list.clear();
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	al_destroy_sample(overworld_theme);
	al_destroy_sample(title_theme);
	al_destroy_sample(game_over_music);
	al_destroy_sample(ending_theme);
	al_destroy_sample(enemy_stomped_sound);
	al_destroy_bitmap(background);
	al_destroy_bitmap(title_image);
	al_destroy_bitmap(game_over_screen);
	al_destroy_bitmap(ending_screen);
	al_destroy_bitmap(enemy_image);
	al_destroy_font(font);
	al_destroy_path(path);
	return 0;
}

void flushEnemyList(std::vector<Enemy*> &enemy_list)
{
	for (int i = 0; i < enemy_list.size();i++)
	{
		delete (enemy_list[i]);
	}
	enemy_list.clear();
}

void loadEnemyList(std::vector<Enemy*> &enemy_list, const char *filename, ALLEGRO_BITMAP *enemy_image, ALLEGRO_SAMPLE *enemy_stomped_sound)
{
	int numberOfEnemies, enemyType, enemyTileX, enemyTileY, direction, currentEnemy{ 0 };
	std::ifstream openfile(filename);
	if (openfile.is_open())
	{
		openfile >> numberOfEnemies;
		while (!openfile.eof())
		{
			while (currentEnemy < numberOfEnemies)
			{
				openfile >> enemyType >> enemyTileX >> enemyTileY >> direction;
				if (enemyType == ENEMY_KOOPA)
				{
					//std::cout << "Loaded: " << enemyType << ", " << enemyTileX << ", " << enemyTileY << std::endl;
					enemy_list.push_back(new GreenKoopa(enemyType, enemyTileX, enemyTileY, direction, enemy_image, enemy_stomped_sound));
				}
				currentEnemy++;
			}
		}
	}
	else
	{
		fprintf(stderr, "unable to load enemy list!\n");
	}
}

void levelLoop(Player &player, std::vector<Enemy*> &enemy_list, Collision &collision, Map &map, ALLEGRO_EVENT_QUEUE *&event_queue, ALLEGRO_EVENT &ev,
	bool (&key)[6], bool &redraw, bool &doexit, ALLEGRO_BITMAP *&background, ALLEGRO_FONT *&font, int &currentScene, int &nextScene, bool &levelExit,
	ALLEGRO_SAMPLE_ID &level_music_id)
{
	if (!player.isDead && !map.getLevelClear() && !levelExit)
	{
		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_TIMER) {
			if (key[KEY_UP]) {
				player.move(false, KEY_UP);
			}
			if (key[KEY_DOWN]) {
				player.move(false, KEY_DOWN);
			}
			if (key[KEY_LEFT] && !key[KEY_Z]) {
				player.move(false, KEY_LEFT);
			}
			else if (key[KEY_LEFT] && key[KEY_Z])
			{
				player.move(true, KEY_LEFT);
			}
			if (key[KEY_RIGHT] && !key[KEY_Z]) {
				player.move(false, KEY_RIGHT);
			}
			else if (key[KEY_RIGHT] && key[KEY_Z])
			{
				player.move(true, KEY_RIGHT);
			}
			if (key[KEY_Z]) {
				//player1.move(KEY_A, world);
			}
			if (key[KEY_X]) {
				player.jump();
			}
			redraw = true;
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			return;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				key[KEY_UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				key[KEY_DOWN] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				key[KEY_LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				key[KEY_RIGHT] = true;
				break;
			case ALLEGRO_KEY_Z:
				key[KEY_Z] = true;
				break;
			case ALLEGRO_KEY_X:
				key[KEY_X] = true;
				break;
			case ALLEGRO_KEY_ESCAPE:
				al_stop_sample(&level_music_id);
				levelExit = true;
				nextScene = MAIN_MENU;
				currentScene = START_SCENE;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				key[KEY_UP] = false;
				player.idle();
				break;
			case ALLEGRO_KEY_DOWN:
				key[KEY_DOWN] = false;
				player.idle();
				break;
			case ALLEGRO_KEY_LEFT:
				key[KEY_LEFT] = false;
				player.idle();
				break;
			case ALLEGRO_KEY_RIGHT:
				key[KEY_RIGHT] = false;
				player.idle();
				break;
			case ALLEGRO_KEY_Z:
				key[KEY_Z] = false;
				break;
			case ALLEGRO_KEY_X:
				key[KEY_X] = false;
				player.notPressingJump();
				break;
			}
		}
		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;
			al_clear_to_color(al_map_rgb(COLOR_PLAINS_SKY_R, COLOR_PLAINS_SKY_G, COLOR_PLAINS_SKY_B));
			al_draw_bitmap(background, 0, 0, 0);
			map.update(player);
			al_draw_textf(font, al_map_rgb(0, 0, 0), 12, 12, 0, "Mario");
			al_draw_textf(font, al_map_rgb(0, 0, 0), 12, 32, 0, "x %d", player.getLives());
			for (int i = 0; i < enemy_list.size(); i++)
			{
				if (enemy_list.at(i)->isDead && !enemy_list.at(i)->keepOnScreen)
				{
					delete enemy_list.at(i);
					enemy_list.erase(enemy_list.begin() + i);
				}
			}
			collision.playerToEnemyCollision(player, enemy_list);
			for (Enemy *i : enemy_list)
			{
				collision.update(*i);
			}
			collision.update(player);
			for (Enemy *i : enemy_list)
			{
				i->update();
			}
			player.update();
			al_flip_display();
		}
	}
	else if (player.isDead)
	{
		for (int i = 0; i < sizeof(key); i++)
		{
			key[i] = false;
		}
		al_stop_sample(&level_music_id);
		al_rest(4);
		if (!player.isGameOver)
		{
			nextScene = currentScene;
			currentScene = START_SCENE;
		}
		else
		{
			currentScene = START_SCENE;
			nextScene = GAME_OVER;
		}
	}
	else if (map.getLevelClear())
	{
		for (int i = 0; i < sizeof(key); i++)
		{
			key[i] = false;
		}
		al_stop_sample(&level_music_id);
		if (currentScene == FINAL_LEVEL)
		{
			al_rest(9);
			nextScene = ENDING;
			currentScene = START_SCENE;
		}
		else
		{
			al_stop_sample(&level_music_id);
			al_rest(8);
			nextScene = currentScene + 1;
			currentScene = START_SCENE;
		}
	}
}

void startLevelProcedure(Player &player, std::vector<Enemy*> &enemy_list, Collision &collision, Map &map, ALLEGRO_SAMPLE *&level_theme, 
	ALLEGRO_SAMPLE_ID *&level_music_id, int &currentScene, int &nextScene, bool &levelExit, bool (&key)[6], const char * const enemy_file, 
	const char * const collision_file, const char * const map_file, ALLEGRO_BITMAP *enemy_image, ALLEGRO_SAMPLE *enemy_stomped_sound)
{
	if (nextScene == LEVEL_4)
	{
		player.initializeForLevel(TILE_SIZE, 0);
	}
	else if (nextScene == LEVEL_5)
	{
		player.initializeForLevel(TILE_SIZE * 5, SCREEN_H - TILE_SIZE - SM_MARIO_IMG_H);
	}
	else
	{
		player.initializeForLevel();
	}
	loadEnemyList(enemy_list, enemy_file, enemy_image, enemy_stomped_sound);
	collision.loadCollisionMap(collision_file);
	map.reinitialize(map_file);
	al_play_sample(level_theme, 0.6, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, level_music_id);
	currentScene = nextScene;
	levelExit = false;
	for (int i = 0; i < sizeof(key); i++)
	{
		key[i] = false;
	}
}

void startSceneProcedure(std::vector<Enemy*> &enemy_list, int &currentScene, int &nextScene, Player &player, Collision &collision, Map &map,
	std::vector<ALLEGRO_SAMPLE*> sample_list, std::vector<ALLEGRO_SAMPLE_ID*> sample_id_list, bool &levelExit, bool(&key)[6], 
	ALLEGRO_BITMAP *enemy_image, ALLEGRO_SAMPLE *enemy_stomped_sound)
{
	flushEnemyList(enemy_list);
	if (nextScene == LEVEL_1)
	{
		startLevelProcedure(player, enemy_list, collision, map, sample_list.at(0), sample_id_list.at(0), currentScene, nextScene, levelExit, key,
			FILE_LEVEL_1_ENEMIES, FILE_LEVEL_1_COLLISION, FILE_LEVEL_1_MAP, enemy_image, enemy_stomped_sound);
	}
	else if (nextScene == LEVEL_2)
	{
		startLevelProcedure(player, enemy_list, collision, map, sample_list.at(0), sample_id_list.at(0), currentScene, nextScene, levelExit, key,
			FILE_LEVEL_2_ENEMIES, FILE_LEVEL_2_COLLISION, FILE_LEVEL_2_MAP, enemy_image, enemy_stomped_sound);
	}
	else if (nextScene == LEVEL_3)
	{
		startLevelProcedure(player, enemy_list, collision, map, sample_list.at(0), sample_id_list.at(0), currentScene, nextScene, levelExit, key,
			FILE_LEVEL_3_ENEMIES, FILE_LEVEL_3_COLLISION, FILE_LEVEL_3_MAP, enemy_image, enemy_stomped_sound);
	}
	else if (nextScene == LEVEL_4)
	{
		startLevelProcedure(player, enemy_list, collision, map, sample_list.at(0), sample_id_list.at(0), currentScene, nextScene, levelExit, key,
			FILE_LEVEL_4_ENEMIES, FILE_LEVEL_4_COLLISION, FILE_LEVEL_4_MAP, enemy_image, enemy_stomped_sound);
	}
	else if (nextScene == LEVEL_5)
	{
		startLevelProcedure(player, enemy_list, collision, map, sample_list.at(0), sample_id_list.at(0), currentScene, nextScene, levelExit, key,
			FILE_LEVEL_5_ENEMIES, FILE_LEVEL_5_COLLISION, FILE_LEVEL_5_MAP, enemy_image, enemy_stomped_sound);
	}
	else if (nextScene == FINAL_LEVEL)
	{
		startLevelProcedure(player, enemy_list, collision, map, sample_list.at(0), sample_id_list.at(0), currentScene, nextScene, levelExit, key,
			FILE_FINAL_LEVEL_ENEMIES, FILE_FINAL_LEVEL_COLLISION, FILE_FINAL_LEVEL_MAP, enemy_image, enemy_stomped_sound);
	}
	else if (nextScene == ENDING)
	{
		al_play_sample(sample_list.at(2), 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, sample_id_list.at(2));
		currentScene = ENDING;
	}
	else if (nextScene == MAIN_MENU)
	{
		al_play_sample(sample_list.at(1), 0.6, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, sample_id_list.at(1));
		currentScene = MAIN_MENU;
	}
	else if (nextScene == GAME_OVER)
	{
		al_play_sample(sample_list.at(3), 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		currentScene = GAME_OVER;
	}
}

void gameOverProcedure(Player &player, int &currentScene, int &nextScene, ALLEGRO_BITMAP *game_over_screen)
{
	al_draw_bitmap(game_over_screen, 0, 0, 0);
	al_flip_display();
	al_rest(6);
	player.reinitialize();
	currentScene = START_SCENE;
	nextScene = MAIN_MENU;
}

void endingProcedure(ALLEGRO_BITMAP *&ending_screen, ALLEGRO_EVENT_QUEUE *&event_queue, ALLEGRO_EVENT &ev, ALLEGRO_SAMPLE_ID &ending_id, 
	int &currentScene, int &nextScene, bool &doExit)
{
	al_draw_bitmap(ending_screen, 0, 0, 0);
	al_flip_display();
	al_wait_for_event(event_queue, &ev);
	if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
	{
		if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
		{
			al_stop_sample(&ending_id);
			currentScene = START_SCENE;
			nextScene = MAIN_MENU;
		}
		if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
		{
			doExit = true;
		}
	}
}

void mainMenuProcedure(Player &player, ALLEGRO_EVENT &ev, ALLEGRO_SAMPLE_ID &title_id, int &currentScene, int &nextScene, bool &doExit,
	ALLEGRO_BITMAP *&title_image)
{
	player.reinitialize();
	if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
	{
		if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
		{
			al_stop_sample(&title_id);
			currentScene = START_SCENE;
			nextScene = LEVEL_1;
		}
		if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
		{
			doExit = true;
		}
	}
	al_draw_bitmap(title_image, 0, 0, 0);
	al_flip_display();
}