#pragma once
#include <Math/Matrix44.hpp>
#include <Math/Quaternion.h>
#include <Math/Vector2.h>
#include <string>
#include <Core/CameraBase.h>

class SceneObject3D;
class ZoranEngine_EXPORT Camera3D : public CameraBase
{
private:
	SceneObject3D * sceneObject;

public:
	Camera3D(std::string name,Vec3D position,Vec3D scale,Vec3D rotation);
	virtual ~Camera3D();

	virtual void Translate(float deltax, float deltay, float deltaz)override;
	virtual void Translate(Vec2D delta)override;
	virtual void Translate(Vec3D delta)override;
	virtual void Rotate(float rotx, float roty, float rotz)override;
	virtual void Rotate(float rotation) override;
	virtual void Rotate(Vec3D euler) override;
	virtual void Rotate(Quat quat)override;
	virtual void Scale(Vec3D scale)override;
	virtual void Scale(Vec2D scale)override;
};

