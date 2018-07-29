#pragma once
#include "Math/Vector3.h"

class ZoranEngine_EXPORT PhysicsObjectBase
{
private:
	SceneObject* sceneObject;

protected:
	float currentDeltaTime;
	float drag;
	float friction;
	float otherFriction;
	
	float calculatedFriction;

	bool shouldSimulate;
	
	float mass; // KG
	float invMass; // KG
	float restitution;
	
	float inertia;
	float invInertia;

	bool isOnGround;
	bool useSweptCollision;

private:
	void RegisterPhysicsObject();

public:
	PhysicsObjectBase(SceneObject* object);
	virtual ~PhysicsObjectBase();

	void StartPhysicsSim();
	void StopPhysicsSim();

	virtual void SetMass(float mass); // in KG
	virtual void SetFriction(float friction);
	virtual void SetRestitution(float restitution);

	float GetFriction() { return friction; }
	float GetInvMass() { return invMass; }
	float GetMass() { return mass; }
	float GetInvInertia() { return invInertia; }
	float GetInertia() { return inertia; }

	void SetSweptCollision(bool newSwept);

	void SetDrag(float drag) { this->drag = drag; }

	inline float GetCurrentDeltaTime() { return currentDeltaTime; }

	inline bool GetShouldSimulate() { return shouldSimulate; }
	inline bool GetIsSweptCollision() { return useSweptCollision; }

	SceneObject* GetSceneObject();

	virtual void UpdateVelocities(float deltaTime) = 0;
	virtual void UpdatePositionsAndRotation(float deltaTime) = 0;

	friend class Collision2D;
};
