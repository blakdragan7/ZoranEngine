#include "stdafx.h"
#include <Core/ZoranEngine.h>
#include <Core/3D/SceneObject3D.h>

#include <Core/3D/Components/Component3DBase.h>

static unsigned long long NextID = 0;

SceneObject3D::SceneObject3D(Component3DBase* rootComponent, std::string name) : root3DComponent(rootComponent), SceneObject(rootComponent,name)
{
}

SceneObject3D::SceneObject3D(std::string name) : root3DComponent(0), SceneObject(name)
{
}

SceneObject3D::~SceneObject3D()
{
}

void SceneObject3D::Destroy()
{
	zEngine->DestroySceneObject(this);
}

void SceneObject3D::SetRotation(const Vector3D& euler)
{
	WaitForMutex();
	root3DComponent->SetRotationFromEuler(euler);
	UnlockMutex();
}

void SceneObject3D::SetRotationFromAxis(const Vector3D& axis)
{
	WaitForMutex();
	root3DComponent->SetRotationFromScaleAxis(axis);
	UnlockMutex();
}

void SceneObject3D::SetPosition(const Vector3D& pos)
{
	WaitForMutex();
	root3DComponent->SetOffset(pos);
	UnlockMutex();
}

void SceneObject3D::SetPosition(float x, float y, float z)
{
	WaitForMutex();

	root3DComponent->SetOffset(x, y, z);

	UnlockMutex();
}

void SceneObject3D::SetScale(const Vector3D& scale)
{
	WaitForMutex();
	root3DComponent->SetScale(scale);
	UnlockMutex();
}

void SceneObject3D::SetScale(float x, float y, float z)
{
	WaitForMutex();
	root3DComponent->SetScale(x, y, z);
	UnlockMutex();
}

void SceneObject3D::SetSize(const Vector3D & size)
{
	root3DComponent->SetSize(size);
}

void SceneObject3D::SetSize(float x, float y, float z)
{
	root3DComponent->SetSize(x, y, z);
}

inline void SceneObject3D::SetRotation(const Quaternion& quat)
{
	WaitForMutex();
	root3DComponent->SetRotation(quat);
	UnlockMutex();
}

void SceneObject3D::RotateByScaledAxis(const Vector3D& axis)
{
	WaitForMutex();
	root3DComponent->RotateByScaledAxis(axis);
	UnlockMutex();
}

void SceneObject3D::RotateByQuat(const Quaternion& quat)
{
	WaitForMutex();
	root3DComponent->Rotate(quat);
	UnlockMutex();
}

void SceneObject3D::RotateByEulor(const Vector3D& euler)
{
	WaitForMutex();
	root3DComponent->Rotate(euler);
	UnlockMutex();
}

void SceneObject3D::Translate(const Vector3D& delta)
{
	WaitForMutex();
	root3DComponent->Translate(delta);
	UnlockMutex();
}

void SceneObject3D::Scale(const Vector3D& scale)
{
	WaitForMutex();
	root3DComponent->Scale(scale);
	UnlockMutex();
}

void SceneObject3D::PreCaclModel()
{
	WaitForMutex();

	ModelMatrixCache = root3DComponent->GetWorldMatrix();

	UnlockMutex();
}

Matrix44 SceneObject3D::GetScaleMatrix4x4()
{
	return Matrix44::ScaleMatrix(root3DComponent->GetScale());
}

float SceneObject3D::DistanceTo(const Vector3D& pos)
{
	return root3DComponent->GetOffset().distance(pos);
}

float SceneObject3D::DistanceTo(SceneObject3D * other)
{
	return root3DComponent->GetOffset().distance(other->GetPosition());
}
