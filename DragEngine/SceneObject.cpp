#include "stdafx.h"
#include "SceneObject.h"
#include "ThreadBase.h"
#include "DragEngine.h"
#include "PhysicsEngine.h"
#include "RenderEngineBase.h"
#include "RenderedObjectBase.h"
#include "ShaderProgramBase.h"


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

	physicsObject = 0;
	collision = 0;  hasCollision = false;
}

SceneObject::SceneObject(RenderEngineBase* engine)
{
	collision = 0;  
	physicsObject = 0;
	hasCollision = false; 
	renderEngine = engine;
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
	UnlockMutex();
}

void SceneObject::SetScale(double x, double y, double z)
{
	WaitForMutex();
	scale.x = x;
	scale.y = y;
	scale.z = z;
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
