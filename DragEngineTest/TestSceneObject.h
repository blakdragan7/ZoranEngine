#pragma once
#include <TexturedSprite.h>
#include <TickableObject.h>
class TextureBase;
class TestSceneObject : public TexturedSprite , public TickableObject
{

public:
	TestSceneObject();
	~TestSceneObject();

	virtual void PreRender()override;
	virtual void Tick(double dt)override;
};

