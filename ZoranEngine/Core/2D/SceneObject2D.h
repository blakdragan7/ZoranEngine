#pragma once
#include "Core\SceneObject.h"
#include <Math/Matrix44.hpp>
#include <Math/Vector2.h>

class Component2DBase;
/*
*  2D scene objects scene objects who's coordinates are represented with 2D vectors and a single float for rotation
*/

class ZoranEngine_EXPORT SceneObject2D : public SceneObject
{
private:
	// used to make GetModel more effecient
	Matrix44 model;

protected:
	Component2DBase* root2DComponent;

public:
	SceneObject2D(std::string name);
	SceneObject2D(Component2DBase* root2DComponent,std::string name);
	~SceneObject2D();

	void SetPosition(const Vector2D& pos);
	void SetScale(const Vector2D& scale);

	void SetPosition(float x, float y);
	void SetScale(float x, float y);
	
	void SetRotation(float rotation);
	
	void Translate(const Vector2D& delta);
	void Translate(float dx, float dy);

	void Scale(const Vector2D& scale);
	void Scale(float dx, float dy);

	void Rotate(float rotation);

	float GetRotationDegree()const;
	float GetRotation()const;

	inline const Vector2D& GetPosition()const { return root2DComponent->GetOffset(); }
	inline const Vector2D& GetScale()const { return root2DComponent->GetScale(); }
	inline const Vector2D& GetSize()const { return root2DComponent->GetSize(); }

	virtual void PreCaclModel()override;

	virtual Matrix44 GetScaleMatrix4x4()override;

	virtual void Destroy()override;
};

