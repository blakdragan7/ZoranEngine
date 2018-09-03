#pragma once
#include <Physics\PhysicsObjectBase.h>
#include <Math/Vector2.h>

class Component2DBase;
class ZoranEngine_EXPORT PhysicsObject2DBase : public PhysicsObjectBase
{
private:
	Component2DBase * affected2DComponent;

	Vector2D velocity;

	Vector2D gravity;
	Vector2D gravityNormal;

	Vector2D force;

	float torque;
	float angularVelocity;

protected:
	bool SweepCollisionTo(Vec2D newPosition, struct SweepCollision2D& response);
	bool FastSweepCollision(Vec2D newPosition); // Broad Sweep
	virtual void CheckIfOnGround(class Collision2D& response);

public:
	PhysicsObject2DBase(Component2DBase* component);
	virtual ~PhysicsObject2DBase();

	void ApplyForce(const Vector2D& Force);
	void SetVeloctiy(const Vector2D& Velocity);
	void SetGravity(const Vector2D& gravity);
	void SetAngularVeloctiy(float Velocity);

	virtual void SetMass(float mass)override; // in KG

	virtual void ReCalculateInertia();

	void OnCollision(class Collision2D& response);
	bool OnSweepCollision(struct SweepCollision2D& response, float deltaTime,int currentDepth=0,int maxDepth=10);
	bool SweepToo(Vec2D targetPosition, struct SweepCollision2D &response);

	float GetAngularVelocity();

	inline const Vec2D& GetVelocity();
	inline const Vec2D& GetScenePos();

	virtual void UpdateVelocities(float deltaTime)override;
	virtual void UpdatePositionsAndRotation(float deltaTime)override;

	void ApplyImpulseToVelocity(const Vector2D& impulse);
	void ApplyImpulseToAngularVelocity(float impulse);
};

