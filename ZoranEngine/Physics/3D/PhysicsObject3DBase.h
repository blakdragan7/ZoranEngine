#pragma once
#include <Physics\PhysicsObjectBase.h>

class SceneObject3D;
class ZoranEngine_EXPORT PhysicsObject3DBase : public PhysicsObjectBase
{
private:
	class AABBoxCollisionObject * sweepCollisionBox;

	SceneObject3D* sceneObject3D;

	Vector3D velocity;

	Vector3D gravity;
	Vector3D gravityNormal;

protected:
	bool SweepCollisionTo(const Vector3D& newPosition, struct SweepCollisionResponse3D& response);
	bool FastSweepCollision(const Vector3D& newPosition); // Broad Sweep

public:
	PhysicsObject3DBase(SceneObject3D* object);
	virtual ~PhysicsObject3DBase();

	void ApplyForce(const Vector3D& Force);
	void SetVeloctiy(const Vector3D& Velocity);
	void SetGravity(const Vector3D& gravity);

	void OnCollision(struct CollisionResponse3D& response);
	bool SweepToo(const Vector3D& targetPosition, SweepCollisionResponse3D &response);

	inline bool GetShouldSimulate() { return shouldSimulate; }

	const Vector3D& GetVelocity();
	const Vector3D& GetScenePos();
	SceneObject3D* GetSceneObject();

	virtual void Update(float deltaTime);
};

