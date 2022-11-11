#include "Ship.h"
#include <glm/gtc/matrix_transform.hpp> 


PirateShip::PirateShip()
{
	startPos = glm::vec3(14.84f, 81.68f, -29.05f);
	endPos = glm::vec3(-110.f, 10.f, 15.f);
	norm = glm::normalize(endPos - startPos);
	pShipOBJ = nullptr;
	lantern = nullptr;
	//pShipOBJ->rotation = glm::vec3(0, atan(norm.x / norm.z), 0);
}

PirateShip::~PirateShip()
{
}

void PirateShip::updatePos()
{
	pShipOBJ->position += glm::vec3(norm.x / 10, norm.y / 10, norm.z / 10);
	pShipOBJ->rotation = glm::vec3(0, atan(norm.x / norm.z), 0);


	if (pShipOBJ->position.y < endPos.y)
	{
		norm = glm::normalize(startPos - endPos);
	}
	if (pShipOBJ->position.y > startPos.y)
	{
		norm = glm::normalize(endPos - startPos);
	}
	lantern->plight[0]->direction = glm::vec4(pShipOBJ->position.x, pShipOBJ->position.y + 8, pShipOBJ->position.z, 1.f);
	lantern->plight[6]->position = glm::vec4(pShipOBJ->position.x, pShipOBJ->position.y + 4, pShipOBJ->position.z + 2, 1.f);
	lantern->plight[7]->position = glm::vec4(pShipOBJ->position.x, pShipOBJ->position.y + 4, pShipOBJ->position.z - 4, 1.f);
	lantern->plight[8]->position = glm::vec4(pShipOBJ->position.x + (10 * cos(pShipOBJ->rotation.y)), pShipOBJ->position.y, pShipOBJ->position.z + (10 * sin(pShipOBJ->rotation.y)), 1.f);
	//lantern->plight[8]->direction = glm::vec4(norm,1.f);
}
