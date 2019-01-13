#pragma once
#include <Core\2D\Components\Component2DBase.h>
#include <Core\Components\SpeakerComponentBase.h>

class RigidBody2DComponent;
class ZoranEngine_EXPORT Speaker2DComponent : public Component2DBase, public SpeakerComponentBase
{
private:
	RigidBody2DComponent * physics;

public:
	Speaker2DComponent(RigidBody2DComponent* physics = 0);
	~Speaker2DComponent();

	virtual void Tick(float dt)override;

	inline void SetPhysics(RigidBody2DComponent* physics) { this->physics = physics; }
};

