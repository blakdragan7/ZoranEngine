#pragma once
#include <Core/2D/TexturedSprite.h>

class TestPlatformObject : public TexturedSprite
{
private:
	OSquareCollisionComponent * collision;
	RigidBody2DComponent* rigid;

public:
	TestPlatformObject(std::string name);
	~TestPlatformObject();
};
