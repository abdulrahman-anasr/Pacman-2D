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


class ScatterBehavior {
public:
	ScatterBehavior();

	glm::ivec2 calculateTargetTile(GhostType mType);
};