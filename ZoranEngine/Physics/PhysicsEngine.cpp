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

#include <Utils/Statistics.h>

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
	DEBUG_BENCH_START_TRACK("PhysicsEngine", "CheckAllCollision2D");
	if (collisionTree2D)collisionTree2D->CheckAllCollision(*collisionFrame2D);
	DEBUG_TRACK_TAKE_BENCH("PhysicsEngine", "CheckAllCollision2D")
	DEBUG_BENCH_START_TRACK("PhysicsEngine", "CheckAllCollision3D");
	if (collisionTree3D)collisionTree3D->CheckAllCollision(*collisionFrame2D);
	DEBUG_TRACK_TAKE_BENCH("PhysicsEngine", "CheckAllCollision3D")
}

void PhysicsEngine::ResolveAllStaticCollisions(float dt)
{
	DEBUG_BENCH_START_TRACK("PhysicsEngine", "ResolveAllStaticCollisions");
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
	DEBUG_TRACK_TAKE_BENCH("PhysicsEngine", "ResolveAllStaticCollisions")
}

void PhysicsEngine::ResolveAllSweptCollisions(float dt)
{
	DEBUG_BENCH_START_TRACK("PhysicsEngine", "ResolveAllSweptCollisions");
	if (collisionFrame2D->sweptCollisions.size() == 0)return;

	SweepCollision2D currentResponse = collisionFrame2D->sweptCollisions.back();
	collisionFrame2D->sweptCollisions.pop_back();

	if (currentResponse.Collision2D.objectBounds[0]->GetDynamics() != CD_Static)
		currentResponse.Collision2D.collidedObjects[0]->OnSweepCollision(currentResponse, currentResponse.Collision2D.collidedObjects[0]->GetCurrentDeltaTime());

	ResolveAllSweptCollisions(dt);
	DEBUG_TRACK_TAKE_BENCH("PhysicsEngine", "ResolveAllSweptCollisions")
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
	DEBUG_BENCH_START_TRACK("PhysicsEngine")
	DEBUG_BENCH_START_TRACK("PhysicsEngine", "RemoveDullCollisions");
	collisionFrame2D->RemoveDullCollisions();
	DEBUG_TRACK_TAKE_BENCH("PhysicsEngine", "RemoveDullCollisions");

	DEBUG_BENCH_START_TRACK("PhysicsEngine", "Simulation - Velocties");
	for (auto& object : *physicsObjects)
	{
		if(object->GetShouldSimulate())
			object->UpdateVelocities(deltaTime);
	}
	DEBUG_TRACK_TAKE_BENCH("PhysicsEngine", "Simulation - Velocties");

	CheckAllCollision();
	ResolveAllCollisions(deltaTime);

	DEBUG_BENCH_START_TRACK("PhysicsEngine", "UpdatePositionsAndRotation");
	for (auto& object : *physicsObjects)
	{
		object->UpdatePositionsAndRotation(deltaTime);
	}
	DEBUG_TRACK_TAKE_BENCH("PhysicsEngine", "UpdatePositionsAndRotation");

	DEBUG_BENCH_START_TRACK("PhysicsEngine", "UpdateAllObjects");
	if (collisionTree2D)
	{
		collisionTree2D->UpdateAllObjects();
	}
	if (collisionTree3D)
	{
		collisionTree3D->UpdateAllObjects();
	}
	DEBUG_TRACK_TAKE_BENCH("PhysicsEngine", "UpdateAllObjects");
	DEBUG_TRACK_TAKE_BENCH("PhysicsEngine")
}

void PhysicsEngine::AddPhysicsObject(PhysicsObjectBase * object)
{
	DEBUG_BENCH_START_TRACK("PhysicsEngine", "AddPhysicsObject");
	physicsObjects->push_back(object);
	DEBUG_TRACK_TAKE_BENCH("PhysicsEngine", "AddPhysicsObject")
}

void PhysicsEngine::AddCollisionObject(CollisionObjectBase * object)
{
	DEBUG_BENCH_START_TRACK("PhysicsEngine", "AddCollisionObject");
	if (is3D)
	{
		if (collisionTree3D)collisionTree3D->AddObject((CollisionObject3DBase*)object);
	}
	else
	{
		if (collisionTree2D)collisionTree2D->AddObject((CollisionObject2DBase*)object);
	}
	DEBUG_TRACK_TAKE_BENCH("PhysicsEngine", "AddCollisionObject")
}

void PhysicsEngine::UpdateCollisionObject(CollisionObjectBase *object)
{
	object->isDirty = true;
}

CollisionObjectBase * PhysicsEngine::RemoveObject(CollisionObjectBase * object)
{
	DEBUG_BENCH_START_TRACK("PhysicsEngine", "RemoveObject");
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
	DEBUG_TRACK_TAKE_BENCH("PhysicsEngine", "RemoveObject")
}

PhysicsObjectBase * PhysicsEngine::RemoveObject(PhysicsObjectBase * object)
{
	DEBUG_BENCH_START_TRACK("PhysicsEngine", "RemoveObject");
	remove(*physicsObjects, object);
	return object;
	DEBUG_TRACK_TAKE_BENCH("PhysicsEngine", "RemoveObject")
}
