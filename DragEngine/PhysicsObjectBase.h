#pragma once
#include "Vector3.h"

class CollisionObjectBase;
class SceneObject;
class DRAGENGINE_EXPORT PhysicsObjectBase
{
private:
	class BoxCollisionObject * sweepCollisionBox;

	SceneObject* sceneObject;

	Vector3D velocity;
	double drag;
	bool shouldSimulate;
	double mass; // KG
	double restitution;

	bool isOnGround;
	Vector3D gravity;

	bool useSweptCollision;

private:
	void RegisterPhysicsObject();

protected:
	float SweepCollisionTo(Vec3D newPosition, struct CollisionResponse& response);
	bool FastSweepCollision(Vec3D newPosition); // Braod Sweep

public:
	PhysicsObjectBase(SceneObject* object);
	virtual ~PhysicsObjectBase();

	void StartPhysicsSim();
	void StopPhysicsSim();

	void SetMass(double mass); // in KG
	void SetRestitution(double restitution);

	void SetSweptCollision(bool newSwept);

	void SetDrag(double drag) { this->drag = drag; }
	void ApplyForce(Vec3D Force);
	void SetVeloctiy(Vec3D Velocity);
	void SetGravity(Vector3D gravity);

	void OnCollision(struct CollisionResponse& response);
	bool SweepToo(Vec3D targetPosition, Vec3D &actualPosition);

	inline bool GetShouldSimulate() { return shouldSimulate; }

	Vec3D GetVelocity();
	Vec3D GetScenePos();
	SceneObject* GetSceneObject();

	virtual void Update(double deltaTime);
};
