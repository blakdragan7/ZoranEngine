#pragma once
#include <Math/Matrix44.h>
#include <Math/Quaternion.h>
#include <Math/Vector2.h>
#include <string>

#include <Core/CameraBase.h>

class SceneObject2D;
class ZoranEngine_EXPORT Camera2D : public CameraBase
{
protected:
	SceneObject2D * sceneObject;

public:
	Camera2D(std::string name,Vec2D position,Vec2D scale,float rotation);
	virtual ~Camera2D();

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

