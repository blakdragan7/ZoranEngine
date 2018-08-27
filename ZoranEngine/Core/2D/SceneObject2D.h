#pragma once
#include "Core\SceneObject.h"
#include <Math/Matrix44.hpp>
#include <Math/Vector2.h>
#include <Math/Matrix22.h>

class CollisionObject2DBase;
class PhysicsObject2DBase;
class CollisionObjectBase;
class PhysicsObjectBase;

/*
*  2D scene objects scene objects who's coordinates are represented with 2D vectors and a single float for rotation
*/

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
	Vector2D size;
	Vector2D startingSize;

public:
	SceneObject2D(std::string name);
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

	float GetRotationRad()const;
	float GetRotation()const;

	inline const Vector2D& GetPosition()const { return pos; }
	inline const Vector2D& GetScale()const { return scale; }
	inline const Vector2D& GetSize()const { return size; }

	Matrix22 GetRotationMatrix()const;
	Matrix22 GetInvRotationMatrix()const;

	virtual void PreCaclModel()override;

	virtual Matrix44 GetScaleMatrix4x4()override;

	virtual void Destroy()override;
};

