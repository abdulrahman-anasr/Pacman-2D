#pragma once
#include <iostream>
#include <string>



#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SpriteRenderer.h"
#include "ResourceManager.h"

#include "Enums.h"
extern SpriteRenderer* renderer;


class GameObject {
public:

	glm::vec2 mPosition;
	glm::vec2 mSize;
	glm::ivec2 mTileCoordinates;
	GameObject();
	GameObject(glm::vec2 position , glm::ivec2 tileCoordinates);
	
};