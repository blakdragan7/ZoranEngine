#pragma once
#include <vector>
#include <Physics/2D/Collision/CollisionObject2DBase.h>
struct CollisionFrame2D
{
	std::vector<CollisionResponse2D> collisions;
	std::vector<SweepCollisionResponse2D> sweptCollisions;

	void Clear()
	{
		collisions.clear();
		sweptCollisions.clear();
	}
};
