#include "Utils.h"
#include "Objects.h"

bool utils::IsBetween(float val, float rangeB, float rangeE)
{
	return val > rangeB && val < rangeE;
}

bool utils::InOnPlate(Player& player, Plate& plate)
{
	const bool betweenX = IsBetween(player.LegsStartX(), plate.StartX(), plate.EndX()) ||
		IsBetween(player.LegsEndX(), plate.StartX(), plate.EndX());

	const bool betweenY = IsBetween(player.LegsY(), plate.TopY(), plate.BottomY());

	return betweenX && betweenY;
}

bool utils::IsShot(Enemy& enemy, Bullet& bullet)
{
	const bool betweenX = IsBetween(bullet.StartX(), enemy.StartX(), enemy.EndX()) ||
		IsBetween(bullet.EndX(), enemy.StartX(), enemy.EndX());

	const bool betweenY = IsBetween(bullet.BottomY(), enemy.TopY(), enemy.BottomY());

	return betweenX && betweenY;
}

bool utils::IsHit(Player& player, Enemy& enemy)
{
	const bool betweenX = IsBetween(player.LegsStartX(), enemy.StartX(), enemy.EndX()) ||
		IsBetween(player.LegsEndX(), enemy.StartX(), enemy.EndX());

	const bool betweenY = IsBetween(player.y + 80, enemy.TopY(), enemy.BottomY()) || IsBetween(player.y, enemy.TopY(), enemy.BottomY());

	return betweenX && betweenY;
}

bool utils::IsOnGround(Player& player, Ground& ground)
{
	const bool betweenX = IsBetween(player.LegsStartX(), ground.StartX(), ground.EndX()) ||
		IsBetween(player.LegsEndX(), ground.StartX(), ground.EndX());

	const bool betweenY = IsBetween(player.LegsY(), ground.TopY() + 60, ground.BottomY());

	return betweenX && betweenY;
}

