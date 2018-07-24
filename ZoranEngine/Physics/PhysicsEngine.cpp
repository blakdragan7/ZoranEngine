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

void PhysicsEngine::ResolveAllStaticCollisions(float dt)
{
	float inv_dt = 1.0f / dt;

	for (auto& collisionIter : collisionFrame2D.collisions)
	{
		std::cout << "Static Collision\n";
		Collision2D* collision = collisionIter.second;
		collision->PreUpdate(inv_dt);
		collision->frame++;

		for (int i = 0; i < 100; ++i)
		{
			collision->UpdateForces();
		}

		if (collision->objectBounds[0]->GetDynamics() != CD_Static)
			collision->collidedObjects[0]->OnCollision(*collision);
		if (collision->objectBounds[1]->GetDynamics() != CD_Static)
		{
			auto reflection = collision->Reflection();
			collision->collidedObjects[1]->OnCollision(*reflection);
			delete reflection;
		}
	}
}

void PhysicsEngine::ResolveAllSweptCollisions(float dt)
{
	if (collisionFrame2D.sweptCollisions.size() == 0)return;

	SweepCollision2D currentResponse = collisionFrame2D.sweptCollisions.front();

	remove<SweepCollision2D>(collisionFrame2D.sweptCollisions, currentResponse);

	if (currentResponse.Collision2D.objectBounds[0]->GetDynamics() != CD_Static)
		currentResponse.Collision2D.collidedObjects[0]->OnSweepCollision(currentResponse, currentResponse.Collision2D.collidedObjects[0]->GetCurrentDeltaTime());

	ResolveAllSweptCollisions(dt);
}

void PhysicsEngine::ResolveAllCollisions(float dt)
{
	ResolveAllStaticCollisions(dt);
	ResolveAllSweptCollisions(dt);
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
	collisionFrame2D.RemoveDullCollisions();
	
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
	ResolveAllCollisions(deltaTime);

	/*for (auto object : physicsObjects)
	{
	}*/

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
