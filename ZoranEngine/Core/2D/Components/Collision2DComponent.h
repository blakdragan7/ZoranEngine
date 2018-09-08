#pragma once
#include <Core\2D\Components\Component2DBase.h>
#include <Core\Components\CollisionComponentBase.h>
class CollisionObject2DBase;
class RigidBody2DComponent;
class ZoranEngine_EXPORT Collision2DComponent : public Component2DBase, public CollisionComponentBase
{
protected:
	CollisionObject2DBase * preCast;

public:
	Collision2DComponent();
	Collision2DComponent(CollisionObject2DBase* collision);
	~Collision2DComponent();

	inline CollisionObject2DBase* Get2DCollision()const { return preCast; }
	inline void SetCollision(CollisionObject2DBase* col) { this->preCast = col; SetCollisionObject((CollisionObjectBase*)col); }
	void SetPhysicsObjectFrom2DRigidBody(RigidBody2DComponent* object);
};

