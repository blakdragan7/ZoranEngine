#pragma once
#include <Physics\PhysicsObjectBase.h>
#include <Math/Vector2.h>

class SceneObject2D;
class ZoranEngine_EXPORT PhysicsObject2DBase : public PhysicsObjectBase
{
private:
	class AABBSquareCollisionObject * sweepCollisionSquare;
	SceneObject2D* sceneObject2D;

	Vector2D velocity;

	Vector2D gravity;
	Vector2D gravityNormal;

protected:
	bool SweepCollisionTo(Vec2D newPosition, struct SweepCollisionResponse2D& response);
	bool FastSweepCollision(Vec2D newPosition); // Broad Sweep

public:
	PhysicsObject2DBase(SceneObject2D* object);
	virtual ~PhysicsObject2DBase();

	void ApplyForce(Vec2D Force);
	void SetVeloctiy(Vec2D Velocity);
	void SetGravity(Vector2D gravity);

	void OnCollision(struct CollisionResponse2D& response);
	bool SweepToo(Vec2D targetPosition, struct SweepCollisionResponse2D &response);

	inline bool GetShouldSimulate() { return shouldSimulate; }

	Vec2D GetVelocity();
	Vec2D GetScenePos();
	SceneObject2D* GetSceneObject();

	virtual void Update(double deltaTime);
};

