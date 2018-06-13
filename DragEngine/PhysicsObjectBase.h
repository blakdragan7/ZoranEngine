#pragma once
#include "Vector3.h"

class CollisionObjectBase;
class SceneObject;
class DRAGENGINE_EXPORT PhysicsObjectBase
{
private:
	SceneObject* sceneObject;

	Vector3D velocity;
	double drag;
	bool shouldSimulate;
	double mass; // KG
	double restitution;
	Vector3D gravity;

	bool isOnGround;

private:
	void RegisterPhysicsObject();

public:
	PhysicsObjectBase(SceneObject* object);
	virtual ~PhysicsObjectBase();

	void StartPhysicsSim();
	void StopPhysicsSim();

	void SetMass(double mass); // in KG
	void SetRestitution(double restitution);

	void SetDrag(double drag) { this->drag = drag; }
	void ApplyForce(Vec3D Force);
	void SetVeloctiy(Vec3D Velocity);
	void SetGravity(Vector3D gravity);

	void OnCollision(struct CollisionResponse& response);

	inline bool GetShouldSimulate() { return shouldSimulate; }

	Vec3D GetVelocity();
	Vec3D GetScenePos();
	SceneObject* GetSceneObject();

	virtual void Update(double deltaTime);
};
