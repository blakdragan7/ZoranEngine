#include "stdafx.h"
#include "QuadTreeCollisionBucket.h"
#include "BoxCollisionObject.h"
#include "SceneObject.h"
#include "ThreadBase.h"
#include "VectorAddons.hpp"

QuadTreeCollisionBucket::QuadTreeCollisionBucket(Vec3D pos, Vec3D size, unsigned maxObjects,QuadTreeCollisionBucket * parent) : CollisionBucketBase(pos,size,maxObjects)
{
	children[0] = 0;
	children[1] = 0;
	children[2] = 0;
	children[3] = 0;

	hasSubdivided = false;

	this->parent = parent;
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
}

void QuadTreeCollisionBucket::AddObject(CollisionObjectBase * object)
{
	while (mutex.try_lock() == false)ThreadBase::Sleep(0.001F);
	if (collisionObjects.size() < maxObjects && !hasSubdivided)
	{
		collisionObjects.push_back(object);
	}
	else 
	{
		if (hasSubdivided == false)Subdivide();
		for (unsigned i = 0; i < 4; i++)
		{
			if (children[i]->ObjectIsWithinBucket(object))
			{
				children[i]->AddObject(object);
				break;
			}
		}
	}

	mutex.unlock();
}

CollisionObjectBase * QuadTreeCollisionBucket::RemoveObject(CollisionObjectBase * object)
{
	while (mutex.try_lock() == false)ThreadBase::Sleep(0.001F);
	auto iter = std::find(collisionObjects.begin(), collisionObjects.end(), object);
	if (iter != collisionObjects.end())
	{
		size_t index = iter - collisionObjects.begin();
		remove(collisionObjects,index);
		return *iter;
	}
	else if(hasSubdivided)
	{
		for (unsigned i = 0; i > 4; i++)
		{
			if (children[i]->RemoveObject(object))
			{
				return object;
			}
		}
	}
	mutex.unlock();
	return nullptr;
}

void QuadTreeCollisionBucket::CheckAllCollision()
{
	for (unsigned i = 0;i<collisionObjects.size();i++)
	{
		CollisionObjectBase* object = collisionObjects[i];
		
		if (parent)
		{
			if (ObjectIsWithinBucket(object) == false)
			{
				RemoveObject(object);
				parent->AddObject(object);
			}
		}

		if (i < collisionObjects.size() - 1)
		{
			for (unsigned j = i+1; j < collisionObjects.size(); j++)
			{
				if (object->CollidesWith(collisionObjects[j]))
				{
					//std::cout << "Collision !!\n";
				}
			}
		}
	}

	if (hasSubdivided)
	{
		children[0]->CheckAllCollision();
		children[1]->CheckAllCollision();
		children[2]->CheckAllCollision();
		children[3]->CheckAllCollision();
	}
}

bool QuadTreeCollisionBucket::ObjectIsWithinBucket(CollisionObjectBase * object)
{
	return object->CollidesWith(collision);
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

	Vec3D TL = Vec3D(pos.x - size.x / 4.0, pos.y + size.y / 4.0, 0);
	Vec3D TR = Vec3D(pos.x - size.x / 4.0, pos.y + size.y / 4.0, 0);
	Vec3D BL = Vec3D(pos.x - size.x / 4.0, pos.y + size.y / 4.0, 0);
	Vec3D BR = Vec3D(pos.x - size.x / 4.0, pos.y + size.y / 4.0, 0);

	children[TL_BUCKET] = new QuadTreeCollisionBucket(TL, subSize, maxObjects, parent);
	children[TR_BUCKET] = new QuadTreeCollisionBucket(TR, subSize, maxObjects, parent);
	children[BL_BUCKET] = new QuadTreeCollisionBucket(BL, subSize, maxObjects, parent);
	children[BR_BUCKET] = new QuadTreeCollisionBucket(BR, subSize, maxObjects, parent);

	for (auto object : collisionObjects)
	{
		for (unsigned i = 0; i < 4; i++)
		{
			if (children[i]->ObjectIsWithinBucket(object))
			{
				children[i]->AddObject(object);
				break;
			}
		}
	}

	collisionObjects.clear();
}