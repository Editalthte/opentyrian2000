#pragma once

#include <SDL2/SDL_surface.h>
#include <cstddef>

struct Sprite2_array
{
	size_t size;
	Uint8 *data;
};

struct JE_SingleEnemyType
{
	Uint8  fillbyte;
	Sint16 ex, ey;     /* POSITION */
	Sint8  exc, eyc;   /* CURRENT SPEED */
	Sint8  exca, eyca; /* RANDOM ACCELERATION */
	Sint8  excc, eycc; /* FIXED ACCELERATION WAITTIME */
	Sint8  exccw, eyccw;
	Uint8  armorleft;
	Uint8  eshotwait[3], eshotmultipos[3]; /* [1..3] */
	Uint8  enemycycle;
	Uint8  ani;
	Uint16 egr[20]; /* [1..20] */
	Uint8  size;
	Uint8  linknum;
	Uint8  aniactive;
	Uint8  animax;
	Uint8  aniwhenfire;

	Sint8  exrev, eyrev;
	Sint16 exccadd, eyccadd;
	Uint8  exccwmax, eyccwmax;
	void   *enemydatofs;
	bool   edamaged;
	Uint16 enemytype;
	Uint8  animin;
	Uint16 edgr;
	Sint8  edlevel;
	Sint8  edani;
	Uint8  fill1;
	Uint8  filter;
	Sint16 evalue;
	Sint16 fixedmovey;
	Uint8  freq[3]; /* [1..3] */
	Uint8  launchwait;
	Uint16 launchtype;
	Uint8  launchfreq;
	Uint8  xaccel;
	Uint8  yaccel;
	Uint8  tur[3]; /* [1..3] */
	
	Uint16 enemydie; /* Enemy created when this one dies */
	bool   enemyground;
	
	Uint8  explonum;
	Uint16 mapoffset;

	bool   scoreitem;
	bool   special;

	Uint8  flagnum;
	bool   setto;

	Uint8  iced; /*Duration*/
	Uint8  launchspecial;

	Sint16  xminbounce;
	Sint16  xmaxbounce;
	Sint16  yminbounce;
	Sint16  ymaxbounce;

	Uint8     fill[3]; /* [1..3] */

	Sprite2_array *sprite2s;
};

class Level_Engine
{
	public:
		
		// Default Constructor
		Level_Engine()
		{ }

		inline static void blit_enemy(SDL_Surface *surface, unsigned int i, signed int x_offset, signed int y_offset, signed int sprite_offset)
		{
			if (enemy[i].sprite2s == NULL)
			{
				fprintf(stderr, "warning: enemy %d sprite missing\n", i);
				return;
			}
			
			const int x = enemy[i].ex + x_offset + tempMapXOfs,
					y = enemy[i].ey + y_offset;
			const unsigned int index = enemy[i].egr[enemy[i].enemycycle - 1] + sprite_offset;

			if (enemy[i].filter != 0)
				blit_sprite2_filter(surface, x, y, *enemy[i].sprite2s, index, enemy[i].filter);
			else
				blit_sprite2(surface, x, y, *enemy[i].sprite2s, index);
		}	
};