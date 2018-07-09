#pragma once
#include <Physics\2D\Collision\CollisionObject2DBase.h>

#define SAT_2D_COLLISION 7

enum SATPolygonType
{
	SATPT_Circle = 0,
	SATPT_Triangle = 3,
	SATPT_Square = 4,
	SATPT_Invalid
};

class SatCollision2DObject : public CollisionObject2DBase
{
private:
	SATPolygonType polygonType;

	Vector2D startingPoints[4];
	Vector2D derivedPoints[4];
	Vector2D size;
	Vector2D scaledSize;

	double radius;
	double derivedRadius;

private:
	bool TestAgainstOtherSquare(SatCollision2DObject* other, CollisionResponse2D & response);
	bool TestAgainstOtherTriagnle(SatCollision2DObject* other, CollisionResponse2D & response);
	bool TestAgainstOtherCircle(SatCollision2DObject* other, CollisionResponse2D & response);
	bool TestAgainstOtherAABBSquare(class AABBSquareCollisionObject* other, CollisionResponse2D & response);

	bool SweepTestAgainstOtherSquare(SatCollision2DObject* other, SweepCollisionResponse2D & response);
	bool SweepTestAgainstOtherTriagnle(SatCollision2DObject* other, SweepCollisionResponse2D & response);
	bool SweepTestAgainstOtherCircle(SatCollision2DObject* other, SweepCollisionResponse2D & response);
	bool SweepTestAgainstOtherAABBSquare(class AABBSquareCollisionObject* other, SweepCollisionResponse2D & response);

public:
	SatCollision2DObject(SceneObject2D *object);
	~SatCollision2DObject();

	void SetAsTriangle(const Vector2D points[3], const Vector2D size);
	void SetAsSquare(const Vector2D points[4], const Vector2D size);
	void SetAsCircle(double radius);

	void SetBoundsBySceneObject()override;

	virtual Vector2D GetSize()override;
	virtual bool CollidesWith(CollisionObject2DBase* other, CollisionResponse2D& response)override;
	virtual Vector2D GetClosestPointTo(Vector2D pos)override;

	virtual bool SweepCollidesWith(CollisionObject2DBase* other, Vector2D newPosition, SweepCollisionResponse2D & response)override;
	virtual bool FastSweepCollidesWith(Vector2D newPosition)override;
};

