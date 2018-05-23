#pragma once
#include <TexturedSprite.h>
class TextureBase;
class TestSceneObject : public TexturedSprite
{

public:
	TestSceneObject();
	~TestSceneObject();

	virtual void PreRender()override;
};

