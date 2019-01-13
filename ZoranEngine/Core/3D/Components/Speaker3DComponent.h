#pragma once
#include <Core\3D\Components\Component3DBase.h>
#include <Core\Components\SpeakerComponentBase.h>

class RigidBody3DComponent;
class ZoranEngine_EXPORT Speaker3DComponent : public Component3DBase, public SpeakerComponentBase
{
private:
	RigidBody3DComponent * physics;
public:
	Speaker3DComponent(RigidBody3DComponent * physics = 0);
	~Speaker3DComponent();

	virtual void Tick(float dt)override;

	inline void SetPhysics(RigidBody3DComponent* physics) { this->physics = physics; }
};

