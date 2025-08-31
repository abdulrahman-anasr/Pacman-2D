#include "Ghost.h"
#include "irrKlang.h"

using namespace irrklang;


extern bool checkCollision(glm::vec2 point, glm::vec2 box, glm::vec2 boxSize);
extern bool checkCenterOfTile(glm::vec2 actorCenter, Tile& tile);
extern bool checkNextTileAvailability(Tile& nextTile);
extern bool checkActorTileCollision(Actor& actor, Tile& tile);

extern ISoundEngine* engine;

std::vector<glm::vec4> Ghost::mFrightenedGhostFlashingSprites;
std::vector<glm::vec4> Ghost::mFrightenedGhostSprites;
std::map<Orientation, std::vector<glm::vec4>> Ghost::mEatenGhostSprites;


Ghost::Ghost() : Actor("blinky"), mNextOrientation(LEFT), mChaseBehavior(BLINKY) {}
Ghost::Ghost(std::string type) : Actor(type) , mNextOrientation(LEFT) , mLeftGhostHouse(false) , 
mPersonalDotCounter(0) , mPersonalDotCounterActivated(false) , mCurrentlyFlashing(false) , mGlobalDotCounterActivated(false)
{
	mGhostState = SCATTER;
	if (type == "blinky")
	{
		mChaseBehavior = ChaseBehavior(BLINKY);
		mType = BLINKY;
		mInsideGhostHouse = false;
		mPersonalDotCounterExitCount = 0;
	}
	else if (type == "inky")
	{
		mChaseBehavior = ChaseBehavior(INKY);
		mType = INKY;
		mInsideGhostHouse = true;
		mPersonalDotCounterExitCount = 30;
	}
	else if (type == "pinky")
	{
		mChaseBehavior = ChaseBehavior(PINKY);
		mType = PINKY;
		mInsideGhostHouse = true;
		mPersonalDotCounterExitCount = 0;
	}
	else
	{
		mChaseBehavior = ChaseBehavior(CLYDE);
		mType = CLYDE;
		mInsideGhostHouse = true;
		mPersonalDotCounterExitCount = 60;
	}

}


void Ghost::render(float timeLeftCurrentState)
{
	if (mGhostState != FRIGHTENED && mGhostState != EATEN)
	{
		float currentXPosition = mPosition.x;
		if (mInsideGhostHouse) mPosition.x = mPosition.x - 16.0f;
		Actor::render();
		mPosition.x = currentXPosition;
	}
	else if(mGhostState == FRIGHTENED)
	{
		ResourceManager* instance = ResourceManager::getInstance();

		std::vector<glm::vec4>& spriteSheet = mCurrentlyFlashing ? mFrightenedGhostFlashingSprites : mFrightenedGhostSprites;
		int currentSprite = (mFrame / 48) % (spriteSheet.size());

		if ((mFrame / 48) >= spriteSheet.size())
		{
			mFrame = 0;
		}

		glm::vec4& currentSpriteData = spriteSheet[currentSprite];
		glm::vec2 spriteCoords = glm::vec2(currentSpriteData.x, currentSpriteData.y);
		glm::vec2 spriteSize = glm::vec2(currentSpriteData.z, currentSpriteData.w);

		float currentXPosition = mPosition.x;
		if (mInsideGhostHouse) mPosition.x = mPosition.x - 16.0f;
		renderer->drawSpriteFromSheet(instance->getTexture("objects"), mPosition, spriteCoords, spriteSize, mSize);
		mPosition.x = currentXPosition;
	}
	else if (mGhostState == EATEN)
	{
		ResourceManager* instance = ResourceManager::getInstance();


		int currentSprite = (mFrame / 48) % (mEatenGhostSprites[mOrientation].size());

		if ((mFrame / 48) >= mEatenGhostSprites[mOrientation].size())
		{
			mFrame = 0;
		}

		glm::vec4& currentSpriteData = mEatenGhostSprites[mOrientation][currentSprite];
		glm::vec2 spriteCoords = glm::vec2(currentSpriteData.x, currentSpriteData.y);
		glm::vec2 spriteSize = glm::vec2(currentSpriteData.z, currentSpriteData.w);


		renderer->drawSpriteFromSheet(instance->getTexture("objects"), mPosition, spriteCoords, spriteSize, mSize);
	}

	
}
void Ghost::update(float deltaTime, std::vector<std::vector<Tile>>& tiles , glm::ivec2 pacmanTile, glm::ivec2 blinkyTile , Orientation pacmanOrientation)
{
	if (mGhostState == EATEN)
	{
		mVelocity = glm::vec2(750.0f);
	}
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

	if (actorTileCoordinates.y + columnOffset > 0 && actorTileCoordinates.y + columnOffset < tiles[0].size() - 1)
	{
		checkActorTileCollision(*this, tiles[actorTileCoordinates.x + rowOffset][actorTileCoordinates.y + columnOffset]);
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

			}
		}
	
	}

	if (checkCenterOfTile(actorCenter, currentTile) && mPreviousTileCalculated == mTileCoordinates)
	{
		calculateNextMovement(tiles, pacmanTile, blinkyTile, pacmanOrientation);
	}
	
}


void Ghost::reorientGhost(std::vector<std::vector<Tile>>& tiles, glm::ivec2 pacmanTile, glm::ivec2 blinkyTile, Orientation pacmanOrientation)
{
	Orientation potentialOrientation = mOrientation == RIGHT ? LEFT : mOrientation == LEFT ? RIGHT : mOrientation == UP ? DOWN : UP;

	int xOffset = potentialOrientation == RIGHT ? 1 : potentialOrientation == LEFT ? -1 : 0;
	int yOffset = potentialOrientation == DOWN ? 1 : potentialOrientation == UP ? -1 : 0;

	if (tiles[mTileCoordinates.x + yOffset][mTileCoordinates.y + xOffset].mType != SOLID && 
		tiles[mTileCoordinates.x + yOffset][mTileCoordinates.y + xOffset].mType != GHOSTDOOR)
	{
		mNextOrientation = potentialOrientation;
	}
	Tile& currentTile = tiles[mTileCoordinates.x][mTileCoordinates.y];
	mPosition.x = currentTile.mPosition.x - 16.0f;
	mPosition.y = currentTile.mPosition.y - 16.0f;

	calculateNextMovement(tiles , pacmanTile , blinkyTile , pacmanOrientation);
}

void Ghost::checkLeaveHouse()
{
	if (mPersonalDotCounterActivated && mPersonalDotCounter >= mPersonalDotCounterExitCount)
	{
		mInsideGhostHouse = false;
		mPersonalDotCounterActivated = false;
	}
}

void Ghost::calculateNextMovement(std::vector<std::vector<Tile>>& tiles, glm::ivec2 pacmanTile, glm::ivec2 blinkyTile, Orientation pacmanOrientation)
{
	if (mInsideGhostHouse)
	{
		mOrientation = mNextOrientation;

		int xOffset = mOrientation == RIGHT ? 1 : mOrientation == LEFT ? -1 : 0;
		int yOffset = mOrientation == DOWN ? 1 : mOrientation == UP ? -1 : 0;

		glm::ivec2 nextTileCoordinates = glm::ivec2(mTileCoordinates.x + yOffset, mTileCoordinates.y + xOffset);

		if (tiles[nextTileCoordinates.x][nextTileCoordinates.y].mType == SOLID || tiles[nextTileCoordinates.x][nextTileCoordinates.y].mType == GHOSTDOOR)
		{
			if (mOrientation == UP) mNextOrientation = DOWN;
			else if (mOrientation == DOWN) mNextOrientation = UP;
		}

		mPreviousTileCalculated = nextTileCoordinates;
	}
	else
	{

		if (mTileCoordinates == glm::ivec2(14, 11) && mGhostState != EATEN && !mInsideGhostHouse)
		{
			mLeftGhostHouse = true;
		}

		bool insideGhostHouse = false;
		if (tiles[mTileCoordinates.x][mTileCoordinates.y].mType == GHOSTHOUSE)
		{
			if (mTileCoordinates.y != 13)
			{
				insideGhostHouse = true;
				mTargetTile = glm::ivec2(mTileCoordinates.x, 13);

				if (mTileCoordinates.y > 13) mNextOrientation = LEFT;
				else mNextOrientation = RIGHT;
			}
			else
			{
				insideGhostHouse = true;

				mTargetTile = glm::ivec2(15, 13);
			}
		}
		else if (tiles[mTileCoordinates.x][mTileCoordinates.y].mType == GHOSTDOOR || !mLeftGhostHouse)
		{
			mTargetTile = glm::ivec2(14, 11);
		}
		else if (mGhostState == SCATTER)
		{
			mTargetTile = mScatterBehavior.calculateTargetTile(mType);
		}
		else if (mGhostState == CHASE)
		{
			mTargetTile = mChaseBehavior.calculateTargetTile(pacmanTile, pacmanOrientation, this->mTileCoordinates, blinkyTile);
		}

		mOrientation = mNextOrientation;

		int xOffset = mOrientation == RIGHT ? 1 : mOrientation == LEFT ? -1 : 0;
		int yOffset = mOrientation == DOWN ? 1 : mOrientation == UP ? -1 : 0;

		glm::ivec2 nextTileCoordinates = glm::ivec2(mTileCoordinates.x + yOffset, mTileCoordinates.y + xOffset);

		int maxWidthIndex = tiles[0].size() - 1;
		if (nextTileCoordinates.y == 0)
		{
			nextTileCoordinates.y = maxWidthIndex;
			mNextOrientation = LEFT;
		}
		else if (nextTileCoordinates.y == maxWidthIndex)
		{
			nextTileCoordinates.y = 0;
			mNextOrientation = RIGHT;
		}
		else
		{
			if (mGhostState == FRIGHTENED && mLeftGhostHouse)
			{
				mNextOrientation = mFrightenedBehavior.chooseRandomTile(tiles, nextTileCoordinates, mOrientation);
			}
			else if (mGhostState == EATEN)
			{
				if (tiles[mTileCoordinates.x][mTileCoordinates.y].mType == GHOSTHOUSE)
				{

					if (mGlobalDotCounterActivated)
					{
						mLeftGhostHouse = false;
						mInsideGhostHouse = true;
						nextTileCoordinates = glm::ivec2(mTileCoordinates.x + 1, mTileCoordinates.y);
						mOrientation = DOWN;
						mNextOrientation = DOWN;
						mGhostState = CHASE;
						mVelocity = glm::vec2(180.0f);
						return;
					}
					else
					{
						mOrientation = UP;
						mNextOrientation = UP;
						nextTileCoordinates = glm::ivec2(mTileCoordinates.x - 1, mTileCoordinates.y);
						mGhostState = CHASE;
						mVelocity = glm::vec2(180.0f);
					}
					
				}
				else
				{
					mNextOrientation = mFrightenedBehavior.chooseTileToGhoseHouse(tiles, nextTileCoordinates, mOrientation , mType);
				}
			}
			else
			{
				mNextOrientation = mChaseBehavior.calculateOptimalTile(tiles, mOrientation, nextTileCoordinates, mTargetTile, insideGhostHouse);
			}
		}
		mPreviousTileCalculated = nextTileCoordinates;
	}
}