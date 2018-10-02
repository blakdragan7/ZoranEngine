#include "stdafx.h"
#include "Camera3D.h"
#include "Named3DSceneObject.h"

Camera3D::Camera3D(std::string name, Vec3D position, Vec3D scale, Vec3D rotation)
{
	sceneObject = new Named3DSceneObject(name);
	sceneObject->SetPosition(position);
	sceneObject->SetScale(scale);
	sceneObject->SetRotation(rotation);

	cameraModelCache = sceneObject->GetModel();
}

Camera3D::~Camera3D()
{
	delete sceneObject;
}

void Camera3D::Translate(float deltax, float deltay, float deltaz)
{
	sceneObject->Translate({ deltax,deltay,deltaz });
	cameraModelCache = sceneObject->GetModel();
	CalculateModelCache();
}

void Camera3D::Translate(Vec2D delta)
{
	sceneObject->Translate(delta);
	cameraModelCache = sceneObject->GetModel();
	CalculateModelCache();
}

void Camera3D::Translate(Vec3D delta)
{
	sceneObject->Translate(delta);
	cameraModelCache = sceneObject->GetModel();
	CalculateModelCache();
}

void Camera3D::Rotate(float rotx, float roty, float rotz)
{
	sceneObject->RotateByEulor({ rotx,roty,rotz });
	cameraModelCache = sceneObject->GetModel();
	CalculateModelCache();
}

void Camera3D::Rotate(float rotation)
{
	sceneObject->RotateByEulor({ rotation,0,0 });
	cameraModelCache = sceneObject->GetModel();
	CalculateModelCache();
}

void Camera3D::Rotate(Vec3D euler)
{
	sceneObject->RotateByEulor(euler);
	cameraModelCache = sceneObject->GetModel();
	CalculateModelCache();
}

void Camera3D::Rotate(Quat quat)
{
	sceneObject->RotateByQuat(quat);
	cameraModelCache = sceneObject->GetModel();
	CalculateModelCache();
}

void Camera3D::Scale(Vec3D scale)
{
	sceneObject->Scale(scale);
	cameraModelCache = sceneObject->GetModel();
	CalculateModelCache();
}

void Camera3D::Scale(Vec2D scale)
{
	sceneObject->Scale(scale);
	cameraModelCache = sceneObject->GetModel();
	CalculateModelCache();
}
