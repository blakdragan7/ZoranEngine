#include "stdafx.h"
#include "CameraBase.h"
#include "SceneObject.h"

CameraBase::CameraBase(std::string name, Vec3D position, Vec3D scale, Vec3D rotation)
{
	sceneObject = new SceneObject(name);
	sceneObject->SetPosition(position);
	sceneObject->SetScale(scale);
	sceneObject->SetRotation(rotation);
}


CameraBase::~CameraBase()
{
}

void CameraBase::Translate(double deltax, double deltay, double deltaz)
{
	sceneObject->Translate(Vec3D(deltax,deltay,deltaz));
}

void CameraBase::Translate(Vec3D delta)
{
	sceneObject->Translate(delta);
}

void CameraBase::Rotate(double rotx, double roty, double rotz)
{
	sceneObject->RotateByEulor(Vec3D(rotx,roty,rotz));
}

void CameraBase::Rotate(Vec3D euler)
{
	sceneObject->RotateByEulor(euler);
}

void CameraBase::Rotate(Quaternion quat)
{
	sceneObject->RotateByQuat(quat);
}
