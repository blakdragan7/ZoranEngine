#include "stdafx.h"
#include "QuadTreeCollisionBucket.h"
#include "BoxCollisionObject.h"
#include "SceneObject.h"
#include "ThreadBase.h"

template <typename T>
void remove(std::vector<T> &v, size_t i);

QuadTreeCollisionBucket::QuadTreeCollisionBucket(Vec3D pos, Vec3D size, unsigned maxObjects,QuadTreeCollisionBucket * parent)
{
	children[0] = 0;
	children[1] = 0;
	children[2] = 0;
	children[3] = 0;

	hasSubdivided = false;

	this->parent = parent;

	this->collision = new BoxCollisionObject(pos - (size/2.0), pos + (size / 2.0), NULL);
	this->sceneObject = new SceneObject(NULL);
	this->sceneObject->SetPosition(pos);
	this->maxObjects = maxObjects;
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

	delete collision;
	delete sceneObject;
}

void QuadTreeCollisionBucket::AddObject(CollisionObjectBase * object)
{
	while (mutex.try_lock() == false)ThreadBase::Sleep(0.001);
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
	while (mutex.try_lock() == false)ThreadBase::Sleep(0.001);
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
}

bool QuadTreeCollisionBucket::ObjectIsWithinBucket(CollisionObjectBase * object)
{
	return object->CollidesWith(collision);
}

void QuadTreeCollisionBucket::Subdivide()
{
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

template <typename T>
void remove(std::vector<T> &v, size_t i)
{
	std::swap(v[i], v.back());
	v.pop_back();
}
