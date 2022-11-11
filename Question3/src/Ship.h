#pragma once
#include "MeshObj/cMeshObj.h"
#include "Light/cLightManager.h"
//#include <chrono>
class PirateShip
{
public:
	PirateShip();
	~PirateShip();

	cMeshObj* pShipOBJ;
	glm::vec3 startPos;
	//glm::vec3 curPos;
	glm::vec3 endPos;
	glm::vec3 norm;
	cLightManager* lantern;
	//std::chrono::milliseconds ms(1000);

	void updatePos();

};

