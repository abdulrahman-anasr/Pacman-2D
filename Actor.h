#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Tile.h"
#include "GameObject.h"
#include "Enums.h"

typedef std::tuple<bool, int> ActorUpdate;

class Actor : public GameObject {
public:
	Orientation mOrientation;
	glm::vec2 mVelocity;
	bool mBlocked;

	int mFrame;
	std::map<Orientation, std::vector<glm::vec4>> mCharacterSprites;

	static std::vector<glm::vec4> mPacmanDeathSprites;

	bool mCurrentlyInAnimation;

	bool mSkipFrame;

	int mFramesSkipped;

	Actor(std::string type);

	virtual ActorUpdate update(float deltaTime ,std::vector<std::vector<Tile>>& tiles);

	virtual void render();
};