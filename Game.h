#pragma once

#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "TextRenderer.h"
#include "Level.h"
#include "Actor.h"
#include "Ghost.h"
enum GameState {
	GAME_MENU,
	GAME_ACTIVE
};


class Game {
public:
	/*								 CONSTRUCTORS AND DESTRUCTORS									*/
	Game(unsigned int width, unsigned int height);
	
	/*										CLASS MEMBERS									*/	
	
	Level mLevel;

	static std::vector<glm::vec4> mGhostsEatenScoreSprites;

	bool mGameOver;
	
	int mCurrentLevel;

	int mCurrentPoints;

	int mHighScorePoints;

	int mTotalDotsCollected;

	bool mFirstFruitAdded;

	bool mSecondFruitAdded;

	int mCurrentFrame1Up;

	GameState mGameState;

	int mLivesLeft;

	int mWidth, mHeight;

	bool mKeys[1024];

	bool mKeysProcessed[1024];

	short mKeysProcessedFrames[1024];

	int mAverageFrameRate;

	glm::vec2 mMaximumVelocity;

	bool mAnimationActive;

	bool mEatingAnimationActive;

	bool mCurrentLevelCompleted;

	float mTimeBeforeAnimationStart;

	int mCurrentGhostsAte;

	int mLastGhostEatenIndex;

	bool mGameStarted;

	Actor mPacman;

	Ghost mGhosts[4];


	/*										FUNCTIONS									*/

	void init();

	void render();

	void processInput(float deltaTime);

	void update(float deltaTime);

	bool checkCollisions();

	void restartLevel();

	void resetLevel();

	void checkForHighScore();

	bool checkGameOver();

private:

	
};