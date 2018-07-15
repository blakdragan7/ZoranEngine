#pragma once
#include "Math/Vector3.h"

class ZoranEngine_EXPORT PhysicsObjectBase
{
private:
	SceneObject* sceneObject;

protected:
	double currentDeltaTime;
	double drag;
	double friction;
	double otherFriction;
	
	double calculatedFriction;

	bool shouldSimulate;
	
	double mass; // KG
	double invMass; // KG
	double restitution;
	
	double inertia;
	double invInertia;

	bool isOnGround;
	bool useSweptCollision;

private:
	void RegisterPhysicsObject();

public:
	PhysicsObjectBase(SceneObject* object);
	virtual ~PhysicsObjectBase();

	void StartPhysicsSim();
	void StopPhysicsSim();

	virtual void SetMass(double mass); // in KG
	virtual void SetFriction(double friction);
	virtual void SetRestitution(double restitution);

	void SetSweptCollision(bool newSwept);

	void SetDrag(double drag) { this->drag = drag; }

	inline double GetCurrentDeltaTime() { return currentDeltaTime; }

	inline bool GetShouldSimulate() { return shouldSimulate; }
	inline bool GetIsSweptCollision() { return useSweptCollision; }

	SceneObject* GetSceneObject();

	virtual void UpdateVelocities(double deltaTime) = 0;
	virtual void UpdatePositionsAndRotation(double deltaTime) = 0;
};
