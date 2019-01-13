#pragma once
#include <Core/2D/TexturedSprite.h>

class AASquareCollisionComponent;
class OSquareCollisionComponent;
class RigidBody2DComponent;
class TestPlatformObject : public TexturedSprite
{
private:
	//OSquareCollisionComponent * collision;
	AASquareCollisionComponent * collision;
	RigidBody2DComponent* rigid;

public:
	TestPlatformObject(std::string name);
	~TestPlatformObject();
};
