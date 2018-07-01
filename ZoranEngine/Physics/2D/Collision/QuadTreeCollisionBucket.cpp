#include "stdafx.h"

#include <Physics/2D/Collision/QuadTreeCollisionBucket.h>
#include <Physics/2D/Collision/CollisionObject2DBase.h>
#include <Physics/3D/Collision/SphereCollisionObject.h>
#include <Physics/2D/Collision/SquareColisionObject.h>
#include <Physics/3D/Collision/BoxCollisionObject.h>
#include <Physics/2D/Collision/QuadTreeCollision.h>
#include <Physics/2D/PhysicsObject2DBase.h>
#include <Core/2D/SceneObject2D.h>
#include <Utils/VectorAddons.hpp>
#include <Core/SceneObject.h>
#include <Core/ZoranEngine.h>
#include <Core/ThreadBase.h>
#include <assert.h>
#include <string>

static unsigned long long numBuckets = 0;

QuadTreeCollisionBucket::QuadTreeCollisionBucket(std::string name, Vec2D pos, Vec2D size, unsigned maxObjects,QuadTreeCollisionBucket * parent) : CollisionBucket2DBase(name,pos,maxObjects)
{
	children[0] = 0;
	children[1] = 0;
	children[2] = 0;
	children[3] = 0;

	hasSubdivided = false;
	this->parent = parent;
	numBuckets++;

	Vec2D halfSize = size / 2.0;

	collision = new SquareColisionObject(pos - halfSize,pos + halfSize, sceneObject,CD_Static);

	Log(LogLevel_Verbose,"Buckets: %i", numBuckets);
}


QuadTreeCollisionBucket::~QuadTreeCollisionBucket()
{
	if (hasSubdivided)
	{
		delete children[0];
		delete children[1];
		delete children[2];
		delete children[3];
	}

	numBuckets--;
}

bool QuadTreeCollisionBucket::AddObject(CollisionObject2DBase * object)
{
	assert(FindObject(object) == -1);

	bool wasAdded = false;

	while (mutex.try_lock() == false)ThreadBase::Sleep(0.001F);
	if ((collisionObjects.size() < maxObjects && !hasSubdivided))
	{
		if (ObjectIsWithinBucket(object))
		{
			collisionObjects.push_back(object);
			wasAdded = true;

			Log(LogLevel_Debug,"Object %s Add To Bucket %s\n", object->GetSceneObject()->readableName.c_str(), sceneObject->readableName.c_str());
		}
	}
	else 
	{
		if (hasSubdivided == false)Subdivide();
		
		for (unsigned i = 0; i < 4; i++)
		{
			if (children[i]->AddObject(object))
			{
				wasAdded = true;
				break;
			}
		}
		if (wasAdded == false)
		{
			bool objectIsIn = ObjectIsWithinBucket(object);
			if (objectIsIn || parent == 0)
			{
				collisionObjects.push_back(object);
				wasAdded = true;
			}
		}
	}

	mutex.unlock();
	return wasAdded;
}

CollisionObject2DBase * QuadTreeCollisionBucket::RemoveObject(CollisionObject2DBase * object)
{
	while (mutex.try_lock() == false)ThreadBase::Sleep(0.001F);
	auto iter = std::find(collisionObjects.begin(), collisionObjects.end(), object);
	if (iter != collisionObjects.end())
	{
		size_t index = iter - collisionObjects.begin();
		remove(collisionObjects,index);
		mutex.unlock();
		return object;
	}
	if(hasSubdivided)
	{
		for (unsigned i = 0; i < 4; i++)
		{
			if (children[i]->RemoveObject(object))
			{
				mutex.unlock();
				return object;
			}
		}
	}
	mutex.unlock();
	return nullptr;
}

bool QuadTreeCollisionBucket::UpdateObject(CollisionObject2DBase * object)
{
	// Static collision objects don't need to be updated
	if (object->GetDynamics() == CD_Static)return false;
	object->SetBoundsBySceneObject();
	int index = FindObject(object);
	if (index != -1)
	{
		bool wasAdded = false;
		if (hasSubdivided)
		{
			for (unsigned i = 0; i < 4; i++)
			{
				if (children[i]->AddObject(object))
				{
					remove(collisionObjects, index);
					wasAdded = true;
					return true;
				}
			}
		}

		if (wasAdded == false)
		{
			if (ObjectIsWithinBucket(object) == false)
			{
				if (parent)
				{
					Log(LogLevel_Debug, "Object %s No Longer in Bucket %s\n", object->GetSceneObject()->readableName.c_str(), sceneObject->readableName.c_str());

					remove(collisionObjects,index);
					return parent->TraverseUpAddObject(object);
				}
			}
		}
	}
	else if(hasSubdivided)
	{
		for (unsigned i = 0; i < 4; i++)
		{
			if (children[i]->UpdateObject(object))
			{
				return true;
			}
		}
	}

	return false;
}

void QuadTreeCollisionBucket::PrintAllContents(unsigned depth)
{
	for (int i = 0; i < depth; i++)Log(LogLevel_None,"\t");
	Log(LogLevel_None, "%s Contains: \n", sceneObject->readableName.c_str());
	for (auto object : collisionObjects)
	{
		for (int i = 0; i < depth; i++)Log(LogLevel_None, "\t");
		Log(LogLevel_None, "\t%s\n", object->GetSceneObject()->readableName.c_str());
	}
	if (hasSubdivided)
	{
		int new_depth = depth + 1;
		children[0]->PrintAllContents(new_depth);
		children[1]->PrintAllContents(new_depth);
		children[2]->PrintAllContents(new_depth);
		children[3]->PrintAllContents(new_depth);
	}
}

void QuadTreeCollisionBucket::UpdateAllObjects()
{
	for (auto object : collisionObjects)
	{
		if (object->isDirty)
		{
			UpdateObject(object);
		}
	}

	if (hasSubdivided)
	{
		children[0]->UpdateAllObjects();
		children[1]->UpdateAllObjects();
		children[2]->UpdateAllObjects();
		children[3]->UpdateAllObjects();
	}
}

void QuadTreeCollisionBucket::CheckAllCollision()
{
 	if (hasSubdivided)
	{
		children[0]->CheckAllCollision();
		children[1]->CheckAllCollision();
		children[2]->CheckAllCollision();
		children[3]->CheckAllCollision();
	}


	for (unsigned i = 0;i<collisionObjects.size();i++)
	{
		CollisionObject2DBase* object = collisionObjects[i];
		
		if (object->isDirty == false)continue;
		object->isDirty = false;

		if (i < collisionObjects.size() - 1)
		{
			for (unsigned j = i+1; j < collisionObjects.size(); j++)
			{
				CollisionResponse2D response;

				if (object->CollidesWith(collisionObjects[j],response))
				{
					object->GetPhysicsObject()->OnCollision(response);
					collisionObjects[j]->GetPhysicsObject()->OnCollision(response.Reflection());
				}
			}
		}

		if (hasSubdivided)
		{
			CollisionResponse2D response;

			if (children[0]->CheckCollisionForObject(object, response))
			{
				PhysicsObject2DBase* pObject = response.collidedObjects[0];
				PhysicsObject2DBase* pObjec2t = response.collidedObjects[1];

				pObject->OnCollision(response);
				pObjec2t->OnCollision(response.Reflection());
			}

			if (children[1]->CheckCollisionForObject(object, response))
			{
				PhysicsObject2DBase* pObject = response.collidedObjects[0];
				PhysicsObject2DBase* pObjec2t = response.collidedObjects[1];

				pObject->OnCollision(response);
				pObjec2t->OnCollision(response.Reflection());
			}

			if (children[2]->CheckCollisionForObject(object, response))
			{
				PhysicsObject2DBase* pObject = response.collidedObjects[0];
				PhysicsObject2DBase* pObjec2t = response.collidedObjects[1];

				pObject->OnCollision(response);
				pObjec2t->OnCollision(response.Reflection());
			}

			if (children[3]->CheckCollisionForObject(object, response))
			{
				PhysicsObject2DBase* pObject = response.collidedObjects[0];
				PhysicsObject2DBase* pObjec2t = response.collidedObjects[1];

				pObject->OnCollision(response);
				pObjec2t->OnCollision(response.Reflection());
			}
		}
	}
}

bool QuadTreeCollisionBucket::ObjectIsWithinBucket(CollisionObject2DBase * object)
{
	// always add to the root tree if it hasnt subdivided yet
	return collision->CollidesWith(object, CollisionResponse2D()) || (hasSubdivided == false && parent == 0);
}

void QuadTreeCollisionBucket::Subdivide()
{
	if (hasSubdivided)
	{
		std::cerr << "QuadTreeCollisionBucket::Subdivide() Already Subdivided !\n";
		return;
	}

	Vec2D subSize = collision->GetSize() / 2.0;
	Vec2D size = collision->GetSize();

	Vec2D pos = collision->GetScenePos();

	Vec2D TL = Vec2D(pos.x - (size.x / 4.0), pos.y + (size.y / 4.0));
	Vec2D BL = Vec2D(pos.x - (size.x / 4.0), pos.y - (size.y / 4.0));
	Vec2D TR = Vec2D(pos.x + (size.x / 4.0), pos.y + (size.y / 4.0));
	Vec2D BR = Vec2D(pos.x + (size.x / 4.0), pos.y - (size.y / 4.0));

	std::string sname = sceneObject->readableName;

	children[TL_BUCKET] = new QuadTreeCollisionBucket((sname + ".topleft").c_str(),TL, subSize, maxObjects, this);
	children[TR_BUCKET] = new QuadTreeCollisionBucket((sname + ".topright").c_str(),TR, subSize, maxObjects, this);
	children[BL_BUCKET] = new QuadTreeCollisionBucket((sname + ".bottomleft").c_str(),BL, subSize, maxObjects, this);
	children[BR_BUCKET] = new QuadTreeCollisionBucket((sname + ".bottomright").c_str(),BR, subSize, maxObjects, this);

	std::vector<CollisionObject2DBase*> keptObjects;

	for (auto object : collisionObjects)
	{
		bool wasAdded = false;
		for (unsigned i = 0; i < 4; i++)
		{
			if (children[i]->AddObject(object))
			{
				wasAdded = true;
				break;
			}
		}
		if (wasAdded == false)
		{
			keptObjects.push_back(object);
		}
	}

	collisionObjects.clear();

	collisionObjects = keptObjects;

	hasSubdivided = true;
}

bool QuadTreeCollisionBucket::CheckCollisionForObject(CollisionObject2DBase * object, CollisionResponse2D& response)
{
	for (unsigned i = 0; i < collisionObjects.size(); i++)
	{
		if (object->CollidesWith(collisionObjects[i],response))
		{
			return true;
		}
	}

	if (hasSubdivided)
	{
		if (children[0]->CheckCollisionForObject(object, response))return true;
		if (children[1]->CheckCollisionForObject(object, response))return true;
		if (children[2]->CheckCollisionForObject(object, response))return true;
		if (children[3]->CheckCollisionForObject(object, response))return true;
	}

	return false;
}

bool QuadTreeCollisionBucket::SweepCollision(CollisionObject2DBase * object, Vec2D newPosition, SweepCollisionResponse2D & response)
{
	int index = FindObject(object);
	if (index == -1)
	{
		if (children)
		{
			if (children[0]->SweepCollision(object, newPosition, response))return true;
			if (children[1]->SweepCollision(object, newPosition, response))return true;
			if (children[2]->SweepCollision(object, newPosition, response))return true;
			if (children[3]->SweepCollision(object, newPosition, response))return true;
		}
	}
	else
	{
		if (SweepCollisionHitTest(object, newPosition, response))return true;

		if (hasSubdivided)
		{
			if (children[0]->SweepCollisionHitTest(object, newPosition, response))return true;
			if (children[1]->SweepCollisionHitTest(object, newPosition, response))return true;
			if (children[2]->SweepCollisionHitTest(object, newPosition, response))return true;
			if (children[3]->SweepCollisionHitTest(object, newPosition, response))return true;
		}
		return true;
	}
	return false;
}

bool QuadTreeCollisionBucket::SweepCollisionHitTest(CollisionObject2DBase * object, Vec2D newPosition, SweepCollisionResponse2D & response)
{
	Vec2D origin = object->GetScenePos();
	Vec2D deltaPos = newPosition - origin;
	Vec2D size = object->GetSize();

	Vec2D InvEntry;
	Vec2D InvExit;

	for (unsigned i = 0; i < collisionObjects.size(); i++)
	{
		CollisionObject2DBase* other = collisionObjects[i];
		if (other == object)continue;

		Vec2D otherPos = other->GetScenePos();
		Vec2D otherSize = other->GetSize();

		// determine closest and farthest axis

		if (deltaPos.x > 0)
		{
			InvEntry.x = otherPos.x - (origin.x + size.w);
			InvExit.x = (otherPos.x + otherSize.w) - origin.x;
		}
		else
		{
			InvEntry.x = (otherPos.x + otherSize.w) - origin.x;
			InvExit.x = otherPos.x - (origin.x + size.w);
		}
		if (deltaPos.y > 0.0f)
		{
			InvEntry.y = otherPos.y - (origin.y + size.h);
			InvExit.y = (otherPos.y + otherSize.h) - origin.y;
		}
		else
		{
			InvEntry.y = (otherPos.y + otherSize.h) - origin.y;
			InvExit.y = otherPos.y - (origin.y + size.h);
		}

		Vec2D entry;
		Vec2D exit;

		if (deltaPos.x == 0)
		{
			entry.x = -std::numeric_limits<double>::infinity();
			exit.x = -std::numeric_limits<double>::infinity();
		}
		else
		{
			entry.x = InvEntry.x / deltaPos.x;
			exit.x = InvExit.x / deltaPos.x;
		}
		if (deltaPos.y == 0)
		{
			entry.y = -std::numeric_limits<double>::infinity();
			exit.y = -std::numeric_limits<double>::infinity();
		}
		else
		{
			entry.y = InvEntry.y / deltaPos.y;
			exit.y = InvExit.y / deltaPos.y;
		}

		double entryTime = max(entry.x,entry.y);
		double exitTime = max(entry.x,entry.y);

		if (entryTime > exitTime || (entry.x < 0.0 && entry.y < 0) || (entry.x > 1.0  && entry.y > 1.0))
		{
			continue;
		}

		Vec2D normal;

		if (entry.x > entry.y)
		{
			if (entry.x < 0) normal = Vec2D(-1, 0);
			else normal = Vec2D(1, 0);
		}
		{
			if (entry.y < 0) normal = Vec2D(0, -1);
			else normal = Vec2D(0, 1);
		}
		

		response.timeHit = entryTime;

		response.CollisionResponse2D.collided = true;
		response.CollisionResponse2D.collidedObjects[0] = object->GetPhysicsObject();
		response.CollisionResponse2D.collidedObjects[1] = collisionObjects[i]->GetPhysicsObject();
		response.CollisionResponse2D.objectBounds[0] = object;
		response.CollisionResponse2D.objectBounds[1] = collisionObjects[i];
		response.CollisionResponse2D.normal = normal;

		return true;
	}

	return false;
}

bool QuadTreeCollisionBucket::CheckObjectAgainstStaic(CollisionObject2DBase * object, CollisionResponse2D & response)
{
	for (unsigned i = 0; i < collisionObjects.size(); i++)
	{
		if (collisionObjects[i]->GetDynamics() != CD_Static)continue;
		CollisionResponse2D response;

		if (object->CollidesWith(collisionObjects[i], response))
		{
			return true;
		}
	}

	if (hasSubdivided)
	{
		if (children[0]->CheckCollisionForObject(object, response))return true;
		if (children[1]->CheckCollisionForObject(object, response))return true;
		if (children[2]->CheckCollisionForObject(object, response))return true;
		if (children[3]->CheckCollisionForObject(object, response))return true;
	}

	return false;
}

// Debug Prints

bool QuadTreeCollisionBucket::PrintCollisionForObject(CollisionObject2DBase * object)
{
	for (unsigned i = 0; i < collisionObjects.size(); i++)
	{
		Log(LogLevel_None, "\tCheck Collision %s => %s\n", object->GetSceneObject()->readableName.c_str(), collisionObjects[i]->GetSceneObject()->readableName.c_str());
		if (object->CollidesWith(collisionObjects[i], CollisionResponse2D()))
		{
			Log(LogLevel_None, "Found Collision\n");
			return true;
		}
	}

	if (hasSubdivided)
	{
		if (children[0]->PrintCollisionForObject(object))return true;
		if (children[1]->PrintCollisionForObject(object))return true;
		if (children[2]->PrintCollisionForObject(object))return true;
		if (children[3]->PrintCollisionForObject(object))return true;
	}

	return false;
}

bool QuadTreeCollisionBucket::TraverseUpAddObject(CollisionObject2DBase * object)
{
	if (AddObject(object) == false)return parent->TraverseUpAddObject(object);
	return true;
}

void QuadTreeCollisionBucket::PrintAllCollisions()
{
	if (hasSubdivided)
	{
		children[0]->PrintAllCollisions();
		children[1]->PrintAllCollisions();
		children[2]->PrintAllCollisions();
		children[3]->PrintAllCollisions();
	}

	for (unsigned i = 0; i<collisionObjects.size(); i++)
	{
		CollisionObject2DBase* object = collisionObjects[i];

		if (i < collisionObjects.size() - 1)
		{
			for (unsigned j = i + 1; j < collisionObjects.size(); j++)
			{
				Log(LogLevel_None, "\tCheck Collision %s => %s\n", object->GetSceneObject()->readableName.c_str(), collisionObjects[i]->GetSceneObject()->readableName.c_str());

				if (object->CollidesWith(collisionObjects[j], CollisionResponse2D()))
				{
					Log(LogLevel_None, "Found Collision\n");
				}
			}
		}

		if (hasSubdivided)
		{

			if (children[0]->PrintCollisionForObject(object))
			{
			}

			if (children[1]->PrintCollisionForObject(object))
			{
			}

			if (children[2]->PrintCollisionForObject(object))
			{
			}

			if (children[3]->PrintCollisionForObject(object))
			{
			}
		}
	}
}
