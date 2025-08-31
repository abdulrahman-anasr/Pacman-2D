#pragma once
#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader {
public:
	unsigned int mID;

	Shader();

	void compile(const char* vertexCode, const char* fragmentCode, const char* geometryCode = nullptr);

	Shader& use();

	

	void setInteger(std::string name, GLint value, bool useShader = false);

	void setFloat(std::string name, GLfloat value, bool useShader = false);

	void setVector2(std::string name, glm::vec2 value, bool useShader = false);

	void setVector3(std::string name, glm::vec3 value, bool useShader = false);

	void setVector4(std::string name, glm::vec4 value, bool useShader = false);

	void setMat4(std::string name, glm::mat4 value, bool useShader = false);

private:
	void checkCompilationErrors(unsigned int object, std::string type);

	GLint getUniformLocation(std::string name);
};