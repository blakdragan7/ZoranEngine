#pragma once
#include "Core\SceneObject.h"
#include <Math/Matrix.hpp>
#include <Math/Vector2.h>

class SceneObject2D : public SceneObject
{
private:
	// used to make GetModel more effecient
	MatrixF model;

	Vector2D scale;
	Vector2D pos;
	double rotation;

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

	double GetRotation();
	Vector2D GetPosition();
	Vector2D GetScale();

	virtual MatrixF GetModel()override;
	virtual MatrixF GetScaleMatrix3x3()override;
	virtual MatrixF GetScaleMatrix4x4()override;
};

