#include "Game.h"
#include "irrKlang.h"

using namespace irrklang;

SpriteRenderer* renderer;
ResourceManager* instance;
TextRenderer* textRenderer;
extern GLFWwindow* window;

ISoundEngine* engine = createIrrKlangDevice();

std::vector<glm::vec4> Game::mGhostsEatenScoreSprites;

constexpr glm::vec2 MAX_SPEED(75.75757625 * 4, 75.75757625 * 4);

bool playingIntro = false;

std::string numberToString(int number)
{
	switch (number)
	{
	case 1: return "one.level"; break;
	case 2: return "two.level"; break;
	case 3: return "three.level"; break;
	case 4: return "four.level"; break;
	case 5: return "five.level"; break;
	default : return "six.level"; break;
	}
}

bool checkCollision(glm::vec2 point, glm::vec2 box, glm::vec2 boxSize)
{
	bool xCollision = point.x > box.x && point.x < (box.x + boxSize.x);
	bool yCollision = point.y > box.y && point.y < (box.y + boxSize.y);

	return xCollision && yCollision;
}

bool checkCenterOfTile(glm::vec2 actorCenter, Tile& tile)
{
	bool isXCenter = glm::abs(actorCenter.x - (tile.mPosition.x + 16.0f)) < 5.0f;
	bool isYCenter = glm::abs(actorCenter.y - (tile.mPosition.y + 16.0f)) < 5.0f;
	return isXCenter && isYCenter;
}
bool checkNextTileAvailability(Tile& nextTile)
{
	return nextTile.mType != SOLID && nextTile.mType != GHOSTDOOR;
}

bool checkActorTileCollision(Actor& actor, Tile& tile)
{
	glm::vec2 actorCenter = glm::vec2(actor.mPosition.x + (actor.mSize.x / 2.0), actor.mPosition.y + (actor.mSize.y / 2.0));

	if (checkCollision(actorCenter, tile.mPosition, glm::vec2(32.0f)))
	{
		actor.mTileCoordinates = tile.mTileLocation;
		return true;
	}

	return false;
}

bool checkPacmanGhostCollision(Actor& actor, Ghost& ghost)
{
	if (actor.mTileCoordinates == ghost.mTileCoordinates)
	{
		return true;
	}
	return false;
}

Game::Game(unsigned int width , unsigned int height) : mWidth(width) , mHeight(height) , mKeys() , mKeysProcessed(), mGameState(GAME_ACTIVE) ,
	 mCurrentLevel(1) , mCurrentPoints(0) , mLevel("levels/one.level"), mLivesLeft(3),
	mKeysProcessedFrames(), mAverageFrameRate(60) , mPacman("pacman") , mGhosts() , mAnimationActive(false) , mEatingAnimationActive(false),
	mCurrentGhostsAte(0) , mLastGhostEatenIndex(-1) , mCurrentFrame1Up(0) , mHighScorePoints(0)
{
}

void Game::init()
{
	mCurrentLevel = 1;
	mCurrentPoints = 0;
	mLevel = Level("levels/one.level");
	mLivesLeft = 3;
	mCurrentGhostsAte = 0;
	mLastGhostEatenIndex = -1;

	instance = ResourceManager::getInstance();

	instance->loadTexture("main", "textures/MainSprite.png", false);

	instance->loadTexture("white_map", "textures/white_map.png", true);

	instance->loadTexture("blue_map", "textures/blue_map.png", true);

	instance->loadTexture("objects", "textures/objects.png", true);

	instance->loadTexture("block", "textures/block.png", false);

	instance->loadShader("main", "sprite.vs", "sprite.fs", nullptr);

	textRenderer = new TextRenderer(mWidth, mHeight);
	textRenderer->load("fonts/pacfont.ttf", 28);

	std::srand(55);

	Ghost::mFrightenedGhostSprites.push_back(glm::vec4(132, 65, 14, 14));
	Ghost::mFrightenedGhostSprites.push_back(glm::vec4(148, 65, 14, 14));

	Ghost::mFrightenedGhostFlashingSprites.push_back(glm::vec4(164 , 65 , 14 , 14));
	Ghost::mFrightenedGhostFlashingSprites.push_back(glm::vec4(180 , 65 , 14 , 14));

	Ghost::mEatenGhostSprites[RIGHT].push_back(glm::vec4(132, 81, 14, 14));
	Ghost::mEatenGhostSprites[LEFT].push_back(glm::vec4(148, 81, 14, 14));
	Ghost::mEatenGhostSprites[UP].push_back(glm::vec4(164, 81, 14, 14));
	Ghost::mEatenGhostSprites[DOWN].push_back(glm::vec4(180, 81, 14, 14));

	Actor::mPacmanDeathSprites.push_back(glm::vec4(52 , 1 , 13 , 13));
	Actor::mPacmanDeathSprites.push_back(glm::vec4(67 , 1 , 15 , 11));
	Actor::mPacmanDeathSprites.push_back(glm::vec4(83 , 1 , 15 , 11));
	Actor::mPacmanDeathSprites.push_back(glm::vec4(99 , 1 , 15 , 11));
	Actor::mPacmanDeathSprites.push_back(glm::vec4(115 , 1 , 15 , 12));
	Actor::mPacmanDeathSprites.push_back(glm::vec4(131 , 1 , 15 , 13));
	Actor::mPacmanDeathSprites.push_back(glm::vec4(148 , 1 , 13 , 14));
	Actor::mPacmanDeathSprites.push_back(glm::vec4(164 , 1 , 13 , 14));
	Actor::mPacmanDeathSprites.push_back(glm::vec4(180 , 1 , 13 , 14));
	Actor::mPacmanDeathSprites.push_back(glm::vec4(196 , 1 , 13 , 15));
	Actor::mPacmanDeathSprites.push_back(glm::vec4(212 , 1 , 13 , 16));
	Actor::mPacmanDeathSprites.push_back(glm::vec4(165, 30, 13, 16));
	Actor::mPacmanDeathSprites.push_back(glm::vec4(165, 30, 13, 16));

	Game::mGhostsEatenScoreSprites.push_back(glm::vec4(3, 133, 15, 7));
	Game::mGhostsEatenScoreSprites.push_back(glm::vec4(19, 133, 15, 7));
	Game::mGhostsEatenScoreSprites.push_back(glm::vec4(35, 133, 15, 7));
	Game::mGhostsEatenScoreSprites.push_back(glm::vec4(51, 133, 16, 7));


	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(mWidth), static_cast<float>(mHeight), 0.0f, -1.0f, 1.0f);
	instance->getShader("main").use().setMat4("projection", projection);
	instance->getShader("main").use().setInteger("spriteTexture", 0);

	renderer = new SpriteRenderer(instance->getShader("main"));

	mPacman.mPosition.x = mLevel.mTiles[26][13].mPosition.x;
	mPacman.mPosition.y = mLevel.mTiles[26][13].mPosition.y - 16.0f;

	mPacman.mTileCoordinates = glm::ivec2(26, 13);

	std::string ghostNames[4] = {"blinky", "inky", "pinky", "clyde"};
	for (int i = 0; i < 4; i++)
	{
		mGhosts[i] = Ghost(ghostNames[i]);
		
		if (ghostNames[i] == "blinky")
		{
			mGhosts[i].mPosition.x = mLevel.mTiles[14][13].mPosition.x - 16.0f;
			mGhosts[i].mPosition.y = mLevel.mTiles[14][13].mPosition.y - 16.0f;
			mGhosts[i].mTileCoordinates = glm::ivec2(14, 13);
			mGhosts[i].mPreviousTileCalculated = glm::ivec2(14, 12);
			mGhosts[i].mOrientation = LEFT;
			mGhosts[i].mNextOrientation = LEFT;
		}
		else if (ghostNames[i] == "inky")
		{
			mGhosts[i].mPosition.x = mLevel.mTiles[17][12].mPosition.x - 16.0f;
			mGhosts[i].mPosition.y = mLevel.mTiles[17][12].mPosition.y - 16.0f;
			mGhosts[i].mTileCoordinates = glm::ivec2(17, 12);
			mGhosts[i].mPreviousTileCalculated = glm::ivec2(18, 12);
			mGhosts[i].mOrientation = DOWN;
			mGhosts[i].mNextOrientation = DOWN;
		}
		else if (ghostNames[i] == "pinky")
		{
			mGhosts[i].mPosition.x = mLevel.mTiles[17][14].mPosition.x - 16.0f;
			mGhosts[i].mPosition.y = mLevel.mTiles[17][14].mPosition.y - 16.0f;
			mGhosts[i].mTileCoordinates = glm::ivec2(17, 14);
			mGhosts[i].mPreviousTileCalculated = glm::ivec2(18, 14);
			mGhosts[i].mOrientation = DOWN;
			mGhosts[i].mNextOrientation = DOWN;
		}
		else if (ghostNames[i] == "clyde")
		{
			mGhosts[i].mPosition.x = mLevel.mTiles[17][16].mPosition.x - 16.0f;
			mGhosts[i].mPosition.y = mLevel.mTiles[17][16].mPosition.y - 16.0f;
			mGhosts[i].mTileCoordinates = glm::ivec2(17, 16);
			mGhosts[i].mPreviousTileCalculated = glm::ivec2(18, 16);
			mGhosts[i].mOrientation = DOWN;
			mGhosts[i].mNextOrientation = DOWN;
		}

		mGhosts[i].mVelocity = MAX_SPEED * mLevel.mGhostSpeed;

	}

	mPacman.mVelocity = MAX_SPEED * mLevel.mPacmanSpeed;

	mTimeBeforeAnimationStart = 4.5f;
	mGameStarted = false;
	mAnimationActive = true;
	
}

void Game::render()
{

	if (mCurrentLevelCompleted && mTimeBeforeAnimationStart < 5.0f)
	{
		int timeDisplay = mTimeBeforeAnimationStart * 100.0f;
		if (((timeDisplay / 50) & 1) == 0)
		{
			
			renderer->drawSprite(instance->getTexture("white_map"), glm::vec2(0.0f, static_cast<float>(3 * 8 * 4)), glm::vec2(28 * 8 * 4, 31 * 8 * 4));
		}
		else
		{
			renderer->drawSprite(instance->getTexture("blue_map"), glm::vec2(0.0f, static_cast<float>(3 * 8 * 4)), glm::vec2(28 * 8 * 4, 31 * 8 * 4));
		}
		return;
	}

	renderer->drawSpriteFromSheet(instance->getTexture("main"), glm::vec2(0.0f, static_cast<float>(3 * 8 * 4)), glm::vec2(228.0f, 0.0f), glm::vec2(224.0f, 248.0f), glm::vec2(28 * 8 * 4, 31 * 8 * 4));

	if (mGameOver)
	{
		textRenderer->renderText("GAME OVER", 250, 630, 1.5, glm::vec3(1.0f, 0.0f, 0.0f));
		return;
	}

	mLevel.render();

	float xLocation = 20;
	float yLocation = (36 * 8 * 4) - 55;
	for (int i = 0; i < mLivesLeft; i++)
	{
		glm::vec4 spriteData = mPacman.mCharacterSprites[LEFT][2];
		renderer->drawSpriteFromSheet(instance->getTexture("objects"), glm::vec2(xLocation, yLocation),
			glm::vec2(spriteData.x, spriteData.y), glm::vec2(spriteData.z, spriteData.w), glm::vec2(48.0f));
		xLocation += 64.0f;
	}


	if (!mEatingAnimationActive)
	{
		mPacman.render();
	}

	if (!mPacman.mCurrentlyInAnimation)
	{
		for (int i = 0; i < 4; i++)
		{
			glm::vec3 color(1.0f);
			switch (mGhosts[i].mType)
			{
			case BLINKY: color = glm::vec3(1.0f, 0.0f, 0.0f); break;
			case INKY: color = glm::vec3(0.0, 0.0, 1.0f); break;
			case PINKY: color = glm::vec3(1.0f, 0.0, 1.0); break;
			case CLYDE: color = glm::vec3(1.0f, 0.5f, 0.0); break;
			}
			if (mGhosts[i].mGhostState == EATEN && mLastGhostEatenIndex == i)
			{
				glm::vec4 spriteData = mGhostsEatenScoreSprites[mCurrentGhostsAte - 1];
				renderer->drawSpriteFromSheet(instance->getTexture("objects"), glm::vec2(mGhosts[i].mPosition.x , mGhosts[i].mPosition.y + 15),
					glm::vec2(spriteData.x, spriteData.y), glm::vec2(spriteData.z, spriteData.w) , glm::vec2(64 , 32));
			}
			else
			{
				mGhosts[i].render();
			}
		}
	}

	if (!mGameStarted && mAnimationActive)
	{
		textRenderer->renderText("READY!", 320, 630, 1.5, glm::vec3(1, 0.984, 0));
	}

	if ((((mCurrentFrame1Up / 72) % 2) == 0) || !mGameStarted)
	{
		textRenderer->renderText("1UP", 90, 0, 1.2);
		if (mGameStarted) mCurrentFrame1Up++;
		
	}
	else
	{
		if (mGameStarted) mCurrentFrame1Up++;
		if (mCurrentFrame1Up / 72 >= 2)
		{
			mCurrentFrame1Up = 0;
		}
	}

	textRenderer->renderText(std::to_string(mCurrentPoints), 150, 40, 1.2);

	textRenderer->renderText("HIGH SCORE", 280, 0, 1.2);

	textRenderer->renderText(std::to_string(mHighScorePoints), 380, 40, 1.2);

}

void Game::update(float deltaTime)
{


	if (mCurrentLevelCompleted)
	{
		mTimeBeforeAnimationStart -= deltaTime;
		if (mTimeBeforeAnimationStart < 0.0f)
		{
			mCurrentLevel++;
			std::string path = numberToString(mCurrentLevel);
			mLevel.setupEntireLevel("levels/" + path);
			this->resetLevel();
			mCurrentLevelCompleted = false;

		}
		return;
	}
	if (!mAnimationActive)
	{


		std::map<GhostType, bool> leftGhostHouse;
		for (int i = 0; i < 4; i++)
		{
			if (mEatingAnimationActive)
			{
				if (mGhosts[i].mGhostState == EATEN && i != mLastGhostEatenIndex)
				{
					mGhosts[i].update(deltaTime, mLevel.mTiles, mPacman.mTileCoordinates, mGhosts[0].mTileCoordinates, mPacman.mOrientation);
				}
			}
			else
			{
				GhostState ghostStateBeforeUpdate = mGhosts[i].mGhostState;
				glm::ivec2 previousTileLocation = mGhosts[i].mTileCoordinates;
				mGhosts[i].update(deltaTime, mLevel.mTiles, mPacman.mTileCoordinates, mGhosts[0].mTileCoordinates, mPacman.mOrientation);
				if (mGhosts[i].mTileCoordinates != previousTileLocation)
				{
					if (mGhosts[i].mGhostState != EATEN)
					{
						if (mGhosts[i].mTileCoordinates.x == 17 && (mGhosts[i].mTileCoordinates.y < 5 ||
							mGhosts[i].mTileCoordinates.y > 21))
						{
							mGhosts[i].mVelocity = MAX_SPEED * mLevel.mGhostTunnelSpeed;
						}
						else if (previousTileLocation.x == 17 && (previousTileLocation.y < 5 ||
							previousTileLocation.y > 21) && (mGhosts[i].mTileCoordinates.y > 5 ||
								mGhosts[i].mTileCoordinates.y < 21))
						{
							if (mGhosts[i].mGhostState == FRIGHTENED)
							{
								mGhosts[i].mVelocity = MAX_SPEED * mLevel.mFrightGhostSpeed;
							}
							else
							{
								if (i == 0)
								{
									if (mLevel.mElroyTwoActive)
									{
										mGhosts[i].mVelocity = MAX_SPEED * mLevel.mElroyTwoSpeed;
									}
									else if (mLevel.mElroyOneActive)
									{
										mGhosts[i].mVelocity = MAX_SPEED * mLevel.mElroyOneSpeed;
									}
									else
									{
										mGhosts[i].mVelocity = MAX_SPEED * mLevel.mGhostSpeed;
									}
								}
								else
								{
									mGhosts[i].mVelocity = MAX_SPEED * mLevel.mGhostSpeed;
								}
								
							}
						}
					}
				}
				if (ghostStateBeforeUpdate == EATEN && mGhosts[i].mGhostState != ghostStateBeforeUpdate)
				{
					mGhosts[i].mVelocity = MAX_SPEED * mLevel.mGhostSpeed;

					bool anyGhostEaten = false;
					for (int j = 0; j < 4; j++)
					{
						if (j == i) continue;

						if (mGhosts[j].mGhostState == EATEN)
						{
							anyGhostEaten = true;
						}
					}
					if (!anyGhostEaten)
					{
						engine->stopAllSounds();
						engine->play2D("audio/ghosts_scared.mp3", true);
					}
				}
			}
			leftGhostHouse[mGhosts[i].mType] = mGhosts[i].mLeftGhostHouse;
		}

		if (!mEatingAnimationActive)
		{

			ActorUpdate update = mPacman.update(deltaTime, mLevel.mTiles);

			if (std::get<0>(update) == true)
			{
				engine->stopAllSounds();
				engine->play2D("audio/ghosts_scared.mp3" , true);
				mLevel.changeToFrightenedState();
				mCurrentGhostsAte = 0;
				mPacman.mVelocity = MAX_SPEED * mLevel.mFrightPacmanSpeed;
				for (unsigned int i = 0; i < 4; i++)
				{
					mGhosts[i].mCurrentlyFlashing = false;
					if (mGhosts[i].mGhostState != EATEN)
					{
						mGhosts[i].mGhostState = FRIGHTENED;
						mGhosts[i].reorientGhost(mLevel.mTiles, mPacman.mTileCoordinates, mGhosts[0].mTileCoordinates, mPacman.mOrientation);
						mGhosts[i].mVelocity = MAX_SPEED * mLevel.mFrightGhostSpeed;
					}
				}
			}

			LevelState previousState = mLevel.mCurrentLevelState;

			bool stateChange = mLevel.update(deltaTime);

			int totalPointsCollected = std::get<1>(update);

			if (totalPointsCollected == 100)
			{
				switch (mLevel.mFruit)
				{
				case CHERRIES: totalPointsCollected = 100; break;
				case STRAWBERRY: totalPointsCollected = 300; break;
				case PEACH: totalPointsCollected = 500; break;
				case APPLE: totalPointsCollected = 700; break;
				case GRAPES: totalPointsCollected = 1000; break;
				case GALAXIAN: totalPointsCollected = 2000; break;
				case BELL: totalPointsCollected = 3000; break;
				case KEY: totalPointsCollected = 5000; break;
				}
			}

			mCurrentPoints += totalPointsCollected;

			bool collectedDot = totalPointsCollected != 0;



			if (collectedDot)
			{
				mLevel.mElapsedTimeSinceLastDotEaten = 0.0f;
				mLevel.mDotsLeft--;
				
				mLevel.checkElroyStates();

				
				if (mLevel.mElroyTwoActive && mGhosts[0].mGhostState != FRIGHTENED 
					&& mGhosts[0].mGhostState != EATEN && !((mGhosts[0].mTileCoordinates.x == 17 && (mGhosts[0].mTileCoordinates.y < 5 ||
						mGhosts[0].mTileCoordinates.y > 21))))
				{
					mGhosts[0].mVelocity = MAX_SPEED * mLevel.mElroyTwoSpeed;
				}
				else if (mLevel.mElroyOneActive && mGhosts[0].mGhostState != FRIGHTENED
					&& mGhosts[0].mGhostState != EATEN && !((mGhosts[0].mTileCoordinates.x == 17 && (mGhosts[0].mTileCoordinates.y < 5 ||
						mGhosts[0].mTileCoordinates.y > 21))))
				{
					mGhosts[0].mVelocity = MAX_SPEED * mLevel.mElroyOneSpeed;
				}



				mTotalDotsCollected++;
				if ((mTotalDotsCollected == 70 && !mFirstFruitAdded ) || (mTotalDotsCollected == 170 && !mSecondFruitAdded))
				{
					mLevel.activateFruit();

					if (!mFirstFruitAdded) mFirstFruitAdded = true;
					else mSecondFruitAdded = true;
				}
				if (mLevel.mGlobalDotCounterActivated) mLevel.mGlobalDotCounter++;
			}

			if (mLevel.mElapsedTimeSinceLastDotEaten > mLevel.mMaxTimeWithoutEatingDot)
			{
				if (!leftGhostHouse[PINKY]) mGhosts[PINKY].mInsideGhostHouse = false;
				else
				{
					if (!leftGhostHouse[INKY]) mGhosts[INKY].mInsideGhostHouse = false;
					else if (!leftGhostHouse[CLYDE]) mGhosts[CLYDE].mInsideGhostHouse = false;
				}
				mLevel.mElapsedTimeSinceLastDotEaten = 0.0f;
			}

			if (!mLevel.mGlobalDotCounterActivated)
			{
				for (int i = 1; i < 4; i++)
				{

					if (mGhosts[i].mPersonalDotCounterActivated)
					{
						if (collectedDot) mGhosts[i].mPersonalDotCounter++;
						mGhosts[i].checkLeaveHouse();
					}

					if (mGhosts[i].mInsideGhostHouse)
					{
						if (mGhosts[i].mType == PINKY && !mGhosts[i].mPersonalDotCounterActivated)
						{
							mGhosts[i].mPersonalDotCounterActivated = true;
						}
						else if (mGhosts[i].mType == INKY)
						{
							if (leftGhostHouse[PINKY] && !mGhosts[i].mPersonalDotCounterActivated) mGhosts[i].mPersonalDotCounterActivated = true;
						}
						else if (mGhosts[i].mType == CLYDE)
						{
							if (leftGhostHouse[INKY] && !mGhosts[i].mPersonalDotCounterActivated) mGhosts[i].mPersonalDotCounterActivated = true;
						}
					}
				}
			}
			else
			{
				if (mLevel.mGlobalDotCounter == 7)
				{
					mGhosts[2].mInsideGhostHouse = false;
				}
				if (mLevel.mGlobalDotCounter == 17)
				{
					mGhosts[1].mInsideGhostHouse = false;
				}
				if (mLevel.mGlobalDotCounter == 32 && mGhosts[3].mInsideGhostHouse)
				{
					std::cout << "DING DING DING" << std::endl;

					mLevel.mGlobalDotCounterActivated = false;
					mLevel.mGlobalDotCounter = 0;
					for (int i = 0; i < 4; i++)
					{
						mGhosts[i].mInsideGhostHouse = false;
						mGhosts[i].mGlobalDotCounterActivated = false;
						mGhosts[i].mPersonalDotCounterActivated = true;
					}

				}
			}


			if (mLevel.mCurrentLevelState == FRIGHTENEDPHASE && mLevel.mTimeLeftCurrentState < 3.0f)
			{
				for (int i = 0; i < 4; i++) mGhosts[i].mCurrentlyFlashing = mLevel.mFlashingFrightenedState;
			}
			
			if (stateChange)
			{
				if (previousState == FRIGHTENEDPHASE)
				{
					mCurrentGhostsAte = 0; 
					mLastGhostEatenIndex = -1;
					engine->stopAllSounds();
					engine->play2D("audio/ghosts_normal.mp3" , true);
					mPacman.mVelocity = MAX_SPEED * mLevel.mPacmanSpeed;
					if (mLevel.mElroyTwoActive)
					{
						mGhosts[0].mVelocity = MAX_SPEED * mLevel.mElroyTwoSpeed;
					}
					else if (mLevel.mElroyOneActive)
					{
						mGhosts[0].mVelocity = MAX_SPEED * mLevel.mElroyOneSpeed;
					}
					else
					{
						mGhosts[0].mVelocity = MAX_SPEED * mLevel.mGhostSpeed;
					}
					for (unsigned int i = 1; i < 4; i++)
					{
						mGhosts[i].mVelocity = MAX_SPEED * mLevel.mGhostSpeed;
					}
				}
				GhostState newState = mLevel.mCurrentLevelState == SCATTERPHASE ? SCATTER : CHASE;
				for (unsigned int i = 0; i < 4; i++)
				{
					if (mGhosts[i].mGhostState != EATEN)
					{
						if (mGhosts[i].mGhostState != FRIGHTENED && mGhosts[i].mLeftGhostHouse)
						{
							mGhosts[i].reorientGhost(mLevel.mTiles, mPacman.mTileCoordinates, mGhosts[0].mTileCoordinates, mPacman.mOrientation);
						}
						if (mGhosts[i].mGhostState == FRIGHTENED)
						{
							mGhosts[i].mCurrentlyFlashing = false;
						}
						mGhosts[i].mGhostState = newState;

					}
				}
			}

			this->checkCollisions();
			this->checkForHighScore();
		}
		else
		{
			mTimeBeforeAnimationStart -= deltaTime;
			if (mTimeBeforeAnimationStart < 0.0f)
			{
				mEatingAnimationActive = false;
				mLastGhostEatenIndex = -1;
				engine->stopAllSounds();
				engine->play2D("audio/dashing.mp3", true);
			}
		}
	}
	else
	{
		mTimeBeforeAnimationStart -= deltaTime;
		if (!mGameStarted)
		{
			if (!playingIntro)
			{
				engine->play2D("audio/intro.mp3");
				playingIntro = true;
			}
			if (mTimeBeforeAnimationStart < 0.0f)
			{
				mGameStarted = true;
				mAnimationActive = false;
				engine->play2D("audio/ghosts_normal.mp3" , true);
			}
		}
		else if (mGameOver)
		{
			if (mTimeBeforeAnimationStart <= 0.0f)
			{
				mGameOver = false;
				this->init();

				return;
			}
		}
		else
		{
			if (mTimeBeforeAnimationStart < 0.0f && mPacman.mFrame == 0)
			{
				engine->play2D("audio/death.mp3");
			}
			if (mTimeBeforeAnimationStart < 0.0f)
			{
				mPacman.mFrame++;
			}
			if (!mPacman.mCurrentlyInAnimation)
			{
				mTimeBeforeAnimationStart = 0.0f;
				mLivesLeft--;
				if (checkGameOver())
				{
					mTimeBeforeAnimationStart = 4.0f;
					mGameOver = true;
					mAnimationActive = true;
				}
				else
				{
					this->restartLevel();
				}
			}
		}
	}

	if (mLevel.checkLevelCompleted())
	{
		engine->stopAllSounds();
		engine->stopAllSounds();
		mCurrentLevelCompleted = true;
		mTimeBeforeAnimationStart = 6.0f;
	}
}

void Game::processInput(float deltaTime)
{

	if (mGameStarted && !mEatingAnimationActive)
	{
		int maxKeyFramesAllowed = 80;
		if (mKeys[GLFW_KEY_A] && mKeysProcessedFrames[GLFW_KEY_A] < maxKeyFramesAllowed)
		{
			if (mPacman.mOrientation != LEFT)
			{
				glm::ivec2 tileCoordinates = mPacman.mTileCoordinates;
				if (tileCoordinates.y - 1 >= 0 && checkNextTileAvailability(mLevel.mTiles[tileCoordinates.x][tileCoordinates.y - 1]))
				{
					mPacman.mOrientation = LEFT;
					mPacman.mPosition.x = mLevel.mTiles[tileCoordinates.x][tileCoordinates.y].mPosition.x - 16.0f;
					mPacman.mPosition.y = mLevel.mTiles[tileCoordinates.x][tileCoordinates.y].mPosition.y - 16.0f;
				}
				mKeysProcessed[GLFW_KEY_A] = true;
				mKeysProcessedFrames[GLFW_KEY_A]++;
			}
		}
		else if (mKeys[GLFW_KEY_D] && mKeysProcessedFrames[GLFW_KEY_D] < maxKeyFramesAllowed)
		{
			if (mPacman.mOrientation != RIGHT)
			{
				glm::ivec2 tileCoordinates = mPacman.mTileCoordinates;
				if (tileCoordinates.y + 1 <= mLevel.mTiles[0].size() - 1 && checkNextTileAvailability(mLevel.mTiles[tileCoordinates.x][tileCoordinates.y + 1]))
				{
					mPacman.mOrientation = RIGHT;
					mPacman.mPosition.x = mLevel.mTiles[tileCoordinates.x][tileCoordinates.y].mPosition.x - 16.0f;
					mPacman.mPosition.y = mLevel.mTiles[tileCoordinates.x][tileCoordinates.y].mPosition.y - 16.0f;
				}
			}
			mKeysProcessed[GLFW_KEY_D] = true;
			mKeysProcessedFrames[GLFW_KEY_D]++;
		}
		else if (mKeys[GLFW_KEY_W] && mKeysProcessedFrames[GLFW_KEY_W] < maxKeyFramesAllowed)
		{
			if (mPacman.mOrientation != UP)
			{
				glm::ivec2 tileCoordinates = mPacman.mTileCoordinates;
				if (checkNextTileAvailability(mLevel.mTiles[tileCoordinates.x - 1][tileCoordinates.y]))
				{
					mPacman.mOrientation = UP;
					mPacman.mPosition.x = mLevel.mTiles[tileCoordinates.x][tileCoordinates.y].mPosition.x - 16.0f;
					mPacman.mPosition.y = mLevel.mTiles[tileCoordinates.x][tileCoordinates.y].mPosition.y - 16.0f;
				}
			}
			mKeysProcessed[GLFW_KEY_W] = true;
			mKeysProcessedFrames[GLFW_KEY_W]++;
		}
		else if (mKeys[GLFW_KEY_S] && mKeysProcessedFrames[GLFW_KEY_S] < maxKeyFramesAllowed)
		{
			if (mPacman.mOrientation != DOWN)
			{
				glm::ivec2 tileCoordinates = mPacman.mTileCoordinates;
				if (checkNextTileAvailability(mLevel.mTiles[tileCoordinates.x + 1][tileCoordinates.y]))
				{
					mPacman.mOrientation = DOWN;
					mPacman.mPosition.x = mLevel.mTiles[tileCoordinates.x][tileCoordinates.y].mPosition.x - 16.0f;
					mPacman.mPosition.y = mLevel.mTiles[tileCoordinates.x][tileCoordinates.y].mPosition.y - 16.0f;
				}
			}
			mKeysProcessed[GLFW_KEY_S] = true;
			mKeysProcessedFrames[GLFW_KEY_S]++;
		}
	}

}

bool Game::checkCollisions()
{

	for (unsigned int i = 0; i < 4; i++)
	{
		if (checkPacmanGhostCollision(mPacman, mGhosts[i]))
		{
			if (mGhosts[i].mGhostState == FRIGHTENED)
			{
				mGhosts[i].mGhostState = EATEN;
				mGhosts[i].mLeftGhostHouse = false;
				mGhosts[i].mCurrentlyFlashing = false;
				mCurrentGhostsAte++;
				mCurrentPoints += 200 * std::pow(2, mCurrentGhostsAte - 1);
				mEatingAnimationActive = true;
				mTimeBeforeAnimationStart = 1.0f;
				mLastGhostEatenIndex = i;
				engine->play2D("audio/eaten.mp3");
			}
			else if(mGhosts[i].mGhostState != EATEN)
			{
				this->mAnimationActive = true;
				mTimeBeforeAnimationStart = 0.6f;
				mPacman.mCurrentlyInAnimation = true;
				mPacman.mFrame = 0;
				mLevel.mLifeLost = true;
				mLevel.mGlobalDotCounterActivated = true;
				engine->stopAllSounds();
				
			}
		}
	}
	
	return true;
}

void Game::resetLevel()
{
	mPacman.mPosition.x = mLevel.mTiles[26][13].mPosition.x;
	mPacman.mPosition.y = mLevel.mTiles[26][13].mPosition.y - 16.0f;

	mPacman.mFrame = 0;

	mTotalDotsCollected = 0;

	mFirstFruitAdded = false;
	mSecondFruitAdded = false;
	mLastGhostEatenIndex = -1;
	mCurrentGhostsAte = 0;

	mPacman.mTileCoordinates = glm::ivec2(26, 13);
	mPacman.mOrientation = LEFT;

	mLevel.mCurrentLevelState = SCATTERPHASE;
	mLevel.mTimeLeftCurrentState = 5.0f;
	mLevel.mCurrentStateIndex--;

	for (int i = 0; i < 4; i++)
	{
		mGhosts[i].mGhostState = SCATTER;
		mGhosts[i].mPersonalDotCounter = 0;
		mGhosts[i].mPersonalDotCounterActivated = 0;

		mGhosts[i].mFrame = 0;

		mGhosts[i].mCurrentlyFlashing = false;
		mLevel.mFlashingFrightenedState = true;

		if (mGhosts[i].mType == BLINKY)
		{
			mGhosts[i].mPosition.x = mLevel.mTiles[14][13].mPosition.x - 16.0f;
			mGhosts[i].mPosition.y = mLevel.mTiles[14][13].mPosition.y - 16.0f;
			mGhosts[i].mTileCoordinates = glm::ivec2(14, 13);
			mGhosts[i].mPreviousTileCalculated = glm::ivec2(14, 12);
			mGhosts[i].mOrientation = LEFT;
			mGhosts[i].mNextOrientation = LEFT;
		}
		else if (mGhosts[i].mType == INKY)
		{
			mGhosts[i].mPosition.x = mLevel.mTiles[17][12].mPosition.x - 16.0f;
			mGhosts[i].mPosition.y = mLevel.mTiles[17][12].mPosition.y - 16.0f;
			mGhosts[i].mTileCoordinates = glm::ivec2(17, 12);
			mGhosts[i].mPreviousTileCalculated = glm::ivec2(18, 12);
			mGhosts[i].mOrientation = DOWN;
			mGhosts[i].mNextOrientation = DOWN;
			mGhosts[i].mLeftGhostHouse = false;
			mGhosts[i].mInsideGhostHouse = true;
		}
		else if (mGhosts[i].mType == PINKY)
		{
			mGhosts[i].mPosition.x = mLevel.mTiles[17][14].mPosition.x - 16.0f;
			mGhosts[i].mPosition.y = mLevel.mTiles[17][14].mPosition.y - 16.0f;
			mGhosts[i].mTileCoordinates = glm::ivec2(17, 14);
			mGhosts[i].mPreviousTileCalculated = glm::ivec2(18, 14);
			mGhosts[i].mOrientation = DOWN;
			mGhosts[i].mNextOrientation = DOWN;
			mGhosts[i].mLeftGhostHouse = false;
			mGhosts[i].mInsideGhostHouse = true;
		}
		else if (mGhosts[i].mType == CLYDE)
		{
			mGhosts[i].mPosition.x = mLevel.mTiles[17][16].mPosition.x - 16.0f;
			mGhosts[i].mPosition.y = mLevel.mTiles[17][16].mPosition.y - 16.0f;
			mGhosts[i].mTileCoordinates = glm::ivec2(17, 16);
			mGhosts[i].mPreviousTileCalculated = glm::ivec2(18, 16);
			mGhosts[i].mOrientation = DOWN;
			mGhosts[i].mNextOrientation = DOWN;
			mGhosts[i].mLeftGhostHouse = false;
			mGhosts[i].mInsideGhostHouse = true;
		}


		mGhosts[i].mVelocity = glm::vec2(180, 180);
	}

	mAnimationActive = true;
	mTimeBeforeAnimationStart = 3.0f;
	mGameStarted = false;
}

void Game::restartLevel()
{
	mPacman.mPosition.x = mLevel.mTiles[26][13].mPosition.x;
	mPacman.mPosition.y = mLevel.mTiles[26][13].mPosition.y - 16.0f;

	mPacman.mFrame = 0;

	mPacman.mTileCoordinates = glm::ivec2(26, 13);
	mPacman.mOrientation = LEFT;

	mLevel.mCurrentLevelState = SCATTERPHASE;
	mLevel.mTimeLeftCurrentState = 5.0f;
	mLevel.mCurrentStateIndex--;

	for (int i = 0; i < 4; i++)
	{
		mGhosts[i].mGhostState = SCATTER;
		mGhosts[i].mPersonalDotCounter = 0;
		mGhosts[i].mPersonalDotCounterActivated = 0;

		mGhosts[i].mFrame = 0;

		mGhosts[i].mCurrentlyFlashing = false;
		mLevel.mFlashingFrightenedState = true;

		if (mGhosts[i].mType == BLINKY)
		{
			mGhosts[i].mPosition.x = mLevel.mTiles[14][13].mPosition.x - 16.0f;
			mGhosts[i].mPosition.y = mLevel.mTiles[14][13].mPosition.y - 16.0f;
			mGhosts[i].mTileCoordinates = glm::ivec2(14, 13);
			mGhosts[i].mPreviousTileCalculated = glm::ivec2(14, 12);
			mGhosts[i].mOrientation = LEFT;
			mGhosts[i].mNextOrientation = LEFT;
		}
		else if (mGhosts[i].mType == INKY)
		{
			mGhosts[i].mPosition.x = mLevel.mTiles[17][12].mPosition.x - 16.0f;
			mGhosts[i].mPosition.y = mLevel.mTiles[17][12].mPosition.y - 16.0f;
			mGhosts[i].mTileCoordinates = glm::ivec2(17, 12);
			mGhosts[i].mPreviousTileCalculated = glm::ivec2(18, 12);
			mGhosts[i].mOrientation = DOWN;
			mGhosts[i].mNextOrientation = DOWN;
			mGhosts[i].mLeftGhostHouse = false;
			mGhosts[i].mInsideGhostHouse = true;
		}
		else if (mGhosts[i].mType == PINKY)
		{
			mGhosts[i].mPosition.x = mLevel.mTiles[17][14].mPosition.x - 16.0f;
			mGhosts[i].mPosition.y = mLevel.mTiles[17][14].mPosition.y - 16.0f;
			mGhosts[i].mTileCoordinates = glm::ivec2(17, 14);
			mGhosts[i].mPreviousTileCalculated = glm::ivec2(18, 14);
			mGhosts[i].mOrientation = DOWN;
			mGhosts[i].mNextOrientation = DOWN;
			mGhosts[i].mLeftGhostHouse = false;
			mGhosts[i].mInsideGhostHouse = true;
		}
		else if (mGhosts[i].mType == CLYDE)
		{
			mGhosts[i].mPosition.x = mLevel.mTiles[17][16].mPosition.x - 16.0f;
			mGhosts[i].mPosition.y = mLevel.mTiles[17][16].mPosition.y - 16.0f;
			mGhosts[i].mTileCoordinates = glm::ivec2(17, 16);
			mGhosts[i].mPreviousTileCalculated = glm::ivec2(18, 16);
			mGhosts[i].mOrientation = DOWN;
			mGhosts[i].mNextOrientation = DOWN;
			mGhosts[i].mLeftGhostHouse = false;
			mGhosts[i].mInsideGhostHouse = true;
		}


		mGhosts[i].mVelocity = glm::vec2(180, 180);
	}

	mAnimationActive = true;
	mTimeBeforeAnimationStart = 3.0f;
	mGameStarted = false;
}

void Game::checkForHighScore()
{
	if (mCurrentPoints > mHighScorePoints)
	{
		mHighScorePoints = mCurrentPoints;
	}
}

bool Game::checkGameOver()
{
	return mLivesLeft <= 0;
}