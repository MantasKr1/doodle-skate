#pragma once

#include "Defines.h"
#include <stdlib.h> 
#include <SFML/Graphics.hpp>
#include <ctime>

using namespace sf;

class Textures
{
private:
	Texture tBackground, tPlayer, tPlatform, tGround1, tGround2, tGround3, tEnemy;
public:
	Textures()
	{
		tBackground.loadFromFile("resources/background.png");
		tPlayer.loadFromFile("resources/doodle2.png");
		tPlatform.loadFromFile("resources/platform.png");
		tGround1.loadFromFile("resources/platform1.png");
		tGround2.loadFromFile("resources/platform2.png");
		tGround3.loadFromFile("resources/platform3.png");
		tEnemy.loadFromFile("resources/Enemy.png");
	}
	Texture &Background()
	{
		return tBackground;
	}
	Texture &Player()
	{
		return tPlayer;
	}
	Texture &Platform()
	{
		return tPlatform;
	}
	Texture &Ground1()
	{
		return tGround1;
	}
	Texture &Ground2()
	{
		return tGround2;
	}
	Texture &Ground3()
	{
		return tGround3;
	}
	Texture &Enemy()
	{
		return tEnemy;
	}
};

class Sprites
{
private:
	Textures texture;
	Sprite sprBackground, sprPlayer, sprPlate, sprGround1, sprGround2, sprGround3, sprEnemy;
public:
	Sprites()
	{
		sprBackground.setTexture(texture.Background());
		sprPlayer.setTexture(texture.Player());
		sprPlate.setTexture(texture.Platform());
		sprGround1.setTexture(texture.Ground1());
		sprGround2.setTexture(texture.Ground2());
		sprGround3.setTexture(texture.Ground3());
		sprEnemy.setTexture(texture.Enemy());
	}
	Sprite &Background()
	{
		return sprBackground;
	}
	Sprite &Player()
	{
		return sprPlayer;
	}
	Sprite &Plate()
	{
		return sprPlate;
	}
	Sprite &Ground1()
	{
		return sprGround1;
	}
	Sprite &Ground2()
	{
		return sprGround2;
	}
	Sprite &Ground3()
	{
		return sprGround3;
	}
	Sprite &Enemy()
	{
		return sprEnemy;
	}
};

class Player
{
private:
	float _x, _y;
	bool _jump;
	bool _doubleJump;
public:
	Player(float x, float y) : _x(x), _y(y), _jump(false), _doubleJump(false) { }

	~Player() {}

	float LegsStartX() { return _x + 24; }

	float LegsEndX() { return _x + 64; }

	float StartY() { return _y; }

	float LegsY() { return _y + 73; }

	float LegsEndY() { return _y + 73; }

	void ChangeX(float temp) { _x += temp; }

	void ChangeY(float temp) { _y += temp; }

	void Jump() { _jump = true; }

	void Jumped() { _jump = false; DoubleJump(); }

	void DoubleJump() { _doubleJump = true; }

	void DoubleJumped() { _doubleJump = false; }

	bool canJump() { return _jump; }

	bool canDoubleJump() { return _doubleJump; }

	void PosY(float temp) { _y = temp; }

	float X() { return _x; }

	float Y() { return _y; }
};

class Plate
{
private:
	float _x, _y;
public:
	Plate(float x, float y) : _x(x), _y(y) { }

	~Plate() {}

	float StartX() { return _x; }

	float EndX() { return _x + PLATES_WIDTH; }

	float TopY() { return _y; }

	float BottomY() { return _y + PLATES_HEIGHT; }

	void ChangeX(float temp) { _x += temp; }

	void ChangeY(float temp) { _y += temp; }

	void SetPosX(float temp) { _x = temp; }

	void SetPosY(float temp) { _y = temp; }

	float X() { return _x; }

	float Y() { return _y; }
};

class Ground
{
private:
	float _x, _y;
	Sprite _ground;
public:
	Ground() : _x(), _y(), _ground() { }

	Ground(float x, float y, Sprite ground) : _x(x), _y(y), _ground(ground) { }

	~Ground() {}

	float StartX() { return _x; }

	float EnemyPos() { return _x + (GROUND_WIDTH / 2 - ENEMY_WIDTH / 2); }

	float EndX() { return _x + GROUND_WIDTH; }

	float TopY() { return _y; }

	float BottomY() { return _y + GROUND_HEIGHT - GROUND_HEIGHT / 2; }

	void ChangeX(float temp) { _x += temp; }

	void ChangeY(float temp) { _y += temp; }

	float X() { return _x; }

	float Y() { return _y; }

	Sprite& Sprite()
	{
		return _ground;
	}
};

class Bullet
{
private:
	float _x, _y;
	clock_t _time;
public:

	Bullet(float x, float y, clock_t time) : _x(x), _y(y), _time(time) { }

	~Bullet() {}

	float StartX() { return _x; }

	float EndX() { return _x + BULLET_WIDTH; }

	float TopY() { return _y; }

	float BottomY() { return _y + BULLET_WIDTH; }

	void ChangeX(float temp) { _x += temp; }

	void ChangeY(float temp) { _y += temp; }

	float X() { return _x; }

	float Y() { return _y; }

	clock_t Time() { return _time; }
};

class Enemy
{
private:
	float _x, _y;
public:
	Enemy(float x, float y) : _x(x), _y(y) { }

	~Enemy() { }

	float StartX() { return _x; }

	float EndX() { return _x + ENEMY_WIDTH; }

	float TopY() { return _y; }

	float BottomY() { return _y + ENEMY_HEIGHT; }

	float X() { return _x; }

	float Y() { return _y; }

	void ChangeX(float temp) { _x += temp; }

	void ChangeY(float temp) { _y += temp; }
};
