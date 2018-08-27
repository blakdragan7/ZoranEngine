#include <stdafx.h>
#include "SceneObject2D.h"
#include <Core/ZoranEngine.h>
#include <Physics/PhysicsEngine.h>
#include <Physics/2D/PhysicsObject2DBase.h>
#include <Physics/2D/Collision/CollisionObject2DBase.h>

#define PI 3.141592653589793
SceneObject2D::SceneObject2D(std::string name) : SceneObject(name)
{
	scale = Vector2D(1.0, 1.0);
	rotation = 0;

	rotationMat.SetRotation(0);
	invRotationMat.SetRotation(0);
}


SceneObject2D::~SceneObject2D()
{
}

void SceneObject2D::SetPosition(Vector2D pos)
{
	this->pos = pos;
}

void SceneObject2D::SetScale(Vector2D scale)
{
	this->scale = scale;
	this->size = (startingSize * scale).getAbs();
}

void SceneObject2D::SetPosition(float x, float y)
{
	this->pos.x = x;
	this->pos.y = y;
}

void SceneObject2D::SetScale(float x, float y)
{
	this->scale.x = x;
	this->scale.y = y;
	this->size = (startingSize * scale).getAbs();
}

void SceneObject2D::SetRotation(float rotation)
{
	this->rotation = rotation;
	rotationMat.SetRotation(rotation);
	invRotationMat = rotationMat.GetInversion();
}

void SceneObject2D::Translate(Vector2D delta)
{
	this->pos += delta;
}

void SceneObject2D::Translate(float x, float y)
{
	this->pos.x += x;
	this->pos.y += y;
}

void SceneObject2D::Scale(Vector2D scale)
{
	this->scale *= scale;
	this->size = (startingSize * scale).getAbs();
}

void SceneObject2D::Scale(float dx, float dy)
{
	this->scale.x *= dx;
	this->scale.y *= dy;
	this->size = (startingSize * scale).getAbs();
}

void SceneObject2D::Rotate(float rotation)
{
	this->rotation += rotation;
	rotationMat.SetRotation(rotation);
	invRotationMat = rotationMat.GetInversion();
}

float SceneObject2D::GetRotationRad()const
{
	return rotation;
}

float SceneObject2D::GetRotation()const
{
	return rotation;
}

Matrix22 SceneObject2D::GetRotationMatrix()const
{
	return rotationMat;
}

Matrix22 SceneObject2D::GetInvRotationMatrix()const
{
	return invRotationMat;
}

void SceneObject2D::PreCaclModel()
{
	model.makeIdentity();

	Matrix44 rotMatrix = Matrix44::GLRotationMatrix(rotation, Vector3D(0, 0, 1));
	model.translate(pos);
	model.scale(scale);

	ModelMatrixCache = model *rotMatrix;
}

Matrix44 SceneObject2D::GetScaleMatrix4x4()
{
	Matrix44 mat;
	mat.scale(scale);
	return mat;
}

void SceneObject2D::Destroy()
{
	SceneObject::Destroy();
}
