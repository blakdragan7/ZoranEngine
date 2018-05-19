#include "stdafx.h"
#include "SceneObject.h"
#include "ThreadBase.h"
#include "DragEngine.h"

void SceneObject::WaitForMutex()
{
	while (mutex.try_lock() == false)ThreadBase::Sleep(0.01F);
}

void SceneObject::UnlockMutex()
{
	mutex.unlock();
}

SceneObject::SceneObject()
{
	renderEngine = dEngine->GetRenderer();
}

void SceneObject::SetRotation(Vector3D eulor)
{
	WaitForMutex();
	rotation = Quaternion::FromEuler(eulor);
}

void SceneObject::SetRotationFromAxis(Vector3D axis)
{
	WaitForMutex();
	rotation = Quaternion::FromScaledAxis(axis);
	UnlockMutex();
}

void SceneObject::SetPosition(Vector3D pos)
{
	WaitForMutex();
	this->pos = pos;
	UnlockMutex();
}

void SceneObject::SetPosition(double x, double y, double z)
{
	WaitForMutex();
	this->pos = Vector3D(x,y,z);
	UnlockMutex();
}

Vector3D SceneObject::GetRotationAsEulor()
{
	return rotation.AsEuler();
}

inline void SceneObject::SetRotation(Quaternion quat)
{
	WaitForMutex();
	rotation = quat;
	UnlockMutex();
}

void SceneObject::RotateByScaledAxis(Vector3D axis)
{
	WaitForMutex();
	rotation = Quaternion::FromScaledAxis(axis) * rotation;
	UnlockMutex();
}

void SceneObject::RotateByQuat(Quaternion quat)
{
	WaitForMutex();
	rotation = quat * rotation;
	UnlockMutex();
}

void SceneObject::RotateByEulor(Vector3D eulor)
{
	WaitForMutex();
	rotation = Quaternion::FromEuler(eulor) * rotation;
	UnlockMutex();
}

void SceneObject::Translate(Vector3D delta)
{
	WaitForMutex();
	pos += delta;
	UnlockMutex();
}

MatrixD SceneObject::GetModel()
{
	MatrixD model = MatrixD::GLIdentityMatrix();
	WaitForMutex();
	model.translate(pos);
	model = rotation.AsMatrix()*model;
	UnlockMutex();
	return model;
}
