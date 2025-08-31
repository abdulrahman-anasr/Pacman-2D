#include "ResourceManager.h"

/*											PUBLIC FUNCTIONS										*/

ResourceManager* ResourceManager::getInstance()
{
	if (instance == nullptr)
	{
		instance = new ResourceManager();
	}

	return instance;
}


void ResourceManager::clear()
{
	for (auto iter : mShaders)
	{
		glDeleteProgram(iter.second.mID);
	}

	for (auto iter : mTextures)
	{
		glDeleteTextures(1, &iter.second.mID);
	}
}

Shader ResourceManager::loadShader(std::string name, const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	mShaders[name] = loadShaderFromFile(vertexPath, fragmentPath, geometryPath);
	return mShaders[name];
}

Shader& ResourceManager::getShader(std::string name)
{
	return mShaders[name];
}

Texture ResourceManager::loadTexture(std::string name, const char* path, bool alpha)
{
	mTextures[name] = loadTextureFromFile(path, alpha);
	return mTextures[name];
}

Texture& ResourceManager::getTexture(std::string name)
{
	return mTextures[name];
}



/*											PRIVATE FUNCTIONS										*/

ResourceManager* ResourceManager::instance = nullptr;

ResourceManager::ResourceManager() = default;

Shader ResourceManager::loadShaderFromFile(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	std::string vertexCode, fragmentCode, geometryCode;
	try
	{
		std::ifstream vertexFile(vertexPath);
		std::ifstream fragmentFile(fragmentPath);
		
		std::stringstream vertexStream, fragmentStream;

		vertexStream << vertexFile.rdbuf();
		fragmentStream << fragmentFile.rdbuf();

		vertexCode = vertexStream.str();
		fragmentCode = fragmentStream.str();

		if (geometryPath != nullptr)
		{
			std::ifstream geometryFile(geometryPath);

			std::stringstream geometryStream;

			geometryStream << geometryFile.rdbuf();

			geometryCode = geometryStream.str();
		}
	}
	catch (std::exception e)
	{
		std::cout << "Failed to open shader file" << std::endl;
	}

	

	Shader shader;
	shader.compile(vertexCode.c_str(), fragmentCode.c_str(), geometryPath == nullptr ? nullptr : geometryCode.c_str());
	return shader;
}


Texture ResourceManager::loadTextureFromFile(const char* path, bool alpha)
{
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

	Texture texture;
	if (data)
	{
		if (alpha)
		{
			texture.mInternalFormat = GL_RGBA;
			texture.mImageFormat = GL_RGBA;
		}
		texture.generate(width, height, data);
	}
	else
	{
		std::cout << "Failed to load texture with path: " << path << std::endl;
	}

	stbi_image_free(data);
	return texture;
}
