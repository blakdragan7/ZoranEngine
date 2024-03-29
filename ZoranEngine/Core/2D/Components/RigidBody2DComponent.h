#pragma once
#include <Core\Components\RigidBodyComponent.h>
#include <Core\2D\Components\Component2DBase.h>

class PhysicsObject2DBase;
class ZoranEngine_EXPORT RigidBody2DComponent : public RigidBodyComponent, public Component2DBase
{
private:
	PhysicsObject2DBase * physics2D;

public:
	RigidBody2DComponent(Component2DBase* affectComponent);
	~RigidBody2DComponent();

	void ApplyForce(const Vector2D& Force);
	void SetVeloctiy(const Vector2D& Velocity);
	void SetGravity(const Vector2D& gravity);
	void SetAngularVeloctiy(float Velocity);

	void ApplyImpulseToVelocity(const Vector2D& impulse);
	void ApplyImpulseToAngularVelocity(float impulse);

	float GetAngularVelocity();

	Vector2D GetVelocity();

	inline PhysicsObject2DBase* GetPhysics2D() { return physics2D; }
};

