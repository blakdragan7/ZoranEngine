#pragma once
#include "Math/Matrix.hpp"
#include "Math/Quaternion.h"

#include <string>

class SceneObject;
class CameraBase
{
protected:
	SceneObject * sceneObject;

public:
	CameraBase(std::string name,Vec3D position,Vec3D scale,Vec3D rotation);
	virtual ~CameraBase();

	virtual MatrixF GetModel() = 0;

	void Translate(double deltax,double deltay,double deltaz);
	void Translate(Vec3D delta);
	void Rotate(double rotx, double roty, double rotz);
	void Rotate(Vec3D euler);
	void Rotate(Quaternion quat);

	inline SceneObject* GetSceneObject() { return sceneObject; }
};

