#pragma once
#include <Math/Matrix.hpp>
#include <Math/Quaternion.h>
#include <Math/Vector2.h>
#include <string>

class SceneObject;
class CameraBase
{
public:
	CameraBase();
	virtual ~CameraBase();

	virtual MatrixF GetModel() = 0;

	virtual void Translate(double deltax, double deltay, double deltaz) = 0;
	virtual void Translate(Vec2D delta) = 0;
	virtual void Translate(Vec3D delta) = 0;
	virtual void Rotate(double rotx, double roty, double rotz) = 0;
	virtual void Rotate(double rotation) = 0;
	virtual void Rotate(Vec3D euler) = 0;
	virtual void Rotate(Quaternion quat) = 0;
};

