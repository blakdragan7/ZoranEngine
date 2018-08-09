#include "stdafx.h"
#include "PhysicsEngine.h"
#include <Core/SceneObject.h>
#include <Physics/PhysicsObjectBase.h>
#include <Physics/Collision/CollisionObjectBase.h>
#include <Physics/2D/PhysicsObject2DBase.h>
#include <Physics/3D/PhysicsObject3DBase.h>
#include <Physics/2D/Collision/CollisionObject2DBase.h>
#include <Physics/3D/Collision/CollisionObject3DBase.h>
#include <Physics/2D/Collision/CollisionBucket2DBase.h>
#include <Physics/3D/Collision/CollisionBucket3DBase.h>
#include <Physics/2D/Collision/QuadTreeCollisionBucket.h>

#include <vector>
#include <Utils/VectorAddons.hpp>

PhysicsEngine::PhysicsEngine()
{
	collisionTree2D = 0;
	collisionTree3D = 0;
	is3D = false;

	physicsObjects = new std::vector<PhysicsObjectBase*>();
	collisionFrame2D = new CollisionFrame2D();
}

PhysicsEngine::~PhysicsEngine()
{
	if (collisionTree2D)delete collisionTree2D;
	if (collisionTree3D)delete collisionTree3D;

	delete physicsObjects;
	delete collisionFrame2D;
}

void PhysicsEngine::CheckAllCollision()
{
	if (collisionTree2D)collisionTree2D->CheckAllCollision(*collisionFrame2D);
	if (collisionTree3D)collisionTree3D->CheckAllCollision(*collisionFrame2D);
}

void PhysicsEngine::ResolveAllStaticCollisions(float dt)
{
	float inv_dt = 1.0f / dt;

	for (auto& collisionIter : collisionFrame2D->collisions)
	{
		collisionIter.second->aIndex = aV.AddObject(collisionIter.second->collidedObjects[0]);
		collisionIter.second->bIndex = aV.AddObject(collisionIter.second->collidedObjects[1]);
	}

	for (auto& collisionIter : collisionFrame2D->collisions)
	{
		Collision2D* collision = collisionIter.second;

		collision->PreUpdate(inv_dt,aV);
		collision->frame++;

		for (int i = 0; i < 10; ++i)
		{
			collision->UpdateForces(aV);
		}
	}

	// apply velocities
	for (unsigned i = 0; i < aV.numObjects; i++)
	{
		aV.objects[i]->SetVeloctiy(aV.velocity[i]);
		aV.objects[i]->SetAngularVeloctiy(aV.angularVelocity[i]);
	}

	aV.numObjects = 0;
}

void PhysicsEngine::ResolveAllSweptCollisions(float dt)
{
	if (collisionFrame2D->sweptCollisions.size() == 0)return;

	SweepCollision2D currentResponse = collisionFrame2D->sweptCollisions.back();
	collisionFrame2D->sweptCollisions.pop_back();

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
	collisionFrame2D->RemoveDullCollisions();
	
	for (auto& object : *physicsObjects)
	{
		if(object->GetShouldSimulate())
			object->UpdateVelocities(deltaTime);
	}

	CheckAllCollision();
	ResolveAllCollisions(deltaTime);

	for (auto& object : *physicsObjects)
	{
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

}

void PhysicsEngine::AddPhysicsObject(PhysicsObjectBase * object)
{
	physicsObjects->push_back(object);
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
	remove(*physicsObjects, object);
	return object;
}
