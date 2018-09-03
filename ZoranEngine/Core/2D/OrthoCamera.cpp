#include "stdafx.h"
#include "OrthoCamera.h"
#include "Core/SceneObject.h"

OrthoCamera::OrthoCamera(std::string name, float width, float height, float rotation) : Camera2D(name,Vec2D(0,0), Vec2D(1,1), 0)
{
	cameraExtents.x = width;
	cameraExtents.y = height;

	halfCameraExtents = cameraExtents / 2.0f;

	cameraModelCache = orthoModel * cameraModelCache;
}

OrthoCamera::~OrthoCamera()
{
}

void OrthoCamera::ScreenResized(float screenWidth, float screenHeight)
{
	float aspect = screenHeight / screenWidth;
	orthoModel = Matrix44::OrthoMatrix(-halfCameraExtents.x, -halfCameraExtents.y * aspect, halfCameraExtents.x, halfCameraExtents.y * aspect, -1, 10);
	cameraModelCache = orthoModel * cameraModelCache;
}

void OrthoCamera::ScreenResized(Vec2D Size)
{
	float aspect = Size.y / Size.x;
	orthoModel = Matrix44::OrthoMatrix(-halfCameraExtents.x, -halfCameraExtents.y*aspect, halfCameraExtents.x, halfCameraExtents.y*aspect, -1, 10);
	cameraModelCache = orthoModel * cameraModelCache;
}

Vec2D OrthoCamera::GetCameraViewingExtentsAtZ(float z)
{
	return Vec2D(halfCameraExtents.x, halfCameraExtents.y * screenSize.y / screenSize.x);
}
