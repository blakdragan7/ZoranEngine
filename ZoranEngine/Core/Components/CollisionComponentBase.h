#pragma once
enum CollisionDynamics;
class CollisionObjectBase;
class RigidBodyComponent;
class CollisionComponentBase
{
protected:
	CollisionObjectBase * collision;

public:
	CollisionComponentBase(CollisionObjectBase * collision);
	virtual ~CollisionComponentBase();

	void SetPhysicsObjectFromRigidBody(RigidBodyComponent* object);

	unsigned GetCollisionLayer();
	void SetCollisionLayer(unsigned layer);

	void SetDynamics(CollisionDynamics dynamics);

	inline CollisionObjectBase * GetCollisionObject()const { return collision; }
};

