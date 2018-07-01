#include "stdafx.h"
#include "SATCollision3DObject.h"
#include "Core/3D/SceneObject3D.h"
#include "Rendering/RenderedObjectBase.h"

SATCollision3DObject::SATCollision3DObject(SceneObject3D* object) : CollisionObject3DBase(object,CD_Dynamic,SAT_COLLISION)
{
	vertData = 0;
	numVerts = 0;
}

SATCollision3DObject::~SATCollision3DObject()
{
}

void SATCollision3DObject::SetBoundsBySceneObject()
{
	if (vertData)delete[] vertData;
	if (GetSceneObject()->GetRenderedObject()->GetVertDataAsDouble(&vertData, numVerts) == false)
	{
		Log(LogLevel_Default,"SATCollisionObject::SetBoundsBySceneObject(): Error Getting VertData For Scene Object %s", GetSceneObject()->readableName);
	}

	
}

Vector3D SATCollision3DObject::GetSize()
{
	return Vector3D();
}

bool SATCollision3DObject::CollidesWith(CollisionObject3DBase * other, CollisionResponse3D & response)
{
	return false;
}

Vector3D SATCollision3DObject::GetClosestPointTo(Vector3D pos)
{
	return Vector3D();
}
