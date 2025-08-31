#pragma once
#include <map>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "ResourceManager.h"
#include "Texture.h"
#include "Shader.h"

struct Character {
	unsigned int mTextureID;
	glm::ivec2 mSize;
	glm::ivec2 mBearing;
	unsigned int mAdvance;
};

class TextRenderer {
public:
	std::map<char, Character> mCharacters;

	Shader textShader;

	TextRenderer(unsigned int width, unsigned int height);

	void load(std::string font, unsigned int fontSize);

	void renderText(std::string text, float x, float y, float scale, glm::vec3 color = glm::vec3(1.0f));

private:
	unsigned int VAO, VBO;

};