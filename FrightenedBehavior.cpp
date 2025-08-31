#include "FrightenedBehavior.h"

FrightenedBehavior::FrightenedBehavior() {}

Orientation FrightenedBehavior::chooseRandomTile(std::vector<std::vector<Tile>>& tiles, glm::ivec2 currentTile , Orientation currentOrientation)
{
	Orientation reverseOrientation = UP;
	switch (currentOrientation)
	{
	case RIGHT: reverseOrientation = LEFT; break;
	case LEFT: reverseOrientation = RIGHT; break;
	case UP: reverseOrientation = DOWN; break;
	case DOWN: reverseOrientation = UP; break;
	}

	Orientation direction = static_cast<Orientation>(rand() % 4);
	int xOffset = 0, yOffset = 0;
	switch (direction)
	{
	case UP: yOffset--; break;
	case LEFT: xOffset--; break;
	case RIGHT: xOffset++; break;
	case DOWN: yOffset++; break;
	}

	Tile* possibleTile = &tiles[currentTile.x + yOffset][currentTile.y + xOffset];
	if (direction != reverseOrientation && possibleTile->mType != SOLID && possibleTile->mType != GHOSTDOOR)
		return direction;

	while (true)
	{
		direction = direction == UP ? RIGHT : direction == RIGHT ? DOWN : direction == DOWN ? LEFT : UP;
		xOffset = 0, yOffset = 0;
		switch (direction)
		{
		case UP: yOffset--; break;
		case LEFT: xOffset--; break;
		case RIGHT: xOffset++; break;
		case DOWN: yOffset++; break;
		}
		possibleTile = &tiles[currentTile.x + yOffset][currentTile.y + xOffset];
		if (direction != reverseOrientation && possibleTile->mType != SOLID && possibleTile->mType != GHOSTDOOR)
			return direction;
	}

	return UP;

}

Orientation FrightenedBehavior::chooseTileToGhoseHouse(std::vector<std::vector<Tile>>& tiles, glm::ivec2 currentTile, Orientation currentOrientation, GhostType type)
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
		if (possibleTile->mType != SOLID)
		{
			possibleTiles.push_back(std::pair<Orientation, glm::ivec2>(orientation, glm::ivec2(currentTile.x + yOffset, currentTile.y + xOffset)));
		}
	}

	if (possibleTiles.size() < 2)
	{
		return possibleTiles[0].first;
	}

	glm::ivec2 targetTile(16, 14);
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

glm::ivec2 FrightenedBehavior::ghostHouseTargetTile(GhostType ghostType)
{
	if (ghostType == PINKY)
	{
		return glm::ivec2(18, 14);
	}
	else if (ghostType == INKY)
	{
		return glm::ivec2(18, 12);
	}
	else if (ghostType == CLYDE)
	{
		return glm::ivec2(18, 16);
	}
	
	return glm::ivec2(16, 14);
}