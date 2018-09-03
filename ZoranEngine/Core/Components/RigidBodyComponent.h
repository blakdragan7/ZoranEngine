#pragma once
class PhysicsObjectBase;
class ComponentBase;
class RigidBodyComponent
{
protected:
	PhysicsObjectBase * physicsObject;
	
public:
	RigidBodyComponent(PhysicsObjectBase* pObject);
	virtual ~RigidBodyComponent();

	inline PhysicsObjectBase* GetPhyicsObject()const { return physicsObject; }

	void StartSimulation();
	void StopSimulation();
	void SetMass(float mass); // in KG
	void SetFriction(float friction);
	void SetRestitution(float restitution);
	void SetDrag(float drag);

	float GetFriction();
	float GetInvMass();
	float GetMass();
	float GetInvInertia();
	float GetInertia();

	bool GetIsSimulating();
};

