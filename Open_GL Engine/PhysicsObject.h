#pragma once
#include <glm\glm.hpp>


using glm::vec2;
using glm::vec3;


enum ShapeType
{
	PLANE	= 0,
	SPHERE	= 1,
	BOX		= 2,
};

class PhysicsObject
{

protected:
	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {}
	PhysicsObject();
	~PhysicsObject();

public:
	virtual bool fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void debug() = 0;
	virtual void makeGizmo() = 0;
	virtual void resetPosition() {};

protected:
	ShapeType		m_shapeID;
};

