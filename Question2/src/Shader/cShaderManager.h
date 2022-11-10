#pragma once
#include "..\globalOpenGL.h"

#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

class cShaderManager
{
public:
	cShaderManager();
	~cShaderManager();

	class cShader
	{
	public:
		cShader();
		~cShader();

		enum eMyShaderType
		{
			VERTEX_SHADER,
			FRAGMENT_SHADER,
			UNDKNOW = 0xff
		};
		eMyShaderType shaderType;

		unsigned int ID;
		std::string fileName;
		std::vector<std::string> vecSource;

	};

	class cShaderProgram
	{
	public:
		cShaderProgram();
		~cShaderProgram();

		int getUniformID(std::string name);
		bool LoadUniformLocation(std::string varName);

		unsigned int ID;
		std::string callsign;
		std::map<std::string, int> mapUniformNameToUniformLocation;

	};

	bool createShaderProgram(const std::string callSign, cShader& vertex_shader, cShader& fragment_shader);
	bool useShaderProgram(unsigned int id);
	bool useShaderPRogram(std::string callSign);
	unsigned int getIDfromName(std::string callSign);

	void setShaderUniformM4fv(std::string name,glm::mat4 value);
	void setShaderUniform1f(std::string name, float value);
	void setShaderUniform4f(std::string name,const float value1, const float value2, const float value3, const float value4);


	//cShaderProgram* pGetShaderProgramFromName(std::string callSign);

private:
	bool cLoadShaderSourceCode(cShader& shader);
	bool cCompileShader(cShader& shader, std::string& error);
	bool cIsCompileOK(const unsigned int shader_id, std::string& error);
	bool cIsLinkOK(const unsigned int program_id, std::string& error);

	GLuint shaderID;
	std::string lastError;
	std::map<unsigned int, cShaderProgram> mapIDtoShader;
	std::map<std::string, unsigned int> mapNameToID;
};

