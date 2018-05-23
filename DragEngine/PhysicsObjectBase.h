#pragma once
class CollisionObjectBase;
class PhysicsObjectBase
{
private:
	CollisionObjectBase * collision;

public:
	PhysicsObjectBase();
	virtual ~PhysicsObjectBase();
};

