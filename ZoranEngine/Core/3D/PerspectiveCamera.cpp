#include "stdafx.h"
#include "PerspectiveCamera.h"


PerspectiveCamera::PerspectiveCamera(std::string name, float fov, float ratio, float nearp, float farp) :
	fov(fov), nearp(nearp), farp(farp), Camera3D(name, {}, { 1,1,1 }, {})
{
	PerspectiveModel = Matrix44::ProjectionMatrix(fov, ratio, nearp, farp);
	CalculateModelCache();
}


PerspectiveCamera::~PerspectiveCamera()
{
}

void PerspectiveCamera::ScreenResized(float screenWidth, float screenHeight)
{
	PerspectiveModel = Matrix44::ProjectionMatrix(fov, screenWidth / screenHeight, nearp, farp);
	CalculateModelCache();
}

void PerspectiveCamera::ScreenResized(Vec2D Size)
{
	PerspectiveModel = Matrix44::ProjectionMatrix(fov, Size.x / Size.y, nearp, farp);
	CalculateModelCache();
}

Vector2D PerspectiveCamera::GetCameraViewingExtentsAtZ(float z)
{
	Log(LogLevel_Error, "PerspectiveCamera does not have GetCameraViewingExtentsAtZ implemented !! \n");
	return {};
}

void PerspectiveCamera::CalculateModelCache()
{
	cameraModelCache = PerspectiveModel * cameraModelCache;
}
