#include "cVAOManager.h"

cVAOManager::cVAOManager()
{
}

cVAOManager::~cVAOManager()
{
}

bool cVAOManager::loadModelToVAO(std::string filename, cModelDrawInfo& drawInfo, unsigned int shaderProgramID)
{
	GLenum error;
	
	drawInfo.meshName = filename;

	drawInfo.CalculateExtents();

	glGenVertexArrays(1, &(drawInfo.VAO_ID));
	glBindVertexArray(drawInfo.VAO_ID);

	//vertices
	glGenBuffers(1, &(drawInfo.VertexBufferID));
	glBindBuffer(GL_ARRAY_BUFFER, drawInfo.VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cModelDrawInfo::sVertex_XYZ_N) * drawInfo.numberOfVertices, (GLvoid*)drawInfo.pVertices, GL_STATIC_DRAW);

	//indices
	glGenBuffers(1, &(drawInfo.IndexBufferID));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawInfo.IndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * drawInfo.numberOfIndices, (GLvoid*)drawInfo.pIndices, GL_STATIC_DRAW);

	//in vec3 vPosition;			
	GLint vPosition_location = glGetAttribLocation(shaderProgramID, "vPosition");
	glEnableVertexAttribArray(vPosition_location);
	glVertexAttribPointer(vPosition_location,
		3, 
		GL_FLOAT,
		GL_FALSE,
		sizeof(cModelDrawInfo::sVertex_XYZ_N),						// Stride	(number of bytes)
		(void*)offsetof(cModelDrawInfo::sVertex_XYZ_N, x));		// Offset the member variable

	//in vec3 vNormal;			
	GLint vNormal_location = glGetAttribLocation(shaderProgramID, "vNormal");
	error = glGetError();
	glEnableVertexAttribArray(vNormal_location);
	error = glGetError();
	glVertexAttribPointer(vNormal_location,
		3, 
		GL_FLOAT,
		GL_FALSE,
		sizeof(cModelDrawInfo::sVertex_XYZ_N),						// Stride	(number of bytes)
		(void*)offsetof(cModelDrawInfo::sVertex_XYZ_N, nx));		// Offset the member variable
	error = glGetError();

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(vPosition_location);
	glDisableVertexAttribArray(vNormal_location);

	this->mapModelNametoVAOID[drawInfo.meshName] = drawInfo;

	return true;
}

bool cVAOManager::loadModelList(std::string filename, unsigned int shaderProgramID)
{
	cXML modelListXML;
	bool result;

	result = modelListXML.loadModelFromXML(filename, mapInstanceNametoMeshObj, pVecInstanceMeshObj);
	if (!result)
	{
		return false;
	}

	std::map<std::string, std::string>::iterator i_mapModel;

	for (i_mapModel = modelListXML.mapModelNameAndPath.begin(); i_mapModel != modelListXML.mapModelNameAndPath.end(); i_mapModel++)
	{
		cModelDrawInfo modelDrawInfo;
		//cMeshObj meshObj;
		std::string error = "";
		result = loadPLYFile(i_mapModel->second, modelDrawInfo, error);
		if (!result)
		{
			std::cout << "cannot load " << i_mapModel->first << std::endl;
			std::cout << "error " << error << std::endl;
			return false;
		}
		result = loadModelToVAO(i_mapModel->first, modelDrawInfo, shaderProgramID);
		if (!result)
		{
			std::cout << "cannot load " << i_mapModel->first << std::endl;
			return false;
		}

		//mapModelNametoMeshObj.emplace(i_mapModel->first, meshObj);
		std::cout << i_mapModel->first << " is loaded" << std::endl;
	}
	cameraEyeFromXML = modelListXML.cameraEyeFromXML;
	

	return true;
}

bool cVAOManager::FindDrawInfo(std::string filename, cModelDrawInfo& drawInfo)
{
	std::map<std::string, cModelDrawInfo>::iterator i_DrawInfo = mapModelNametoVAOID.find(filename);
	
	if (i_DrawInfo==mapModelNametoVAOID.end())
	{
		return false;
	}

	drawInfo = i_DrawInfo->second;

	return true;
}

bool cVAOManager::loadPLYFile(std::string filename, cModelDrawInfo& modelDrawInfo, std::string error)
{
	sTrianglePLY* pTheModelTriangleArray = NULL;

	std::ifstream modelFile(filename);
	if (!modelFile.is_open())
	{
		error = " cannot open " + filename;
		return false;
	}

	char buffer[MODEL_LOAD_BUFFER];
	modelFile.getline(buffer, MODEL_LOAD_BUFFER);
	std::string nextToken;

	while (modelFile >> nextToken)
	{
		if (nextToken == "vertex")
		{
			break;
		}
	}
	modelFile >> modelDrawInfo.numberOfVertices;

	while (modelFile >> nextToken)
	{
		if (nextToken == "face")
		{
			break;
		}
	}
	modelFile >> modelDrawInfo.numberOfTriangles;

	while (modelFile >> nextToken)
	{
		if (nextToken == "end_header")
		{
			break;
		}
	}

	modelDrawInfo.pVertices = new cModelDrawInfo::sVertex_XYZ_N[modelDrawInfo.numberOfVertices];

	for (unsigned int i = 0; i != modelDrawInfo.numberOfVertices; i++)
	{

		modelFile >> modelDrawInfo.pVertices[i].x;
		modelFile >> modelDrawInfo.pVertices[i].y;
		modelFile >> modelDrawInfo.pVertices[i].z;

		modelFile >> modelDrawInfo.pVertices[i].nx;
		modelFile >> modelDrawInfo.pVertices[i].ny;
		modelFile >> modelDrawInfo.pVertices[i].nz;

	}

	pTheModelTriangleArray = new sTrianglePLY[modelDrawInfo.numberOfTriangles];
	for (unsigned int count = 0; count != modelDrawInfo.numberOfTriangles; count++)
	{
		unsigned int discard = 0;
		modelFile >> discard;

		modelFile >> pTheModelTriangleArray[count].vertexIndices[0];
		modelFile >> pTheModelTriangleArray[count].vertexIndices[1];
		modelFile >> pTheModelTriangleArray[count].vertexIndices[2];
	}

	modelFile.close();

	modelDrawInfo.numberOfIndices = modelDrawInfo.numberOfTriangles * 3;
	modelDrawInfo.pIndices = new unsigned int[modelDrawInfo.numberOfIndices];

	unsigned int vertex_element_index_index = 0;

	for (unsigned int i = 0; i != modelDrawInfo.numberOfTriangles; i++)
	{
		modelDrawInfo.pIndices[vertex_element_index_index + 0] = pTheModelTriangleArray[i].vertexIndices[0];
		modelDrawInfo.pIndices[vertex_element_index_index + 1] = pTheModelTriangleArray[i].vertexIndices[1];
		modelDrawInfo.pIndices[vertex_element_index_index + 2] = pTheModelTriangleArray[i].vertexIndices[2];
		vertex_element_index_index += 3;
	}
	return true;
}

bool cVAOManager::setInstanceObjScale(std::string meshObjName, float value)
{
	std::map<std::string, cMeshObj* >::iterator itCurrentMesh = mapInstanceNametoMeshObj.find(meshObjName);
	if (itCurrentMesh== mapInstanceNametoMeshObj.end())
	{
		return false;
	}
	cMeshObj* pCurrentMeshObject = itCurrentMesh->second;

	pCurrentMeshObject->scale = value;

	return true;
}

bool cVAOManager::setInstanceObjWireframe(std::string meshObjName, bool value)
{
	std::map<std::string, cMeshObj* >::iterator itCurrentMesh = mapInstanceNametoMeshObj.find(meshObjName);
	if (itCurrentMesh == mapInstanceNametoMeshObj.end())
	{
		return false;
	}
	cMeshObj* pCurrentMeshObject = itCurrentMesh->second;

	pCurrentMeshObject->isWireframe = value;

	return true;
}

bool cVAOManager::setInstanceObjRGB(std::string meshObjName, glm::vec4 value)
{
	std::map<std::string, cMeshObj* >::iterator itCurrentMesh = mapInstanceNametoMeshObj.find(meshObjName);
	if (itCurrentMesh == mapInstanceNametoMeshObj.end())
	{
		return false;
	}
	cMeshObj* pCurrentMeshObject = itCurrentMesh->second;

	pCurrentMeshObject->color_RGBA = value;

	return true;
}

bool cVAOManager::setInstanceObjVisible(std::string meshObjName, bool value)
{
	std::map<std::string, cMeshObj* >::iterator itCurrentMesh = mapInstanceNametoMeshObj.find(meshObjName);
	if (itCurrentMesh == mapInstanceNametoMeshObj.end())
	{
		return false;
	}
	cMeshObj* pCurrentMeshObject = itCurrentMesh->second;

	pCurrentMeshObject->isVisible = value;

	return true;
}

bool cVAOManager::setInstanceObjLighting(std::string meshObjName, bool value)
{
	std::map<std::string, cMeshObj* >::iterator itCurrentMesh = mapInstanceNametoMeshObj.find(meshObjName);
	if (itCurrentMesh == mapInstanceNametoMeshObj.end())
	{
		return false;
	}
	cMeshObj* pCurrentMeshObject = itCurrentMesh->second;
	pCurrentMeshObject->bDoNotLight = !value;
	return true;
}

bool cVAOManager::setInstanceObjSpecularPower(std::string meshObjName, glm::vec4 value)
{
	std::map<std::string, cMeshObj* >::iterator itCurrentMesh = mapInstanceNametoMeshObj.find(meshObjName);
	if (itCurrentMesh == mapInstanceNametoMeshObj.end())
	{
		return false;
	}
	cMeshObj* pCurrentMeshObject = itCurrentMesh->second;
	pCurrentMeshObject->specular_colour_and_power = value;

	return true;
}

bool cVAOManager::setInstanceObjPosition(std::string meshObjName, glm::vec4 value)
{
	std::map<std::string, cMeshObj* >::iterator itCurrentMesh = mapInstanceNametoMeshObj.find(meshObjName);
	if (itCurrentMesh == mapInstanceNametoMeshObj.end())
	{
		return false;
	}
	cMeshObj* pCurrentMeshObject = itCurrentMesh->second;
	pCurrentMeshObject->position = value;

	return true;
}
