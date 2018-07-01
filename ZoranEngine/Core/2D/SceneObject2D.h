#pragma once
#include "Core\SceneObject.h"
#include <Math/Matrix.hpp>
#include <Math/Vector2.h>

class CollisionObject2DBase;
class PhysicsObject2DBase;
class CollisionObjectBase;
class PhysicsObjectBase;

class ZoranEngine_EXPORT SceneObject2D : public SceneObject
{
private:
	// used to make GetModel more effecient
	MatrixF model;

	Vector2D scale;
	Vector2D pos;
	double rotation; // degrees

protected:
	CollisionObject2DBase * collision;
	PhysicsObject2DBase* physicsObject;

public:
	SceneObject2D(std::string name);
	SceneObject2D(std::string name, RenderEngineBase* engine);
	~SceneObject2D();

	void SetPosition(Vector2D pos);
	void SetScale(Vector2D scale);

	void SetPosition(double x, double y);
	void SetScale(double x, double y);
	
	void SetRotation(double rotation);
	
	void Translate(Vector2D delta);
	void Translate(double dx, double dy);

	void Scale(Vector2D scale);
	void Scale(double dx, double dy);

	void Rotate(double rotation);

	double GetRotationRad();
	double GetRotation();
	Vector2D GetPosition();
	Vector2D GetScale();

	virtual MatrixF GetModel()override;
	virtual MatrixF GetScaleMatrix3x3()override;
	virtual MatrixF GetScaleMatrix4x4()override;

	inline CollisionObject2DBase* GetCollision2D() { return collision; }
	inline class PhysicsObject2DBase* GetPhysics2D() { return physicsObject; }

	inline class CollisionObjectBase* GetCollision() { return (CollisionObjectBase*)collision; }
	inline PhysicsObjectBase* GetPhysics() { return (PhysicsObjectBase*)physicsObject; }

	virtual void Destroy()override;
};

