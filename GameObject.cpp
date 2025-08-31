#include "GameObject.h"

GameObject::GameObject() :mPosition(0.0f, 0.0f), mSize(64.0f),
mTileCoordinates(0, 0)
{

}

GameObject::GameObject(glm::vec2 position, glm::ivec2 tileCoordinates) :
	mPosition(position), mTileCoordinates(tileCoordinates) , mSize(glm::vec2(32.0f , 32.0f))
{
}

