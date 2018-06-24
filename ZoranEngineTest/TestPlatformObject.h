#pragma once
#include <Core/TexturedSprite.h>

class TestPlatformObject : public TexturedSprite
{
public:
	TestPlatformObject();
	~TestPlatformObject();

	virtual void PreRender()override;
};
