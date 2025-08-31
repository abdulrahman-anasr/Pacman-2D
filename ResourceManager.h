#pragma once
#include <iostream>
#include <string>
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <sstream>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#include "Texture.h"
#include "Shader.h"

class ResourceManager {
public:
	std::map<std::string, Texture> mTextures;
	std::map<std::string, Shader> mShaders;

	static ResourceManager* getInstance();

	void clear();

	Shader loadShader(std::string name, const char* vertexPath, const char* fragmentPath, const char* geometryPath);

	Shader& getShader(std::string name);

	Texture loadTexture(std::string name, const char* path , bool alpha);

	Texture& getTexture(std::string name);

	

private:
	static ResourceManager* instance;

	ResourceManager();

	Shader loadShaderFromFile(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);

	Texture loadTextureFromFile(const char* path , bool alpha);
};