#include <stdafx.h>
#include "SceneObject2D.h"
#include <Core/ZoranEngine.h>
#include <Physics/PhysicsEngine.h>
#include <Physics/2D/PhysicsObject2DBase.h>
#include <Physics/2D/Collision/CollisionObject2DBase.h>

#define PI 3.141592653589793
SceneObject2D::SceneObject2D(std::string name) : SceneObject(name)
{
	model = MatrixF::GLIdentityMatrix();
	scale = Vector2D(1.0, 1.0);
	rotation = 0;
	collision = 0;

	rotationMat.SetRotation(0);
	invRotationMat.SetRotation(0);
}

SceneObject2D::SceneObject2D(std::string name, RenderEngineBase* engine) : SceneObject(name, engine)
{
	model = MatrixF::GLIdentityMatrix();
	scale = Vector2D(1.0, 1.0);
	rotation = 0;
	collision = 0;
}

SceneObject2D::~SceneObject2D()
{
	if (collision)delete collision;
	if (physicsObject)delete physicsObject;
}

void SceneObject2D::SetPosition(Vector2D pos)
{
	this->pos = pos;
	if (collision)pEngine->UpdateCollisionObject(collision);
}

void SceneObject2D::SetScale(Vector2D scale)
{
	this->scale = scale;
	this->size = (startingSize * scale).getAbs();
	if (collision)pEngine->UpdateCollisionObject(collision);
}

void SceneObject2D::SetPosition(float x, float y)
{
	this->pos.x = x;
	this->pos.y = y;
	if (collision)pEngine->UpdateCollisionObject(collision);
}

void SceneObject2D::SetScale(float x, float y)
{
	this->scale.x = x;
	this->scale.y = y;
	this->size = (startingSize * scale).getAbs();
	if (collision)pEngine->UpdateCollisionObject(collision);
}

void SceneObject2D::SetRotation(float rotation)
{
	this->rotation = rotation;
	rotationMat.SetRotation(rotation);
	invRotationMat = rotationMat.GetInversion();
	if (collision)pEngine->UpdateCollisionObject(collision);
}

void SceneObject2D::Translate(Vector2D delta)
{
	this->pos += delta;
	if (collision)pEngine->UpdateCollisionObject(collision);
}

void SceneObject2D::Translate(float x, float y)
{
	this->pos.x += x;
	this->pos.y += y;
	if (collision)pEngine->UpdateCollisionObject(collision);
}

void SceneObject2D::Scale(Vector2D scale)
{
	this->scale *= scale;
	this->size = (startingSize * scale).getAbs();
	if (collision)pEngine->UpdateCollisionObject(collision);
}

void SceneObject2D::Scale(float dx, float dy)
{
	this->scale.x *= dx;
	this->scale.y *= dy;
	this->size = (startingSize * scale).getAbs();
	if (collision)pEngine->UpdateCollisionObject(collision);
}

void SceneObject2D::Rotate(float rotation)
{
	rotation += rotation;
	rotationMat.SetRotation(rotation);
	invRotationMat = rotationMat.GetInversion();
	if (collision)pEngine->UpdateCollisionObject(collision);
}

float SceneObject2D::GetRotationRad()
{
	return rotation;
}

float SceneObject2D::GetRotation()
{
	return rotation;
}

Vector2D SceneObject2D::GetPosition()
{
	return pos;
}

Vector2D SceneObject2D::GetScale()
{
	return scale;
}

Vector2D SceneObject2D::GetSize()
{
	return size;
}

Matrix22 SceneObject2D::GetRotationMatrix()
{
	return rotationMat;
}

Matrix22 SceneObject2D::GetInvRotationMatrix()
{
	return invRotationMat;
}

MatrixF SceneObject2D::GetModel()
{
	model.makeIdentity();

	MatrixF rotMatrix = MatrixF::GLRotationMatrix(rotation, Vector3D(0, 0, 1));
	model.translate(pos);
	model.scale(scale);

	return model *rotMatrix;
}

MatrixF SceneObject2D::GetScaleMatrix3x3()
{
	MatrixF mat(3, 3);
	mat.scale(scale);
	return mat;
}

MatrixF SceneObject2D::GetScaleMatrix4x4()
{
	MatrixF mat(4, 4);
	mat.scale(scale);
	return mat;
}

void SceneObject2D::Destroy()
{
	if (collision)pEngine->RemoveObject(collision);
	if (physicsObject)pEngine->RemoveObject(physicsObject);

	SceneObject::Destroy();
}
