#include "cModelDrawInfo.h"

cModelDrawInfo::cModelDrawInfo()
{
	this->VAO_ID = 0;

	this->VertexBufferID = 0;
	this->VertexBuffer_Start_Index = 0;
	this->numberOfVertices = 0;

	this->IndexBufferID = 0;
	this->IndexBuffer_Start_Index = 0;
	this->numberOfIndices = 0;
	this->numberOfTriangles = 0;

	// The "local" (i.e. "CPU side" temporary array)
	this->pVertices = NULL;		// or 0 or nullptr
	this->pIndices = NULL;		// or 0 or nullptr

	this->minX = this->minY = this->minZ = 0.0f;
	this->maxX = this->maxY = this->maxZ = 0.0f;
	this->extentX = this->extentY = this->extentZ = 0.0f;
	this->maxExtent = 0.0f;

	return;
}

cModelDrawInfo::~cModelDrawInfo()
{
}

void cModelDrawInfo::CalculateExtents(void)
{
	// Do we even have an array?
	if (this->pVertices)		// same as != NULL
	{
		// Assume that the 1st vertex is both the min and max
		this->minX = this->maxX = this->pVertices[0].x;
		this->minY = this->maxY = this->pVertices[0].y;
		this->minZ = this->maxZ = this->pVertices[0].z;

		for (unsigned int index = 0; index != this->numberOfVertices; index++)
		{
			this->minX = std::min(this->minX, this->pVertices[index].x);
			this->minY = std::min(this->minY, this->pVertices[index].y);
			this->minZ = std::min(this->minZ, this->pVertices[index].z);

			this->maxX = std::max(this->maxX, this->pVertices[index].x);
			this->maxY = std::max(this->maxY, this->pVertices[index].y);
			this->maxZ = std::max(this->maxZ, this->pVertices[index].z);

		}//for (unsigned int index = 0...
	}//if ( this->pVertices )

	// Update the extents
	this->extentX = this->maxX - this->minX;
	this->extentY = this->maxY - this->minY;
	this->extentZ = this->maxZ - this->minZ;

	// What's the largest of the three extents
	this->maxExtent = this->extentX;

	this->maxExtent = std::max(this->maxExtent, this->extentY);
	this->maxExtent = std::max(this->maxExtent, this->extentZ);

	return;
}
