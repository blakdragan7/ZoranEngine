#pragma once
#include <Physics\PhysicsObjectBase.h>

class Component3DBase;
class ZoranEngine_EXPORT PhysicsObject3DBase : public PhysicsObjectBase
{
private:
	class AABBoxCollisionObject * sweepCollisionBox;

	Component3DBase* affected3DComponent;

	Vector3D velocity;

	Vector3D gravity;
	Vector3D gravityNormal;

protected:
	bool SweepCollisionTo(const Vector3D& newPosition, struct SweepCollisionResponse3D& response);
	bool FastSweepCollision(const Vector3D& newPosition); // Broad Sweep

public:
	PhysicsObject3DBase(Component3DBase* component);
	virtual ~PhysicsObject3DBase();

	void ApplyForce(const Vector3D& Force);
	void SetVeloctiy(const Vector3D& Velocity);
	void SetGravity(const Vector3D& gravity);

	void OnCollision(struct CollisionResponse3D& response);
	bool SweepToo(const Vector3D& targetPosition, SweepCollisionResponse3D &response);

	inline bool GetShouldSimulate() { return shouldSimulate; }

	const Vector3D& GetVelocity();
	const Vector3D GetScenePos();
	Component3DBase* GetAffected3DComponent();

	virtual void Update(float deltaTime);
};

