#include "stdafx.h"
#include "SceneObject3D.h"
#include "Core/ThreadBase.h"
#include "Core/ZoranEngine.h"
#include "Physics/PhysicsEngine.h"
#include "Rendering/RenderEngineBase.h"
#include "Rendering/ShaderProgramBase.h"
#include "Physics/PhysicsObjectBase.h"
#include "Rendering/RenderedObjectBase.h"
#include "Physics/Collision/CollisionBucketBase.h"
#include "Physics/Collision/CollisionObjectBase.h"
#include "Physics/Collision/3D/BoxCollisionObject.h"
#include "Physics/Collision/3D/SphereCollisionObject.h"

static unsigned long long NextID = 0;

SceneObject3D::SceneObject3D(std::string name) : SceneObject(name)
{
	model = MatrixF::GLIdentityMatrix();

	scale = Vector3D(1.0, 1.0, 1.0);
}

SceneObject3D::SceneObject3D(std::string name, RenderEngineBase* engine) : SceneObject(name, engine)
{
	model = MatrixF::GLIdentityMatrix();
	scale = Vector3D(1.0, 1.0, 1.0);
}

SceneObject3D::~SceneObject3D()
{
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

void SceneObject3D::SetPosition(double x, double y, double z)
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

void SceneObject3D::SetScale(double x, double y, double z)
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

MatrixF SceneObject3D::GetModel()
{
	model.makeIdentity();

	WaitForMutex();
	model.translate(pos);
	model.scale(scale);
	model = model * rotation.AsRotationMatrix();
	UnlockMutex();
	return model;
}

MatrixF SceneObject3D::GetScaleMatrix3x3()
{
	MatrixF mat(3, 3);
	mat.scale(scale);
	return mat;
}

MatrixF SceneObject3D::GetScaleMatrix4x4()
{
	MatrixF mat(4, 4);
	mat.scale(scale);
	return mat;
}

double SceneObject3D::DistanceTo(Vector3D pos)
{
	return this->pos.distance(pos);
}

double SceneObject3D::DistanceTo(SceneObject3D * other)
{
	return this->pos.distance(other->pos);
}
