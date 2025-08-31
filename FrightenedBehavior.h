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

class FrightenedBehavior {
public:

	FrightenedBehavior();

	Orientation chooseRandomTile(std::vector<std::vector<Tile>>& tiles, glm::ivec2 currentTile, Orientation currentOrientation);

	Orientation chooseTileToGhoseHouse(std::vector<std::vector<Tile>>& tiles, glm::ivec2 currentTile, Orientation currentOrientation , GhostType type);

	glm::ivec2 ghostHouseTargetTile(GhostType ghostType);
};