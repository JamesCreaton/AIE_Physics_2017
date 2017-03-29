#include "Shader.h"
#include <gl_core_4_4.h>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(std::string vertexPath, std::string fragmentPath)
	: m_programID(-1)
{
	MakeShaderProgram(vertexPath, fragmentPath);
}

Shader::Shader(Shader && other)
{
	m_programID = other.m_programID;
	other.m_programID = -1;
}

void Shader::Draw()
{
	if (PolygonMode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glUseProgram(m_programID);
}

void Shader::SetUniform(std::string varName, glm::mat4x4 mat)
{
	int loc = glGetUniformLocation(m_programID, varName.c_str());
	//assert(loc != -1);
	glUniformMatrix4fv(loc, 1, false, glm::value_ptr(mat));
}

void Shader::SetUniform(std::string varName, glm::vec3 vec3)
{
	int loc = glGetUniformLocation(m_programID, varName.c_str());
	//assert(loc != -1);
	glUniform3f(loc, vec3.x, vec3.y, vec3.z);
}

void Shader::SetUniform(std::string varName, glm::vec4 vec4)
{
	int loc = glGetUniformLocation(m_programID, varName.c_str());
	//assert(loc != -1);
	glUniform4f(loc, vec4.x, vec4.y, vec4.z, vec4.w);
}

void Shader::SetUniform(std::string varName, float f1)
{
	int loc = glGetUniformLocation(m_programID, varName.c_str());
//	assert(loc != -1);
	glUniform1f(glGetUniformLocation(m_programID, varName.c_str()), f1);
}

void Shader::SetUniform(std::string varName, GLuint GLuint1)
{
	int loc = glGetUniformLocation(m_programID, varName.c_str());
	//assert(loc != -1);
	glUniform1f(glGetUniformLocation(m_programID, varName.c_str()), GLuint1);
}

void Shader::SetUniform(std::string varName, int i1)
{
	int loc = glGetUniformLocation(m_programID, varName.c_str());
//	assert(loc != -1);
	glUniform1i(glGetUniformLocation(m_programID, varName.c_str()), i1);
}

void Shader::SetUniform(std::string varName, float f1, float f2)
{
	int loc = glGetUniformLocation(m_programID, varName.c_str());
//	assert(loc != -1);
	glUniform2f(glGetUniformLocation(m_programID, varName.c_str()), f1, f2);
}

void Shader::MakeShaderProgram(std::string vertPath, std::string fragPath)
{
	// load shaders from disk
	unsigned int vertexShader = MakeShader(GL_VERTEX_SHADER, vertPath);
	unsigned int fragmentShader = MakeShader(GL_FRAGMENT_SHADER, fragPath);
	
	assert(vertexShader != -1 && fragmentShader != -1 && "ERROR::FILELOAD::CANNOT_LOAD_FILE");

	// create shader program, attach shaders
	// link shaders
	m_programID = glCreateProgram();
	glAttachShader(m_programID, vertexShader);
	glAttachShader(m_programID, fragmentShader);

	glLinkProgram(m_programID);
	// --error check
	int success = 0;
	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if(success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(m_programID, infoLogLength, 0, infoLog);
		std::cout << "ERROR::LINKING_SHADERS(" << vertPath << " | "
			<< fragPath << "):" << std::endl;
		std::cout << infoLog << std::endl;
		delete[] infoLog;
		assert(false && "ERROR::SHADER_FAILED_TO_COMPILE");
	}
	// --end error check

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
unsigned int Shader::MakeShader(unsigned int type, std::string path)
{
	// create a file stream
	std::ifstream file;
	file.open(path.c_str(), std::ifstream::in);
	// is the file stream good?
	if (!file.good())
		return -1;
	// store and close filestream
	std::stringstream ss;
	ss << file.rdbuf();
	file.close();

	std::string codeString = ss.str();
	// store shaderhandle
	unsigned int shaderHandle = glCreateShader(type);
	const char* shaderCode = codeString.c_str();
	// load and compile shader
	glShaderSource(shaderHandle, 1, (const char**)&shaderCode, 0);
	glCompileShader(shaderHandle);


	// return compiled shader handle
	return shaderHandle;
}
Shader::~Shader()
{
	if (m_programID - 1)
	{
		glDeleteProgram(m_programID);
	}
}

