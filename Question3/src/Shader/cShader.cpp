#include "cShaderManager.h"

cShaderManager::cShader::cShader()
{
	this->ID = 0;
	this->shaderType = UNDKNOW;
	this->fileName = "";
	this->vecSource.clear();
}

cShaderManager::cShader::~cShader()
{
}
