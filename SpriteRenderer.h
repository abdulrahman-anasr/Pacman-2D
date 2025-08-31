#pragma once

#include <iostream>
#include <string>
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Texture.h"

class SpriteRenderer {
public:
	SpriteRenderer(Shader& shader);
	~SpriteRenderer();

	void drawSprite(Texture& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));

	void drawSpriteFromSheet(Texture& texture, glm::vec2 position, glm::vec2 sheetPosition, glm::vec2 spriteSize ,
		glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));

private:
	Shader mShader;
	unsigned int VAO;
	unsigned int spriteFromSheetVAO;
	unsigned int spriteFromSheetVBO;
	void initRenderData();

};