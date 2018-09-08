#pragma once
#include <Core/2D/SceneObject2D.h>
#include "Math/Vector2.h"
#include "Rendering/RenderTypes.h"

class TexturedSpriteComponent;
class ZoranEngine_EXPORT TexturedSprite : public SceneObject2D
{
protected:
	TexturedSpriteComponent * spriteComponent;

public:
	TexturedSprite(std::string name);
	TexturedSprite(std::string name, const char* texture, RenderDataType type, RenderDataFormat format);
	virtual ~TexturedSprite();

	void SetTexture(const char* path,RenderDataType type,RenderDataFormat format);

};

