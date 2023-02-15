#include "Utils.h"
#include "Objects.h"

bool utils::IsBetween(float val, float rangeB, float rangeE)
{
	return val > rangeB && val < rangeE;
}

bool utils::InOnPlate(Player &player, Plate &plate)
{
	const bool betweenX = IsBetween(player.LegsStartX(), plate.StartX(), plate.EndX()) ||
		IsBetween(player.LegsEndX(), plate.StartX(), plate.EndX());

	const bool betweenY = IsBetween(player.LegsY(), plate.TopY(), plate.BottomY());

	return betweenX && betweenY;
}
