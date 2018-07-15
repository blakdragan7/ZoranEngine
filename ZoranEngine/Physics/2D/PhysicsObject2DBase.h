#pragma once
#include <Physics\PhysicsObjectBase.h>
#include <Math/Vector2.h>

class SceneObject2D;
class ZoranEngine_EXPORT PhysicsObject2DBase : public PhysicsObjectBase
{
private:
	SceneObject2D* sceneObject2D;

	Vector2D velocity;

	Vector2D gravity;
	Vector2D gravityNormal;

	Vector2D force;

	float torque;
	float angularVelocity;

protected:
	bool SweepCollisionTo(Vec2D newPosition, struct SweepCollision2D& response);
	bool FastSweepCollision(Vec2D newPosition); // Broad Sweep
	virtual void CheckIfOnGround(struct Collision2D& response);

public:
	PhysicsObject2DBase(SceneObject2D* object);
	virtual ~PhysicsObject2DBase();

	void ApplyForce(Vec2D Force);
	void SetVeloctiy(Vec2D Velocity);
	void SetGravity(Vector2D gravity);

	virtual void SetMass(float mass)override; // in KG

	void OnCollision(struct Collision2D& response);
	bool OnSweepCollision(struct SweepCollision2D& response, float deltaTime,int currentDepth=0,int maxDepth=10);
	bool SweepToo(Vec2D targetPosition, struct SweepCollision2D &response);

	float GetAngularVelocity();

	Vec2D GetVelocity();
	Vec2D GetScenePos();
	SceneObject2D* GetSceneObject();

	virtual void UpdateVelocities(float deltaTime)override;
	virtual void UpdatePositionsAndRotation(float deltaTime)override;

	void ApplyImpulseToVelocity(Vector2D impulse);
	void ApplyImpulseToAngularVelocity(float impulse);
};

