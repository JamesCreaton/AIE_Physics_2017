#pragma once
#include "AIE Classes\SphereBoundingVolume.h"

class RenderData
{
public:
	RenderData();
	~RenderData();

	RenderData(const RenderData& other) = delete;
	RenderData(RenderData&& other);
	RenderData& operator=(const RenderData& other) = delete;

	void SetNumberOfIndicies(unsigned int indicies) { m_numberofIndicies = indicies; }
	unsigned int GetNumberOfIndicies()const { return m_numberofIndicies; }
	void Bind() const;
	void Unbind() const;
	void Render() const;

	unsigned int& GetVAO() { return m_VAO; }
	unsigned int& GetVBO() { return m_VBO; }
	unsigned int& GetIBO() { return m_IBO; }


	unsigned int GetVAO() const { return m_VAO; }
	unsigned int GetVBO() const { return m_VBO; }
	unsigned int GetIBO() const { return m_IBO; }


	void GenerateBuffers(bool generateIndexBuffer = true);

	const SphereBoundingVolume& GetBoundsAsConst() const { return m_bounds; }
	SphereBoundingVolume& GetBounds() { return m_bounds; }

private:
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
	bool		 m_hasIndexBuffer;
	unsigned int m_numberofIndicies;

	SphereBoundingVolume m_bounds;
};

