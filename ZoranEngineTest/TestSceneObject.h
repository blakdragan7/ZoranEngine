#pragma once
#include <Core/2D/TexturedSprite.h>
#include <Core/TickableObject.h>
class TextureBase;
class TestSceneObject : public TexturedSprite
{

public:
	TestSceneObject(std::string name);
	~TestSceneObject();

	virtual void PreRender()override;
	virtual void Tick(float dt)override;
};

