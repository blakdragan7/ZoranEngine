#include "stdafx.h"
#include "QuadTreeCollisionBucket.h"
#include "ThreadBase.h"

template <typename T>
void remove(std::vector<T> &v, size_t i);

QuadTreeCollisionBucket::QuadTreeCollisionBucket(Vec2L pos, Vec2L size, unsigned maxObjects,QuadTreeCollisionBucket * parent)
{
	children[0] = 0;
	children[1] = 0;
	children[2] = 0;
	children[3] = 0;

	this->parent = parent;

	this->pos = pos;
	this->size = size;

	this->min = Vec2L(pos - (size / 2.0));
	this->max = Vec2L(pos + (size / 2.0));

	this->maxObjects = maxObjects;
}


QuadTreeCollisionBucket::~QuadTreeCollisionBucket()
{
}

void QuadTreeCollisionBucket::AddObject(CollisionObjectBase * object)
{
	while (mutex.try_lock() == false)ThreadBase::Sleep(0.001);
	collisionObjects.push_back(object);
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
	mutex.unlock();
	return nullptr;
}

void QuadTreeCollisionBucket::Subdivide()
{
}

template <typename T>
void remove(std::vector<T> &v, size_t i)
{
	std::swap(v[i], v.back());
	v.pop_back();
}
