#pragma once
#include <Core/3D\Components/Component3DBase.h>
#include <Core\Components\CollisionComponentBase.h>
class CollisionObject3DBase;
class ZoranEngine_EXPORT Collision3DComponent : public Component3DBase, CollisionComponentBase
{
private:
	CollisionObject3DBase * preCast;

public:
	Collision3DComponent(CollisionObject3DBase * collision);
	~Collision3DComponent();

	inline CollisionObject3DBase* Get3DCollision()const { return preCast; }
};

