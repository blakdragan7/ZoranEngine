#pragma once
#include "Physics/Collision/CollisionObjectBase.h"

#define SAT_COLLISION 6

class SATCollisionObject : public CollisionObjectBase
{
private:
	double* vertData;
	unsigned* edgeIndexes;
	unsigned numVerts;
	bool is3D;

public:
	SATCollisionObject(bool is3D, SceneObject* object);
	~SATCollisionObject();

	void SetBoundsBySceneObject()override;

	virtual Vector3D GetSize()override;
	virtual bool CollidesWith(CollisionObjectBase* other, CollisionResponse& response)override;
	virtual Vector3D GetClosestPointTo(Vector3D pos)override;
};

