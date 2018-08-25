#include "stdafx.h"
#include "CollisionObject2DBase.h"
#include <Core/2D/SceneObject2D.h>
#include <Core/2D/DebugSceneObject2D.h>
#include <Rendering/RenderedObjectBase.h>
#include <Rendering/RenderEngineBase.h>
unsigned long long Collision2D::sID = 0;

void CollisionObject2DBase::UpdateDebugObject(float verts[], unsigned numVerts)
{
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
			debugObject = new DebugSceneObject2D("debug collision " + sceneObject2D->readableName);
			debugObject->GetRenderedObject()->CreateObjectFromMemory(PT_Line_Loop, VT_Float, DT_Dynamic, numVerts, verts, 0);
		}
		else
		{
			debugObject = new DebugSceneObject2D("debug collision unkown");
			debugObject->GetRenderedObject()->CreateObjectFromMemory(PT_Line_Loop, VT_Float, DT_Dynamic, numVerts, verts, 0);
		}

		debugObject->SetColor(Vec3D(1.0,1.0,0.0));

		zEngine->AddSceneObject(debugObject);
	}
	else
	{
		debugObject->GetRenderedObject()->UpdateObjectFromMemory(numVerts, 0, verts, 0);
	}
}

CollisionObject2DBase::CollisionObject2DBase(SceneObject2D *object, CollisionDynamics collisionDynamics, unsigned collisionType) : CollisionObjectBase(object, collisionDynamics, collisionType)
{
	debugObject = 0;
	physicsObject2D = 0;
	sceneObject2D = object;
}

CollisionObject2DBase::~CollisionObject2DBase()
{
}

void CollisionObject2DBase::SetSceneObject(SceneObject2D* object)
{
	sceneObject2D = object;
}

void CollisionObject2DBase::SetPhysicsObject(PhysicsObject2DBase* object)
{
	physicsObject2D = object;
}

Vector2D CollisionObject2DBase::GetScenePos()
{
	if (sceneObject2D != NULL) return sceneObject2D->GetPosition();
	else
	{
		std::cerr << "Getting Position Before SceneObject Set !\n";
		return Vector2D(0, 0);
	}
}

SceneObject2D* CollisionObject2DBase::GetSceneObject() { return sceneObject2D; }
PhysicsObject2DBase* CollisionObject2DBase::GetPhysicsObject() { return physicsObject2D; }
