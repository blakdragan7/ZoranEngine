#include "stdafx.h"
#include "CollisionObject2DBase.h"
#include <Core/2D/SceneObject2D.h>
#include <Core/2D/DebugSceneObject2D.h>
#include <Rendering/RenderedObjectBase.h>
#include <Rendering/RenderEngineBase.h>

#include <Core/2D/Components/RigidBody2DComponent.h>

unsigned long long Collision2D::sID = 0;

void CollisionObject2DBase::UpdateDebugObject(float verts[], unsigned numVerts)
{
	/*
	if (ZoranEngine::canRenderDebug == false)return;

	static bool once = true;
	if (once)
	{
		rEngine->SetLineWidth(5);
		once = false;
	}
	if (debugObject == 0)
	{
		if (sceneObject2D)
		{
			debugObject = new DebugSceneObject2D("debug collision " + *sceneObject2D->readableName);
			debugObject->GetRenderedObject()->CreateObjectFromMemory(PT_Line_Loop, VT_Float, DT_Dynamic, numVerts, verts, 0);
		}
		else
		{
			debugObject = new DebugSceneObject2D("debug collision unkown");
			debugObject->GetRenderedObject()->CreateObjectFromMemory(PT_Line_Loop, VT_Float, DT_Dynamic, numVerts, verts, 0);
		}

		//debugObject->SetColor(Vec3D(1.0,1.0,0.0));

		//zEngine->AddSceneObject(debugObject);
	}
	else
	{
		//debugObject->GetRenderedObject()->UpdateObjectFromMemory(numVerts, 0, verts, 0);
	}*/
}

CollisionObject2DBase::CollisionObject2DBase(Component2DBase* component, CollisionDynamics collisionDynamics, unsigned collisionType) :
	debugObject(0),physicsObject2D(0),affectedComponent(component),CollisionObjectBase(collisionDynamics, collisionType)
{
}

CollisionObject2DBase::~CollisionObject2DBase()
{
}

void CollisionObject2DBase::SetAffectedComponent(RigidBody2DComponent* component)
{
	affectedComponent = component;
}

void CollisionObject2DBase::SetPhysicsObject(PhysicsObject2DBase* object)
{
	physicsObject2D = object;
}

const Vector2D& CollisionObject2DBase::GetScenePos()
{
	if (affectedComponent != NULL) return affectedComponent->GetWorldLocation();
	else
	{
		Log(LogLevel_Error,"Getting Position Before SceneObject Set !\n");
		return Vector2D::Zero;
	}
}
