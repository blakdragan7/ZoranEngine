#pragma once
#include <map>
#include <Physics/2D/Collision/CollisionObject2DBase.h>
struct CollisionFrame2D
{
	typedef std::pair<Collision2DKey, Collision2D> CollisionPair;
	std::map<Collision2DKey,Collision2D> collisions;
	std::vector<SweepCollision2D> sweptCollisions;

	void Clear()
	{
		collisions.clear();
		sweptCollisions.clear();
	}

	void UpdateCollisionWithKey(Collision2DKey& key,Collision2D& collision)
	{
		auto iter = collisions.find(key);
		if (iter == collisions.end())
		{
			collisions.insert(CollisionPair(key, collision));
		}
		else
		{
			iter->second.Update(collision.collisionPoints);
		}
	}

	void RemoveDullCollisions()
	{
		auto itr = collisions.begin();
		while (itr != collisions.end()) {
			if ((*itr).second.wasUpdated == false) {
				itr = collisions.erase(itr);
			}
			else {
				++itr;
			}
		}
	}
};
