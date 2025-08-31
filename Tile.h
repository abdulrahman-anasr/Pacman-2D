#pragma once
#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GameObject.h"


enum TileType {
	SOLID,
	NORMAL,
	DOT,
	ENERGIZER,
	FRUIT,
	GHOSTDOOR,
	GHOSTHOUSE
};

class Tile {
public:
	Tile();
	Tile(glm::vec2 position, glm::ivec2 tileLocation, TileType type);
	glm::vec2 mPosition;
	glm::ivec2 mTileLocation;
	TileType mType;

	void render();


};