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

	float radius;
	float derivedRadius;

private:
	char EdgeFromNormal(Vec2D Normal);

	bool TestAgainstOtherSquare(SatCollision2DObject* other, Collision2D *  response);
	bool TestAgainstOtherTriagnle(SatCollision2DObject* other, Collision2D *  response);
	bool TestAgainstOtherCircle(SatCollision2DObject* other, Collision2D *  response);
	bool TestAgainstOtherAABBSquare(class AABBSquareCollisionObject* other, Collision2D *  response);

	bool SweepTestAgainstOtherSquare(SatCollision2DObject* other, SweepCollision2D & response);
	bool SweepTestAgainstOtherTriagnle(SatCollision2DObject* other, SweepCollision2D & response);
	bool SweepTestAgainstOtherCircle(SatCollision2DObject* other, SweepCollision2D & response);
	bool SweepTestAgainstOtherAABBSquare(class AABBSquareCollisionObject* other, SweepCollision2D & response);

	Vec2D ClosesPointOnSegment(const Vec2D& point, const Vec2D& minp, const Vec2D& maxp);
	int FindCollisionPoints(CollisionPoint CollisionPointA[2], const Vector2D& CollisionNormal, const Vector2D otherVertecies[4]);

public:
	SatCollision2DObject(SceneObject2D *object);
	~SatCollision2DObject();

	void SetAsTriangle(const Vector2D points[3], const Vector2D size);
	void SetAsSquare(const Vector2D points[4], const Vector2D size);
	void SetAsCircle(float radius);

	void SetBoundsBySceneObject()override;

	virtual Vector2D GetSize()override;
	virtual bool CollidesWith(CollisionObject2DBase* other, Collision2D * response)override;
	virtual Vector2D GetClosestPointTo(Vector2D pos)override;

	virtual bool SweepCollidesWith(CollisionObject2DBase* other, Vector2D newPosition, SweepCollision2D & response)override;
	virtual bool FastSweepCollidesWith(Vector2D newPosition)override;

	virtual bool CollidesWithNoCollision(CollisionObject2DBase* other)override { return false; }
};
