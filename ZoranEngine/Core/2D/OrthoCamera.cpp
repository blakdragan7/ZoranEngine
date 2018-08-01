#include "stdafx.h"
#include "OrthoCamera.h"
#include "Core/SceneObject.h"

OrthoCamera::OrthoCamera(std::string name, float width, float height, float rotation) : Camera2D(name,Vec2D(0,0), Vec2D(1,1), 0)
{
	cameraExtents.x = width;
	cameraExtents.y = height;

	halfCameraExtents = cameraExtents / 2.0f;

	orthoModel = MatrixF::GLIdentityMatrix();
}

OrthoCamera::~OrthoCamera()
{
}

MatrixF OrthoCamera::GetModel()
{
	MatrixF sceneModel = Camera2D::GetModel();

	return orthoModel * sceneModel;
}

void OrthoCamera::ScreenResized(float screenWidth, float screenHeight)
{
	float aspect = screenHeight / screenWidth;
	orthoModel = MatrixF::GLOrthoMatrix(-halfCameraExtents.x, -halfCameraExtents.y * aspect, halfCameraExtents.x, halfCameraExtents.y * aspect, -1, 10);
}

void OrthoCamera::ScreenResized(Vec2D Size)
{
	float aspect = Size.y / Size.x;
	orthoModel = MatrixF::GLOrthoMatrix(-halfCameraExtents.x, -halfCameraExtents.y*aspect, halfCameraExtents.x, halfCameraExtents.y*aspect, -1, 10);
}