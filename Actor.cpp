#include "Actor.h"
#include "irrKlang.h"

using namespace irrklang;


extern bool checkCollision(glm::vec2 point, glm::vec2 box, glm::vec2 boxSize);
extern bool checkCenterOfTile(glm::vec2 actorCenter, Tile& tile);
extern bool checkNextTileAvailability(Tile& nextTile);
extern bool checkActorTileCollision(Actor& actor, Tile& tile);


extern ISoundEngine* engine;

bool currentlyWa = true;

std::vector<glm::vec4> Actor::mPacmanDeathSprites;

Actor::Actor(std::string type) : GameObject(), mOrientation(LEFT), mVelocity(77.7 * 4, 77.7 * 4), mBlocked(false) , mCurrentlyInAnimation(false),
	mSkipFrame(false) , mFramesSkipped(0)
{
	mFrame = 0;

	if (type == "pacman")
	{
		mCharacterSprites[UP].push_back(glm::vec4(36, 1, 13, 13));
		mCharacterSprites[UP].push_back(glm::vec4(4, 33, 13, 13));
		mCharacterSprites[UP].push_back(glm::vec4(20, 33, 13, 13));

		mCharacterSprites[RIGHT].push_back(glm::vec4(36, 1, 13, 13));
		mCharacterSprites[RIGHT].push_back(glm::vec4(4, 1, 13, 13));
		mCharacterSprites[RIGHT].push_back(glm::vec4(20, 1, 13, 13));

		mCharacterSprites[LEFT].push_back(glm::vec4(36, 1, 13, 13));
		mCharacterSprites[LEFT].push_back(glm::vec4(4, 17, 13, 13));
		mCharacterSprites[LEFT].push_back(glm::vec4(20, 17, 13, 13));

		mCharacterSprites[DOWN].push_back(glm::vec4(36, 1, 13, 13));
		mCharacterSprites[DOWN].push_back(glm::vec4(4, 49, 13, 13));
		mCharacterSprites[DOWN].push_back(glm::vec4(20, 49, 13, 13));
	}
	else if (type == "blinky")
	{
		mCharacterSprites[UP].push_back(glm::vec4(68, 65, 14, 14));
		mCharacterSprites[UP].push_back(glm::vec4(84, 65, 14, 14));

		mCharacterSprites[RIGHT].push_back(glm::vec4(4, 65, 14, 14));
		mCharacterSprites[RIGHT].push_back(glm::vec4(20, 65, 14, 14));

		mCharacterSprites[LEFT].push_back(glm::vec4(36, 65, 14, 14));
		mCharacterSprites[LEFT].push_back(glm::vec4(52, 65, 14, 14));

		mCharacterSprites[DOWN].push_back(glm::vec4(100, 65, 14, 14));
		mCharacterSprites[DOWN].push_back(glm::vec4(116, 65, 14, 14));
	}
	else if (type == "pinky")
	{
		mCharacterSprites[UP].push_back(glm::vec4(68, 81, 14, 14));
		mCharacterSprites[UP].push_back(glm::vec4(84, 81, 14, 14));

		mCharacterSprites[RIGHT].push_back(glm::vec4(4, 81, 14, 14));
		mCharacterSprites[RIGHT].push_back(glm::vec4(20, 81, 14, 14));

		mCharacterSprites[LEFT].push_back(glm::vec4(36, 81, 14, 14));
		mCharacterSprites[LEFT].push_back(glm::vec4(52, 81, 14, 14));

		mCharacterSprites[DOWN].push_back(glm::vec4(100, 81, 14, 14));
		mCharacterSprites[DOWN].push_back(glm::vec4(116, 81, 14, 14));
	}
	else if (type == "inky")
	{
		mCharacterSprites[UP].push_back(glm::vec4(68, 97, 14, 14));
		mCharacterSprites[UP].push_back(glm::vec4(84, 97, 14, 14));

		mCharacterSprites[RIGHT].push_back(glm::vec4(4, 97, 14, 14));
		mCharacterSprites[RIGHT].push_back(glm::vec4(20, 97, 14, 14));

		mCharacterSprites[LEFT].push_back(glm::vec4(36, 97, 14, 14));
		mCharacterSprites[LEFT].push_back(glm::vec4(52, 97, 14, 14));

		mCharacterSprites[DOWN].push_back(glm::vec4(100, 97, 14, 14));
		mCharacterSprites[DOWN].push_back(glm::vec4(116, 97, 14, 14));
	}
	else if (type == "clyde")
	{
		mCharacterSprites[UP].push_back(glm::vec4(68, 113, 14, 14));
		mCharacterSprites[UP].push_back(glm::vec4(84, 113, 14, 14));

		mCharacterSprites[RIGHT].push_back(glm::vec4(4, 113, 14, 14));
		mCharacterSprites[RIGHT].push_back(glm::vec4(20, 113, 14, 14));

		mCharacterSprites[LEFT].push_back(glm::vec4(36, 113, 14, 14));
		mCharacterSprites[LEFT].push_back(glm::vec4(52, 113, 14, 14));

		mCharacterSprites[DOWN].push_back(glm::vec4(100, 113, 14, 14));
		mCharacterSprites[DOWN].push_back(glm::vec4(116, 113, 14, 14));
	}
};

ActorUpdate Actor::update(float deltaTime, std::vector<std::vector<Tile>>& tiles)
{
	if (mSkipFrame)
	{
		mFramesSkipped++;
		if (mFramesSkipped >= 3)
		{
			mFramesSkipped = 0;
			mSkipFrame = false;
		}
		return std::make_tuple(false, 0);
	}

	bool energizerEaten = false;
	int points = 0;
	glm::ivec2 actorTileCoordinates = this->mTileCoordinates;
	glm::vec2 actorCenter = glm::vec2(this->mPosition.x + (this->mSize.x / 2.0)
		, this->mPosition.y + (this->mSize.y / 2.0));

	Tile& currentTile = tiles[this->mTileCoordinates.x][this->mTileCoordinates.y];

	glm::vec2 distanceMoved = glm::vec2(0.0f, 0.0f);
	glm::vec2 actorVelocity = this->mVelocity;
	int rowOffset = 0, columnOffset = 0;
	switch (this->mOrientation)
	{
	case LEFT:
		distanceMoved.x -= actorVelocity.x * deltaTime;
		columnOffset--;
		break;
	case RIGHT:
		distanceMoved.x += actorVelocity.x * deltaTime;
		columnOffset++;
		break;
	case DOWN:
		distanceMoved.y += actorVelocity.y * deltaTime;
		rowOffset++;
		break;
	case UP:
		distanceMoved.y -= actorVelocity.y * deltaTime;
		rowOffset--;
		break;
	}

	this->mFrame++;
	this->mPosition += distanceMoved;
	if (checkCenterOfTile(actorCenter, currentTile))
	{
		if (!checkNextTileAvailability(tiles[actorTileCoordinates.x + rowOffset][actorTileCoordinates.y + columnOffset]))
		{
			this->mPosition -= distanceMoved;
			this->mFrame--;
		}
	}
	if (actorTileCoordinates.y + columnOffset > 0 && actorTileCoordinates.y + columnOffset < tiles[0].size() - 1)
	{
		if (checkActorTileCollision(*this, tiles[actorTileCoordinates.x + rowOffset][actorTileCoordinates.y + columnOffset]))
		{
			if (tiles[actorTileCoordinates.x + rowOffset][actorTileCoordinates.y + columnOffset].mType == ENERGIZER)
			{
				points = 50;
				energizerEaten = true;
			}
			else if(tiles[actorTileCoordinates.x + rowOffset][actorTileCoordinates.y + columnOffset].mType == DOT)
			{
				points = 10;
			}
			else if (tiles[actorTileCoordinates.x + rowOffset][actorTileCoordinates.y + columnOffset].mType == FRUIT)
			{
				points = 100;
			}
			tiles[actorTileCoordinates.x + rowOffset][actorTileCoordinates.y + columnOffset].mType = NORMAL;
		}
	}
	else
	{
		if (actorTileCoordinates.y + columnOffset == 0)
		{
			if (checkCollision(actorCenter, glm::vec2(-36.0f, currentTile.mPosition.y), glm::vec2(36.0f)))
			{
				glm::ivec2 newTileCoordinates = glm::ivec2(actorTileCoordinates.x, tiles[0].size() - 1);
				Tile& edgeTile = tiles[actorTileCoordinates.x][actorTileCoordinates.y + columnOffset];
				Tile& newEdgeTile = tiles[newTileCoordinates.x][newTileCoordinates.y];
				this->mPosition.x = newEdgeTile.mPosition.x - 16.0f;
				this->mPosition.y = newEdgeTile.mPosition.y - 16.0f;
				this->mTileCoordinates = newTileCoordinates;


				if (edgeTile.mType == DOT)
				{
					points += 10;
					edgeTile.mType = NORMAL;
				}
				if (newEdgeTile.mType == DOT)
				{
					points += 10;
					newEdgeTile.mType = NORMAL;
				}
			}
		}
		else if (actorTileCoordinates.y + columnOffset == tiles[0].size() - 1)
		{
			if (checkCollision(actorCenter, glm::vec2(currentTile.mPosition.x + 36.0f, currentTile.mPosition.y), glm::vec2(36.0f)))
			{
				glm::ivec2 newTileCoordinates = glm::ivec2(actorTileCoordinates.x, 0);
				Tile& edgeTile = tiles[actorTileCoordinates.x][actorTileCoordinates.y + columnOffset];
				Tile& newEdgeTile = tiles[newTileCoordinates.x][newTileCoordinates.y];
				this->mPosition.x = newEdgeTile.mPosition.x - 16.0f;
				this->mPosition.y = newEdgeTile.mPosition.y - 16.0f;
				this->mTileCoordinates = newTileCoordinates;

				if (edgeTile.mType == DOT)
				{
					points += 10;
					edgeTile.mType = NORMAL;
				}
				if (newEdgeTile.mType == DOT)
				{
					points += 10;
					newEdgeTile.mType = NORMAL;
				}
			}
		}
	}
	if (points == 10)
	{
		mSkipFrame = true;
		if (currentlyWa)
		{
			engine->play2D("audio/wa.mp3");
		}
		else
		{
			engine->play2D("audio/ka.mp3");
		}
		currentlyWa = !currentlyWa;
	}
	else if (points == 100)
	{
		mSkipFrame = true;

	}
	ActorUpdate value = std::make_tuple(energizerEaten, points);
	return value;
}

void Actor::render()
{
	ResourceManager* instance = ResourceManager::getInstance();

	if (mCurrentlyInAnimation)
	{
		int currentSprite = (mFrame / 32) % (mPacmanDeathSprites.size());
		if ((mFrame / 32) >= mPacmanDeathSprites.size())
		{
			mCurrentlyInAnimation = false;
		}

		glm::vec4& currentSpriteData = mPacmanDeathSprites[currentSprite];
		glm::vec2 spriteCoords = glm::vec2(currentSpriteData.x, currentSpriteData.y);
		glm::vec2 spriteSize = glm::vec2(currentSpriteData.z, currentSpriteData.w);


		renderer->drawSpriteFromSheet(instance->getTexture("objects"), mPosition, spriteCoords, spriteSize, mSize);
	}
	else
	{
		int currentSprite = (mFrame / 16) % (mCharacterSprites[mOrientation].size());

		if ((mFrame / 16) >= mCharacterSprites[mOrientation].size())
		{
			mFrame = 0;
		}

		glm::vec4& currentSpriteData = mCharacterSprites[mOrientation][currentSprite];
		glm::vec2 spriteCoords = glm::vec2(currentSpriteData.x, currentSpriteData.y);
		glm::vec2 spriteSize = glm::vec2(currentSpriteData.z, currentSpriteData.w);


		renderer->drawSpriteFromSheet(instance->getTexture("objects"), mPosition, spriteCoords, spriteSize, mSize);
	}
	




}