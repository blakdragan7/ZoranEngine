#pragma once
#include <Math/Matrix.hpp>
#include <Math/Quaternion.h>
#include <Math/Vector2.h>
#include <string>

class SceneObject;
class ZoranEngine_EXPORT CameraBase
{
protected:
	Vec2D screenSize;
	Vec2D cameraExtents;
	Vec2D halfCameraExtents;

public:
	CameraBase();
	virtual ~CameraBase();

	virtual MatrixF GetModel() = 0;

	virtual void Translate(float deltax, float deltay, float deltaz) = 0;
	virtual void Translate(Vec2D delta) = 0;
	virtual void Translate(Vec3D delta) = 0;
	virtual void Rotate(float rotx, float roty, float rotz) = 0;
	virtual void Rotate(float rotation) = 0;
	virtual void Rotate(Vec3D euler) = 0;
	virtual void Rotate(Quaternion quat) = 0;

	virtual Vec2D GetCameraViewingExtentsAtZ(float z) = 0;
	virtual void ScreenResized(float screenWidth, float screenHeight) = 0;
	virtual void ScreenResized(Vec2D Size) = 0;

	
};

