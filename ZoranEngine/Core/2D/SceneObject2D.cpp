#include <stdafx.h>
#include "SceneObject2D.h"
#include <Core/ZoranEngine.h>
#include <Physics/PhysicsEngine.h>
#include <Physics/2D/PhysicsObject2DBase.h>
#include <Physics/2D/Collision/CollisionObject2DBase.h>

#include <Core/2D/Components/Component2DBase.h>

#define PI 3.141592653589793

bool SceneObject2D::CheckRootComponent(const char* func)const
{
	if (root2DComponent == NULL)
	{
		Log(LogLevel_Error, "%s called on %s without a root Component !!\n",func,readableName->c_str());
		return false;
	}

	return true;
}

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
	if (CheckRootComponent("SceneObject2D::SetPosition"))
	{
		root2DComponent->SetOffset(pos);
	}
}

void SceneObject2D::SetScale(const Vector2D& scale)
{
	if (CheckRootComponent("SceneObject2D::SetPosition"))
	{
		root2DComponent->SetScale(scale);
	}
}

void SceneObject2D::SetPosition(float x, float y)
{
	if (CheckRootComponent("SceneObject2D::SetPosition"))
	{
		root2DComponent->SetOffset(x, y);
	}
}

void SceneObject2D::SetScale(float x, float y)
{
	if (CheckRootComponent("SceneObject2D::SetPosition"))
	{
		root2DComponent->SetScale(x, y);
	}
}

void SceneObject2D::SetRotation(float rotation)
{
	if (CheckRootComponent("SceneObject2D::SetPosition"))
	{
		root2DComponent->SetRotatation(rotation);
	}
}

void SceneObject2D::Translate(const Vector2D& delta)
{
	if (CheckRootComponent("SceneObject2D::SetPosition"))
	{
		root2DComponent->Translate(delta);
	}
}

void SceneObject2D::Translate(float x, float y)
{
	if (CheckRootComponent("SceneObject2D::SetPosition"))
	{
		root2DComponent->Translate(x, y);
	}
}

void SceneObject2D::Scale(const Vector2D& scale)
{
	if (CheckRootComponent("SceneObject2D::SetPosition"))
	{
		root2DComponent->Scale(scale);
	}
}

void SceneObject2D::Scale(float dx, float dy)
{
	if (CheckRootComponent("SceneObject2D::SetPosition"))
	{
		root2DComponent->Scale(dx, dy);
	}
}

void SceneObject2D::Rotate(float rotation)
{
	if (CheckRootComponent("SceneObject2D::SetPosition"))
	{
		root2DComponent->Rotate(rotation);
	}
}

float SceneObject2D::GetRotationDegree()const
{
	CheckRootComponent("SceneObject2D::SetPosition");
	return (root2DComponent->GetRotation() * 180.0f) / 3.14159265359f;
}

float SceneObject2D::GetRotation()const
{
	CheckRootComponent("SceneObject2D::SetPosition");
	return root2DComponent->GetRotation();
}

void SceneObject2D::PreCaclModel()
{
	CheckRootComponent("SceneObject2D::SetPosition");
	ModelMatrixCache = root2DComponent->GetWorldMatrix();
}

Matrix44 SceneObject2D::GetScaleMatrix4x4()
{
	CheckRootComponent("SceneObject2D::SetPosition");
	return Matrix44::ScaleMatrix(root2DComponent->GetScale());
}

void SceneObject2D::Destroy()
{
	SceneObject::Destroy();
}
