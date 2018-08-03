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
	bool SweepCollisionTo(Vec3D newPosition, struct SweepCollisionResponse3D& response);
	bool FastSweepCollision(Vec3D newPosition); // Broad Sweep

public:
	PhysicsObject3DBase(SceneObject3D* object);
	virtual ~PhysicsObject3DBase();

	void ApplyForce(Vec3D Force);
	void SetVeloctiy(Vec3D Velocity);
	void SetGravity(Vector3D gravity);

	void OnCollision(struct CollisionResponse3D& response);
	bool SweepToo(Vec3D targetPosition, SweepCollisionResponse3D &response);

	inline bool GetShouldSimulate() { return shouldSimulate; }

	Vec3D GetVelocity();
	Vec3D GetScenePos();
	SceneObject3D* GetSceneObject();

	virtual void Update(float deltaTime);
};

