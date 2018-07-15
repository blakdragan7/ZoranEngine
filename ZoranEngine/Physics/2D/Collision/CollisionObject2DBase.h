#pragma once
#include <Physics/Collision/CollisionObjectBase.h>
#include <Math/Vector2.h>

class SceneObject2D;
class PhysicsObject2DBase;

struct ZoranEngine_EXPORT CollisionPoint
{
	Vector2D pos;
	Vector2D normal;
	Vector2D penetration;
	double penetrationDepth;

	CollisionPoint()
	{
		penetrationDepth = 0;
	}

	CollisionPoint Reflection()
	{
		CollisionPoint res;

		res.pos = pos;
		res.normal = -normal;
		res.penetration = -penetration;
		res.penetrationDepth = penetrationDepth;

		return res;
	}
};

struct ZoranEngine_EXPORT CollisionResponse2D
{
	bool collided;
	Vector2D velocitySnapshot[2];
	PhysicsObject2DBase* collidedObjects[2];
	std::vector<CollisionPoint> collisionPoints;
	class CollisionObject2DBase* objectBounds[2];
	unsigned long long ID;
	static unsigned long long sID;

	CollisionResponse2D()
	{
		collided = false;
		collidedObjects[0] = 0;
		collidedObjects[1] = 0;
		objectBounds[0] = 0;
		objectBounds[1] = 0;
		velocitySnapshot[0] = 0;
		velocitySnapshot[1] = 0;
		ID = sID++;
	}

	CollisionResponse2D Reflection()
	{
		CollisionResponse2D res;

		res.ID = ID;
		res.collided = collided;
		res.collidedObjects[0] = collidedObjects[1];
		res.collidedObjects[1] = collidedObjects[0];
		res.objectBounds[0] = objectBounds[1];
		res.objectBounds[1] = objectBounds[0];
		res.velocitySnapshot[0] = velocitySnapshot[1];
		res.velocitySnapshot[1] = velocitySnapshot[0];

		for (unsigned int i = 0; i < collisionPoints.size(); i++)
			res.collisionPoints.push_back(collisionPoints[i].Reflection());

		return res;
	};

	bool operator==(const CollisionResponse2D& other)
	{
		return ID == other.ID;
	}
};


struct ZoranEngine_EXPORT SweepCollisionResponse2D
{
	CollisionResponse2D CollisionResponse2D;
	double timeHit;

	SweepCollisionResponse2D()
	{
		timeHit = 1.0;
	}

	bool operator==(const SweepCollisionResponse2D& other)
	{
		return CollisionResponse2D.sID == other.CollisionResponse2D.sID;
	}
};

class ZoranEngine_EXPORT CollisionObject2DBase : public CollisionObjectBase
{
private:
	SceneObject2D * sceneObject2D;
	PhysicsObject2DBase* physicsObject2D;

public:
	CollisionObject2DBase(SceneObject2D *object, CollisionDynamics collisionDynamics = CD_Dynamic, unsigned collisionType = NO_COLLISION);
	~CollisionObject2DBase();

	void SetSceneObject(SceneObject2D* object);
	void SetPhysicsObject(PhysicsObject2DBase* object);

	virtual bool CollidesWith(CollisionObject2DBase* other, CollisionResponse2D& response) = 0;
	virtual Vector2D GetClosestPointTo(Vector2D pos) = 0;
	virtual Vector2D GetSize() = 0;

	virtual bool SweepCollidesWith(CollisionObject2DBase* other, Vector2D newPosition, SweepCollisionResponse2D & response) = 0;
	virtual bool FastSweepCollidesWith(Vector2D newPosition) = 0;

	Vector2D GetScenePos();
	SceneObject2D* GetSceneObject();
	PhysicsObject2DBase* GetPhysicsObject();
};
