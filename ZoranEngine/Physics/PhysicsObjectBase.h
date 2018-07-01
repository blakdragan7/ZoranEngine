#pragma once
#include "Math/Vector3.h"

class ZoranEngine_EXPORT PhysicsObjectBase
{
private:
	SceneObject* sceneObject;

protected:
	double drag;
	double friction;
	double otherFriction;
	bool shouldSimulate;
	double mass; // KG
	double restitution;
	bool isOnGround;
	bool useSweptCollision;

private:
	void RegisterPhysicsObject();

public:
	PhysicsObjectBase(SceneObject* object);
	virtual ~PhysicsObjectBase();

	void StartPhysicsSim();
	void StopPhysicsSim();

	void SetMass(double mass); // in KG
	void SetFriction(double friction);
	void SetRestitution(double restitution);

	void SetSweptCollision(bool newSwept);

	void SetDrag(double drag) { this->drag = drag; }

	inline bool GetShouldSimulate() { return shouldSimulate; }

	SceneObject* GetSceneObject();

	virtual void Update(double deltaTime) = 0;
};
