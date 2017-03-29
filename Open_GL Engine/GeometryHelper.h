#pragma once
#include <glm\glm.hpp>
#include <vector>

class RenderData;
namespace GeometryHelper
{
	struct SimpleVertex {
		glm::vec4 position;
		glm::vec4 colour;
	};

	struct ObjVertex
	{
		glm::vec4 position;
		glm::vec3 normal;
		glm::vec2 uv;
		glm::vec4 tangent;
		glm::vec4 biTangent;

	};

	struct GLInfo {
		unsigned int vao;
		unsigned int vbo1;
		unsigned int vbo2;
		unsigned int faceCount;
	};

	RenderData* CreateGrid(unsigned int rows, unsigned int cols, float width, float height, glm::vec4 colour);
	RenderData* CreateQuad(float width, float height, glm::vec3 facingDir);
	std::vector<RenderData*> LoadOBJFromDisk(std::string path);

	void calculateTangents(std::vector<ObjVertex>& vertices, const std::vector<unsigned int>& indices);

	void LoadHandTest(std::vector<GLInfo>& a_meshes);
	void DrawHand(std::vector<GLInfo>& a_meshes);
};

