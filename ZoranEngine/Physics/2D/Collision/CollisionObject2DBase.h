#pragma once

#include <Physics/2D/Collision/Collision2D.h>
#include <Physics/Collision/CollisionObjectBase.h>

class SceneObject2D;
class PhysicsObject2DBase;




struct ZoranEngine_EXPORT SweepCollision2D
{
	Collision2D Collision2D;
	float timeHit;

	SweepCollision2D()
	{
		timeHit = 1.0;
	}

	bool operator==(const SweepCollision2D& other)
	{
		return Collision2D.sID == other.Collision2D.sID;
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

	virtual bool CollidesWithNoCollision(CollisionObject2DBase* other) = 0;
	virtual bool CollidesWith(CollisionObject2DBase* other, Collision2D* response) = 0;
	virtual Vector2D GetClosestPointTo(Vector2D pos) = 0;
	virtual Vector2D GetSize() = 0;

	virtual bool SweepCollidesWith(CollisionObject2DBase* other, Vector2D newPosition, SweepCollision2D & response) = 0;
	virtual bool FastSweepCollidesWith(Vector2D newPosition) = 0;

	Vector2D GetScenePos();
	SceneObject2D* GetSceneObject();
	PhysicsObject2DBase* GetPhysicsObject();
};
