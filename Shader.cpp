#include "Shader.h"

/*											PUBLIC FUNCTIONS										*/

Shader::Shader() = default;


void Shader::compile(const char* vertexCode, const char* fragmentCode, const char* geometryCode)
{
	unsigned int vertex, fragment, geometry;

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexCode, NULL);
	glCompileShader(vertex);
	checkCompilationErrors(vertex, "VERTEX");

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentCode, NULL);
	glCompileShader(fragment);
	checkCompilationErrors(fragment, "FRAGMENT");

	if (geometryCode != nullptr)
	{
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &geometryCode, NULL);
		glCompileShader(geometry);
		checkCompilationErrors(geometry, "GEOMETRY");
	}

	this->mID = glCreateProgram();

	glAttachShader(mID, vertex);
	
	glAttachShader(mID, fragment);
	
	if(geometryCode != nullptr) glAttachShader(mID, geometry);

	glLinkProgram(this->mID);

	checkCompilationErrors(this->mID, "PROGRAM");
}

Shader& Shader::use()
{
	glUseProgram(this->mID);
	return *this;
}


void Shader::setInteger(std::string name, GLint value, bool useShader)
{
	if (useShader) this->use();

	glUniform1i(getUniformLocation(name), value);
}

void Shader::setFloat(std::string name, GLfloat value, bool useShader)
{
	if (useShader) this->use();

	glUniform1f(getUniformLocation(name), value);
}

void Shader::setVector2(std::string name, glm::vec2 value, bool useShader)
{
	if (useShader) this->use();

	glUniform2fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::setVector3(std::string name, glm::vec3 value, bool useShader)
{
	if (useShader) this->use();

	glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::setVector4(std::string name, glm::vec4 value, bool useShader)
{
	if (useShader) this->use();

	glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::setMat4(std::string name, glm::mat4 value, bool useShader)
{
	if (useShader) this->use();

	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}





/*											PRIVATE FUNCTIONS										*/

GLint Shader::getUniformLocation(std::string name)
{
	return glGetUniformLocation(this->mID, name.c_str());
}

void Shader::checkCompilationErrors(unsigned int object, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(object, 1024, NULL, infoLog);
			std::cout << "SHADER::ERROR: Compilation Error, Type: " << type << "\n" << infoLog << "\n" <<
				"------------------------------------------------------------------------" << std::endl;
		}
	}
	else
	{
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(object, 1024, NULL, infoLog);
			std::cout << "PROGRAM::ERROR: Linking Error, Type: " << type << "\n" << infoLog << "\n" <<
				"------------------------------------------------------------------------" << std::endl;
		}
	}
}