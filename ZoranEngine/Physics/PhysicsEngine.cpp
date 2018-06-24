#include "stdafx.h"
#include "PhysicsEngine.h"
#include "PhysicsObjectBase.h"
#include "Physics/Collision/CollisionBucketBase.h"
#include "Physics/Collision/3D/QuadTreeCollisionBucket.h"
#include "Utils/VectorAddons.hpp"
#include "Core/SceneObject.h"
#include "Physics/Collision/CollisionObjectBase.h"

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
	collisionTree = new QuadTreeCollisionBucket("root_tree",mapCenter,mapSize);
}

void PhysicsEngine::SetupFor3D()
{
	throw std::exception("PhysicsEngine::SetupFor3D() Not Implemented !!");
}

CollisionBucketBase * PhysicsEngine::GetCollisionBucketRoot()
{
	return collisionTree;
}

void PhysicsEngine::UpdateAll(double deltaTime)
{
	for (auto object : physicsObjects)
	{
		object->Update(deltaTime);
	}
	// no longer looping for collision, instead checking collision on position change
#ifndef CON_COLLISION
	if (collisionTree)collisionTree->UpdateAllObjects();
	CheckAllCollision();
#endif
}

void PhysicsEngine::AddPhysicsObject(PhysicsObjectBase * object)
{
	physicsObjects.push_back(object);
}

void PhysicsEngine::AddCollisionObject(CollisionObjectBase * object)
{
	if (collisionTree)collisionTree->AddObject(object);
}

void PhysicsEngine::UpdateCollisionObject(CollisionObjectBase *object)
{
#ifdef CON_COLLISION
	if (collisionTree)collisionTree->UpdateObject(object);
#else
	object->isDirty = true;
#endif
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
