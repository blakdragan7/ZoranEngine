#pragma once
#include "Vector3.h"

class CollisionObjectBase;
class SceneObject;
class PhysicsObjectBase
{
private:
	CollisionObjectBase * collision;
	SceneObject* sceneObject;

	Vector3D velocity;
	double drag;
	double gravity;
	bool shouldSimulate;

private:
	void RegisterPhysicsObject();

public:
	PhysicsObjectBase(SceneObject* object);
	virtual ~PhysicsObjectBase();

	void StartPhysicsSim();
	void StopPhysicsSim();

	void SetDrag(double drag) { this->drag = drag; }
	void ApplyForce(Vec3D Force);
	void SetVeloctiy(Vec3D Velocity);
	void SetGravity(double gravity);

	inline bool GetShouldSimulate() { return shouldSimulate; }

	Vec3D GetVelocity();
	Vec3D GetScenePos();
	SceneObject* GetSceneObject();

	virtual void Update(double deltaTime);
};
