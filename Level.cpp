#include "Level.h"


extern SpriteRenderer* renderer;


std::vector<std::string> splitString(std::string str, char characterToSplitOn)
{
	std::stringstream stream;
	std::string segment;
	stream << str;


	std::vector<std::string> seglist;



	while (std::getline(stream, segment, characterToSplitOn))
	{
		seglist.push_back(segment);
	}

	return seglist;
}

void assignFruitFromString(FruitType& fruit, std::string str)
{
	if (str == "cherries")
	{
		fruit = CHERRIES;
	}
	else if (str == "strawberry")
	{
		fruit = STRAWBERRY;
	}
	else if (str == "peach")
	{
		fruit = PEACH;
	}
	else if (str == "apple")
	{
		fruit = APPLE;
	}
}

Level::Level(std::string path)
{
	setupEntireLevel(path);
}

void Level::setupEntireLevel(std::string path)
{
	mElapsedTime = 0.0f;
	mMaxTimeWithoutEatingDot = 5.0f;
	
	mNumberOfFrightenedFlashes = 5;
	mElapsedTimeSinceLastDotEaten = 0.0f;
	mTimeLeftFruitActive = 0.0f;
	mCurrentFlashInterval = 0.0f;
	mGlobalDotCounterActivated = false;
	mGlobalDotCounter = 0;
	mFruitActive = false;
	mFruitEaten = false;

	mDotsLeft = 244;

	mElroyOneActive = false;
	mElroyTwoActive = false;

	setupTiles("tile/setup.tile");

	setupLevel(path);

	setupStates(path);

	mCurrentLevelState = mLevelStates[mCurrentStateIndex].first;
	mTimeLeftCurrentState = mLevelStates[mCurrentStateIndex].second;
}

void Level::setupTiles(std::string path)
{
	for (int i = 0; i < mTiles.size(); i++)
	{
		mTiles[i].clear();
	}
	mTiles.clear();
	try
	{
		unsigned int tileCode;
		std::string line;
		std::ifstream fstream(path);

		int tileSize = 32;
		if (fstream)
		{
			int columnCounter = 0;
			int rowCounter = 0;
			while (std::getline(fstream, line))
			{
				std::istringstream sstream(line);
				std::vector<Tile> tilesRow;
				while (sstream >> tileCode)
				{
					Tile tile;
					switch (tileCode)
					{
					case 0: tile.mType = SOLID; break;
					case 1: tile.mType = DOT; break;
					case 2: tile.mType = ENERGIZER; break;
					case 3: tile.mType = NORMAL; break;
					case 4: tile.mType = GHOSTDOOR; break;
					case 5: tile.mType = GHOSTHOUSE; break;
					}
					tile.mPosition = glm::vec2(columnCounter * tileSize, rowCounter * tileSize);
					tile.mTileLocation = glm::ivec2(rowCounter, columnCounter);
					tilesRow.push_back(tile);
					columnCounter++;
				}

				mTiles.push_back(tilesRow);
				columnCounter = 0;
				rowCounter++;
			}
		}

		std::cout << "vector size is now: " << mTiles.size() << std::endl;
		std::cout << "row size is now: " << mTiles[0].size() << std::endl;
	}
	catch (std::exception e)
	{
		std::cout << "Failed to load tiles" << std::endl;
	}
}

void Level::setupLevel(std::string path)
{
	try
	{
		std::string line;
		std::ifstream fstream(path);

		if (fstream)
		{
			while (std::getline(fstream, line))
			{
				std::vector<std::string> result = splitString(line, ' ');
				std::string statement = splitString(result[0], ':')[0];

				if (statement == "bonus")
				{
					assignFruitFromString(mFruit, result[1]);
				}
				else if (statement == "pacmanspeed")
				{
					mPacmanSpeed = std::stof(result[1]);
				}
				else if (statement == "ghostspeed")
				{
					mGhostSpeed = std::stof(result[1]);
				}
				else if (statement == "ghosttunnel")
				{
					mGhostTunnelSpeed = std::stof(result[1]);
				}
				else if (statement == "elroy1speed")
				{
					mElroyOneSpeed = std::stof(result[1]);
				}
				else if (statement == "elroy1dots")
				{
					mElroyOneDotsLeft = std::stoi(result[1]);
				}
				else if (statement == "elroy2speed")
				{
					mElroyTwoSpeed = std::stof(result[1]);
				}
				else if (statement == "elroy2dots")
				{
					mElroyTwoDotsLeft = std::stoi(result[1]);
				}
				else if (statement == "frightpacmanspeed")
				{
					mFrightPacmanSpeed = std::stof(result[1]);
				}
				else if (statement == "frightghostspeed")
				{
					mFrightGhostSpeed = std::stof(result[1]);
				}
				else if (statement == "frighttime")
				{
					mFrightDuration = std::stof(result[1]);
				}
				else if (statement == "numberofflashes")
				{
					mNumberOfFrightenedFlashes = std::stof(result[1]);
				}
				
			}
		}
	}
	catch (std::exception e)
	{
		std::cout << "Failed to setup level" << std::endl;
	}
}


void Level::setupStates(std::string path)
{
	mCurrentStateIndex = 0;
	if (path == "levels/one.level")
	{
		mLevelStates.push_back(std::pair<LevelState, float>(SCATTERPHASE, 7.0f));
		mLevelStates.push_back(std::pair<LevelState, float>(CHASEPHASE, 20.0f));
		mLevelStates.push_back(std::pair<LevelState, float>(SCATTERPHASE, 7.0f));
		mLevelStates.push_back(std::pair<LevelState, float>(CHASEPHASE, 20.0f));
		mLevelStates.push_back(std::pair<LevelState, float>(SCATTERPHASE, 5.0f));
		mLevelStates.push_back(std::pair<LevelState, float>(CHASEPHASE, 20.0f));
		mLevelStates.push_back(std::pair<LevelState, float>(SCATTERPHASE, 5.0f));
		mLevelStates.push_back(std::pair<LevelState, float>(CHASEPHASE, 0.0f));
	}
	else if (path == "levels/two.level" || path == "levels/three.level" || path == "levels/four.level")
	{
		mLevelStates.push_back(std::pair<LevelState, float>(SCATTERPHASE, 7.0f));
		mLevelStates.push_back(std::pair<LevelState, float>(CHASEPHASE, 20.0f));
		mLevelStates.push_back(std::pair<LevelState, float>(SCATTERPHASE, 7.0f));
		mLevelStates.push_back(std::pair<LevelState, float>(CHASEPHASE, 20.0f));
		mLevelStates.push_back(std::pair<LevelState, float>(SCATTERPHASE, 5.0f));
		mLevelStates.push_back(std::pair<LevelState, float>(CHASEPHASE, 1033.0f));
		mLevelStates.push_back(std::pair<LevelState, float>(SCATTERPHASE, 1.0f / 60.0f));
		mLevelStates.push_back(std::pair<LevelState, float>(CHASEPHASE, 0.0f));
	}
	else
	{
		mLevelStates.push_back(std::pair<LevelState, float>(SCATTERPHASE, 5.0f));
		mLevelStates.push_back(std::pair<LevelState, float>(CHASEPHASE, 20.0f));
		mLevelStates.push_back(std::pair<LevelState, float>(SCATTERPHASE, 5.0f));
		mLevelStates.push_back(std::pair<LevelState, float>(CHASEPHASE, 20.0f));
		mLevelStates.push_back(std::pair<LevelState, float>(SCATTERPHASE, 5.0f));
		mLevelStates.push_back(std::pair<LevelState, float>(CHASEPHASE, 1037.0f));
		mLevelStates.push_back(std::pair<LevelState, float>(SCATTERPHASE, 1.0f / 60.0f));
		mLevelStates.push_back(std::pair<LevelState, float>(CHASEPHASE, 0.0f));
	}

	mCurrentLevelState = mLevelStates[mCurrentStateIndex].first;
	mTimeLeftCurrentState = mLevelStates[mCurrentStateIndex].second;
}

void Level::render()
{
	ResourceManager* instance = ResourceManager::getInstance();
	for (unsigned int i = 0; i < mTiles.size(); i++)
	{

		unsigned int rowLength = mTiles[i].size();
		for (unsigned int j = 0; j < rowLength; j++)
		{
			if (mTiles[i][j].mType != DOT && mTiles[i][j].mType != ENERGIZER) continue;
			mTiles[i][j].render();
		}
	}
	
	if (mTiles[20][13].mType == FRUIT)
	{
		this->renderFruit();
	}

	if (mFruitEatenAnimationTimer > 0.0f)
	{
		this->renderFruitEatenAnimation();
	}
}

void Level::renderFruit()
{

	ResourceManager* instance = ResourceManager::getInstance();

	glm::vec2 spriteCoordinates;
	glm::vec2 spriteSize;
	glm::vec2 position = glm::vec2(13 * 32.0f, (20 * 32.0f) - 18.0f);

	switch (mFruit)
	{
	case CHERRIES: {
		spriteCoordinates = glm::vec2(37, 50);
		spriteSize = glm::vec2(12, 12);
	} break;
	case STRAWBERRY: {
		spriteCoordinates = glm::vec2(53, 50);
		spriteSize = glm::vec2(11, 12);
	} break;
	case PEACH: {
		spriteCoordinates = glm::vec2(69, 50);
		spriteSize = glm::vec2(12, 12);
	} break;
	case APPLE: {
		spriteCoordinates = glm::vec2(85, 50);
		spriteSize = glm::vec2(12, 12);
	} break;
	case GRAPES: {
		spriteCoordinates = glm::vec2(102, 49);
		spriteSize = glm::vec2(11, 14);
	} break;
	case GALAXIAN: {
		spriteCoordinates = glm::vec2(117, 50);
		spriteSize = glm::vec2(11, 12);
	} break;
	case BELL: {
		spriteCoordinates = glm::vec2(133, 50);
		spriteSize = glm::vec2(12, 13);
	} break;
	default: {
		spriteCoordinates = glm::vec2(151, 50);
		spriteSize = glm::vec2(7, 13);
	} break;
	}


	renderer->drawSpriteFromSheet(instance->getTexture("objects"), position, spriteCoordinates, spriteSize, glm::vec2(64));
}


bool Level::update(float deltaTime)
{
	bool stateChanged = false;
	mElapsedTime += deltaTime;

	if (mTimeLeftCurrentState > 0.0f)
	{
		mTimeLeftCurrentState -= deltaTime;
	}

	mElapsedTimeSinceLastDotEaten += deltaTime;

	if (mCurrentLevelState == FRIGHTENEDPHASE)
	{
		if (mTimeLeftCurrentState < 3.0f)
		{
			if (mCurrentFlashInterval <= 0.0f)
			{
				mCurrentFlashInterval = mIntervalPerFlash;
				mFlashingFrightenedState = !mFlashingFrightenedState;
			}
			mCurrentFlashInterval -= deltaTime;
		}
	}

	if (mTimeLeftCurrentState < 0.0f)
	{
		stateChanged = true;
		if (mCurrentLevelState != FRIGHTENEDPHASE)
		{
			mCurrentStateIndex++;
		}
		mCurrentLevelState = mLevelStates[mCurrentStateIndex].first;
		mTimeLeftCurrentState = mLevelStates[mCurrentStateIndex].second;
	}

	if (mFruitActive)
	{
		mTimeLeftFruitActive -= deltaTime;
		if (mTimeLeftFruitActive < 0.0f)
		{
			mTimeLeftFruitActive = 0.0f;
			mTiles[20][13].mType = NORMAL;
			mFruitActive = false;
		}
		else if (mTiles[20][13].mType == NORMAL)
		{
			mFruitEaten = true;
			mFruitActive = false;
			mTimeLeftFruitActive = 0.0f;
			mFruitEatenAnimationTimer = 2.0f;
		}
	}

	if (mFruitEatenAnimationTimer > 0.0f)
	{
		mFruitEatenAnimationTimer -= deltaTime;
	}

	return stateChanged;
}

void Level::changeToFrightenedState()
{
	mLevelStates[mCurrentStateIndex].second = mTimeLeftCurrentState;
	mTimeLeftCurrentState = mFrightDuration;
	mCurrentLevelState = FRIGHTENEDPHASE;
	mFlashingFrightenedState = true;
	mIntervalPerFlash = 3.0f / static_cast<float>(mNumberOfFrightenedFlashes * 2);
}

bool Level::checkLevelCompleted()
{
	
	for (int i = 0; i < mTiles.size(); i++)
	{
		for (int j = 0; j < mTiles[i].size(); j++)
		{
			if (mTiles[i][j].mType == DOT || mTiles[i][j].mType == ENERGIZER)
			{
				return false;
			}
		}
	}
	return true;
}

void Level::activateFruit()
{
	mFruitActive = true;
	mTiles[20][13].mType = FRUIT;

	mTimeLeftFruitActive = 9.0 + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (10.0 - 9.0));
}

void Level::renderFruitEatenAnimation()
{
	ResourceManager* instance = ResourceManager::getInstance();
	glm::vec2 position = glm::vec2(13 * 32.0f, (20 * 32.0f));
	glm::vec2 renderSize = glm::vec2(64, 32);

	glm::vec2 spriteCoordinates;
	glm::vec2 spriteSize;

	switch (mFruit)
	{
	case CHERRIES: {
		spriteCoordinates = glm::vec2(5, 148);
		spriteSize = glm::vec2(13, 7);
	} break;
	case STRAWBERRY: {
		spriteCoordinates = glm::vec2(19, 148);
		spriteSize = glm::vec2(15, 7);
	} break;
	case PEACH: {
		spriteCoordinates = glm::vec2(35, 148);
		spriteSize = glm::vec2(15, 7);
	} break;
	case APPLE: {
		spriteCoordinates = glm::vec2(51, 148);
		spriteSize = glm::vec2(15, 7);
	} break;
	case GRAPES: {
		spriteCoordinates = glm::vec2(67, 148);
		spriteSize = glm::vec2(18, 7);
	} break;
	case GALAXIAN: {
		spriteCoordinates = glm::vec2(65, 164);
		spriteSize = glm::vec2(20, 7);
	} break;
	case BELL: {
		spriteCoordinates = glm::vec2(65, 180);
		spriteSize = glm::vec2(20, 7);
	} break;
	default: {
		spriteCoordinates = glm::vec2(65, 196);
		spriteSize = glm::vec2(20, 7);
	} break;
	}
	
	renderer->drawSpriteFromSheet(instance->getTexture("objects"), position, spriteCoordinates, spriteSize, renderSize);
}

void Level::checkElroyStates()
{
	std::cout << "Dots left is: " << mDotsLeft << " " << "and elroy two needed is: " << mElroyTwoDotsLeft << std::endl;
	if (!mElroyTwoActive && mDotsLeft < mElroyTwoDotsLeft)
	{
		std::cout << "ACTIVATED ELROY TWO" << std::endl;
		mElroyTwoActive = true;
	}
	else if (!mElroyOneActive && mDotsLeft < mElroyOneDotsLeft)
	{
		std::cout << "ACTIVATED ELROY ONE" << std::endl;
		mElroyOneActive = true;
	}
	
}