#include "stdafx.h"
#include "PhysicsEngine.h"
#include "PhysicsObjectBase.h"
#include "CollisionBucketBase.h"
#include "QuadTreeCollisionBucket.h"
#include "VectorAddons.hpp"

PhysicsEngine::PhysicsEngine()
{
	collisionTree = 0;
}


PhysicsEngine::~PhysicsEngine()
{
	if (collisionTree)delete collisionTree;
}

void PhysicsEngine::CheckAllCollision()
{
	if (collisionTree)collisionTree->CheckAllCollision();
}

void PhysicsEngine::SetupFor2D(Vector3D mapCenter, Vector3D mapSize)
{
	collisionTree = new QuadTreeCollisionBucket(mapCenter,mapSize);
}

void PhysicsEngine::SetupFor3D()
{
	throw std::exception("PhysicsEngine::SetupFor3D() Not Implemented !!");
}

void PhysicsEngine::UpdateAll(double deltaTime)
{
	for (auto object : physicsObjects)
	{
		object->Update(deltaTime);
	}
	CheckAllCollision();
}

void PhysicsEngine::AddPhysicsObject(PhysicsObjectBase * object)
{
	physicsObjects.push_back(object);
}

void PhysicsEngine::AddCollisionObject(CollisionObjectBase * object)
{
	if (collisionTree)collisionTree->AddObject(object);
}

CollisionObjectBase * PhysicsEngine::RemoveObject(CollisionObjectBase * object)
{
	if(collisionTree == NULL)return nullptr;
	return collisionTree->RemoveObject(object);
}

PhysicsObjectBase * PhysicsEngine::RemoveObject(PhysicsObjectBase * object)
{
	return remove(physicsObjects, object);
}
