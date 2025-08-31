#include "Tile.h"


extern SpriteRenderer* renderer;

Tile::Tile(glm::vec2 position, glm::ivec2 tileLocation, TileType type) : mPosition(position),
mTileLocation(tileLocation), mType(type)
{

}

Tile::Tile() : mPosition(glm::vec2(0.0f, 0.0f)), mTileLocation(glm::ivec2(0)), mType(SOLID)
{

}

void Tile::render()
{
	ResourceManager* instance = ResourceManager::getInstance();
	
	if (mType == DOT)
	{
		glm::vec2 spriteCoordinates = glm::vec2(8, 8);
		glm::vec2 spriteSize = glm::vec2(8, 8);
		renderer->drawSpriteFromSheet(instance->getTexture("main"), mPosition, spriteCoordinates, spriteSize, glm::vec2(32));
	}
	else if (mType == ENERGIZER)
	{
		glm::vec2 spriteCoordinates = glm::vec2(8, 24);
		glm::vec2 spriteSize = glm::vec2(8, 8);
		renderer->drawSpriteFromSheet(instance->getTexture("main"), mPosition, spriteCoordinates, spriteSize, glm::vec2(32));
	}
	else if (mType == FRUIT)
	{
		glm::vec2 spriteCoordinates = glm::vec2(37, 50);
		glm::vec2 spriteSize = glm::vec2(12 , 12);
		glm::vec2 position = glm::vec2(mPosition.x, mPosition.y - 18.0f);
		renderer->drawSpriteFromSheet(instance->getTexture("objects"), position, spriteCoordinates, spriteSize, glm::vec2(64));
	}
}