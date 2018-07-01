#include "stdafx.h"
#include "PhysicsEngine.h"
#include "Core/SceneObject.h"
#include "PhysicsObjectBase.h"
#include "Utils/VectorAddons.hpp"
#include "Physics/Collision/CollisionObjectBase.h"
#include "Physics/2D/Collision/CollisionObject2DBase.h"
#include "Physics/3D/Collision/CollisionObject3DBase.h"
#include "Physics/2D/Collision/CollisionBucket2DBase.h"
#include "Physics/3D/Collision/CollisionBucket3DBase.h"
#include "Physics/2D/Collision/QuadTreeCollisionBucket.h"

PhysicsEngine::PhysicsEngine()
{
	collisionTree2D = 0;
	collisionTree3D = 0;
	is3D = false;
}

PhysicsEngine::~PhysicsEngine()
{
	if (collisionTree2D)delete collisionTree2D;
	if (collisionTree3D)delete collisionTree3D;
}

void PhysicsEngine::CheckAllCollision()
{
	if (collisionTree2D)collisionTree2D->CheckAllCollision();
	if (collisionTree3D)collisionTree3D->CheckAllCollision();
}

void PhysicsEngine::SetupFor2D(Vector2D mapCenter, Vector2D mapSize)
{
	collisionTree2D = new QuadTreeCollisionBucket("root_tree",mapCenter,mapSize);
}

void PhysicsEngine::SetupFor3D()
{
	throw std::exception("PhysicsEngine::SetupFor3D() Not Implemented !!");
	is3D = true;
}

CollisionBucket2DBase * PhysicsEngine::GetCollisionBucketRootFor2D()
{
	return collisionTree2D;
}

CollisionBucket3DBase * PhysicsEngine::GetCollisionBucketRootFor3D()
{
	return collisionTree3D;
}

CollisionBucketBase * PhysicsEngine::GetCollisionBucketRoot()
{
	if (is3D)return collisionTree3D;
	else return collisionTree2D;
}

void PhysicsEngine::UpdateAll(double deltaTime)
{
	for (auto object : physicsObjects)
	{
		object->Update(deltaTime);
	}
	// no longer looping for collision, instead checking collision on position change
#ifndef CON_COLLISION
	if (collisionTree2D)collisionTree2D->UpdateAllObjects();
	if (collisionTree3D)collisionTree3D->UpdateAllObjects();
	CheckAllCollision();
#endif
}

void PhysicsEngine::AddPhysicsObject(PhysicsObjectBase * object)
{
	physicsObjects.push_back(object);
}

void PhysicsEngine::AddCollisionObject(CollisionObjectBase * object)
{
	if (is3D)
	{
		if (collisionTree3D)collisionTree3D->AddObject((CollisionObject3DBase*)object);
	}
	else
	{
		if (collisionTree2D)collisionTree2D->AddObject((CollisionObject2DBase*)object);
	}
}

void PhysicsEngine::UpdateCollisionObject(CollisionObjectBase *object)
{
	object->isDirty = true;
}

CollisionObjectBase * PhysicsEngine::RemoveObject(CollisionObjectBase * object)
{
	if (is3D)
	{
		if (collisionTree3D == NULL)return nullptr;
		return collisionTree3D->RemoveObject((CollisionObject3DBase*)object);
	}
	else
	{
		if (collisionTree2D == NULL)return nullptr;
		return collisionTree2D->RemoveObject((CollisionObject2DBase*)object);
	}
}

PhysicsObjectBase * PhysicsEngine::RemoveObject(PhysicsObjectBase * object)
{
	return remove(physicsObjects, object);
}
