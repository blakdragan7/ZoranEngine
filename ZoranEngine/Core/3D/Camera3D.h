#pragma once
#include <Math/Matrix.hpp>
#include <Math/Quaternion.h>
#include <Math/Vector2.h>
#include <string>
#include <Core/CameraBase.h>

class SceneObject3D;
class ZoranEngine_EXPORT Camera3D : CameraBase
{
private:
	SceneObject3D * sceneObject;

public:
	Camera3D(std::string name,Vec3D position,Vec3D scale,Vec3D rotation);
	virtual ~Camera3D();

	virtual MatrixF GetModel()override;

	virtual void Translate(double deltax, double deltay, double deltaz)override;
	virtual void Translate(Vec2D delta)override;
	virtual void Translate(Vec3D delta)override;
	virtual void Rotate(double rotx, double roty, double rotz)override;
	virtual void Rotate(double rotation) override;
	virtual void Rotate(Vec3D euler) override;
	virtual void Rotate(Quaternion quat)override;
};

