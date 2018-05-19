#pragma once
#include "SceneObject.h"

class TextureBase;
class DRAGENGINE_EXPORT TexturedSprite : public SceneObject
{
private:
	TextureBase* texture;

public:
	TexturedSprite(const char* texture);
	virtual ~TexturedSprite();
};

