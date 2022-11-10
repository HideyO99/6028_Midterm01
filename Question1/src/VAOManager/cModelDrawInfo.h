#pragma once
#include <string>
class cModelDrawInfo
{
public:
	cModelDrawInfo();
	~cModelDrawInfo();

	struct sVertex_XYZ_N
	{
		float x, y, z;		//	in vec3 vPosition;		
		float nx, ny, nz;	//	in vec3 vNormal;	
	};



	std::string meshName;

	unsigned int VAO_ID;

	unsigned int VertexBufferID;
	unsigned int VertexBuffer_Start_Index;
	unsigned int numberOfVertices;

	unsigned int IndexBufferID;
	unsigned int IndexBuffer_Start_Index;
	unsigned int numberOfIndices;
	unsigned int numberOfTriangles;

	void CalculateExtents(void);

	float maxX, maxY, maxZ;
	float minX, minY, minZ;

	float extentX, extentY, extentZ;

	float maxExtent;


	sVertex_XYZ_N* pVertices;	

	unsigned int* pIndices;
};

