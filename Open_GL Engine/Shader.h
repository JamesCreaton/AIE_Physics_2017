#pragma once
#include <string>
#include <glm\glm.hpp>
#include <glm\ext.hpp>

#include <gl_core_4_4.h>
class Shader
{
public:

	~Shader();
	Shader(std::string vertexPath, std::string fragmentPath);
	Shader(const Shader& other) = delete;
	Shader(Shader&& other);
	Shader& operator=(const Shader& other) = delete;
	unsigned int GetProgramID() const { return m_programID; }
	void Draw();
	void SetUniform(std::string varName, glm::mat4x4 mat);
	void SetUniform(std::string varName, glm::vec3 vec3);
	void SetUniform(std::string varName, glm::vec4 vec4);
	void SetUniform(std::string varName, float f1);
	void SetUniform(std::string varName, GLuint GLuint1);
	void SetUniform(std::string varName, int i1);
	void SetUniform(std::string varName, float f1, float f2);
	bool PolygonMode = false;


private:
	void MakeShaderProgram(std::string vertPath, std::string fragPath);
	unsigned int MakeShader(unsigned int type, std::string path);
	unsigned int m_programID;
};

