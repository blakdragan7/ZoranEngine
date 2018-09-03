#include <stdafx.h>
#include "SceneObject2D.h"
#include <Core/ZoranEngine.h>
#include <Physics/PhysicsEngine.h>
#include <Physics/2D/PhysicsObject2DBase.h>
#include <Physics/2D/Collision/CollisionObject2DBase.h>

#include <Core/2D/Components/Component2DBase.h>

#define PI 3.141592653589793

SceneObject2D::SceneObject2D(std::string name) : root2DComponent(0), SceneObject(root2DComponent, name)
{
}

SceneObject2D::SceneObject2D(Component2DBase* root2DComponent, std::string name) : root2DComponent(root2DComponent), SceneObject(root2DComponent,name)
{
}

SceneObject2D::~SceneObject2D()
{
}

void SceneObject2D::SetPosition(const Vector2D& pos)
{
	root2DComponent->SetOffset(pos);
}

void SceneObject2D::SetScale(const Vector2D& scale)
{
	root2DComponent->SetScale(scale);
}

void SceneObject2D::SetPosition(float x, float y)
{
	root2DComponent->SetOffset(x,y);
}

void SceneObject2D::SetScale(float x, float y)
{
	root2DComponent->SetScale(x, y);
}

void SceneObject2D::SetRotation(float rotation)
{
	root2DComponent->SetRotatation(rotation);
}

void SceneObject2D::Translate(const Vector2D& delta)
{
	root2DComponent->Translate(delta);
}

void SceneObject2D::Translate(float x, float y)
{
	root2DComponent->Translate(x, y);
}

void SceneObject2D::Scale(const Vector2D& scale)
{
	root2DComponent->Scale(scale);
}

void SceneObject2D::Scale(float dx, float dy)
{
	root2DComponent->Scale(dx, dy);
}

void SceneObject2D::Rotate(float rotation)
{
	root2DComponent->Rotate(rotation);
}

float SceneObject2D::GetRotationDegree()const
{
	return (root2DComponent->GetRotation() * 180.0f) / 3.14159265359f;
}

float SceneObject2D::GetRotation()const
{
	return root2DComponent->GetRotation();
}

void SceneObject2D::PreCaclModel()
{
	ModelMatrixCache = root2DComponent->GetWorldMatrix();
}

Matrix44 SceneObject2D::GetScaleMatrix4x4()
{
	return Matrix44::ScaleMatrix(root2DComponent->GetScale());
}

void SceneObject2D::Destroy()
{
	SceneObject::Destroy();
}
