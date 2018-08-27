#pragma once
#include "Core/2D/Camera2D.h"

class ZoranEngine_EXPORT OrthoCamera : public Camera2D
{
private:
	Matrix44 orthoModel;

public:
	OrthoCamera(std::string name,float width,float height,float rotation);
	~OrthoCamera();

	virtual void ScreenResized(float screenWidth, float screenHeight)override;
	virtual void ScreenResized(Vec2D Size)override;
	virtual Vec2D GetCameraViewingExtentsAtZ(float z)override; // ignores Z because Ortho isn't effected by it
};

