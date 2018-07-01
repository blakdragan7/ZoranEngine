#pragma once
#include <Physics/3D/Collision/CollisionObject3DBase.h>

#define SAT_COLLISION 6

class SATCollision3DObject : public CollisionObject3DBase
{
private:
	double* vertData;
	unsigned* edgeIndexes;
	unsigned numVerts;

public:
	SATCollision3DObject(SceneObject3D* object);
	~SATCollision3DObject();

	void SetBoundsBySceneObject()override;

	virtual Vector3D GetSize()override;
	virtual bool CollidesWith(CollisionObject3DBase* other, CollisionResponse3D& response)override;
	virtual Vector3D GetClosestPointTo(Vector3D pos)override;
};

