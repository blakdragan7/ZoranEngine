#pragma once
#include <Core\Components\RigidBodyComponent.h>
#include <Core\3D\Components\Component3DBase.h>
class PhysicsObject3DBase;
class RigidBody3DComponent : public RigidBodyComponent, public Component3DBase
{
private:
	PhysicsObject3DBase * physics3D;

public:
	RigidBody3DComponent(PhysicsObject3DBase * physics3D);
	~RigidBody3DComponent();

	void ApplyForce(const Vector3D& Force);
	void SetVeloctiy(const Vector3D& Velocity);
	void SetGravity(const Vector3D& gravity);

	const Vector3D& GetVelocity();
};

