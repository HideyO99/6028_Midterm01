#include "cMeshObj.h"

cMeshObj::cMeshObj()
{
	this->meshName.clear();

	this->position = glm::vec3(0.f);
	this->rotation = glm::vec3(0.0f);
	this->scale = 1.f;
	this->isWireframe = false;

	this->color_RGBA = glm::vec4(0.f, 0.f, 0.f, 1.f);
	this->specular_colour_and_power = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	this->isVisible = true;
	this->bDoNotLight = false;
}

cMeshObj::~cMeshObj()
{
}
