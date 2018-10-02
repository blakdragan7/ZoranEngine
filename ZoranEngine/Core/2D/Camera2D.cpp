#include "stdafx.h"
#include "Camera2D.h"
#include <Core/2D/SceneObject2D.h>
#include <Core/2D/Named2DSceneObject.h>

Camera2D::Camera2D(std::string name, Vec2D position, Vec2D scale, float rotation)
{
	sceneObject = new Named2DSceneObject(name);
	sceneObject->SetPosition(position);
	sceneObject->SetScale(scale);
	sceneObject->SetRotation(rotation);
	cameraModelCache = sceneObject->GetModel();
}

Camera2D::~Camera2D()
{
	delete sceneObject;
}

void Camera2D::Translate(float deltax, float deltay, float deltaz)
{
	sceneObject->Translate({ deltax,deltay });
	cameraModelCache = sceneObject->GetModel();
	CalculateModelCache();
}

void Camera2D::Translate(Vec2D delta)
{
	sceneObject->Translate(delta);
	cameraModelCache = sceneObject->GetModel();
	CalculateModelCache();
}

void Camera2D::Translate(Vec3D delta)
{
	sceneObject->Translate(delta.GetXY());
	cameraModelCache = sceneObject->GetModel();
	CalculateModelCache();
}

void Camera2D::Rotate(float rotx, float roty, float rotz)
{
	sceneObject->Rotate(rotx);
	cameraModelCache = sceneObject->GetModel();
	CalculateModelCache();
}

void Camera2D::Rotate(float rotation)
{
	sceneObject->Rotate(rotation);
	cameraModelCache = sceneObject->GetModel();
	CalculateModelCache();
}

void Camera2D::Rotate(Quat quat)
{
	sceneObject->Rotate(quat.AsEuler().x);
	cameraModelCache = sceneObject->GetModel();
	CalculateModelCache();
}

void Camera2D::Rotate(Vec3D euler)
{
	sceneObject->Rotate(euler.x);
	cameraModelCache = sceneObject->GetModel();
	CalculateModelCache();
}

void Camera2D::Scale(Vec3D scale)
{
	sceneObject->Scale(scale);
	cameraModelCache = sceneObject->GetModel();
	CalculateModelCache();
}

void Camera2D::Scale(Vec2D scale)
{
	sceneObject->Scale(scale);
	cameraModelCache = sceneObject->GetModel();
	CalculateModelCache();
}
