#include "GreenKoopa.h"

GreenKoopa::GreenKoopa()
{

}

GreenKoopa::GreenKoopa(int type, int start_tile_x, int start_tile_y, int direction, ALLEGRO_BITMAP *enemy_image, ALLEGRO_SAMPLE *enemy_stomped_sound) :
	Enemy(type, start_tile_x, start_tile_y, direction, enemy_image, enemy_stomped_sound)
{
}

// draw routine specific to Green Koopas
void GreenKoopa::draw()
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
			al_draw_bitmap_region(enemy_image, ENEMY_DEATH_SX, ENEMY_DEATH_SY, ENEMY_IMG_W, ENEMY_IMG_H, pos_x, pos_y, ALLEGRO_FLIP_HORIZONTAL);
		}
		else
		{
			al_draw_bitmap_region(enemy_image, ENEMY_DEATH_SX, ENEMY_DEATH_SY, ENEMY_IMG_W, ENEMY_IMG_H, pos_x, pos_y, 0);
		}
	}
}