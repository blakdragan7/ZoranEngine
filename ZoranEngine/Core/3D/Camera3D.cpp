#include "stdafx.h"
#include "Camera3D.h"
#include "SceneObject3D.h"

Camera3D::Camera3D(std::string name, Vec3D position, Vec3D scale, Vec3D rotation)
{
	sceneObject = new SceneObject3D(name);
	sceneObject->SetPosition(position);
	sceneObject->SetScale(scale);
	sceneObject->SetRotation(rotation);
}

Camera3D::~Camera3D()
{
	delete sceneObject;
}

MatrixF Camera3D::GetModel()
{
	return sceneObject->GetModel();
}

void Camera3D::Translate(double deltax, double deltay, double deltaz)
{
	sceneObject->Translate(Vec3D(deltax,deltay,deltaz));
}

void Camera3D::Translate(Vec2D delta)
{
	sceneObject->Translate(delta);
}

void Camera3D::Translate(Vec3D delta)
{
	sceneObject->Translate(delta);
}

void Camera3D::Rotate(double rotx, double roty, double rotz)
{
	sceneObject->RotateByEulor(Vec3D(rotx,roty,rotz));
}

void Camera3D::Rotate(double rotation)
{
	sceneObject->RotateByEulor(Vec3D(rotation,0,0));
}

void Camera3D::Rotate(Vec3D euler)
{
	sceneObject->RotateByEulor(euler);
}

void Camera3D::Rotate(Quaternion quat)
{
	sceneObject->RotateByQuat(quat);
}
