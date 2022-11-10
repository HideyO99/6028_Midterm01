#include "cShaderManager.h"

cShaderManager::cShaderProgram::cShaderProgram()
{
	this->ID = 0;
	this->callsign.clear();
	this->mapUniformNameToUniformLocation.clear();
}

cShaderManager::cShaderProgram::~cShaderProgram()
{
}

int cShaderManager::cShaderProgram::getUniformID(std::string name)
{
	std::map<std::string, int>::iterator i_uniform = mapUniformNameToUniformLocation.find(name);

	if (i_uniform == mapUniformNameToUniformLocation.end())
	{
		return -1;
	}

	return i_uniform->second;
}

bool cShaderManager::cShaderProgram::LoadUniformLocation(std::string varName)
{
	GLint location = glGetUniformLocation(this->ID, varName.c_str());

	if (location == -1)
	{
		return false;
	}

	mapUniformNameToUniformLocation[varName.c_str()] = location;

	return true;
}
