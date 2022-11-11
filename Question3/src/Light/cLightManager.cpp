#include "cLightManager.h"

cLightManager::cLightManager()
{
	for (size_t i = 0; i < MAX_LIGHT_SOURCE; i++)
	{
		cLight* light = new cLight();
		this->plight[i] = light;
	}
}

cLightManager::~cLightManager()
{
}

void cLightManager::loadLightUniformLocation(unsigned int shaderID)
{
	plight[0]->position_location = glGetUniformLocation(shaderID, "Light[0].position");
	plight[0]->diffuse_location = glGetUniformLocation(shaderID, "Light[0].diffuse");
	plight[0]->specular_location = glGetUniformLocation(shaderID, "Light[0].specular");
	plight[0]->attenuation_location = glGetUniformLocation(shaderID, "Light[0].attenuation");
	plight[0]->direction_location = glGetUniformLocation(shaderID, "Light[0].direction");
	plight[0]->angle_location = glGetUniformLocation(shaderID, "Light[0].angle");
	plight[0]->type_location = glGetUniformLocation(shaderID, "Light[0].type");
	plight[0]->turnON_location = glGetUniformLocation(shaderID, "Light[0].turnON");

	plight[1]->position_location = glGetUniformLocation(shaderID, "Light[1].position");
	plight[1]->diffuse_location = glGetUniformLocation(shaderID, "Light[1].diffuse");
	plight[1]->specular_location = glGetUniformLocation(shaderID, "Light[1].specular");
	plight[1]->attenuation_location = glGetUniformLocation(shaderID, "Light[1].attenuation");
	plight[1]->direction_location = glGetUniformLocation(shaderID, "Light[1].direction");
	plight[1]->angle_location = glGetUniformLocation(shaderID, "Light[1].angle");
	plight[1]->type_location = glGetUniformLocation(shaderID, "Light[1].type");
	plight[1]->turnON_location = glGetUniformLocation(shaderID, "Light[1].turnON");
			
	plight[2]->position_location = glGetUniformLocation(shaderID, "Light[2].position");
	plight[2]->diffuse_location = glGetUniformLocation(shaderID, "Light[2].diffuse");
	plight[2]->specular_location = glGetUniformLocation(shaderID, "Light[2].specular");
	plight[2]->attenuation_location = glGetUniformLocation(shaderID, "Light[2].attenuation");
	plight[2]->direction_location = glGetUniformLocation(shaderID, "Light[2].direction");
	plight[2]->angle_location = glGetUniformLocation(shaderID, "Light[2].angle");
	plight[2]->type_location = glGetUniformLocation(shaderID, "Light[2].type");
	plight[2]->turnON_location = glGetUniformLocation(shaderID, "Light[2].turnON");
		
	plight[3]->position_location = glGetUniformLocation(shaderID, "Light[3].position");
	plight[3]->diffuse_location = glGetUniformLocation(shaderID, "Light[3].diffuse");
	plight[3]->specular_location = glGetUniformLocation(shaderID, "Light[3].specular");
	plight[3]->attenuation_location = glGetUniformLocation(shaderID, "Light[3].attenuation");
	plight[3]->direction_location = glGetUniformLocation(shaderID, "Light[3].direction");
	plight[3]->angle_location = glGetUniformLocation(shaderID, "Light[3].angle");
	plight[3]->type_location = glGetUniformLocation(shaderID, "Light[3].type");
	plight[3]->turnON_location = glGetUniformLocation(shaderID, "Light[3].turnON");
		
	plight[4]->position_location = glGetUniformLocation(shaderID, "Light[4].position");
	plight[4]->diffuse_location = glGetUniformLocation(shaderID, "Light[4].diffuse");
	plight[4]->specular_location = glGetUniformLocation(shaderID, "Light[4].specular");
	plight[4]->attenuation_location = glGetUniformLocation(shaderID, "Light[4].attenuation");
	plight[4]->direction_location = glGetUniformLocation(shaderID, "Light[4].direction");
	plight[4]->angle_location = glGetUniformLocation(shaderID, "Light[4].angle");
	plight[4]->type_location = glGetUniformLocation(shaderID, "Light[4].type");
	plight[4]->turnON_location = glGetUniformLocation(shaderID, "Light[4].turnON");

	plight[5]->position_location = glGetUniformLocation(shaderID, "Light[5].position");
	plight[5]->diffuse_location = glGetUniformLocation(shaderID, "Light[5].diffuse");
	plight[5]->specular_location = glGetUniformLocation(shaderID, "Light[5].specular");
	plight[5]->attenuation_location = glGetUniformLocation(shaderID, "Light[5].attenuation");
	plight[5]->direction_location = glGetUniformLocation(shaderID, "Light[5].direction");
	plight[5]->angle_location = glGetUniformLocation(shaderID, "Light[5].angle");
	plight[5]->type_location = glGetUniformLocation(shaderID, "Light[5].type");
	plight[5]->turnON_location = glGetUniformLocation(shaderID, "Light[5].turnON");

	plight[6]->position_location = glGetUniformLocation(shaderID, "Light[6].position");
	plight[6]->diffuse_location = glGetUniformLocation(shaderID, "Light[6].diffuse");
	plight[6]->specular_location = glGetUniformLocation(shaderID, "Light[6].specular");
	plight[6]->attenuation_location = glGetUniformLocation(shaderID, "Light[6].attenuation");
	plight[6]->direction_location = glGetUniformLocation(shaderID, "Light[6].direction");
	plight[6]->angle_location = glGetUniformLocation(shaderID, "Light[6].angle");
	plight[6]->type_location = glGetUniformLocation(shaderID, "Light[6].type");
	plight[6]->turnON_location = glGetUniformLocation(shaderID, "Light[6].turnON");

	plight[7]->position_location = glGetUniformLocation(shaderID, "Light[7].position");
	plight[7]->diffuse_location = glGetUniformLocation(shaderID, "Light[7].diffuse");
	plight[7]->specular_location = glGetUniformLocation(shaderID, "Light[7].specular");
	plight[7]->attenuation_location = glGetUniformLocation(shaderID, "Light[7].attenuation");
	plight[7]->direction_location = glGetUniformLocation(shaderID, "Light[7].direction");
	plight[7]->angle_location = glGetUniformLocation(shaderID, "Light[7].angle");
	plight[7]->type_location = glGetUniformLocation(shaderID, "Light[7].type");
	plight[7]->turnON_location = glGetUniformLocation(shaderID, "Light[7].turnON");

	plight[8]->position_location = glGetUniformLocation(shaderID, "Light[8].position");
	plight[8]->diffuse_location = glGetUniformLocation(shaderID, "Light[8].diffuse");
	plight[8]->specular_location = glGetUniformLocation(shaderID, "Light[8].specular");
	plight[8]->attenuation_location = glGetUniformLocation(shaderID, "Light[8].attenuation");
	plight[8]->direction_location = glGetUniformLocation(shaderID, "Light[8].direction");
	plight[8]->angle_location = glGetUniformLocation(shaderID, "Light[8].angle");
	plight[8]->type_location = glGetUniformLocation(shaderID, "Light[8].type");
	plight[8]->turnON_location = glGetUniformLocation(shaderID, "Light[8].turnON");

	plight[9]->position_location = glGetUniformLocation(shaderID, "Light[9].position");
	plight[9]->diffuse_location = glGetUniformLocation(shaderID, "Light[9].diffuse");
	plight[9]->specular_location = glGetUniformLocation(shaderID, "Light[9].specular");
	plight[9]->attenuation_location = glGetUniformLocation(shaderID, "Light[9].attenuation");
	plight[9]->direction_location = glGetUniformLocation(shaderID, "Light[9].direction");
	plight[9]->angle_location = glGetUniformLocation(shaderID, "Light[9].angle");
	plight[9]->type_location = glGetUniformLocation(shaderID, "Light[9].type");
	plight[9]->turnON_location = glGetUniformLocation(shaderID, "Light[9].turnON");
}			

void cLightManager::setLightToShader(unsigned int shaderID)
{
	for (size_t i = 0; i < MAX_LIGHT_SOURCE; i++)
	{


		glUniform4f(plight[i]->position_location,
			plight[i]->position.x,
			plight[i]->position.y,
			plight[i]->position.z,
			plight[i]->position.w);

		glUniform4f(plight[i]->diffuse_location,
			plight[i]->diffuse.x,
			plight[i]->diffuse.y,
			plight[i]->diffuse.z,
			plight[i]->diffuse.w);

		glUniform4f(plight[i]->specular_location,
			plight[i]->specular.x,
			plight[i]->specular.y,
			plight[i]->specular.z,
			plight[i]->specular.w);

		glUniform4f(plight[i]->attenuation_location,
			plight[i]->attenuation.x,
			plight[i]->attenuation.y,
			plight[i]->attenuation.z,
			plight[i]->attenuation.w);

		glUniform4f(plight[i]->direction_location,
			plight[i]->direction.x,
			plight[i]->direction.y,
			plight[i]->direction.z,
			plight[i]->direction.w);

		glUniform2f(plight[i]->angle_location,
			plight[i]->angle.x,
			plight[i]->angle.y);

		glUniform1i(plight[i]->turnON_location,
			plight[i]->turnON);

	}
}
