#include "stdafx.h"
#include "QuadTreeCollisionBucket.h"
#include "QuadTreeCollision.h"
#include "BoxCollisionObject.h"
#include "SceneObject.h"
#include "ThreadBase.h"
#include "VectorAddons.hpp"
#include "PhysicsObjectBase.h"
#include "BoxCollisionObject.h"
#include "SphereCollisionObject.h"
#include "DragEngine.h"
#include <assert.h>
#include <string>

static unsigned long long numBuckets = 0;

QuadTreeCollisionBucket::QuadTreeCollisionBucket(std::string name, Vec3D pos, Vec3D size, unsigned maxObjects,QuadTreeCollisionBucket * parent) : CollisionBucketBase(name,pos,size,maxObjects)
{
	children[0] = 0;
	children[1] = 0;
	children[2] = 0;
	children[3] = 0;

	hasSubdivided = false;

	this->parent = parent;

	numBuckets++;

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

bool QuadTreeCollisionBucket::AddObject(CollisionObjectBase * object)
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

CollisionObjectBase * QuadTreeCollisionBucket::RemoveObject(CollisionObjectBase * object)
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

bool QuadTreeCollisionBucket::UpdateObject(CollisionObjectBase * object)
{
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
		CollisionObjectBase* object = collisionObjects[i];
		
		if (object->isDirty == false)continue;
		object->isDirty = false;

		if (i < collisionObjects.size() - 1)
		{
			for (unsigned j = i+1; j < collisionObjects.size(); j++)
			{
				if (object->CollidesWith(collisionObjects[j]))
				{
					if (collisionObjects[j]->GetCollisionType() == BOX_COLLISION)
					{
						CollisionResponse response;
						Vec3D normal = collisionObjects[j]->GetNormalBetween(object);
						response.collided = true;
						response.normal = normal;
						response.objectBounds[0] = object;
						response.objectBounds[1] = collisionObjects[j];
						response.collidedObjects[0] = object->GetPhysicsObject();
						response.collidedObjects[1] = collisionObjects[j]->GetPhysicsObject();
						response.intersection = object->GetScenePos() - collisionObjects[j]->GetScenePos();

						object->GetPhysicsObject()->OnCollision(response);
						collisionObjects[j]->GetPhysicsObject()->OnCollision(response.Reflection());
					}
					else
					{
						CollisionResponse response;
						Vec3D normal = object->GetNormalBetween(collisionObjects[j]);
						response.collided = true;
						response.normal = normal;
						response.objectBounds[0] = object;
						response.objectBounds[1] = collisionObjects[j];
						response.collidedObjects[0] = object->GetPhysicsObject();
						response.collidedObjects[1] = collisionObjects[j]->GetPhysicsObject();
						response.intersection = collisionObjects[j]->GetScenePos() - object->GetScenePos();

						collisionObjects[j]->GetPhysicsObject()->OnCollision(response);
						object->GetPhysicsObject()->OnCollision(response.Reflection());
					}
				}
			}
		}

		if (hasSubdivided)
		{
			CollisionResponse response;

			if (children[0]->CheckCollisionForObject(object, response))
			{
				PhysicsObjectBase* pObject = response.collidedObjects[0];
				PhysicsObjectBase* pObjec2t = response.collidedObjects[1];

				pObject->OnCollision(response);
				pObjec2t->OnCollision(response.Reflection());
			}

			if (children[1]->CheckCollisionForObject(object, response))
			{
				PhysicsObjectBase* pObject = response.collidedObjects[0];
				PhysicsObjectBase* pObjec2t = response.collidedObjects[1];

				pObject->OnCollision(response);
				pObjec2t->OnCollision(response.Reflection());
			}

			if (children[2]->CheckCollisionForObject(object, response))
			{
				PhysicsObjectBase* pObject = response.collidedObjects[0];
				PhysicsObjectBase* pObjec2t = response.collidedObjects[1];

				pObject->OnCollision(response);
				pObjec2t->OnCollision(response.Reflection());
			}

			if (children[3]->CheckCollisionForObject(object, response))
			{
				PhysicsObjectBase* pObject = response.collidedObjects[0];
				PhysicsObjectBase* pObjec2t = response.collidedObjects[1];

				pObject->OnCollision(response);
				pObjec2t->OnCollision(response.Reflection());
			}
		}
	}
}

bool QuadTreeCollisionBucket::ObjectIsWithinBucket(CollisionObjectBase * object)
{
	// always add to the root tree if it hasnt subdivided yet
	return collision->CollidesWith(object) || (hasSubdivided == false && parent == 0);
}

bool QuadTreeCollisionBucket::CheckAllCollisionForObject(CollisionObjectBase * object, CollisionResponse& response)
{
	int index = FindObject(object);

	if (index == -1 && hasSubdivided)
	{
		if (children[0]->CheckAllCollisionForObject(object, response))return true;
		if (children[1]->CheckAllCollisionForObject(object, response))return true;
		if (children[2]->CheckAllCollisionForObject(object, response))return true;
		if (children[3]->CheckAllCollisionForObject(object, response))return true;
	}

	if (index != -1)
	{
		if (parent)
		{
			if (ObjectIsWithinBucket(object) == false)
			{
				RemoveObject(object);
				parent->AddObject(object);
				return parent->CheckAllCollisionForObject(object,response);
			}
		}

		for (unsigned i = 0; i < collisionObjects.size(); i++)
		{
			if (i == index)continue;
			if (object->CollidesWith(collisionObjects[i]))
			{
				if (collisionObjects[i]->GetCollisionType() == BOX_COLLISION)
				{
					Vec3D normal = collisionObjects[i]->GetNormalBetween(object);
					response.collided = true;
					response.normal = normal;
					response.objectBounds[0] = object;
					response.objectBounds[1] = collisionObjects[i];
					response.collidedObjects[0] = object->GetPhysicsObject();
					response.collidedObjects[1] = collisionObjects[i]->GetPhysicsObject();
					response.intersection = object->GetScenePos() - collisionObjects[i]->GetScenePos();
					return true;
				}
				else
				{
					Vec3D normal = object->GetNormalBetween(collisionObjects[i]);
					response.collided = true;
					response.normal = normal;
					response.objectBounds[0] = object;
					response.objectBounds[1] = collisionObjects[i];
					response.collidedObjects[0] = object->GetPhysicsObject();
					response.collidedObjects[1] = collisionObjects[i]->GetPhysicsObject();
					response.intersection = collisionObjects[i]->GetScenePos() - object->GetScenePos();
					return true;
				}
			}
		}
		if (hasSubdivided)
		{
			if (children[0]->CheckCollisionForObject(object, response))return true;
			if (children[1]->CheckCollisionForObject(object, response))return true;
			if (children[2]->CheckCollisionForObject(object, response))return true;
			if (children[3]->CheckCollisionForObject(object, response))return true;
		}
		
		return true;
	}

	return false;
}

void QuadTreeCollisionBucket::Subdivide()
{
	if (hasSubdivided)
	{
		std::cerr << "QuadTreeCollisionBucket::Subdivide() Already Subdivided !\n";
		return;
	}

	Vec3D subSize = collision->GetSize() / 2.0;
	Vec3D size = collision->GetSize();

	Vec3D pos = collision->GetScenePos();

	Vec3D TL = Vec3D(pos.x - (size.x / 4.0), pos.y + (size.y / 4.0), 0);
	Vec3D BL = Vec3D(pos.x - (size.x / 4.0), pos.y - (size.y / 4.0), 0);
	Vec3D TR = Vec3D(pos.x + (size.x / 4.0), pos.y + (size.y / 4.0), 0);
	Vec3D BR = Vec3D(pos.x + (size.x / 4.0), pos.y - (size.y / 4.0), 0);

	std::string sname = sceneObject->readableName;

	children[TL_BUCKET] = new QuadTreeCollisionBucket((sname + ".topleft").c_str(),TL, subSize, maxObjects, this);
	children[TR_BUCKET] = new QuadTreeCollisionBucket((sname + ".topright").c_str(),TR, subSize, maxObjects, this);
	children[BL_BUCKET] = new QuadTreeCollisionBucket((sname + ".bottomleft").c_str(),BL, subSize, maxObjects, this);
	children[BR_BUCKET] = new QuadTreeCollisionBucket((sname + ".bottomright").c_str(),BR, subSize, maxObjects, this);

	std::vector<CollisionObjectBase*> keptObjects;

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

bool QuadTreeCollisionBucket::CheckCollisionForObject(CollisionObjectBase * object, CollisionResponse& response)
{
	for (unsigned i = 0; i < collisionObjects.size(); i++)
	{
		if (object->CollidesWith(collisionObjects[i]))
		{
			if (collisionObjects[i]->GetCollisionType() == BOX_COLLISION)
			{
				Vec3D normal = collisionObjects[i]->GetNormalBetween(object);
				response.collided = true;
				response.normal = normal;
				response.objectBounds[0] = object;
				response.objectBounds[1] = collisionObjects[i];
				response.collidedObjects[0] = object->GetPhysicsObject();
				response.collidedObjects[1] = collisionObjects[i]->GetPhysicsObject();
				response.intersection = object->GetScenePos() - collisionObjects[i]->GetScenePos();
				return true;
			}
			else
			{
				Vec3D normal = object->GetNormalBetween(collisionObjects[i]);
				response.collided = true;
				response.normal = normal;
				response.objectBounds[0] = object;
				response.objectBounds[1] = collisionObjects[i];
				response.collidedObjects[0] = object->GetPhysicsObject();
				response.collidedObjects[1] = collisionObjects[i]->GetPhysicsObject();
				response.intersection = collisionObjects[i]->GetScenePos() - object->GetScenePos();
				return true;
			}
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

bool QuadTreeCollisionBucket::PrintCollisionForObject(CollisionObjectBase * object)
{
	for (unsigned i = 0; i < collisionObjects.size(); i++)
	{
		Log(LogLevel_None, "\tCheck Collision %s => %s\n", object->GetSceneObject()->readableName.c_str(), collisionObjects[i]->GetSceneObject()->readableName.c_str());
		if (object->CollidesWith(collisionObjects[i]))
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

bool QuadTreeCollisionBucket::TraverseUpAddObject(CollisionObjectBase * object)
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
		CollisionObjectBase* object = collisionObjects[i];

		if (i < collisionObjects.size() - 1)
		{
			for (unsigned j = i + 1; j < collisionObjects.size(); j++)
			{
				Log(LogLevel_None, "\tCheck Collision %s => %s\n", object->GetSceneObject()->readableName.c_str(), collisionObjects[i]->GetSceneObject()->readableName.c_str());

				if (object->CollidesWith(collisionObjects[j]))
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
