#pragma once
#include <Core\3D\Camera3D.h>

class ZoranEngine_EXPORT PerspectiveCamera : public Camera3D
{
private:
	Matrix44 PerspectiveModel;
	float fov;
	float nearp;
	float farp;

public:
	PerspectiveCamera(std::string name, float fov, float ratio, float nearp, float farp);
	~PerspectiveCamera();

	virtual void ScreenResized(float screenWidth, float screenHeight)override;
	virtual void ScreenResized(Vec2D Size)override;
	virtual Vec2D GetCameraViewingExtentsAtZ(float z)override; // ignores Z because Ortho isn't effected by it

	virtual void CalculateModelCache()override;
};

