#pragma once
#include <iostream>
#include <string>
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Texture {
public:
	unsigned int mID;

	unsigned int mWidth, mHeight;

	float mUnitPixelWidth, mUnitPixelHeight;

	unsigned int mInternalFormat;
	unsigned int mImageFormat;

	unsigned int mWrap_S;
	unsigned int mWrap_T;

	unsigned int mFilterMag;
	unsigned int mFilterMin;

	Texture();

	void generate(unsigned int width, unsigned int height, unsigned char* data);

	void bind() const;
};