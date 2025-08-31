#include "ChaseBehavior.h"


void helperPrintOrientation(Orientation helper)
{
	switch (helper)
	{
	case UP: std::cout << "UP" << std::endl; break;
	case LEFT: std::cout << "LEFT" << std::endl; break;
	case RIGHT: std::cout << "RIGHT" << std::endl; break;
	case DOWN: std::cout << "DOWN" << std::endl; break;
	}
}


ChaseBehavior::ChaseBehavior() {

}


ChaseBehavior::ChaseBehavior(GhostType type)
{
	mType = type;
}

glm::ivec2 ChaseBehavior::calculateTargetTile(glm::ivec2 pacmanTilePosition, Orientation pacmanOrientation, glm::ivec2 ghostTilePosition, glm::ivec2 blinkyTilePosition)
{
	if (mType == BLINKY)
	{
		return pacmanTilePosition;
	}
	else if (mType == PINKY)
	{
		if (pacmanOrientation == RIGHT)
		{
			return pacmanTilePosition + glm::ivec2(0, 4);
		}
		else if (pacmanOrientation == LEFT)
		{
			return pacmanTilePosition + glm::ivec2(0, -4);
		}
		else if (pacmanOrientation == DOWN)
		{
			return pacmanTilePosition + glm::ivec2(4, 0);
		}

		return pacmanTilePosition + glm::ivec2(-4, -4);
	}
	else if (mType == INKY)
	{
		glm::ivec2 offset = glm::ivec2(0, 0);
		switch (pacmanOrientation)
		{
		case RIGHT: offset = glm::ivec2(0, 2); break;
		case LEFT: offset = glm::ivec2(0, -2); break;
		case UP: offset = glm::ivec2(-2, -2); break;
		case DOWN: offset = glm::ivec2(2, 0); break;
		}

		glm::ivec2 offsetTile = pacmanTilePosition + offset;

		glm::ivec2 vector = offsetTile - blinkyTilePosition;
		glm::ivec2 targetTile;
		targetTile.x = blinkyTilePosition.x + (vector.x * 2);
		targetTile.y = blinkyTilePosition.y + (vector.y * 2);

		return targetTile;
	}
	else
	{
		int distanceSquared = std::pow(ghostTilePosition.x - pacmanTilePosition.x, 2) + std::pow(ghostTilePosition.y - pacmanTilePosition.y, 2);
		float distance = glm::sqrt(distanceSquared);
		int distanceCeil = std::ceilf(distance);
		if (distanceCeil > 8)
		{
			return pacmanTilePosition;
		}

		return glm::ivec2(35, 0);
	}
}

Orientation ChaseBehavior::calculateOptimalTile(std::vector<std::vector<Tile>>& tiles, Orientation currentOrientation, glm::ivec2 currentTile, glm::ivec2 targetTile, bool insideHouse)
{


	Orientation reverseOrientation = UP;
	switch (currentOrientation)
	{
	case RIGHT: reverseOrientation = LEFT; break;
	case LEFT: reverseOrientation = RIGHT; break;
	case UP: reverseOrientation = DOWN; break;
	case DOWN: reverseOrientation = UP; break;
	}

	std::vector<std::pair<Orientation, glm::ivec2>> possibleTiles;
	for (unsigned int i = 0; i < 4; i++)
	{
		Orientation orientation = (Orientation)i;


		if (orientation == reverseOrientation) continue;


		Tile* possibleTile = nullptr;
		int xOffset = 0, yOffset = 0;
		switch (orientation)
		{
		case UP: yOffset--; break;
		case LEFT: xOffset--; break;
		case RIGHT: xOffset++; break;
		case DOWN: yOffset++; break;
		}
		possibleTile = &tiles[currentTile.x + yOffset][currentTile.y + xOffset];
		if (possibleTile->mType != SOLID && (possibleTile->mType != GHOSTDOOR || insideHouse))
		{
			possibleTiles.push_back(std::pair<Orientation, glm::ivec2>(orientation, glm::ivec2(currentTile.x + yOffset, currentTile.y + xOffset)));
		}
	}

	if (possibleTiles.size() < 2)
	{
		return possibleTiles[0].first;
	}

	Orientation bestDirection = UP;
	float bestDistance = 999999;
	for (std::pair<Orientation, glm::ivec2> possibleTile : possibleTiles)
	{
		glm::ivec2 possibleTileCoordinates = possibleTile.second;
		Orientation possibleTileOrientation = possibleTile.first;
		float distance = std::pow((possibleTileCoordinates.x - targetTile.x), 2) + std::pow((possibleTileCoordinates.y - targetTile.y), 2);
		if (distance < bestDistance)
		{
			bestDistance = distance;
			bestDirection = possibleTileOrientation;
		}
		else if (std::abs(distance - bestDistance) < 0.00001)
		{
			if (bestDirection > possibleTileOrientation)
			{
				bestDistance = distance;
				bestDirection = possibleTileOrientation;
			}
		}
	}


	return bestDirection;
}