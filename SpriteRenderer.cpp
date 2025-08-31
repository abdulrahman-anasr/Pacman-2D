#include "SpriteRenderer.h"


/*											PUBLIC FUNCTIONS										*/
SpriteRenderer::SpriteRenderer(Shader& shader)
{
	this->mShader = shader;

	initRenderData();
}

SpriteRenderer::~SpriteRenderer()
{

}

void SpriteRenderer::drawSprite(Texture& texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
{
	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(position, 0.0f));

	model = glm::translate(model, glm::vec3(size.x * 0.5, size.y * 0.5, 0.0f));
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(size.x * -0.5, size.y * -0.5, 0.0f));

	model = glm::scale(model, glm::vec3(size, 1.0f));

	mShader.setMat4("model", model , true);
	mShader.setVector3("spriteColor", color);

	glBindVertexArray(this->VAO);

	glActiveTexture(GL_TEXTURE0);
	texture.bind();

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);



}

void SpriteRenderer::drawSpriteFromSheet(Texture& texture, glm::vec2 position, glm::vec2 sheetPosition, glm::vec2 spriteSize,
	glm::vec2 size, float rotate, glm::vec3 color)
{
	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(position, 0.0f));

	model = glm::translate(model, glm::vec3(size.x * 0.5, size.y * 0.5, 0.0f));
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(size.x * -0.5, size.y * -0.5, 0.0f));

	model = glm::scale(model, glm::vec3(size, 1.0f));

	mShader.setMat4("model", model, true);
	mShader.setVector3("spriteColor", color);

	float startingPositionX = sheetPosition.x * texture.mUnitPixelWidth;
	float startingPositionY = sheetPosition.y * texture.mUnitPixelHeight;

	float spriteWidthX = spriteSize.x * texture.mUnitPixelWidth;
	float spriteWidthY = spriteSize.y * texture.mUnitPixelHeight;

	float maxPositionX = startingPositionX + spriteWidthX;
	float maxPositionY = startingPositionY + spriteWidthY;

	glBindVertexArray(this->spriteFromSheetVAO);
	
	float spriteFromSheetVertices[] = {
		0.0f, 1.0f, startingPositionX , maxPositionY,
		1.0f, 0.0f, maxPositionX , startingPositionY,
		0.0f, 0.0f, startingPositionX , startingPositionY,
		0.0f, 1.0f, startingPositionX , maxPositionY,
		1.0f, 1.0f, maxPositionX , maxPositionY,
		1.0f, 0.0f, maxPositionX , startingPositionY
	};


	texture.bind();

	glBindBuffer(GL_ARRAY_BUFFER, this->spriteFromSheetVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(spriteFromSheetVertices), spriteFromSheetVertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

}


/*											PRIVATE FUNCTIONS										*/

void SpriteRenderer::initRenderData()
{
	unsigned int VBO;
	float vertices[] = {
		// pos // tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) (2 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	float spriteFromSheetVertices[] = {
		0.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
	};

	glGenVertexArrays(1, &this->spriteFromSheetVAO);
	glBindVertexArray(this->spriteFromSheetVAO);

	glGenBuffers(1, &this->spriteFromSheetVBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->spriteFromSheetVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

