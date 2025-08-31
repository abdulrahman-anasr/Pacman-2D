#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Tile.h"
#include "Enums.h"



class ChaseBehavior {
public:
	ChaseBehavior();
	ChaseBehavior(GhostType type);
	GhostType mType;

	glm::ivec2 calculateTargetTile(glm::ivec2 pacmanTilePosition, Orientation pacmanOrientation, glm::ivec2 ghostTilePosition = glm::ivec2(0 , 0), glm::ivec2 blinkyTilePosition = glm::ivec2(0, 0));

	Orientation calculateOptimalTile(std::vector<std::vector<Tile>>& tiles , Orientation currentOrientation , glm::ivec2 currentTile, glm::ivec2 targetTile, bool insideHouse);

	

};