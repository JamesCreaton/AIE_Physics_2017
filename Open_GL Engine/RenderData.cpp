#include "RenderData.h"
#include <gl_core_4_4.h>
#include <assert.h>

RenderData::RenderData()
	: m_VAO(-1)
	, m_VBO(-1)
	, m_IBO(-1)
	, m_numberofIndicies(0)
	, m_hasIndexBuffer(true)
{

}


RenderData::~RenderData()
{
	if (m_IBO != -1)
		glDeleteBuffers(1, &m_IBO);
	if (m_VBO != -1)
		glDeleteBuffers(1, &m_VBO);
	if (m_VAO != -1)
		glDeleteVertexArrays(1, &m_VAO);

}


RenderData::RenderData(RenderData && other)
{
	m_VBO = other.m_VBO;
	m_IBO = other.m_IBO;
	m_VAO = other.m_VAO;
	m_numberofIndicies = other.m_numberofIndicies;
	m_hasIndexBuffer = other.m_hasIndexBuffer;


	other.m_VBO = -1;
	other.m_IBO = -1;
	other.m_VAO = -1;
}

void RenderData::Bind() const
{
	assert(m_VAO != -1 || "ERROR::BUFFER_DOES_NOT_EXIST");
	glBindVertexArray(m_VAO);
}

void RenderData::Unbind() const
{
	glBindVertexArray(0);
}

void RenderData::Render() const
{
	Bind();
	if (m_hasIndexBuffer)
	{

		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLES, m_numberofIndicies,
			GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, m_numberofIndicies);
	}
	Unbind();
}

void RenderData::GenerateBuffers(bool generateIndexBuffer)
{
	// checks
	assert(m_VAO == -1 || "ERROR::BUFFER_ALREADY_GENERATED");
	assert(m_IBO == -1);
	assert(m_VBO == -1);

	// gen buffers
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	if (generateIndexBuffer) glGenBuffers(1, &m_IBO);
	// bind buffers
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	if (generateIndexBuffer)glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBindVertexArray(0);

	m_hasIndexBuffer = generateIndexBuffer;
}
