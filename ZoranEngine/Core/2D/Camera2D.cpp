#include "stdafx.h"
#include "Camera2D.h"
#include <Core/2D/SceneObject2D.h>

Camera2D::Camera2D(std::string name, Vec2D position, Vec2D scale, float rotation)
{
	sceneObject = new SceneObject2D(name);
	sceneObject->SetPosition(position);
	sceneObject->SetScale(scale);
	sceneObject->SetRotation(rotation);
}

Camera2D::~Camera2D()
{
	delete sceneObject;
}

MatrixF Camera2D::GetModel()
{
	return sceneObject->GetModel();
}

void Camera2D::Translate(float deltax, float deltay, float deltaz)
{
	sceneObject->Translate(Vec2D(deltax,deltay));
}

void Camera2D::Translate(Vec2D delta)
{
	sceneObject->Translate(delta);
}

void Camera2D::Translate(Vec3D delta)
{
	sceneObject->Translate(delta.GetXY());
}

void Camera2D::Rotate(float rotx, float roty, float rotz)
{
	sceneObject->Rotate(rotx);
}

void Camera2D::Rotate(float rotation)
{
	sceneObject->Rotate(rotation);
}

void Camera2D::Rotate(Vec3D euler)
{
	sceneObject->Rotate(euler.x);
}

void Camera2D::Rotate(Quaternion quat)
{
	sceneObject->Rotate(quat.AsEuler().x);
}
