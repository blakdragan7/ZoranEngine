#pragma once
#include <Core/2D/TexturedSprite.h>
class TextureBase;
class RigidBody2DComponent;
class OSquareCollisionComponent;

class TestSceneObject : public TexturedSprite
{
	SceneObject2D* target;
	RigidBody2DComponent* rigid;
	OSquareCollisionComponent* collision;

public:
	TestSceneObject(std::string name);
	~TestSceneObject();

	virtual void Tick(float dt)override;

	inline void SetTarget(SceneObject2D* target)
	{
		this->target = target;
	}

	RigidBody2DComponent* GetPhysics() { return rigid; }

};

