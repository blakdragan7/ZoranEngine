#pragma once
#include <Math/Vector2.h>
#include <Math/Matrix22.h>

enum Axis
{
	FACE_A_X,
	FACE_A_Y,
	FACE_B_X,
	FACE_B_Y
};

enum EdgeNumbers
{
	NO_EDGE = 0,
	EDGE1,
	EDGE2,
	EDGE3,
	EDGE4
};

union Edges
{
	struct
	{
		char inEdge1;
		char outEdge1;
		char inEdge2;
		char outEdge2;
	} e;
	int value;

	void Flip()
	{
		char temp = e.inEdge1;
		e.inEdge1 = e.inEdge2;
		e.inEdge2 = temp;

		temp = e.outEdge1;
		e.outEdge1 = e.outEdge2;
		e.outEdge2 = temp;
	}
};

struct ClipedVertex
{
	Edges edge;
	Vector2D vertex;
	ClipedVertex() { edge.value = 0; }
};

struct  ZoranEngine_EXPORT CollisionPoint
{
	Vector2D pos;
	Vector2D normal;
	Edges edges;
	Vector2D r1;
	Vector2D r2;

	float separation;
	float normalImpulse;
	float normalTangetImpulses;
	float noramlBiasImpulses;
	float massNormal;
	float massTangent;
	float bias;

	CollisionPoint()
	{
		separation = 0;
		normalImpulse = 0;
		normalTangetImpulses = 0;
		noramlBiasImpulses = 0;
		massNormal = 0;
		massTangent = 0;
		bias = 0;
	}
};

struct ZoranEngine_EXPORT Accumulated2DVelocities
{
	Vec2D velocity[256];
	float angularVelocity[256];

	class PhysicsObject2DBase* objects[256];

	unsigned numObjects;

	unsigned AddObject(class PhysicsObject2DBase* object);
};

class ZoranEngine_EXPORT Collision2D
{
public:
	class DebugSceneObject2D* debugObjects[2];
	CollisionPoint collisionPoints[2];

	unsigned aIndex;
	unsigned bIndex;

	unsigned numCollisionPoints;

	bool shouldRender;

public:

	bool collided;
	bool wasUpdated;
	int frame;

	class SceneObject2D* objects[2];
	Matrix22 invRotationSnapshots[2];
	class PhysicsObject2DBase* collidedObjects[2];

	class CollisionObject2DBase* objectBounds[2];
	unsigned long long ID;
	static unsigned long long sID;

	float friction;

	void AddCollisionPoint(CollisionPoint &point);

	Collision2D();
	~Collision2D();
	Collision2D* Reflection();
	bool operator==(const Collision2D& other);
	void PreUpdate(float inv_dt, Accumulated2DVelocities& aV);
	void UpdateForces(Accumulated2DVelocities& aV);
	void Update(Collision2D* other);
	size_t GetNumCollisionPoints() { return numCollisionPoints; }
};

struct ZoranEngine_EXPORT Collision2DKey
{
	SceneObject2D* objectA,* objectB;
	Collision2DKey(SceneObject2D* a, SceneObject2D* b)
	{
		if (a < b)
		{
			objectA = a;
			objectB = b;
		}
		else
		{
			objectA = b;
			objectB = a;
		}
	}
};

inline bool operator < (const Collision2DKey& a1, const Collision2DKey& a2)
{
	if (a1.objectA < a2.objectA)
		return true;

	if (a1.objectA == a2.objectA && a1.objectB < a2.objectB)
		return true;

	return false;
}