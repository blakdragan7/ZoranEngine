#include "stdafx.h"
#include "SATCollision3DObject.h"
#include "Core/3D/SceneObject3D.h"
#include "Rendering/RenderedObjectBase.h"
#include <Core/3D/Components/Component3DBase.h> 

SATCollision3DObject::SATCollision3DObject(Component3DBase* object) : CollisionObject3DBase(object,CD_Dynamic,SAT_COLLISION)
{
	vertData = 0;
	numVerts = 0;
}

SATCollision3DObject::~SATCollision3DObject()
{
}

void SATCollision3DObject::SetBoundsBySceneObject()
{
	// TODO re implement this with correct collision system

	if (vertData)delete[] vertData;
	/*if (GetAffectedComponent()->GetRenderedObject()->GetVertDataAsfloat(&vertData, numVerts) == false)
	{
		Log(LogLevel_Error,"SATCollisionObject::SetBoundsBySceneObject(): Error Getting VertData For Scene Object %s", GetSceneObject()->readableName);
	}*/

	
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
