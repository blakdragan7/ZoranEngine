#pragma once

#include <Physics/2D/Collision/Collision2D.h>
#include <Physics/Collision/CollisionObjectBase.h>

class Component2DBase;
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

class RigidBody2DComponent;
class ZoranEngine_EXPORT CollisionObject2DBase : public CollisionObjectBase
{
private:
	Component2DBase * affectedComponent;
	PhysicsObject2DBase* physicsObject2D;
	class DebugSceneObject2D* debugObject;

protected:
	void UpdateDebugObject(float verts[], unsigned numVerts);

public:
	CollisionObject2DBase(Component2DBase* component, CollisionDynamics collisionDynamics = CD_Dynamic, unsigned collisionType = NO_COLLISION);
	~CollisionObject2DBase();

	void SetAffectedComponent(RigidBody2DComponent* object);
	void SetPhysicsObject(PhysicsObject2DBase* object);

	virtual bool CollidesWithNoCollision(CollisionObject2DBase* other) = 0;
	virtual Collision2D* CollidesWith(CollisionObject2DBase* other) = 0;
	virtual Vector2D GetClosestPointTo(Vector2D pos) = 0;
	virtual Vector2D GetSize() = 0;

	virtual bool SweepCollidesWith(CollisionObject2DBase* other, Vector2D newPosition, SweepCollision2D & response) = 0;
	virtual bool FastSweepCollidesWith(Vector2D newPosition) = 0;

	const Vector2D GetScenePos();

	Component2DBase* CollisionObject2DBase::GetAffectedComponent() { return affectedComponent; }
	PhysicsObject2DBase* CollisionObject2DBase::GetPhysicsObject() { return physicsObject2D; }
};
