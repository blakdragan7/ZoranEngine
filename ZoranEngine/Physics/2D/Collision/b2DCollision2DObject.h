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

class b2DCollision2DObject : public CollisionObject2DBase
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
	bool SquareAgainstOtherSquare(b2DCollision2DObject* other, Collision2D * response);
	bool SquareAgainstOtherTriagnle(b2DCollision2DObject* other, Collision2D * response);
	bool SquareAgainstOtherCircle(b2DCollision2DObject* other, Collision2D * response);
	bool SquareAgainstOtherAABBSquare(class AABBSquareCollisionObject* other, Collision2D * response);

	bool SweepSquareAgainstOtherSquare(b2DCollision2DObject* other, SweepCollision2D & response);
	bool SweepSquareAgainstOtherTriagnle(b2DCollision2DObject* other, SweepCollision2D & response);
	bool SweepSquareAgainstOtherCircle(b2DCollision2DObject* other, SweepCollision2D & response);
	bool SweepSquareAgainstOtherAABBSquare(class AABBSquareCollisionObject* other, SweepCollision2D & response);

	bool NewSATAlgorithim(CollisionObject2DBase* other, Collision2D* response);

public:
	b2DCollision2DObject(SceneObject2D *object);
	~b2DCollision2DObject();

	void SetAsTriangle(const Vector2D points[3], const Vector2D size);
	void SetAsSquare(const Vector2D points[4], const Vector2D size);
	void SetAsCircle(float radius);

	void SetBoundsBySceneObject()override;

	virtual Vector2D GetSize()override;
	virtual bool CollidesWithNoCollision(CollisionObject2DBase* other)override;
	virtual bool CollidesWith(CollisionObject2DBase* other, Collision2D* response)override;
	virtual Vector2D GetClosestPointTo(Vector2D pos)override;

	virtual bool SweepCollidesWith(CollisionObject2DBase* other, Vector2D newPosition, SweepCollision2D & response)override;
	virtual bool FastSweepCollidesWith(Vector2D newPosition)override;
};

