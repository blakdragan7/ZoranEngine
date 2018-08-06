#pragma once
#include <Core/2D/TexturedSprite.h>
class TextureBase;
class TestSceneObject : public TexturedSprite
{
	SceneObject2D* target;

public:
	TestSceneObject(std::string name);
	~TestSceneObject();

	virtual void PreRender()override;
	virtual void Tick(float dt)override;

	inline void SetTarget(SceneObject2D* target)
	{
		this->target = target;
	}
};

