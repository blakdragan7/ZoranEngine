#pragma once
enum CollisionDynamics;
class CollisionObjectBase;
class RigidBodyComponent;

class ZoranEngine_EXPORT CollisionComponentBase
{
private:
	CollisionObjectBase * collision;

protected:
	void SetPhysicsObjectFromRigidBody(RigidBodyComponent* object);

public:
	CollisionComponentBase();
	CollisionComponentBase(CollisionObjectBase * collision);
	virtual ~CollisionComponentBase();

	void SetCollisionObject(CollisionObjectBase* object);

	unsigned GetCollisionLayer()const;
	void SetCollisionLayer(unsigned layer);

	void SetDynamics(CollisionDynamics dynamics);

	inline CollisionObjectBase * GetCollisionObject()const { return collision; }
};

