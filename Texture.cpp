#include "Texture.h"

Texture::Texture() : mWidth(0) , mHeight(0) , mInternalFormat(GL_RGB) , mImageFormat(GL_RGB) ,
 mWrap_S(GL_REPEAT) , mWrap_T(GL_REPEAT) , mFilterMag(GL_NEAREST) , mFilterMin(GL_NEAREST) ,
	mUnitPixelWidth(0) , mUnitPixelHeight(0)
{
	glGenTextures(1, &this->mID);
}

void Texture::generate(unsigned int width, unsigned int height, unsigned char* data)
{

	this->mWidth = width;
	this->mHeight = height;

	mUnitPixelWidth = 1.0f / static_cast<float>(mWidth);
	mUnitPixelHeight = 1.0f / static_cast<float>(mHeight);

	glBindTexture(GL_TEXTURE_2D, this->mID);

	glTexImage2D(GL_TEXTURE_2D, 0, mInternalFormat, mWidth, mHeight, 0, mImageFormat, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mWrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mWrap_T);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mFilterMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mFilterMag);

	glBindTexture(GL_TEXTURE_2D, 0);

}

void Texture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->mID);
}