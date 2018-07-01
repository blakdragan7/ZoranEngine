#pragma once
#include <Core/2D/TexturedSprite.h>

class TestPlatformObject : public TexturedSprite
{
public:
	TestPlatformObject(std::string name);
	~TestPlatformObject();

	virtual void PreRender()override;
};
