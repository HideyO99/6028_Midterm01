#pragma once
#include "..\globalOpenGL.h"
#include "cLight.h"
#include <string>

#define MAX_LIGHT_SOURCE 10

class cLightManager
{
public:
	cLightManager();
	~cLightManager();

	cLight* plight[MAX_LIGHT_SOURCE];

	void loadLightUniformLocation(unsigned int shaderID);
	void setLightToShader(unsigned int shaderID);
};

