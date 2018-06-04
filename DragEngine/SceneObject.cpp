#include "stdafx.h"
#include "SceneObject.h"
#include "ThreadBase.h"
#include "DragEngine.h"
#include "PhysicsEngine.h"
#include "RenderEngineBase.h"
#include "ShaderProgramBase.h"
#include "PhysicsObjectBase.h"
#include "RenderedObjectBase.h"
#include "CollisionBucketBase.h"
#include "CollisionObjectBase.h"
#include "BoxCollisionObject.h"
#include "SphereCollisionObject.h"

void SceneObject::WaitForMutex()
{
	while (mutex.try_lock() == false)ThreadBase::Sleep(0.01F);
}

void SceneObject::UnlockMutex()
{
	mutex.unlock();
}

void SceneObject::SetShaderProgram(ShaderProgramBase * newShaderProgram)
{
	shaderProgram = newShaderProgram;
}

void SceneObject::SetRenderedObject(RenderedObjectBase * newRenderedObject)
{
	renderedObject = newRenderedObject;
}

SceneObject::SceneObject()
{
	renderEngine = dEngine->GetRenderer();
	renderedObject = renderEngine->CreateRenderedObject();
	scale = Vector3D(1.0,1.0,1.0);

	physicsObject = new PhysicsObjectBase(this);
	collision = 0;  
	hasCollision = true;
}

SceneObject::SceneObject(RenderEngineBase* engine)
{
	scale = Vector3D(1.0, 1.0, 1.0);

	collision = 0;  
	hasCollision = true; 
	renderEngine = engine;
	physicsObject = new PhysicsObjectBase(this);
}

SceneObject::~SceneObject()
{
	if (collision)delete collision;
	if (physicsObject)delete physicsObject;
}


void SceneObject::PostRender()
{
	// profit ?
}

void SceneObject::RenderScene()
{
	renderedObject->RenderObject();
}

void SceneObject::PreRender()
{
	if (shaderProgram)
	{
		shaderProgram->BindProgram();
		shaderProgram->SetupShaderFromSceneObject(this);
	}
}

void SceneObject::Destroy()
{
	if(collision)pEngine->RemoveObject(collision);
	if(physicsObject)pEngine->RemoveObject(physicsObject);
	dEngine->RemoveTickableObject(this);
	dEngine->DestroySceneObject(this);
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

void SceneObject::TrySetPosition(Vector3D pos)
{
	WaitForMutex();
	Vec3D old_pos = pos;
	this->pos = pos;

	CollisionResponse response;
	if (pEngine->CheckCollisionForObject(this, response))
	{
		if (response.collided)
		{
			if (collision->GetCollisionType() == BOX_COLLISION)
			{
				BoxCollisionObject* bcollision = (BoxCollisionObject*)collision;
				Vec3D size = bcollision->GetSize();
				Vec3D penetration = response.normal * (size - response.intersection.getAbs());
				this->pos += penetration;
				physicsObject->CollidedWith(response.point, response.normal, response.collidedObjects[1]);
			}
			else if (collision->GetCollisionType() == SPHERE_COLLISION)
			{
				SphereCollisionObject* scollision = (SphereCollisionObject*)collision;
				physicsObject->CollidedWith(response.point, response.normal, response.collidedObjects[1]);
				Vec3D penetration = response.normal * (scollision->GetRadiusSqr() - response.intersection.getMagnitudeSqr());
				this->pos += penetration;
			}
		}
	}
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

void SceneObject::SetScale(Vector3D scale)
{
	WaitForMutex();
	scale = scale;
	if (collision)
	{
		pEngine->RemoveObject(collision);
		pEngine->AddCollisionObject(collision);
	}
	UnlockMutex();
}

void SceneObject::SetScale(double x, double y, double z)
{
	WaitForMutex();
	scale.x = x;
	scale.y = y;
	scale.z = z;
	if (collision)
	{
		pEngine->RemoveObject(collision);
		pEngine->AddCollisionObject(collision);
	}
	UnlockMutex();
}

Vector3D SceneObject::GetPosition()
{
	return pos;
}

Vector3D SceneObject::GetScale()
{
	return scale;
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

void SceneObject::Scale(Vector3D scale)
{
	WaitForMutex();
	this->scale *= scale;
	UnlockMutex();
}

MatrixF SceneObject::GetModel()
{
	MatrixF model = MatrixF::GLIdentityMatrix();

	WaitForMutex();
	model.translate(pos);
	model.scale(scale);
	model = rotation.AsMatrix()*model;
	UnlockMutex();
	return model;
}

MatrixF SceneObject::GetScaleMatrix3x3()
{
	MatrixF mat(3, 3);
	mat.scale(scale);
	return mat;
}

MatrixF SceneObject::GetScaleMatrix4x4()
{
	MatrixF mat(4,4);
	mat.scale(scale);
	return mat;
}

double SceneObject::DistanceTo(Vector3D pos)
{
	return this->pos.distance(pos);
}

double SceneObject::DistanceTo(SceneObject * other)
{
	return this->pos.distance(other->pos);
}
