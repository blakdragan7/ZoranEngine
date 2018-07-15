#include "stdafx.h"
#include "PhysicsEngine.h"
#include "Core/SceneObject.h"
#include "PhysicsObjectBase.h"
#include "Utils/VectorAddons.hpp"
#include "Physics/Collision/CollisionObjectBase.h"
#include <Physics/2D/PhysicsObject2DBase.h>
#include <Physics/3D/PhysicsObject3DBase.h>
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
	if (collisionTree2D)collisionTree2D->CheckAllCollision(collisionFrame2D);
	if (collisionTree3D)collisionTree3D->CheckAllCollision(collisionFrame2D);
}

void PhysicsEngine::ResolveAllStaticCollisions()
{
	if (collisionFrame2D.collisions.size() == 0)return;

	CollisionResponse2D biggestResponse;
	biggestResponse.collisionPoints[0].penetrationDepth = 0;

	for (auto object : collisionFrame2D.collisions)
	{
		if (object.collisionPoints[0].penetrationDepth > biggestResponse.collisionPoints[0].penetrationDepth)
			biggestResponse = object;
	}

	remove<CollisionResponse2D>(collisionFrame2D.collisions, biggestResponse);

	if (biggestResponse.collisionPoints[0].penetrationDepth < 0.1)
	{
		collisionFrame2D.collisions.clear();
		return;
	}

	if (biggestResponse.objectBounds[0]->GetDynamics() != CD_Static)
		biggestResponse.collidedObjects[0]->OnCollision(biggestResponse);
	if (biggestResponse.objectBounds[1]->GetDynamics() != CD_Static)
		biggestResponse.collidedObjects[1]->OnCollision(biggestResponse.Reflection());

	ResolveAllStaticCollisions();
}

void PhysicsEngine::ResolveAllSweptCollisions()
{
	if (collisionFrame2D.sweptCollisions.size() == 0)return;

	SweepCollisionResponse2D currentResponse = collisionFrame2D.sweptCollisions.front();

	remove<SweepCollisionResponse2D>(collisionFrame2D.sweptCollisions, currentResponse);

	if (currentResponse.CollisionResponse2D.objectBounds[0]->GetDynamics() != CD_Static)
		currentResponse.CollisionResponse2D.collidedObjects[0]->OnSweepCollision(currentResponse, currentResponse.CollisionResponse2D.collidedObjects[0]->GetCurrentDeltaTime());

	ResolveAllSweptCollisions();
}

void PhysicsEngine::ResolveAllCollisions()
{
	ResolveAllStaticCollisions();
	ResolveAllSweptCollisions();
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

void PhysicsEngine::UpdateAll(float deltaTime)
{
	if (is3D == false)
	{
		collisionFrame2D.Clear();
	}

	for (auto object : physicsObjects)
	{
		object->UpdateVelocities(deltaTime);
		object->UpdatePositionsAndRotation(deltaTime);
	}

	if (collisionTree2D)
	{
		collisionTree2D->UpdateAllObjects();
	}
	if (collisionTree3D)
	{
		collisionTree3D->UpdateAllObjects();
	}

	CheckAllCollision();
	ResolveAllCollisions();
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
	if (remove(physicsObjects, object))return object;
	return NULL;
}
