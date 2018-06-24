#include "stdafx.h"
#include "SATCollisionObject.h"
#include "Core/SceneObject.h"
#include "Rendering/RenderedObjectBase.h"

SATCollisionObject::SATCollisionObject(bool is3D, SceneObject* object) : CollisionObjectBase(object,CD_Dynamic,SAT_COLLISION)
{
	this->is3D = is3D;
	vertData = 0;
	numVerts = 0;
}

SATCollisionObject::~SATCollisionObject()
{
}

void SATCollisionObject::SetBoundsBySceneObject()
{
	if (vertData)delete[] vertData;
	if (GetSceneObject()->GetRenderedObject()->GetVertDataAsDouble(&vertData, numVerts) == false)
	{
		Log(LogLevel_Default,"SATCollisionObject::SetBoundsBySceneObject(): Error Getting VertData For Scene Object %s", GetSceneObject()->readableName);
	}

	if (is3D == false)
	{
		edgeIndexes = new unsigned[4];
	}
}

Vector3D SATCollisionObject::GetSize()
{
	return Vector3D();
}

bool SATCollisionObject::CollidesWith(CollisionObjectBase * other, CollisionResponse & response)
{
	return false;
}

Vector3D SATCollisionObject::GetClosestPointTo(Vector3D pos)
{
	return Vector3D();
}
