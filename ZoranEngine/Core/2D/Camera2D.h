#pragma once
#include <Math/Matrix.hpp>
#include <Math/Quaternion.h>
#include <Math/Vector2.h>
#include <string>

#include <Core/CameraBase.h>

class SceneObject2D;
class Camera2D : public CameraBase
{
private:
	SceneObject2D * sceneObject;

public:
	Camera2D(std::string name,Vec2D position,Vec2D scale,double rotation);
	virtual ~Camera2D();

	virtual MatrixF GetModel()override;

	virtual void Translate(double deltax, double deltay, double deltaz)override;
	virtual void Translate(Vec2D delta)override;
	virtual void Translate(Vec3D delta)override;
	virtual void Rotate(double rotx, double roty, double rotz)override;
	virtual void Rotate(double rotation) override;
	virtual void Rotate(Vec3D euler) override;
	virtual void Rotate(Quaternion quat)override;
};

