#pragma once
#include "SceneObject.h"
class TextureBase;
class TestSceneObject : public SceneObject
{
private:
	TextureBase * texture;

public:
	TestSceneObject();
	~TestSceneObject();

	virtual void PreRender()override;
};

