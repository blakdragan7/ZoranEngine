#include <stdafx.h>
#include "SceneObject2D.h"

SceneObject2D::SceneObject2D(std::string name) : SceneObject(name)
{

}

SceneObject2D::SceneObject2D(std::string name, RenderEngineBase* engine) : SceneObject(name, engine)
{

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
}

void SceneObject2D::SetPosition(double x, double y)
{
	this->pos.x = x;
	this->pos.y = y;
}

void SceneObject2D::SetScale(double x, double y)
{
	this->scale.x = x;
	this->scale.y = y;
}

void SceneObject2D::SetRotation(double rotation)
{
	this->rotation = rotation;
}

void SceneObject2D::Translate(Vector2D delta)
{
	this->pos += delta;
}

void SceneObject2D::Translate(double x, double y)
{
	this->pos.x += x;
	this->pos.y += y;
}

void SceneObject2D::Scale(Vector2D scale)
{
	this->scale *= scale;
}

void SceneObject2D::Scale(double dx, double dy)
{
	this->scale.x *= dx;
	this->scale.y *= dy;
}

void SceneObject2D::Rotate(double rotation)
{
	rotation += rotation;
}

double SceneObject2D::GetRotation()
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

MatrixF SceneObject2D::GetModel()
{
	model.makeIdentity();

	MatrixF rotMatrix = MatrixF::GLRotationMatrix(rotation, Vector3D(1, 0, 0));
	model.translate(pos);

	return model * rotMatrix;
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
