#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Tile.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"

#include "Enums.h"

typedef std::pair<LevelState, float> LevelStates;

class Level {
public:
	Level(std::string path);

	std::vector<std::vector<Tile>> mTiles;

	int mDotsLeft;

	float mElapsedTime;
	float mTimeLeftCurrentState;

	bool mLifeLost;

	bool mGlobalDotCounterActivated;

	int mGlobalDotCounter;

	

	float mIntervalPerFlash;

	float mCurrentFlashInterval;

	bool mFlashingFrightenedState;

	LevelState mCurrentLevelState;

	int mCurrentStateIndex;

	std::vector<LevelStates> mLevelStates;

	bool mFruitActive;

	bool mFruitEaten;

	float mMaxTimeWithoutEatingDot;

	float mElapsedTimeSinceLastDotEaten;

	float mTimeLeftFruitActive;

	float mFruitEatenAnimationTimer;

	FruitType mFruit;
	float mPacmanSpeed;
	float mGhostSpeed;
	float mGhostTunnelSpeed;
	float mElroyOneSpeed;
	int mElroyOneDotsLeft;
	float mElroyTwoSpeed;
	int mElroyTwoDotsLeft;
	float mFrightPacmanSpeed;
	float mFrightGhostSpeed;
	float mFrightDuration;
	int mNumberOfFrightenedFlashes;

	bool mElroyOneActive;

	bool mElroyTwoActive;

	void render();

	void renderFruit();

	void renderFruitEatenAnimation();

	void activateFruit();

	void changeToFrightenedState();

	void checkElroyStates();

	bool update(float deltaTime);

	bool checkLevelCompleted();

	void setupEntireLevel(std::string path);

	void setupTiles(std::string path);

	void setupLevel(std::string path);

	void setupStates(std::string path);

private:

	
	
};