#include "cShaderManager.h"
#include <iostream>

cShaderManager::cShaderManager()
{
	this->shaderID = 0;
	this->lastError.clear();
	this->mapIDtoShader.clear();
	this->mapNameToID.clear();
}

cShaderManager::~cShaderManager()
{
}

bool cShaderManager::createShaderProgram(std::string callSign, cShader& vertex_shader, cShader& fragment_shader)
{
	bool result;
	std::string error = "";
	//vertex shader
	vertex_shader.ID = glCreateShader(GL_VERTEX_SHADER);
	vertex_shader.shaderType = cShader::VERTEX_SHADER;
	
	//load vertex shader from file
	result = cLoadShaderSourceCode(vertex_shader);
	if (!result)
	{
		return false;
	}
	//compile vertex shader
	result = cCompileShader(vertex_shader, error);
	if (!result)
	{
		return false;
	}

	//fragment shader
	fragment_shader.ID = glCreateShader(GL_FRAGMENT_SHADER);
	fragment_shader.shaderType = cShader::FRAGMENT_SHADER;

	//load fragment shader from file
	result = cLoadShaderSourceCode(fragment_shader);
	if (!result)
	{
		return false;
	}
	//compile fragment shader
	result = cCompileShader(fragment_shader, error);
	if (!result)
	{
		return false;
	}

	//clear error for link
	error.clear();
	//create program
	cShaderProgram currentProgram;
	currentProgram.ID = glCreateProgram();

	glAttachShader(currentProgram.ID, vertex_shader.ID);
	glAttachShader(currentProgram.ID, fragment_shader.ID);
	glLinkProgram(currentProgram.ID);
	glValidateProgram(currentProgram.ID);

	result = cIsLinkOK(currentProgram.ID, error);
	if (!result)
	{
		return false;
	}

	glDeleteShader(vertex_shader.ID);
	glDeleteShader(fragment_shader.ID);

	currentProgram.callsign = callSign;

	this->mapIDtoShader[currentProgram.ID] = currentProgram;
	this->mapNameToID[currentProgram.callsign] = currentProgram.ID;

	return true;
}

bool cShaderManager::useShaderProgram(unsigned int id)
{
	glUseProgram(id);
	shaderID = id;
	return true;
}

bool cShaderManager::useShaderPRogram(std::string callSign)
{
	std::map<std::string, unsigned int>::iterator i_shader = mapNameToID.find(callSign);
	if (i_shader == mapNameToID.end())
	{
		return false;
	}

	glUseProgram(i_shader->second);
	shaderID = i_shader->second;

 	return true;
}

unsigned int cShaderManager::getIDfromName(std::string callSign)
{
	std::map<std::string, unsigned int>::iterator i_shader = mapNameToID.find(callSign);
	if (i_shader == mapNameToID.end())
	{
		return 0;
	}

	return i_shader->second;
}

void cShaderManager::setShaderUniformM4fv(std::string name, glm::mat4 value)
{
	glUniformMatrix4fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void cShaderManager::setShaderUniform1f(std::string name, float value)
{
	GLint lo = glGetUniformLocation(shaderID, name.c_str());
	glUniform1f(lo, value);
}

void cShaderManager::setShaderUniform4f(std::string name, const float value1, const float value2, const float value3, const float value4)
{
	GLint lo = glGetUniformLocation(shaderID, name.c_str());
	glUniform4f(lo, value1, value2, value3, value4);
}



bool cShaderManager::cLoadShaderSourceCode(cShader& shader)
{
	std::string code;
	std::ifstream shaderFile(shader.fileName.c_str());
	if (!shaderFile.is_open())
	{
		return false;
	}
	shader.vecSource.clear();
	while (std::getline(shaderFile, code))
	{
		shader.vecSource.push_back(code);
	}

	shaderFile.close();

	return true;
}

bool cShaderManager::cCompileShader(cShader& shader, std::string& error)
{
	bool result;
	unsigned int numLine = static_cast<unsigned int>(shader.vecSource.size());
	char** arraySource = new char*[numLine];
	memset(arraySource, 0, numLine);

	for (unsigned int lineIndex = 0; lineIndex != numLine; lineIndex++)
	{
		unsigned int numChar = (unsigned int)shader.vecSource[lineIndex].length();
		arraySource[lineIndex] = new char[numChar + 2];
		memset(arraySource[lineIndex], 0, numChar + 2);

		for (unsigned int charIndex = 0; charIndex != numChar; charIndex++)
		{
			arraySource[lineIndex][charIndex] = shader.vecSource[lineIndex][charIndex];
		}
		arraySource[lineIndex][numChar] = '\n';
		arraySource[lineIndex][numChar+1] = '\0';
	}
	//
	glShaderSource(shader.ID, numLine, arraySource, NULL);
	glCompileShader(shader.ID);

	//release mem
	for (unsigned int lineIndex = 0; lineIndex != numLine; lineIndex++)
	{
		delete [] arraySource[lineIndex];
	}
	delete [] arraySource;

	//check compile error
	
	result = cIsCompileOK(shader.ID, error);
	if (!result)
	{
		std::string shaderType;
		if (shader.shaderType == cShader::VERTEX_SHADER)
		{
			shaderType = "Vertex";
		}
		if (shader.shaderType == cShader::FRAGMENT_SHADER)
		{
			shaderType = "Fragment";
		}
		std::cout << shaderType <<"Shader compile error: " << error << std::endl;
		return false;
	}

	return true;
}

bool cShaderManager::cIsCompileOK(const unsigned int shader_id, std::string& error)
{
	GLint isCompiled = 0;
	error = "";
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled==GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &maxLength);
		
		char* pLog = new char[maxLength];
		glGetShaderInfoLog(shader_id, maxLength, &maxLength, pLog);
		error.append(pLog);
		
		lastError.append("\n"+error);

		delete [] pLog;

		return false;
	}
	return true;
}

bool cShaderManager::cIsLinkOK(const unsigned int program_id, std::string& error)
{
	GLint isLinked = 0;
	error = "";
	glGetProgramiv(program_id, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &maxLength);

		char* pLog = new char[maxLength];
		glGetProgramInfoLog(program_id, maxLength, &maxLength, pLog);
		error.append(pLog);

		lastError.append("\n" + error);

		delete[] pLog;

		return false;
	}
	return true;
}
