#pragma once
#include <Core/2D/TexturedSprite.h>


class TestCircleObject : public TexturedSprite
{
private:
	SceneObject2D * target;
	class RigidBody2DComponent* rigidBody;

public:

	TestCircleObject(std::string name);
	~TestCircleObject();

	inline void SetTarget(SceneObject2D* target)
	{
		this->target = target;
	}

	virtual void Tick(float dt)override;

	RigidBody2DComponent* GetPhysics() { return rigidBody; }
};

