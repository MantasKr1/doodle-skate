#pragma once

#include "Defines.h"

struct Player
{
	float LegsStartX() { return x + 24; }

	float LegsEndX() { return x + 64; }

	float StartY() { return y; }

	float LegsY() { return y + 73; }

	float LegsEndY() { return y; +73; }

	float x, y; // virsutinio kairiojo kampo koordinates
};

struct Plate
{
	float StartX() { return x; }

	float EndX() { return x + PLATES_WIDTH; }

	float TopY() { return y; }

	float BottomY() { return y + PLATES_HEIGHT; }

	float x, y; // virsutinio kairiojo kampo koordinates
};

struct Ground
{
	float StartX() { return x; }

	float EndX() { return x + GROUND_WIDTH; }

	float TopY() { return y; }

	float BottomY() { return y + GROUND_HEIGHT - GROUND_HEIGHT / 2; }

	float x, y; // virsutinio kairiojo kampo koordinates

	int kuris;
};

struct Bullet
{
	float StartX() { return x; }

	float EndX() { return x + BULLET_WIDTH; }

	float TopY() { return y; }

	float BottomY() { return y + BULLET_WIDTH; }

	float x, y; // virsutinio kairiojo kampo koordinates
};

struct Enemy
{
	float StartX() { return x; }

	float EndX() { return x + ENEMY_WIDTH; }

	float TopY() { return y; }

	float BottomY() { return y + ENEMY_HEIGHT; }

	float x, y; // virsutinio kairiojo kampo koordinates
};
