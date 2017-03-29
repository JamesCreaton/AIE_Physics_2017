#include "PhysicsScene.h"
#include <algorithm>


PhysicsScene::PhysicsScene()
{
}


PhysicsScene::~PhysicsScene()
{
}

void PhysicsScene::addActor(PhysicsObject * actor)
{
	m_actors.push_back(actor);
}

void PhysicsScene::removeActor(PhysicsObject * actor)
{
	auto position = std::find(m_actors.begin(), m_actors.end(), actor);
	m_actors.erase(position);
}

void PhysicsScene::update(float dt)
{
	auto updateIterator = m_actors.begin();
	for (; updateIterator != m_actors.end(); updateIterator++)
	{
		(*updateIterator)->fixedUpdate(m_gravity, m_timeStep);
	}
}

void PhysicsScene::updateGizmos()
{
	auto gizmoIterator = m_actors.begin();
	for (; gizmoIterator != m_actors.end(); gizmoIterator++)
	{
		(*gizmoIterator)->makeGizmo();
	}
}
