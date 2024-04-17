#pragma once

class Player;
class Plate;
class Enemy;
class Bullet;
class Ground;

namespace utils
{
	bool IsBetween(float val, float rangeB, float rangeE);
	bool InOnPlate(Player& player, Plate& plate);
	bool IsShot(Enemy& enemy, Bullet& bullet);
	bool IsHit(Player& player, Enemy& enemy);
	bool IsOnGround(Player& player, Ground& ground);
	bool HitGround(Ground& ground, Bullet& bullet);
}
