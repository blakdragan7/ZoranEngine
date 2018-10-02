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
	orthoModel = Matrix44::OrthoMatrix(-halfCameraExtents.x, -halfCameraExtents.y * aspect, halfCameraExtents.x, halfCameraExtents.y * aspect, -1, 10);
	CalculateModelCache();
}

void OrthoCamera::ScreenResized(Vec2D Size)
{
	float aspect = Size.y / Size.x;
	orthoModel = Matrix44::OrthoMatrix(-halfCameraExtents.x, -halfCameraExtents.y*aspect, halfCameraExtents.x, halfCameraExtents.y*aspect, -1, 10);
	CalculateModelCache();
}

Vector2D OrthoCamera::GetCameraViewingExtentsAtZ(float z)
{
	return { halfCameraExtents.x, halfCameraExtents.y * screenSize.y / screenSize.x };
	CalculateModelCache();
}

void OrthoCamera::CalculateModelCache()
{
	cameraModelCache = orthoModel * cameraModelCache;
}
