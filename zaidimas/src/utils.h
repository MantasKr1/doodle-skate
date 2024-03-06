#pragma once

struct Player;
struct Plate;
struct Enemy;
struct Bullet;
struct Ground;

namespace utils
{
	bool IsBetween(float val, float rangeB, float rangeE);
	bool InOnPlate(Player& player, Plate& plate);
	bool IsShot(Enemy& enemy, Bullet& bullet);
	bool IsHit(Player& player, Enemy& enemy);
	bool IsOnGround(Player& player, Ground& ground);
}
