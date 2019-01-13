#include "stdafx.h"
#include "OrthoCamera.h"
#include "Core/SceneObject.h"

OrthoCamera::OrthoCamera(std::string name, float width, float height, float rotation) : Camera2D(name, { 0,0 }, { 1, 1 }, 0)
{
	cameraExtents.x = width;
	cameraExtents.y = height;

	halfCameraExtents = cameraExtents / 2.0f;
	CalculateModelCache();
}

OrthoCamera::~OrthoCamera()
{
}

void OrthoCamera::ScreenResized(float screenWidth, float screenHeight)
{
	float aspect = screenHeight / screenWidth;
	orthoModel = Matrix44::OrthoMatrix(-halfCameraExtents.x, halfCameraExtents.x, -halfCameraExtents.y * aspect, halfCameraExtents.y * aspect, -100, 100);
	CalculateModelCache();
}

void OrthoCamera::ScreenResized(Vec2D Size)
{
	float aspect = Size.y / Size.x;
	orthoModel = Matrix44::OrthoMatrix(-halfCameraExtents.x, halfCameraExtents.x, -halfCameraExtents.y * aspect, halfCameraExtents.y * aspect, -100, 10);
	CalculateModelCache();
}

Vector2D OrthoCamera::GetCameraViewingExtentsAtZ(float z)
{
	return { halfCameraExtents.x, halfCameraExtents.y * screenSize.y / screenSize.x };
}

void OrthoCamera::CalculateModelCache()
{
	cameraModelCache = orthoModel * cameraModelCache;
}
