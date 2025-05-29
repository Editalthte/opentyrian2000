#pragma once

#include <SDL2/SDL_stdinc.h>

struct Sprite
{
	Uint16 width, height;
	Uint16 size;
	Uint8 *data;
};

struct Sprite_Table
{
	unsigned int count;	

	static constexpr int SPRITES_PER_TABLE_MAX{152};
	Sprite sprite[SPRITES_PER_TABLE_MAX];
};

struct Sprite_Sheet
{
	size_t size;
	Uint8 *data;
};

struct Sprite_Sheets
{
	Sprite_Sheet shop; // fka shapes6
	Sprite_Sheet explosions; // fka shapes6

	Sprite_Sheet enemies; // fka eShapes1, eShapes2, eShapes3, eShapes4
	Uint8 enemySpriteSheetIds[4];  // fka enemyShapeTables

	// Static sprite sheets.  Player shots, player ships, power-ups, coins, etc.
	Sprite_Sheet sprite_sheet_8;  // fka shapesC1
	Sprite_Sheet sprite_sheet_9;  // fka shapes9
	Sprite_Sheet sprite_sheet_10;  // fka eShapes6
	Sprite_Sheet sprite_sheet_11;  // fka eShapes5
	Sprite_Sheet sprite_sheet_12;  // fka shapesW2
	Sprite_Sheet sprite_sheet_T2000; // fka shapesT2k
};