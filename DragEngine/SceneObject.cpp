#include "stdafx.h"
#include "SceneObject.h"

void SceneObject::SetRotation(Vector3D eulor)
{
	rotation = Quaternion::FromEuler(eulor);
}

void SceneObject::SetRotationFromAxis(Vector3D axis)
{
	rotation = Quaternion::FromScaledAxis(axis);
}

void SceneObject::SetPosition(Vector3D pos)
{
	this->pos = pos;
}

void SceneObject::SetPosition(double x, double y, double z)
{
	this->pos = Vector3D(x,y,z);
}

Vector3D SceneObject::GetRotationAsEulor()
{
	return rotation.AsEuler();
}

void SceneObject::RotateByScaledAxis(Vector3D axis)
{
	rotation = Quaternion::FromScaledAxis(axis) * rotation;
}

void SceneObject::RotateByQuat(Quaternion quat)
{
	rotation = quat * rotation;
}

void SceneObject::RotateByEulor(Vector3D eulor)
{
	rotation = Quaternion::FromEuler(eulor) * rotation;
}

void SceneObject::Translate(Vector3D delta)
{
	pos += delta;
}

Mat4D SceneObject::GetModel()
{
	Mat4D model = Mat4D::GLIdentityMatrix();
	model.translate(pos);
	model = rotation.AsMatrix()*model;
	return model;
}
