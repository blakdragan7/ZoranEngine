#pragma once
#include "Core\SceneObject.h"
#include <Math/Matrix44.hpp>
#include <Math/Vector2.h>
#include <Math/Matrix22.h>

class CollisionObject2DBase;
class PhysicsObject2DBase;
class CollisionObjectBase;
class PhysicsObjectBase;

class ZoranEngine_EXPORT SceneObject2D : public SceneObject
{
private:
	// used to make GetModel more effecient
	Matrix44 model;

	Vector2D scale;
	Vector2D pos;
	float rotation; // degrees

	Matrix22 rotationMat;
	Matrix22 invRotationMat;

protected:
	CollisionObject2DBase * collision;
	PhysicsObject2DBase* physicsObject;
	Vector2D size;
	Vector2D startingSize;

public:
	SceneObject2D(std::string name);
	SceneObject2D(std::string name, RenderEngineBase* engine);
	~SceneObject2D();

	void SetPosition(Vector2D pos);
	void SetScale(Vector2D scale);

	void SetPosition(float x, float y);
	void SetScale(float x, float y);
	
	void SetRotation(float rotation);
	
	void Translate(Vector2D delta);
	void Translate(float dx, float dy);

	void Scale(Vector2D scale);
	void Scale(float dx, float dy);

	void Rotate(float rotation);

	float GetRotationRad();
	float GetRotation();
	Vector2D GetPosition();
	Vector2D GetScale();
	Vector2D GetSize();

	Matrix22 GetRotationMatrix();
	Matrix22 GetInvRotationMatrix();

	virtual void PreCaclModel()override;

	//virtual MatrixF GetScaleMatrix3x3()override;
	virtual Matrix44 GetScaleMatrix4x4()override;

	inline CollisionObject2DBase* GetCollision2D() { return collision; }
	inline class PhysicsObject2DBase* GetPhysics2D() { return physicsObject; }

	inline class CollisionObjectBase* GetCollision() { return (CollisionObjectBase*)collision; }
	inline PhysicsObjectBase* GetPhysics() { return (PhysicsObjectBase*)physicsObject; }

	virtual void Destroy()override;
};

