#pragma once

#include "Actor.h"
#include "ChaseBehavior.h"
#include "ScatterBehavior.h"
#include "FrightenedBehavior.h"
#include "Enums.h"



class Ghost : public Actor {
public:
	Orientation mNextOrientation;
	GhostState mGhostState;
	ScatterBehavior mScatterBehavior;
	ChaseBehavior mChaseBehavior;
	FrightenedBehavior mFrightenedBehavior;

	static std::vector<glm::vec4> mFrightenedGhostSprites;

	static std::vector<glm::vec4> mFrightenedGhostFlashingSprites;

	

	static std::map<Orientation , std::vector<glm::vec4>> mEatenGhostSprites;

	glm::ivec2 mTargetTile;

	glm::ivec2 mPreviousTileCalculated;

	GhostType mType;

	bool mLeftGhostHouse;

	bool mCurrentlyFlashing;

	bool mInsideGhostHouse;

	int mPersonalDotCounter;

	int mPersonalDotCounterExitCount;

	bool mPersonalDotCounterActivated;

	bool mGlobalDotCounterActivated;

	Ghost();
	Ghost(std::string type);
	void render(float timeLeftCurrentState = -99.9f);
	void update(float deltaTime, std::vector<std::vector<Tile>>& tiles, glm::ivec2 pacmanTile , glm::ivec2 blinkyTile , Orientation pacmanOrientation);
	void checkLeaveHouse();
	void reorientGhost(std::vector<std::vector<Tile>>& tiles, glm::ivec2 pacmanTile, glm::ivec2 blinkyTile, Orientation pacmanOrientation);
	void calculateNextMovement(std::vector<std::vector<Tile>>& tiles, glm::ivec2 pacmanTile, glm::ivec2 blinkyTile, Orientation pacmanOrientation);
};