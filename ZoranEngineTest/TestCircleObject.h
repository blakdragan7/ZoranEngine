#pragma once
#include <Core/2D/TexturedSprite.h>


class TestCircleObject : public TexturedSprite
{
private:
	SceneObject2D * target;

public:

	TestCircleObject(std::string name, float radius);
	~TestCircleObject();

	inline void SetTarget(SceneObject2D* target)
	{
		this->target = target;
	}

	virtual void Tick(float dt)override;
};

