#include "stdafx.h"
#include <Core/ThreadBase.h>
#include <Core/ZoranEngine.h>
#include <Core/3D/SceneObject3D.h>
#include <Physics/PhysicsEngine.h>
#include <Physics/PhysicsObjectBase.h>
#include <Rendering/RenderEngineBase.h>
#include <Rendering/ShaderProgramBase.h>
#include <Rendering/RenderedObjectBase.h>
#include <Physics/3D/PhysicsObject3DBase.h>
#include <Physics/Collision/CollisionBucketBase.h>
#include <Physics/Collision/CollisionObjectBase.h>
#include <Physics/3D/Collision/AABBoxCollisionObject.h>
#include <Physics/3D/Collision/SphereCollisionObject.h>

static unsigned long long NextID = 0;

SceneObject3D::SceneObject3D(std::string name) : SceneObject(name)
{
	scale = Vector3D(1.0, 1.0, 1.0);
}

SceneObject3D::SceneObject3D(std::string name, RenderEngineBase* engine) : SceneObject(name, engine)
{
	scale = Vector3D(1.0, 1.0, 1.0);
}

SceneObject3D::~SceneObject3D()
{
	if (collision)delete collision;
	if (physicsObject)delete physicsObject;
}


void SceneObject3D::PostRender()
{
	// profit ?
}

void SceneObject3D::RenderScene()
{
	renderedObject->RenderObject();
}

void SceneObject3D::PreRender()
{
	if (shaderProgram)
	{
		shaderProgram->BindProgram();
		shaderProgram->SetupShaderFromSceneObject(this);
	}
}

void SceneObject3D::Destroy()
{
	if (collision)pEngine->RemoveObject(collision);
	if (physicsObject)pEngine->RemoveObject(physicsObject);
	zEngine->RemoveTickableObject(this);
	zEngine->DestroySceneObject(this);
}

void SceneObject3D::SetRotation(Vector3D eulor)
{
	WaitForMutex();
	rotation = Quaternion::FromEuler(eulor);
	UnlockMutex();
}

void SceneObject3D::SetRotationFromAxis(Vector3D axis)
{
	WaitForMutex();
	rotation = Quaternion::FromScaledAxis(axis);
	UnlockMutex();
}

void SceneObject3D::SetPosition(Vector3D pos)
{
	WaitForMutex();
	this->pos = pos;
	if (collision)pEngine->UpdateCollisionObject(collision);
	UnlockMutex();
}

void SceneObject3D::SetPosition(float x, float y, float z)
{
	WaitForMutex();

	this->pos.x = x;
	this->pos.y = y;
	this->pos.z = z;

	if (collision)pEngine->UpdateCollisionObject(collision);
	UnlockMutex();
}

void SceneObject3D::SetScale(Vector3D scale)
{
	WaitForMutex();
	scale = scale;
	if (collision)pEngine->UpdateCollisionObject(collision);
	UnlockMutex();
}

void SceneObject3D::SetScale(float x, float y, float z)
{
	WaitForMutex();
	scale.x = x;
	scale.y = y;
	scale.z = z;
	if (collision)pEngine->UpdateCollisionObject(collision);
	UnlockMutex();
}

Vector3D SceneObject3D::GetPosition()
{
	return pos;
}

Vector3D SceneObject3D::GetScale()
{
	return scale;
}

Vector3D SceneObject3D::GetRotationAsEulor()
{
	return rotation.AsEuler();
}

inline void SceneObject3D::SetRotation(Quaternion quat)
{
	WaitForMutex();
	rotation = quat;
	UnlockMutex();
}

void SceneObject3D::RotateByScaledAxis(Vector3D axis)
{
	WaitForMutex();
	rotation = Quaternion::FromScaledAxis(axis) * rotation;
	UnlockMutex();
}

void SceneObject3D::RotateByQuat(Quaternion quat)
{
	WaitForMutex();
	rotation = quat * rotation;
	UnlockMutex();
}

void SceneObject3D::RotateByEulor(Vector3D eulor)
{
	WaitForMutex();
	rotation = Quaternion::FromEuler(eulor) * rotation;
	UnlockMutex();
}

void SceneObject3D::Translate(Vector3D delta)
{
	WaitForMutex();
	pos += delta;
	if (collision)pEngine->UpdateCollisionObject(collision);
	UnlockMutex();
}

void SceneObject3D::Scale(Vector3D scale)
{
	WaitForMutex();
	this->scale *= scale;
	if (collision)pEngine->UpdateCollisionObject(collision);
	UnlockMutex();
}

void SceneObject3D::PreCaclModel()
{
	model.makeIdentity();

	WaitForMutex();
	model.translate(pos);
	model.scale(scale);
	ModelMatrixCache = model * rotation.AsRotationMatrix();
	UnlockMutex();
}

/*MatrixF SceneObject3D::GetScaleMatrix3x3()
{
	MatrixF mat(3, 3);
	mat.scale(scale);
	return mat;
}*/

Matrix44 SceneObject3D::GetScaleMatrix4x4()
{
	Matrix44 mat;
	mat.scale(scale);
	return mat;
}

float SceneObject3D::DistanceTo(Vector3D pos)
{
	return this->pos.distance(pos);
}

float SceneObject3D::DistanceTo(SceneObject3D * other)
{
	return this->pos.distance(other->pos);
}
